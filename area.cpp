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

       created  :  Dec 28, 2001
       filename :  area.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#include <string.h>
#include <ssobjects/file.h>
#include "area.h"

/**
   Creates an area object. Loads the map file for this area.
**/
Area::Area(
  const unsigned32 nID,
  const char* pszName,
  const unsigned32 nMin,
  const unsigned32 nMax)
  : m_nID(nID),
    m_sName(pszName),
    m_nMinRoomNum(nMin),
    m_nMaxRoomNum(nMax)
{
}

Area::~Area()
{
}

/**
   Checks if a room number is within the range of rooms that defines this
   area. A room is in range if the room id falls between

     >= m_nMinRoomNum && <= m_nMaxRoomNum

   \param iRoomID Room number you are checking.

   \return true if iRoomID is within range, false otherwise.
**/
bool
Area::isRoomInArea(const unsigned32 nRoomNum)
{
  if(nRoomNum >= m_nMinRoomNum && nRoomNum <= m_nMaxRoomNum)
    return true;
  return false;
}
