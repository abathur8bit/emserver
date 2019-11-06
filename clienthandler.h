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
       filename :  clienthandler.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/


#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

class EMScript;
class Player;
class World;
class Room;
class MudObject;

/**
   \brief Client handler processes user commands

   The client handler is call upon to process any commands sent from the
   user. It will call upon the EMScript object to run scripts. This is where the
   processCmd function is located.
**/

class ClientHandler
{
  protected:
    World&      m_world;    ///< The world object that contains game information.
    EMScript*   m_pScript;  ///< The script we are running.
    bool        m_bLogin;   ///< Currently logging in.

  public:
    ClientHandler(World& w);
    ~ClientHandler();

    //methods
    void    login(Player* pPlayer);
    bool    processCmd(Player* pPlayer,const char* pszString);
    static void saveUser(Player* pPlayer);

  protected:
    bool    runScript(Room* pRoom,Player* pPlayer,MudObject* pItem,const char* pszPathname,const char* pszSub);
};

#endif //CLIENTHANDLER_H
