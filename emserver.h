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
       filename :  emserver.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef EMSERVER_H
#define EMSERVER_H

#include <ssobjects/ssobjects.h>
#include <ssobjects/socketinstance.h>
//#include <ssobjects/linkedlist.h>
#include <ssobjects/telnetserver.h>
#include <ssobjects/telnetserversocket.h>

#include "world.h"
//#include "scriptcmd.h"

using namespace ssobjects;

//class MudObject;
//class Room;
class Player;
//class ScriptCmd;
class World;

#define TIMEOUT (60*60) //60 minutes
#define MAX_USERNAME_LEN    255


/**
   \brief Easye Mud Server

   The server object manages connections (waits for new connections) and data
   coming from the client connections, and will call upon the client handler to
   process the data that was read. It will also handle sending any unsent data.
**/
class EMServer : public TelnetServer
{
  private:
    static unsigned32 m_nUniqueID;
    unsigned32 uniqueID() {return ++m_nUniqueID;};

  protected:
    World               m_world;
    SockAddr            m_saBind;
    SocketInstance      m_sockListen;

  public:
    EMServer(SockAddr saBind);
    ~EMServer() {};

  public:
    SockAddr    sockAddress() {return m_saBind;}

  protected:
    virtual void processSingleMsg(PacketMessage* pmsg);
    virtual void idle(unsigned32);

    void processNewConnection(PacketMessage* pmsg);
    void processClosed(PacketMessage* pmsg);
    void processFullLine(PacketMessage* pmsg);
    void processTextCmd(PacketMessage* pmsg);
};

#endif //EMSERVER_H
