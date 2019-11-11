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
       filename :  world.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#include <ssobjects/logs.h>

//#include <string>
//#include <ssobjects/ssobjects.h>
//#include <ssobjects/file.h>


#include "configparser.h"
#include "stringtokenizer.h"
#include "room.h"
#include "player.h"
#include "emscript.h"
#include "scriptcmd.h"
#include "mudobject.h"
#include "area.h"
#include "world.h"

#include <string>
//#include <list>

using namespace ssobjects;
using namespace std;

World::World()
        : m_listRooms(),m_listCmds(),m_listPlayers(),m_listAreas()
{
  initRooms();
  readCommands();
  readExtraWords();
}


World::~World()
{
  m_listAreas.clear();  // erase all area objects
}


void World::initRooms()
{
  try
  {
    const char* areasIni = "scripts/areas.ini";
    LOG("loading ini file [%s]",areasIni);
    ConfigParser areaINI(areasIni);
    char buf[80];
    int roomID,objectID;
    char name[80];
    Room* pRoom;
    MudObject* pObject;

    areaINI.getString("areas","arealist","",buf,sizeof buf);
    printf("area list=[%s]\n",buf);

    int iRoomMin,iRoomMax;
    Area* a;
    const char* pszSection;
    int areaID;
    char *brkt;
    const char *sep = ",";

    for(pszSection=strtok_r(buf,sep,&brkt); pszSection; pszSection=strtok_r(NULL,sep,&brkt))
      //while((pszSection = areaTokens.nextToken()))
    {
      LOG("debug: looking for area [%s]",pszSection);

      areaINI.getNumber(pszSection,"id",0,&areaID);
      areaINI.getString(pszSection,"name","No Name",name,sizeof name);
      areaINI.getNumber(pszSection,"roommin",0,&iRoomMin);
      areaINI.getNumber(pszSection,"roommax",0,&iRoomMax);

      try
      {
        //TODO: make sure that the area id isn't already in our list
        a = new Area(areaID,name,iRoomMin,iRoomMax);
        m_listAreas.push_back(a);
      }
      catch(GeneralException& e)
      {
        LOG("warning: %s - Skipping area [%s]",e.getErrorMsg(),name);
      }
    }

    for(list<Area*>::iterator it=m_listAreas.begin(); it!=m_listAreas.end(); it++)
    {
      a = (Area*)*it;
      // create the rooms for this area
      for(int i=a->minRoomNum(); i < (int)a->maxRoomNum(); i++)
      {
        sprintf(buf,"scripts/rooms/room%d.b",i);
        try
        {
          // if construction of file ob succeeds, the file exists,
          // otherwise, it doesn't and we should not try running it's script.
          File f(buf);

          pRoom = new Room(i,buf);
          try
          {
            runScript(pRoom,NULL,buf,"onInit");
          }
          catch(SmallBasicException&) {}   // script has no onInit sub
          m_listRooms.push_back(pRoom);
          DLOG("debug: Added room %d",i);
        }
        catch(FileException&) {}
      }
    }
    LOG("debug: Done looking for areas");

    const char* objectsIni = "scripts/objects.ini";
    LOG("Loading ini file [%s]",objectsIni);

//    File f(objectsIni);
    FILE* f = fopen(objectsIni,"r");

    while(!feof(f))
    {
      // set to error defaults in case there is not enough params in object statement.
      roomID = 1;
      objectID = -1;
      strcpy(name,"nothing");


      LOG("debug: trying to read a line");
//      f.fgetsTrimNL(buf,sizeof buf);
      if(NULL == fgets(buf,sizeof buf,f))
        LOG("debug: fgets returned NULL");
      else
      {
        LOG("debug: read [%s]",buf);
        if('#' != buf[0])
        {
          sscanf(buf,"%d %d %s",&roomID,&objectID,name);
          pObject = new MudObject(objectID,name);
          pRoom = findRoom(roomID);
          if(pRoom)
          {
            try
            {
              runScript(NULL,pObject,FmtString("scripts/objects/object%04d.b",objectID),"onInit");
            } catch(SmallBasicException&) {}
            pRoom->addItem(pObject);
          }
          else
          {
            LOG("warning: Object %d:[%s] has an incorrect room id of %d. Object removed.",objectID,name,roomID);
            DELETE_NULL(pObject);   // the object has an incorrect room id
          }
        }
      }
    }
  }
  catch(GeneralException& e)
  {
    LOG("File error [%s]\n",e.getErrorMsg());
  }
}

// script types - what type of script to run

void World::removeScriptCmds()
{
  m_listCmds.erase(m_listCmds.begin(),m_listCmds.end());
}



void World::readCommands()
{
//  AutoLock l(m_lockCommandList);
  ScriptCmd* pCmd;

  removeScriptCmds();

  try
  {
    File f("scripts/commands.ini","r");
    char buf[1024];
    char cmd[80],handler[80],description[80],type[10];

    while(!feof((FILE*)f))
    {
      f.fgetsTrimNL(buf,sizeof buf);
      string parsebuf(buf);
      sscanf(buf,"%s %s %s",cmd,handler,type);
      int start;
      int end;

      memset(description,0,sizeof description);
      start = parsebuf.find_first_of('\"');
      if(-1!=start)
        end = parsebuf.find_last_of('\"');
      if(-1!=start && -1!=end)
      {
        string desc = parsebuf.substr(start+1,end-1-start+1);
        strcpy(description,desc.c_str());
      }

      if(!strcasecmp(type,"room"))
        pCmd = new ScriptCmd(cmd,handler,description,ScriptCmd::ST_ROOM);
      else if(!strcasecmp(type,"player"))
        pCmd = new ScriptCmd(cmd,handler,description,ScriptCmd::ST_PLAYER);
      else if(!strcasecmp(type,"object"))
        pCmd = new ScriptCmd(cmd,handler,description,ScriptCmd::ST_OBJECT);

      m_listCmds.push_back(*pCmd);
      DELETE_NULL(pCmd);
    }
  }
  catch(FileException& e) {puts(e.getErrorMsg());}
}

/**
    Searches our room list for the room that has an ID matches the room number
    you pass in.

    \param iRoomNum Room ID you are searching for.

    \return Pointer to the room object, or NULL if not found.
**/
Room*
World::findRoom(const int iRoomID)
{
//  AutoLock l(m_lockRoomList);
  list<Room*>::iterator it;
  for(it = m_listRooms.begin(); it != m_listRooms.end(); it++)
  {
    if((*it)->roomNum() == iRoomID)
    {
      return *it;
    }
  }
  return NULL;
}


void
World::runScript(Room* pRoom,MudObject* pObject,const char* pszPathname,const char* pszSub)
{
  EMScript script(*this,pRoom,NULL,pObject,pszPathname);
  script.run(pszSub);
}


/**
   Attempts to find a the command in our command list, and fills the passed in
   script command object with the information from the found object. If the
   command is not found, pFoundCmd is unchanged.

   \param pFoundCmd This is filled with the information from the command we
   find. Pass NULL if you don't want the information returned; as in the case of you
   just want to check for the presents of a command.

   \param pszLookup Command you are looking up.

   \return true if the command was found, false otherwise.
**/
bool
World::lookupCmd(ScriptCmd* pFoundCmd,const char* pszLookup)
{
//  AutoLock l(m_lock);

  printf("Looking up command %s\n",pszLookup);
  list<ScriptCmd>::iterator it;
  for(it=m_listCmds.begin(); it!=m_listCmds.end(); it++)
  {
    if(!strcasecmp((*it).cmd(),pszLookup))
    {
      if(pFoundCmd)
        *pFoundCmd = *it;   // copy the contents
      return true;
    }
  }
  return false;
}

bool
World::lookupWord(ScriptCmd* pFoundCmd,const char* pszLookup)
{
//  AutoLock l(m_lock);
  list<ScriptCmd>::iterator it;
  for(it=m_listExtraWords.begin(); it!=m_listExtraWords.end(); it++)
  {
    if(!strcasecmp((*it).cmd(),pszLookup))
    {
      if(pFoundCmd)
        *pFoundCmd = *it;   // copy the contents
      return true;
    }
  }
  return false;
}

void
World::removeWords()
{
  m_listExtraWords.erase(m_listExtraWords.begin(),m_listExtraWords.end());
}


void
World::readExtraWords()
{
//  AutoLock l(m_lock);
  removeWords();
  try
  {
    char buf[256];
    char cmd[80],handler[80],type[10];
    ScriptCmd* pCmd=NULL;
    File f("scripts/extrawords.ini","r");
    while(!feof((FILE*)f))
    {
      f.fgetsTrimNL(buf,sizeof buf);
      sscanf(buf,"%s %s %s",cmd,handler,type);
      if(!strcasecmp(type,"room"))
        pCmd = new ScriptCmd(cmd,handler,"",ScriptCmd::ST_ROOM);
      else if(!strcasecmp(type,"player"))
        pCmd = new ScriptCmd(cmd,handler,"",ScriptCmd::ST_PLAYER);
      else if(!strcasecmp(type,"object"))
        pCmd = new ScriptCmd(cmd,handler,"",ScriptCmd::ST_OBJECT);
      m_listExtraWords.push_back(*pCmd);
      DELETE_NULL(pCmd);
    }
  }
  catch(GeneralException&) {printf("read %u words\n",m_listExtraWords.size());}
}

void
World::showPlayers(int roomId)
{
  for(list<Player*>::iterator it=m_listPlayers.begin(); it!=m_listPlayers.end(); it++)
  {
    Player* pPlayer = *it;
    printf("Player id %d name %s\n",pPlayer->id(),pPlayer->name());
  }
}

/**
 * Copys the player names from the specified room to the specified string list.
 * The list is thread safe, and the string objects are all safe to go out of scope.
 *
 * @param nameList Recieves the names found.
 * @param roomId Id of the room to get players from.
 * @param skipPlayerId Player id that should not be added to the list of names.
 * @return
 */
list<string>
World::getPlayerNames(list<string>& nameList,int roomId,int skipPlayerId)
{
  for(list<Player*>::iterator it=m_listPlayers.begin(); it!=m_listPlayers.end(); it++)
  {
    Player* pPlayer = *it;
    if(pPlayer->roomNum() == roomId && pPlayer->id() != skipPlayerId) {
      string name(pPlayer->name());
      nameList.push_back(name);
      printf("Player id %d name %s\n", pPlayer->id(), pPlayer->name());
    }
  }
  return nameList;
}

int
World::numPlayersInRoom(int id) {
  int count = 0;
  for(list<Player*>::iterator it=m_listPlayers.begin(); it!=m_listPlayers.end(); it++)
  {
    Player* pPlayer = *it;
    if(pPlayer->roomNum() == id) {
      printf("Player id %d name %s\n", pPlayer->id(), pPlayer->name());
      count++;
    }
  }
  return count;
}

void
World::runScriptForRoom(Room* pRoom,const char* pszPathname,const char* pszSub,Player* pPlayerAnnounced) {
  for(list<Player*>::iterator it=m_listPlayers.begin(); it!=m_listPlayers.end(); it++)
  {
    Player* pPlayer = *it;
    if(pPlayer->room() == pRoom && pPlayer != pPlayerAnnounced) {
//      printf("Running script [%s:%s] for Player id %d name %s\n", pszPathname, pszSub, pPlayer->id(), pPlayer->name());
      EMScript script(*this,pRoom,pPlayer,NULL,pszPathname);
      Variable* v = script.createNewVariable("announceName");
      v->setStringValue(pPlayerAnnounced->name());
      script.run(pszSub);
//      printf("Running DONE for script [%s:%s] for Player id %d name %s\n", pszPathname, pszSub, pPlayer->id(), pPlayer->name());
    }
  }
}

int
World::copyCmdList(list<ScriptCmd>* pcommands)
{
//  AutoLock l(m_lockPlayerList,AutoLock::READ);
  *pcommands = m_listCmds;
  return m_listCmds.size();
}


void
World::addPlayer(Player* p)
{
  //AutoLock l(m_lockPlayerList);
  m_listPlayers.push_back(p);
#ifdef DEBUG
  printf("world: added player with socket %d\n",p->socketID());
#endif
}


/**
   Copies the player list. It should be noted that although a copy is made
   of the list, the player object still may go out of scope if a player logs
   off. This will be fixed in the future.

   \param plistCopyTo The list you want to copy the player list into.
**/
//TODO: make a safe copy, as in if a player object goes out of scope
void
World::copyPlayerList(list<Player*>* plistCopyTo)
{
  //AutoLock l(m_lockPlayerList);//TODO: thread issue to resolve
  *plistCopyTo = m_listPlayers;
}


/**
   Finds the specifiect player, and puts a read lock on the player list.
   Once you are finished with the player, you should make a call to
   releasePlayerLock() to release the lock.

   \param iSocket The socket of the player you are looking up.

   \return Pointer to the player object, or NULL if not found.
**/
Player*
World::findPlayer(const int iSocket)
{
  m_lockPlayerList.rlock();
  for(list<Player*>::iterator it=m_listPlayers.begin(); it!=m_listPlayers.end(); it++)
  {
    if(iSocket == (*it)->socketID())
      return *it;
  }

  // did not find the player
  m_lockPlayerList.runlock();
  return NULL;
}


/**
   Removes the player from the player list. Does not delete the player object.

   \param socket The socket in the player object to search for.

   \return The player object that was removed from the player list.
**/
Player*
World::removePlayer(const int iSocket)
{
  //AutoLock l(m_lockPlayerList);//TODO: thread issue to resolve

  for(list<Player*>::iterator it=m_listPlayers.begin(); it!=m_listPlayers.end(); it++)
  {
    printf("world: socket %p\n",(*it)->socket());
    if(iSocket == (*it)->socketID())
    {
      Player* p = *it;
      m_listPlayers.erase(it);
      return p;
    }
  }

  // did not find the player
#ifdef DEBUG
  printf("world: Did not find player with socket %d\n",iSocket);
#endif
  return NULL;
}


void
World::releasePlayerLock()
{
  m_lockPlayerList.runlock();
}



/**
   Checks all areas to see if roomID is in the area's range. If it is,
   we return the area id.

   \param iRoomID The room number to check.

   \return The area number iRoomID belongs to, or 0 if none match.
**/
int
World::findRoomsArea(const int iRoomID)
{
  AutoLock l(m_lockAreaList);
  Area* a;
  for(list<Area*>::iterator it=m_listAreas.begin(); it!=m_listAreas.end(); it++)
  {
    a = (Area*)*it;
    if(a->isRoomInArea(iRoomID))
      return a->id();
  }
  return 0;
}


/**
   Searchs for an area that matches the specified area id.

   \param iAreaID The area number to look for.

   \return The a pointer to the area object, NULL if area id not found.
**/
Area*
World::findArea(const int iAreaID)
{
  AutoLock l(m_lockAreaList);
  Area* a;
  for(list<Area*>::iterator it=m_listAreas.begin(); it!=m_listAreas.end(); it++)
  {
    a = (Area*)*it;
    if(a->id() == (unsigned32)iAreaID)
      return a;
  }
  return NULL;
}
