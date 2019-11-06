/********************************************************************
       Copyright 2019 Lee Patterson

       Licensed under the Apache License, Version 2.0 (the "License");
       you may not use this file except in compliance with the License.
       You may obtain a copy of the License at

           http://www.apache.org/licenses/LICENSE-2.0

       Unless required by applicable law or agreed to in writing, software
       distributed under the License is distributed on an "AS IS" BASIS,
       WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
       See the License for the specific language governing permissions and
       limitations under the License.

       https://github.com/abathur8bit/emserver

       created  :  November 30, 2001
       filename :  lockableobject.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef LOCKABLEOBJECT_H
#define LOCKABLEOBJECT_H

#include <ssobjects/threadutils.h>

using namespace ssobjects;

/**
    This object is meant to be used to be derived from.
    It allows an object to lock and unlock itself, and
    keeps a flag indicating if the object is locked or
    not.
**/
class LockableObject
{
  protected:
    bool m_bLocked;
    ssobjects::Lock m_lock;

  public:
    LockableObject() : m_bLocked(false),m_lock() {}
    ~LockableObject() {unlock();}

  public:
    void lock() {m_lock.lock(); m_bLocked = true;}
    void unlock() {m_bLocked = false; m_lock.unlock();}
    bool islocked() const {return m_bLocked;}
};

#endif //LOCKABLEOBJECT_H
