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
       filename :  emserver.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

       Player list is handled in the world object. When we are
       processing a script, the list is read locked for the
       duration of the script. This is because we need only
       reference type information from the list.

       When the connection is closed, a write lock is put on the
       player list by the world object (presumably from a
       removeUser or addUser method) and the player list is
       updated. This ensures that any script that is running that
       has a reference to a player object will not have it
       deleted while it is working with it.

*********************************************************************/

//#include "mudobject.h"
#include "player.h"
//#include "room.h"
//#include "smallbasic.h"
//#include "clienthandler.h"
//#include "emscript.h"
#include "world.h"
#include "emserver.h"
//#include "area.h"
//#include "empacket.h"

//#include <list>

//using namespace std;
//using namespace ssobjects;

unsigned32 EMServer::m_nUniqueID = 1000;

EMServer::EMServer(SockAddr saBind)
  : TelnetServer(saBind,30),
    m_world(),
    m_sockListen()
{
}


void
EMServer::idle(unsigned32)
{
}


void
EMServer::processSingleMsg(PacketMessage* pmsg)
{
  TelnetServerSocket* psocket = (TelnetServerSocket*)pmsg->socket();
  PacketBuffer* ppacket = pmsg->packet();

  switch(ppacket->getCmd())
  {
    case PacketBuffer::pcNewConnection:
      printf("New connection from %s on socket %d\n",psocket->getAddr().dottedDecimal(),psocket->m_hSocket);
      processNewConnection(pmsg);
      break;

    case PacketBuffer::pcClosed:
      processClosed(pmsg);
      break;

//    case EMPacket::pcTextCmd:
//      processTextCmd(pmsg);
//      break;

  case TelnetServerSocket::pcFullLine:
    processFullLine(pmsg);
    break;
  }
}


void
EMServer::processNewConnection(PacketMessage* pmsg)
{
  TelnetServerSocket* psocket = (TelnetServerSocket*)pmsg->socket();
  Player* pPlayer = new Player(psocket,m_world);
  m_world.addPlayer(pPlayer);
  pPlayer->login();
}

void
EMServer::processClosed(PacketMessage* pmsg)
{
  PacketBuffer* ppacket = pmsg->packet();
  signed32 iSocket;
  char szIP[255];

  *ppacket >> iSocket;
  *ppacket >> szIP;

  printf("Connection from %s on socket %lu closed\n",szIP,iSocket);
  Player* pPlayer = m_world.removePlayer(iSocket);
  if(pPlayer)
  {
    pPlayer->saveUser();

    delete pPlayer;
  }
}


/**
   This will become the main loop that reads data, finda a client handler and
   calls the client handlers processCmd.
**/
void
EMServer::processFullLine(PacketMessage* pmsg)
{
  TelnetServerSocket* psocket = (TelnetServerSocket*)pmsg->socket();
  Player* pPlayer = m_world.findPlayer(psocket->m_hSocket);     // find and read lock player list
  if(pPlayer)
  {
    char* pszString = (char*)pmsg->packet()->getBuffer();
    try
    {
      pPlayer->processCmd(pszString);
    }
    catch(SocketInstanceException& e)
    {
      printf("Socket error %s\n",e.getErrorMsg());
      removeSocket(psocket);
    }
    catch(GeneralException& e)
    {
      psocket->sendString("Script Error. Aborting.\r\n");
      printf("Script Error %s\n",e.getFullErrorMsg());
    }
    m_world.releasePlayerLock();
  }
}

/**
   Client sent a text command. Parse and process it.
**/
void
EMServer::processTextCmd(PacketMessage* pmsg)
{
  UNUSED(pmsg);
  BREAK;
  //  TelnetServerSocket* psocket = (TelnetServerSocket*)pmsg->socket();
  //Player* pPlayer = m_world.findPlayer(psocket->m_hSocket);     // find and read lock player list

  //if(pPlayer)
  //{
  //  char* pszString = (char*)pmsg->packet()->getBuffer();
  //  printf("buffer read=%s\n",pszString);
  //  try
  //  {
  //   pPlayer->processCmd(pszString);
  //  }
  // catch(SocketInstanceException& e)
  // {
  //   printf("Socket error %s\n",e.getErrorMsg());
  //   removeSocket(psocket);
  //  }
  //  catch(GeneralException& e)
  //  {
  //  char* szError = "Script Error. Aborting.\r\n";
  //  //psocket->print(szError);
  //  PacketBuffer packet(EMPacket::pcTextCmd);
  //  packet << szError;
  //  psocket->sendPacket(packet);
  //  printf("Script Error %s\n",e.getErrorMsg());
  //}
  //m_world.releasePlayerLock();
  //}
}
