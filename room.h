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
       filename :  room.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef ROOM_H
#define ROOM_H

#include <list>

#include "mudobject.h"
#include "player.h"

class MudObject;
class Player;

class Room : public MudObject
{
  protected:
    Variable* m_pvRoomNum;
    list<Player*> m_listPlayers;    ///< Players in this room.

  public:       // construction
    Room(const int iRoomNumber,const char* pszName);

  public:       // access methods
    int roomNum();             ///< Return the id of this room.
    void setRoomNum(const int iRoomID);     ///< Set this rooms id.
    //TODO: add ability to access the list of players
    int numPlayers() {m_listPlayers.size();}

};

#endif //ROOM_H
