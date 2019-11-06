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
       filename :  player.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/


#ifndef PLAYER_H
#define PLAYER_H

#include <ssobjects/ssobjects.h>
#include <ssobjects/socketinstance.h>
#include <ssobjects/telnetserversocket.h>
#include <ssobjects/threadutils.h>

#include "mudobject.h"
#include "room.h"
#include "variable.h"
#include "clienthandler.h"

using namespace ssobjects;

//class TelnetSocket;
class World;
class MudObject;
class Room;
class Player;
class ClientHandler;
class EMScript;


/**
   \brief Player contains information on a user.

   Player object contains player vars, and is the connection
   to the users socket.

   A players name is the players handle. This is accessed via the name()
   method of the MudObject. The username is the players login name. It is
   separate so for security; if a user was able to guess the username,
   it would be that much easier to guess the username/password combination
   as you are already half way there.
**/

//TODO: There should be a user name and password, as well as a handle.
//TODO  The handle should be changable, but the username should not be.
//TODO  This will protect from saving over a different user. Password
//TODO  should be changable however to allow the player to change his
//TODO  when he wants.

class Player : public MudObject
{
    friend class ClientHandler;
    friend class EMScript;

  protected:
    // attributes
    TelnetServerSocket* m_psocket;
    Room*       m_pCurrentRoom;         ///< Pointer to current room for easy access.
    ClientHandler m_clientHandler;      ///< Client handler
    int         m_iSocketID;            ///< Kept for record keeping incase socket is closed.

    // Default script variables that are created upon object creation
    Variable*   m_pvUserName;           ///< User name string.
    Variable*   m_pvPassword;           ///< Password string.
    Variable*   m_pvHealth;             ///< Health points integer.
    Variable*   m_pvCredits;            ///< Player worth, gold. Integer.
    Variable*   m_pvLoginCount;         ///< How many times this player has logged in. Integer.
    Variable*   m_pvRoomNum;            ///< The room id of the room the player is currently in. Integer.
    Variable*   m_pvAreaNum;            ///< The area id that a room number belongs in.
    Variable*   m_pvCarryPoints;        ///< How much the player can carry. Integer.
    Variable*   m_pvWord;               ///< Just the word used.
    Variable*   m_pvVerb;               ///< Just the verb of the command.
    Variable*   m_pvCommand;            ///< The entire commmand that the user typed in.

  public:
    // construction
    Player(TelnetServerSocket* psocket,World& w);
    ~Player();

  public:
    // access methods
    int         socketID() const {return m_iSocketID;}
    TelnetServerSocket* socket() {return m_psocket;}
    void        setSocket(TelnetServerSocket* psocket);
    Room*       room();
    int         roomNum();
    void        setRoomNum(const int iRoomID);      // set the script variable for room number.
    void        setRoom(Room* pRoom);
    CONSTCHAR*  userName();

    //maze releated
    int         areaNum();                           ///< Returns the current area number that the players current room belongs to.
    void        setAreaNum(const int id);            ///< Set the current tile id the player is standing on.

    void setWord(const char* newVal) {m_pvWord->setStringValue(newVal);};
    void setVerb(const char* newVal) {m_pvVerb->setStringValue(newVal);};
    void setCommand(const char* newVal) {m_pvCommand->setStringValue(newVal);};

    int         outputString(const char* pszString);     ///< Adds data to the outgoing buffer.
    bool        processCmd(const char* pszString);
    void        login();
    void        saveUser();
};

#endif  //PLAYER_H
