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
       filename :  player.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#include <ssobjects/logs.h>
#include <ssobjects/telnetserversocket.h>

#include "empacket.h"
#include "clienthandler.h"
#include "world.h"
#include "player.h"

Player::Player(TelnetServerSocket* psocket,World& w)
  : MudObject(uniqueID()),
    m_psocket(psocket),
    m_pCurrentRoom(NULL),
    m_clientHandler(w),
    m_iSocketID(psocket->m_hSocket)
{
  // create the vars
  m_pvUserName = new Variable("username$",Variable::TYPE_STRING);
  m_pvPassword = new Variable("password$",Variable::TYPE_STRING);
  m_pvWord = new Variable("word$",Variable::TYPE_STRING);
  m_pvVerb = new Variable("verb$",Variable::TYPE_STRING);
  m_pvCommand = new Variable("command$",Variable::TYPE_STRING);
  m_pvRoomNum = new Variable("roomnum",Variable::TYPE_INT);
  m_pvAreaNum = new Variable("areanum",Variable::TYPE_INT);
  m_pvHealth = new Variable("health",Variable::TYPE_INT);
  m_pvCredits = new Variable("credits",Variable::TYPE_INT);
  m_pvCarryPoints = new Variable("carrypoints",Variable::TYPE_INT);
  m_pvLoginCount = new Variable("logincount",Variable::TYPE_INT);

  // set the default values (some have 0 or empty string as default value)
  m_pvCarryPoints->setIntValue(1);

  //add to the variables to our variable list.
  m_listVariables.push_back(m_pvUserName);
  m_listVariables.push_back(m_pvPassword);
  m_listVariables.push_back(m_pvRoomNum);
  m_listVariables.push_back(m_pvAreaNum);
  m_listVariables.push_back(m_pvHealth);
  m_listVariables.push_back(m_pvCredits);
  m_listVariables.push_back(m_pvCarryPoints);
  m_listVariables.push_back(m_pvLoginCount);
  m_listVariables.push_back(m_pvWord);
  m_listVariables.push_back(m_pvVerb);
  m_listVariables.push_back(m_pvCommand);
}


Player::~Player()
{
  FUNCLOG("Player::~Player()");
}


void
Player::setRoom(Room* pRoom)
{
  AutoLock l(m_lock);
  m_pCurrentRoom = pRoom;
  m_pvRoomNum->setIntValue(pRoom->id());
}


void
Player::setRoomNum(const int iRoomID)
{
  AutoLock l(m_lock);
  m_pvRoomNum->setIntValue(iRoomID);
}


int
Player::roomNum()
{
  AutoLock l(m_lock);
  int i= m_pvRoomNum->intValue();
  return i;
}

Room*
Player::room()
{
  AutoLock l(m_lock);
  Room* r = m_pCurrentRoom;
  return r;
}


const char*
Player::userName()
{
  AutoLock l(m_lock);
  const char* n = m_pvUserName->stringValue();
  return n;
}


void
Player::setSocket(TelnetServerSocket* psocket)
{
  AutoLock l(m_lock);
  m_psocket = psocket;
}


int
Player::outputString(const char* pszString)
{
  AutoLock l(m_lock);
  if(m_psocket)
  {
    return m_psocket->sendString(pszString);
  }
  return 0;
}


bool
Player::processCmd(const char* pszString)
{
  return m_clientHandler.processCmd(this,pszString);
}


void
Player::login()
{
  m_clientHandler.login(this);
}


void
Player::saveUser()
{
  m_clientHandler.saveUser(this);
}

int
Player::areaNum()
{
  AutoLock l(m_lock);
  int t = m_pvAreaNum->intValue();
  return t;
}

void
Player::setAreaNum(const int id)
{
  AutoLock l(m_lock);
  m_pvAreaNum->setIntValue(id);
}
