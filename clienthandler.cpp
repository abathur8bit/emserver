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
       filename :  clienthandler.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

//#include <list>
#include <ssobjects/logs.h>
#include <ssobjects/cstr.h>

#include "world.h"
#include "player.h"
#include "room.h"
#include "mudobject.h"
#include "scriptcmd.h"
#include "emscript.h"
#include "clienthandler.h"

using namespace ssobjects;

ClientHandler::ClientHandler(World& w) : m_world(w),m_pScript(NULL),m_bLogin(false)
{
  FUNCLOG("ClientHandler::ClientHandler");
}


ClientHandler::~ClientHandler()
{
  FUNCLOG("ClientHandler::~ClientHandler");
  if(m_pScript)
    DELETE_NULL(m_pScript);
}


/**
   Called when the client first connects.
**/
void
ClientHandler::login(Player* pPlayer)
{
  FUNCLOG("ClientHandler::login");
  m_bLogin = true;
  bool bContinueInput = runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onLogin");
  if(!bContinueInput)
  {
    // room was set right away, and not input statements encountered
    // Set the current room, and do the appropreate enter room stuff.
    Room* pNewRoom = m_world.findRoom(pPlayer->roomNum());
    if(!pNewRoom)
      throwGeneralException("Invalid  player room was defined after login.");

    pPlayer->setRoom(pNewRoom);
    runScript(pPlayer->room(),pPlayer,NULL,FmtString("scripts/rooms/room%04d.b",pPlayer->roomNum()),"onEnter");
    m_bLogin = false;
  }
}


/**
   Takes a line of text the player has entered, and parses it out, looks up the
   words the player entered, then runs the appropreate script.

   If m_pScript is already set, then we are continuing a previously running
   script. This happens during an INPUT script command.

   \param pPlayer    Player we are running the command for.
   \param pszString  String the player typed.

   \return true
**/
bool
ClientHandler::processCmd(Player* pPlayer,const char* pszString)
{
  FUNCLOG("ClientHandler::processCmd");
  if(m_pScript)
  {
    // we are continueing a script that was at an input command
    int iCurrentRoomNum;
    int iCurrentAreaNum;
    if(!m_bLogin)
    {
      iCurrentRoomNum = pPlayer->roomNum();
      iCurrentAreaNum = pPlayer->areaNum();
    }
    bool bContinue = m_pScript->continueInput(pszString);
    if(!bContinue)
    {
      DELETE_NULL(m_pScript);
      if(m_bLogin)
      {
        Room* pNewRoom = m_world.findRoom(pPlayer->roomNum());
        int iAreaNum = m_world.findRoomsArea(pPlayer->roomNum());
        if(!pNewRoom)
          throwGeneralException(FmtString("Invalid room number %d during login",pPlayer->roomNum()));
        if(!iAreaNum)
          throwGeneralException(FmtString("No area defined for room number %d during login",pPlayer->roomNum()));
        pPlayer->setRoom(pNewRoom);
        pPlayer->setAreaNum(iAreaNum);
        runScript(pPlayer->room(),pPlayer,NULL,FmtString("scripts/rooms/room%04d.b",pPlayer->roomNum()),"onEnter");
        m_bLogin = false;
      }
      else
      {
        // Check if the player has changed rooms
        if(pPlayer->roomNum() != iCurrentRoomNum)
        {
          Room* pNewRoom = m_world.findRoom(pPlayer->roomNum());
//          int iAreaNum = m_world.findRoomsArea(pPlayer->roomNum());
          if(!pNewRoom)
          {
            pPlayer->setRoomNum(iCurrentRoomNum);   // Set the player back to the old room.
            pPlayer->setAreaNum(iCurrentAreaNum);
            throwGeneralException(FmtString("Invalid room number %d ",pPlayer->roomNum()));
          }
          pPlayer->setRoom(pNewRoom);

          char script[MAX_PATH];
          snprintf(script,sizeof(script)-1,"scripts/rooms/room%04d.b",pPlayer->roomNum());
          NULL_TERMINATE(script,sizeof script);
          runScript(pPlayer->room(),pPlayer,NULL,script,"onEnter");
        }
      }
      runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onShowPrompt");
    }
  }
  else
  {
    if(!strlen(pszString))  // blank lines mean nothing
    {
      runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onShowPrompt");
      return true;
    }

    char verb[20],word[20];
    ScriptCmd scriptCmd;

    memset(verb,0,sizeof verb);
    memset(word,0,sizeof word);
    sscanf(pszString,"%s %s",verb,word);
    pPlayer->setWord(word);
    pPlayer->setVerb(verb);
    pPlayer->setCommand(pszString);

    char script[MAX_PATH] = "no script defined";
    bool bContinueInput = false;
    try
    {
      if(!m_world.lookupCmd(&scriptCmd,verb))
      {
        runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onCmdNotUnderstood");
      }
      else
      {
        int iCurrentRoomNum = pPlayer->roomNum();
        int iCurrentAreaNum = pPlayer->areaNum();
        switch(scriptCmd.type())
        {
          case ScriptCmd::ST_PLAYER:
            bContinueInput = runScript(pPlayer->room(),pPlayer,NULL,"scripts/character/player.b",scriptCmd.handler());
            break;

          case ScriptCmd::ST_ROOM:
            {
              snprintf(script,sizeof(script)-1,"scripts/rooms/room%04d.b",pPlayer->roomNum());
              NULL_TERMINATE(script,sizeof script);
              bContinueInput = runScript(pPlayer->room(),pPlayer,NULL,script,scriptCmd.handler());
            }
            break;

          case ScriptCmd::ST_OBJECT:
            {
              bool bFound=false;
              if(strlen(word))
              {
                // check for object in players inventory, then the room
                MudObject* pObject;
                pObject = pPlayer->lookupItem(word);
                if(!pObject)
                  pObject = pPlayer->room()->lookupItem(word);
                if(pObject)
                {
                  snprintf(script,sizeof(script)-1,"scripts/objects/object%04d.b",pObject->id());
                  NULL_TERMINATE(script,sizeof script);
                  bContinueInput = runScript(pPlayer->room(),pPlayer,pObject,script,scriptCmd.handler());
                  bFound=true;
                }
                if(!bFound && strlen(word))
                {
                  ScriptCmd cmdWord;
                  bFound = m_world.lookupWord(&cmdWord,word);
                  if(bFound)
                  {
                    snprintf(script,sizeof(script)-1,"scripts/objects/%s",cmdWord.handler());
                    NULL_TERMINATE(script,sizeof script);
                    bContinueInput = runScript(pPlayer->room(),pPlayer,NULL,script,scriptCmd.handler());
                  }
                }
                if(!bFound && !strlen(word))
                  runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onCmdNotUnderstood");
                else if(!bFound)
                  runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onObjectNotFound");
              }
            }
            break;
        }

        // Check if the player has changed rooms
        if(pPlayer->roomNum() != iCurrentRoomNum)
        {
          Room* pNewRoom = m_world.findRoom(pPlayer->roomNum());
          int iAreaNum = m_world.findRoomsArea(pPlayer->roomNum());
          if(!pNewRoom || !iAreaNum)
          {
            pPlayer->setRoomNum(iCurrentRoomNum);   // Set the player back to the old room.
            pPlayer->setAreaNum(iCurrentAreaNum);
            throwGeneralException(FmtString("Invalid (room number or area) %d possibly for script [%s]",pPlayer->roomNum(),script));
          }
          pPlayer->setRoom(pNewRoom);
          pPlayer->setAreaNum(iAreaNum);

          snprintf(script,sizeof(script)-1,"scripts/rooms/room%04d.b",pPlayer->roomNum());
          NULL_TERMINATE(script,sizeof script);
          runScript(pPlayer->room(),pPlayer,NULL,script,"onEnter");
        }
      }
      if(!bContinueInput)
      {
        runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onShowPrompt");
      }
    }
    catch(SmallBasicException& e)
    {
      printf("Script error %s possibly in script [%s]\n",e.getErrorMsg(),script);
      runScript(NULL,pPlayer,NULL,"scripts/character/player.b","onCmdNotUnderstood");
    }
    catch(GeneralException& e)
    {
      printf("Script error %s possibly in script [%s]\n",e.getErrorMsg(),script);
      pPlayer->outputString("Fatal scrit error, aborting\r\n");
      return false;
    }
  }
  return true;
}

//TODO: Need to make sure that a script can't change the players user name
void
ClientHandler::saveUser(Player* pPlayer)
{
  try
  {
    CStr sScriptPathName = "users/";
    sScriptPathName += pPlayer->userName();
    sScriptPathName += ".b";
    printf("Opening %s to save player vars\n",(CONSTCHAR*)sScriptPathName);

    File f((CONSTCHAR*)sScriptPathName,"w");
    list<Variable*>::iterator it;
    list<Variable*>& listVars = pPlayer->m_listVariables;
    for(it = listVars.begin(); it!=listVars.end(); it++)
    {
      if((*it)->type() == Variable::TYPE_INT)
        fprintf(f,"pv.%s = %d\n",(*it)->name(),(*it)->intValue());
      else
        fprintf(f,"pv.%s = \"%s\"\n",(*it)->name(),(*it)->stringValue());
    }
  }
  catch(FileException& e)
  {
    printf("Unable to save player vars [%s]\n",e.getErrorMsg());
  }
}


/**
   \return true if input operation needs to continue, false otherwise.

   \throw SocketInstanceException
   \throw SmallBasicException
**/
bool
ClientHandler::runScript(Room* pRoom,Player* pPlayer,MudObject* pItem,const char* pszPathname,const char* pszSub)
{
  m_pScript = new EMScript(m_world,pRoom,pPlayer,pItem,pszPathname);
  bool bContinue = m_pScript->run(pszSub);
  if(!bContinue)
    DELETE_NULL(m_pScript);
  return m_pScript ? true:false;
}
