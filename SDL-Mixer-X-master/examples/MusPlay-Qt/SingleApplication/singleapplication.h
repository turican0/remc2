#pragma once
#if !defined(APPLICATION_H) && !defined(MUSPLAY_USE_WINAPI)
#define APPLICATION_H

#include <QObject>
#include <QUdpSocket>
#include <QStringList>
#include <QSystemSemaphore>
#include <QSharedMemory>

#include "localserver.h"

/**
 * @brief The Application class handles trivial application initialization procedures
 */
class SingleApplication : public QObject
{
Q_OBJECT
public:
    explicit SingleApplication(QStringList &args);
    ~SingleApplication();
    bool shouldContinue();
    QStringList arguments();

public slots:

signals:
    void showUp();
    void stopServer();
    void openFile(QString path);
    void acceptedCommand(QString cmd);

private slots:
    void slotShowUp();
    void slotOpenFile(QString path);
    void slotAcceptedCommand(QString cmd);

private:
    //! Semaphore, avoids races
    QSystemSemaphore m_sema;
    //! Shared memory, stable way to avoid concurrent running multiple copies of same application
    QSharedMemory m_shmem;
    //! Client socket pointer
    QUdpSocket* socket;
    //! Pointer to currently working local server copy
    LocalServer* server;
    //! Recently accepted arguments
    QStringList _arguments;
    //! Allows contination of application running. If false - another copy of same application already ranned
    bool _shouldContinue;

};

#endif // APPLICATION_H
