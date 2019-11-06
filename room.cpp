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
       filename :  room.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#include "room.h"

Room::Room(const int iRoomNumber,const char* pszName)
  : MudObject(iRoomNumber,pszName),m_pvRoomNum(NULL)
{
  m_pvRoomNum = new Variable("roomnum",Variable::TYPE_INT);
  m_pvRoomNum->setIntValue(iRoomNumber);
  m_listVariables.push_back(m_pvRoomNum);
}


void
Room::setRoomNum(const int n)
{
  AutoLock l(m_lock);
  m_pvRoomNum->setIntValue(n);
}


int
Room::roomNum()
{
  AutoLock l(m_lock);
  int n = m_pvRoomNum->intValue();
  return n;
}
