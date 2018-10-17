///////////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/mixerlck.h $
// $Author: dc $
// $Date: 1997/11/10 10:28:05 $
// $Revision: 1.1 $
//
// Tools for thread synchronization and management.
//
//

#ifndef __MIXERLCK_H
#define __MIXERLCK_H

#ifdef _WIN32

///////////////////////////////////////////////////////////////////////////////
//
// CLASS: cMixerAutoLock
//
// a threadMutex autolock for the mixer
// really could just be cMutexAutoLock, but hey
//

class cMixerAutoLock
{
public:
    cMixerAutoLock(cThreadMutex & mutex);
    ~cMixerAutoLock();

private:
    cThreadMutex & m_mutex;

    // make copy constructor and assignment operator inaccessible
    cMixerAutoLock(const cMixerAutoLock &);
    cMixerAutoLock &operator=(const cMixerAutoLock &);
};

//////////////////////////////////////
//
// CLASS: cMixerAutoLock
//

inline
cMixerAutoLock::cMixerAutoLock(cThreadMutex & mutex)
  : m_mutex(mutex)
{
#ifndef SHIP
   DWORD hr=m_mutex.Wait();
   switch (hr)
   {
      case WAIT_TIMEOUT:
      case WAIT_ABANDONED:
         CriticalMsg("AutoLock of Mutex Wait Infinite Timeout or Abandon");
         break;
      case WAIT_OBJECT_0:
         break;
   }
#else
   m_mutex.Wait();
#endif    
}

///////////////////////////////////////

inline
cMixerAutoLock::~cMixerAutoLock()
{
    m_mutex.Release();
}

#endif // _WIN32

#endif /* !__MIXERLCK_H */
