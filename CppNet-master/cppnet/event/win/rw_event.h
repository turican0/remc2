// Use of this source code is governed by a BSD 3-Clause License
// that can be found in the LICENSE file.

// Author: caozhiyi (caozhiyi5@gmail.com)

#ifndef CPPNET_EVENT_WIN_RW_EVENT
#define CPPNET_EVENT_WIN_RW_EVENT

#include "cppnet/cppnet_config.h"
#include "cppnet/event/event_interface.h"

namespace cppnet {

class RWEvent:
    public Event{
public:
    RWEvent(): 
       _ex_data(nullptr) {}
    virtual ~RWEvent() {}

    void SetExData(void* data) { _ex_data = data; }
    void* GetExData() { return _ex_data; }

private:
    void* _ex_data;
};

}

#endif