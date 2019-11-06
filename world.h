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
       filename :  world.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef WORLD_H
#define WORLD_H

#include <list>
#include <string>

#include <ssobjects/threadutils.h>

#include "scriptcmd.h"

using namespace std;
using namespace ssobjects;

class MudObject;
class Room;
class Player;
class ScriptCmd;
class Area;

/**
   \brief World container that holds master list of rooms, objects, and commands.

   The world object contains everything that has to do with the game; in other
   words, it contains all the game information. Other processes like the command
   interpreter and script interpreter will use things from the world. For
   instance, if a script needed to list all the available commands to a user, it
   would get this list from the world object.

   The world object will manage it's resources such as read and write locks on
   lists. For examine, when the script needs to have commands re-read, the world
   object will put a write lock on the list, and re-read it from storage.

   There is a lot of search methods. There is room for a MUCH improved search
   algorithem. An indexed method like that used in dbases would be awsome. For
   now, be sure that all searching is kept to this object, and it will be easier
   to impliment better search functionality.
**/

//TODO: thread safe the room and command lists

class World
{
  protected:
    //attibutes
    RWLock              m_lockPlayerList;                   ///< Keep things synchornized.
    Lock                m_lockCommandList;                  ///< Keep things synchornized.
    Lock                m_lockRoomList;                     ///< Keep things synchornized.
    Lock                m_lockAreaList;                     ///< Keep things synchornized.
    list<Room*>         m_listRooms;
    list<ScriptCmd>     m_listCmds;
    list<ScriptCmd>     m_listExtraWords;
    list<Player*>       m_listPlayers;
    list<Area*>         m_listAreas;                        ///< Area list.

  public:
    World();
    virtual ~World();

    //methods
    Room*   findRoom(const int iRoomID);                                ///< Searches our room list for the specifed room id.
    int     findRoomsArea(const int iRoomID);                           ///< Look for an area that has a range that room id falls in.
    Area*   findArea(const int iAreaID);                                ///< Look for an area that has a range that room id falls in.

    bool    lookupCmd(ScriptCmd* pFoundCmd,const char* pszLookup);      ///< Searches for pszLookup in our command list.
    bool    lookupWord(ScriptCmd* pFoundCmd,const char* pszLookup);     ///< Searches for pszLookup in our extra word list.
    int     copyCmdList(list<ScriptCmd>* pcommands);

    void    readCommands();                                 ///< Read or re-read our list of commands from storage.
    void    readExtraWords();                               ///< Read or re-read our list of extra words.

    void    addPlayer(Player* pPlayer);                     ///< Adds a player to the list of known players in the world.
    void    copyPlayerList(list<Player*>* listCopyTo);      ///< Copy the player list.
    Player* findPlayer(const int iSocket);                  ///< find a player based on his socket id and puts a read lock on the player list lock.
    void    releasePlayerLock();                            ///< Releases a read lock on the player list that was put on with findPlayer.
    Player* removePlayer(const int iSocket);                ///< Remove a player from the player list.
    list<string> getPlayerNames(list<string>& nameList,int roomId,int skipPlayerId);  ///< Copy the names of all players in the specified room to the specified list.
    int     numPlayersInRoom(int id);   // TODO object count needs to use this
    void    runScriptForRoom(Room* pRoom,const char* pszPathname,const char* pszSub,Player* pPlayerAnnounced);

    void showPlayers(int roomId);   // TODO lee xxx

  protected:
    void initRooms();
    void removeScriptCmds();
    void removeWords();
    void runScript(Room* pRoom,MudObject* pObject,const char* pszPathname,const char* pszSub);
};

#endif //WORLD_H
