// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#include <thread>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "epoll_action.h"
#include "include/cppnet_type.h"
#include "cppnet/cppnet_config.h"
#include "cppnet/socket/rw_socket.h"
#include "cppnet/socket/connect_socket.h"
#include "cppnet/event/event_interface.h"

#include "common/log/log.h"
#include "common/util/time.h"
#include "common/os/convert.h"
#include "common/network/socket.h"
#include "common/network/io_handle.h"
#include "common/alloter/alloter_interface.h"

namespace cppnet {

std::shared_ptr<EventActions> MakeEventActions() {
    return std::make_shared<EpollEventActions>();
}

EpollEventActions::EpollEventActions():
    _epoll_handler(-1) {
    _active_list.resize(1024);

}

EpollEventActions::~EpollEventActions() {
    if (_epoll_handler > 0) {
        close(_epoll_handler);
    }
}

bool EpollEventActions::Init(uint32_t thread_num) {
    //get epoll handle. the param is invalid since linux 2.6.8
    _epoll_handler = epoll_create(1500);
    if (_epoll_handler == -1) {
        LOG_FATAL("epoll init failed! error : %d", errno);
        return false;
    }
    if (pipe((int*)_pipe) == -1) {
        LOG_FATAL("pipe init failed! error : %d", errno);
        return false;
    }

    SocketNoblocking(_pipe[1]);
    SocketNoblocking(_pipe[0]);

    _pipe_content.events = EPOLLIN;
    _pipe_content.data.fd = _pipe[0];
    int32_t ret = epoll_ctl(_epoll_handler, EPOLL_CTL_ADD, _pipe[0], &_pipe_content);
    if (ret < 0) {
        LOG_FATAL("add pipe handle to epoll faild! error :%d", errno);
        return false;
    }
    return true;
}

bool EpollEventActions::Dealloc() {
    Wakeup();
    return true;
}

bool EpollEventActions::AddSendEvent(std::shared_ptr<Event>& event) {
    if (event->GetType() & ET_WRITE) {
        return false;
    }
    event->AddType(ET_WRITE);

    auto sock = event->GetSocket();
    if (sock) {
        epoll_event* ep_event = (epoll_event*)event->GetData();
        if (!ep_event) {
            auto rw_sock = std::dynamic_pointer_cast<RWSocket>(sock);
            ep_event = rw_sock->GetAlloter()->PoolNew<epoll_event>();
            memset(ep_event, 0, sizeof(epoll_event));
            event->SetData(ep_event);
        }
        ep_event->data.ptr = (void*)&event;
        if (AddEvent(ep_event, EPOLLOUT, sock->GetSocket(), event->GetType() & ET_INACTIONS)) {
            event->AddType(ET_INACTIONS);
            return true;
        }
    }
    LOG_WARN("socket is already distroyed! event %s", "AddSendEvent");
    return false;
}

bool EpollEventActions::AddRecvEvent(std::shared_ptr<Event>& event) {
    if (event->GetType() & ET_READ) {
        return false;
    }
    event->AddType(ET_READ);
    
    auto sock = event->GetSocket();
    if (sock) {
        epoll_event* ep_event = (epoll_event*)event->GetData();
        if (!ep_event) {
            auto rw_sock = std::dynamic_pointer_cast<RWSocket>(sock);
            ep_event = rw_sock->GetAlloter()->PoolNew<epoll_event>();
            memset(ep_event, 0, sizeof(epoll_event));
            event->SetData(ep_event);
        }
        ep_event->data.ptr = (void*)&event;
        if (AddEvent(ep_event, EPOLLIN, sock->GetSocket(), event->GetType() & ET_INACTIONS)) {
            event->AddType(ET_INACTIONS);
            return true;
        }
    }
    LOG_WARN("socket is already distroyed! event %s", "AddRecvEvent");
    return false;
}

bool EpollEventActions::AddAcceptEvent(std::shared_ptr<Event>& event) {
    if (event->GetType() & ET_ACCEPT) {
        return false;
    }
    event->AddType(ET_ACCEPT);
    
    auto sock = event->GetSocket();
    if (sock) {
        epoll_event* ep_event = (epoll_event*)event->GetData();
        if (!ep_event) {
            ep_event = new epoll_event;
            memset(ep_event, 0, sizeof(epoll_event));
            event->SetData(ep_event);
        }
        ep_event->data.ptr = (void*)&event;
        if (AddEvent(ep_event, EPOLLIN, sock->GetSocket(), event->GetType() & ET_INACTIONS)) {
            event->AddType(ET_INACTIONS);
            return true;
        }
    }
    LOG_WARN("socket is already distroyed! event %s", "AddAcceptEvent");
    return false;
}

bool EpollEventActions::AddConnection(std::shared_ptr<Event>& event, Address& addr) {
    if (event->GetType() & ET_CONNECT) {
        return false;
    }
    event->AddType(ET_CONNECT);

    auto sock = event->GetSocket();
    if (sock) {
        //the socket must not in epoll
        if (event->GetType() & ET_INACTIONS) {
            return false;
        }

        //block here in linux
        SocketNoblocking(sock->GetSocket());

        auto ret = OsHandle::Connect(sock->GetSocket(), addr);

        auto rw_sock = std::dynamic_pointer_cast<RWSocket>(sock);
        if (ret._return_value == 0) {
            rw_sock->OnConnect(CEC_SUCCESS);
            return true;

        } else if (ret._errno == EINPROGRESS) {
            if (CheckConnect(rw_sock->GetSocket())) {
                rw_sock->OnConnect(CEC_SUCCESS);
                return true;
            }
        }
        rw_sock->OnConnect(CEC_CONNECT_REFUSE);
        LOG_WARN("connect event failed! %d", ret._errno);
        return false;
    }
    LOG_WARN("connection event is already destroyed!,%s", "AddConnection");
    return false;
}

bool EpollEventActions::AddDisconnection(std::shared_ptr<Event>& event) {
    if (event->GetType() & ET_DISCONNECT) {
        return false;
    }
    event->AddType(ET_DISCONNECT);
    
    auto sock = event->GetSocket();
    if (!sock) {
        return false;
    }
    
    std::shared_ptr<RWSocket> socket = std::dynamic_pointer_cast<RWSocket>(sock);
    if (!DelEvent(event)) {
        return false;
    }
    OsHandle::Close(socket->GetSocket());
    socket->OnDisConnect(CEC_SUCCESS);
    return true;
}

bool EpollEventActions::DelEvent(std::shared_ptr<Event>& event) {
    auto sock = event->GetSocket();
    if (!sock) {
        return false;
    }
    epoll_event* ev = (epoll_event*)event->GetData();
    int32_t ret = epoll_ctl(_epoll_handler, EPOLL_CTL_DEL, sock->GetSocket(), ev);
    if (ret < 0) {
        LOG_ERROR("remove event from epoll faild! error :%d, socket : %d", errno, sock->GetSocket());
        return false;
    }

    event->ClearType();
    LOG_DEBUG("del a socket from epoll, %d", sock->GetSocket());
    return true;
}

void EpollEventActions::ProcessEvent(int32_t wait_ms) {
    int16_t ret = epoll_wait(_epoll_handler, &*_active_list.begin(), (int)_active_list.size(), wait_ms);
    if (ret == -1) {
        if (errno == EINTR) {
            return;
        }
        LOG_ERROR("epoll wait faild! error:%d, info:%s", errno, ErrnoInfo(errno));

    } else {
        LOG_DEBUG("epoll get events! num:%d, TheadId: %lld", ret, std::this_thread::get_id());

        OnEvent(_active_list, ret);
    }
}

void EpollEventActions::Wakeup() {
   write(_pipe[1], "1", 1);
}

void EpollEventActions::OnEvent(std::vector<epoll_event>& event_vec, int16_t num) {
    std::shared_ptr<Socket> sock;
    std::shared_ptr<Event> event;

    for (int i = 0; i < num; i++) {
        if (event_vec[i].data.fd == _pipe[0]) {
            LOG_WARN("weak up the io thread, index : %d", i);
            char buf[4];
            read(_pipe[0], buf, 1);
            continue;
        }

        event = *(std::shared_ptr<Event>*)event_vec[i].data.ptr;
        sock = event->GetSocket();
        if (!sock) {
            LOG_WARN("epoll weak up but socket already destroy, index : %d", i);
            continue;
        }

        // accept event
        if (event->GetType() & ET_ACCEPT) {
            std::shared_ptr<ConnectSocket> connect_sock = std::dynamic_pointer_cast<ConnectSocket>(sock);
            connect_sock->OnAccept();

        } else {
            std::shared_ptr<RWSocket> rw_sock = std::dynamic_pointer_cast<RWSocket>(sock);
            if (event_vec[i].events & EPOLLIN) {
                // close
                if (event_vec[i].events & EPOLLRDHUP) {
                    rw_sock->OnDisConnect(CEC_CLOSED);
                }
                rw_sock->OnRead();
            }

            if (event_vec[i].events & EPOLLOUT) {
                rw_sock->OnWrite();
            }
        }
    }
}

bool EpollEventActions::AddEvent(epoll_event* ev, int32_t event_flag, uint64_t sock, bool in_actions) {
    //if not add to epoll
    if (!(ev->events & event_flag)) {
        if (__epoll_use_et) {
            ev->events |= event_flag | EPOLLET;

        } else {
             ev->events |= event_flag;
        }
            
        int32_t ret = 0;
        if (in_actions) {
            ret = epoll_ctl(_epoll_handler, EPOLL_CTL_MOD, sock, ev);

        } else {
            ret = epoll_ctl(_epoll_handler, EPOLL_CTL_ADD, sock, ev);
        }
            
        if (ret == 0) {
            return true;
        }
        LOG_ERROR("modify event to epoll faild! error :%d, sock: %d", errno, sock);
    }
    return false;
}

}