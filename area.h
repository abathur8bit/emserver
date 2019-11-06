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
       filename :  area.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef AREA_H
#define AREA_H

#include <ssobjects/str.h>

using namespace ssobjects;

/**
   \brief Area object that contains area map information.

   The area object is simply a container that holds the area name, and the
   range of rooms (min & max) allowed in this area.
**/
class Area
{
  public:
  protected:
    unsigned32 m_nID;                ///< Numeric identifier of this area.
    Str m_sName;                 ///< Name of this area.
    Str m_sMapFilename;          ///< Filename that map data is in.
    unsigned32 m_nMinRoomNum;       ///< Lowest room number.
    unsigned32 m_nMaxRoomNum;       ///< Highest room number.

  public:
    //construction
    Area(
      const unsigned32 nID,
      const char* pszName,
      const unsigned32 nMin,
      const unsigned32 nMax);
    virtual ~Area();

  public:
    bool isRoomInArea(const unsigned32 nRoomNum);   ///< Check if a room is in an area.

    /// Id assigned to this area.
    unsigned32 id() {return m_nID;}

    /// Return the name of this area.
    CONSTCHAR* name() {return m_sName;}

    /// Min possible room number in this area.
    unsigned32 minRoomNum() {return m_nMinRoomNum;}

    /// Max possible room number in this area.
    unsigned32 maxRoomNum() {return m_nMaxRoomNum;}
};

#endif //AREA_H
