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
       filename :  emscript.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef EMSCRIPT_H
#define EMSCRIPT_H

#include <list>
#include <string>
#include "smallbasic.h"
#include "emserver.h"   // needed for timeout define

using namespace std;

class Room;
class Player;
class MudMObject;
class World;
/**
   \brief Easye Mud script engine.

    Basic like script engine that is customized for easye mud. Impliemnts many
    new commands specific to EM that extends the basic script engine. New commands are
    added in the constructor. Once added a script can then call the new command just
    like built in commands like for and gosub.

    For example, if the following line were in the constructor:

      m_loadUser = addCommand("loaduser");

    then the player.b script file could have a new command to call:
      ...

      getlogin:
        input "Login: ";pv.username$
        loadUser found,pv.username$

      ...
**/
class EMScript : public SmallBasic
{
  protected:
    World&      m_world;            ///< The world object that contains game information.
    Room*       m_pRoom;            ///< The current room associated with the current script.
    Player*     m_pPlayer;          ///< The player that is assiciated with the current script.
    MudObject*  m_pMudObject;       ///< The object that is assiciated with the current script.
    bool        m_bContinueInput;   ///< You are in the middle of an input operation.
    string      m_scriptPath;       ///< Filename path of this script

    // command tokens
    int m_removeItem;
    int m_addItem;
    int m_showItems;
    int m_pickupItem;
    int m_showPlayersInventory;
    int m_checkInventory;
    int m_dropItem;
    int m_help;
    int m_reload;
    int m_setObject;
    int m_setRoom;
    int m_loadUser;
    int m_exit;
    int m_showPlayers;
    int m_countRoomItems;
    int m_showFunctions;
    int m_announcePlayer;

  public:
    EMScript(
      World& world,
      Room* pRoom,
      Player* pclient,
      MudObject* pObject,
      const char* pszScript);    ///< Construct an instance of the interpreter
    virtual ~EMScript() {};

    virtual bool run(const char* pszStartSubroutine);       ///< Start running the script.
    bool continueInput(const char* pszString);            ///< Continue running a script that had been stopped at input.
    virtual Variable* createNewVariable(const char* token);

  protected:
    virtual bool exec_command();
    virtual bool input(const char* pszString);

    virtual int outputString(const char* pszOutput);
    virtual void outputNewLine();
//    virtual void inputNumber(int* i);
//    virtual void inputString(char* s,const unsigned32 n);
    virtual bool execAddonCommand(const int tok);
    virtual Variable* findVariableName(const char* token,list<Variable*>& searchList);
    const char* scriptPath() {return m_scriptPath.c_str();}
    void loadUser();
    void saveUser();
    void setObject();
    void setRoom();
    void help();
    void removeItem() {};
    void addItem() {};
    void showItems();
    void showPlayersInventory();
    void checkInventory();
    void countRoomItems();
    int  countVisibleRoomItems(Room* pRoom);
    void pickupItem();
    void dropItem();
    void reload();
    void showPlayers();
    void showFunctions();
    void announcePlayer();
    void runScript(Room* pRoom,Player* pPlayer,MudObject* pItem,const char* pszPathname,const char* pszSub);
};

#endif //EMSCRIPT_H
