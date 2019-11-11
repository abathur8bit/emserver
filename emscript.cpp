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
       filename :  emscript.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#include <ssobjects/logs.h>
#include "variable.h"
#include "mudobject.h"
#include "player.h"
#include "room.h"
#include "world.h"
#include "clienthandler.h"
#include "emscript.h"

using namespace ssobjects;

EMScript::EMScript(
        World& world,
        Room* pRoom,
        Player* pclient,
        MudObject* pObject,
        const char* pszScript)
        : SmallBasic(pszScript),
          m_world(world),
          m_pRoom(pRoom),
          m_pPlayer(pclient),
          m_pMudObject(pObject),
          m_bContinueInput(false)
{
    FUNCLOG("EMScript::EMScript");

    //Custom commands added to the basic commands. Scripts can call these commands.
    m_removeItem = addCommand("removeItem");
    m_addItem = addCommand("additem");
    m_showItems = addCommand("showitems");
    m_pickupItem = addCommand("pickupitem");
    m_showPlayersInventory = addCommand("showinventory");
    m_checkInventory = addCommand("checkInventory");
    m_countRoomItems = addCommand("countRoomItems");
    m_dropItem = addCommand("dropitem");
    m_help = addCommand("help");
    m_reload = addCommand("reload");
    m_setObject = addCommand("setobject");
    m_setRoom = addCommand("setroom");
    m_loadUser = addCommand("loaduser");
    m_exit = addCommand("logoutuser");
    m_showPlayers = addCommand("showplayers");
    m_showFunctions = addCommand("showFunctions");
    m_announcePlayer = addCommand("announcePlayer");

//  printf("show functions tok = %d showPlayers = %d\n",m_showFunctions,m_showPlayers);
//
//  showKnownCommands();
}

/**
   Starts running the script from the start of the script, or of you have
   specified a start subroutine, that routine will be called. When input is
   called in the script, the script will pause execution and run will return
   true. This indicates that we need to read a line of text and call the
   continue method with the value of what the user typed. This is done so the
   script isn't stalled and the thread can continue work on other things. Once
   continue has been called, the script will continue finish the input call and
   finish running the rest of the script.

   \param pszStartSubroutine  Label that execution will start at.

   \return true when the script needs to continue, false when it does not.
**/
bool
EMScript::run(const char* pszStartSubroutine)
{
    m_bContinueInput = false;
#ifdef DEBUG
    DLOG("ems: run addr %p",this);
#endif
    printf("EMScript::run [%s:%s]\n",m_szFilename,pszStartSubroutine?pszStartSubroutine:"main");
    if(m_programStart)
    {
        m_program = m_programStart;
        scan_labels();
        ftos = 0;
        gtos = 0;
        source_line = 1;
        if(pszStartSubroutine)
        {
            char* loc = find_label(const_cast<char*>(pszStartSubroutine));
            if(loc)
            {
                m_bStartedSubroutine = true;
                m_program = loc;
            }
            else
                throwSmallBasicException(FmtString("Label [%s] not found",pszStartSubroutine));
        }
        do
        {
            token_type = m_exparse.get_token();
        } while(exec_command());
    }
    return m_bContinueInput;
}

void EMScript::showFunctions() {
    SmallBasic::showKnownCommands();
}

void EMScript::announcePlayer() {
    Variable* v;
    bool arrive = false;
    m_exparse.get_token();
    v = findVariableName(token,m_listVars);
    if(v) {
        if(v->intValue() != 0) {
            arrive = true;
        }
    }
    else {
        if(token[0] != '0') {
          arrive = true;
        }
    }

    m_exparse.get_token();
    if(token[0] == ',' ) {
      m_exparse.get_token();
      v = findVariableName(token,m_listVars);
      printf("variable is %s",v->stringValue());
    }

    m_world.runScriptForRoom(
            m_pPlayer->room(),
            FmtString("scripts/rooms/room%d.b",m_pPlayer->roomNum()),
            arrive ? "onAnnounceArrive":"onAnnounceLeave",
            m_pPlayer);
}

/**
    Called to continue an input operation, and let the rest of the script run.
    When this is called, it is assumed that the script has already been running.

    \param pszString The string the user typed for input.

    \return true if the script should be continued, false otherwise.
**/
bool
EMScript::continueInput(const char* pszString)
{
    assert(m_program);    // was this script run yet?

    input(pszString);
    do
    {
        token_type = m_exparse.get_token();
    } while(exec_command());

    return m_bContinueInput;
}


bool
EMScript::exec_command()
{
    if(token_type == VARIABLE)
    {
        m_exparse.putback();
        assignment();
    }
    else
    {
        switch (tok) {
            case PRINT:
                print();
                break;

            case GOTO:
                exec_goto();
                break;

            case IF:
                exec_if();
                break;

            case FOR:
                exec_for();
                break;

            case NEXT:
                next();
                break;

            case INPUT:
                if (input(NULL)) {
                    // stop the script, as the input operation needs to be continued
                    return false;
                }
                break;

            case GOSUB:
                gosub();
                break;

            case RETURN:
                if (!greturn())
                    return false;
                break;

            case END:
            case SB_FINISHED:
                return false;
                break;

            default:
                if (tok > END)
                    return execAddonCommand(tok);
                break;
        }
    }

    return true;
}


/**
   Sets up for reading from the user. If pszString is supplied, then the input
   operation is continuing, when NULL we are setting up for a input operation.
   The input operation is broken into two calls, to facilitate not blocking the
   current thread while waiting for input from the user.

   \param pszString The value read from the user, or NULL if setting up for first call.

   \return true if we should stop running the script and continue the input operation later, false if not.
**/
bool
EMScript::input(const char* pszString)
{
#ifdef DEBUG
    DLOG("ems: input addr %p string [%s]",this,pszString);
#endif

    if(!pszString)
    {
        // set ourself up for an input operation
        m_exparse.get_token();
        if(token_type == QUOTE)
        {
            outputStringf("%s",token);
            m_exparse.get_token();
            if(*token != ',' && *token != ';') m_exparse.serror(1);
            m_exparse.get_token();
        }
        else outputString("? ");
        m_bContinueInput = true;
    }
    else
    {
        // continue the input operation
        Variable* pVar;
        Variable* pNewVar = NULL;
        DLOG("emscript: token is [%s]",token);
        pVar = findVariableName(token,m_listVars);
        if(!pVar)
        {
            pNewVar = createNewVariable(token);
            pVar = pNewVar;
        }
        if(pVar->type() == Variable::TYPE_INT)
        {
            pVar->setIntValue(atoi(pszString));
        }
        else
        {
            pVar->setStringValue(pszString);
        }
        m_bContinueInput = false;
    }

    return m_bContinueInput;
}


int
EMScript::outputString(const char* pszOutput)
{
    if(m_pPlayer)
        return m_pPlayer->outputString(pszOutput);
    return strlen(pszOutput);
}


void
EMScript::outputNewLine()
{
    if(m_pPlayer)
        m_pPlayer->outputString("\r\n");
}

/**
 Called to handle custom tokens added to the Basic command set.
 \param tok Token ID to run.
 **/
bool EMScript::execAddonCommand(const int tok)
{
    printf("exec addon command tok = %d\n",tok);

    if(tok == m_removeItem) removeItem();
    if(tok == m_addItem) addItem();
    if(tok == m_showItems) showItems();
    if(tok == m_pickupItem) pickupItem();
    if(tok == m_showPlayersInventory) showPlayersInventory();
    if(tok == m_checkInventory) checkInventory();
    if(tok == m_countRoomItems) countRoomItems();
    if(tok == m_dropItem) dropItem();
    if(tok == m_help) help();
    if(tok == m_reload) reload();
    if(tok == m_setObject) setObject();
    if(tok == m_setRoom) setRoom();
    if(tok == m_loadUser) loadUser();
    if(tok == m_showPlayers) showPlayers();
    if(tok == m_showFunctions) showFunctions();
    if(tok == m_announcePlayer) announcePlayer();
    if(tok == m_exit) throwSocketInstanceException("Forcing closed to logout");
    return true;
}

void
EMScript::loadUser()
{
    // try loading and running the player script used for persistant storage
    // get the result variable
    Variable* pvResult;
    m_exparse.get_token();
    pvResult = findVariableName(token,m_listVars);
    if(!pvResult)
        pvResult = createNewVariable(token);

    // get the username
    char szUserName[MAX_USERNAME_LEN];
    m_exparse.get_token();
    if(*token != ',')
        throwSmallBasicException("loadUser: Syntax error - Expecting ','");

    m_exparse.get_token();
    if(token_type == QUOTE)
    {
        strncpy(szUserName,token,sizeof szUserName);
        NULL_TERMINATE(szUserName,sizeof szUserName);
    }
    else
    {
        Variable* v = findVariableName(token,m_listVars);
        if(!v) v = createNewVariable(token);
        if(v->type() != Variable::TYPE_STRING)
            throwSmallBasicException("loadUser: Syntax error - Expecting string variable, got number");
        strncpy(szUserName,v->stringValue(),sizeof szUserName);
        NULL_TERMINATE(szUserName,sizeof szUserName);
    }

    try
    {
        char szPath[MAX_PATH];
        snprintf(szPath,sizeof szPath,"users/%s.b",szUserName);
        NULL_TERMINATE(szPath,sizeof szPath);
        runScript(m_pRoom,m_pPlayer,m_pMudObject,szPath,NULL);  // use the script to load and create the required variables
        pvResult->setIntValue(1);
    }
    catch(SmallBasicException&)
    {
        pvResult->setIntValue(0);
    }
}


void
EMScript::saveUser()
{
    int userID;
    m_exparse.get_exp(&userID);
    if(!userID)
        throwSmallBasicException("saveUser: Invalid user id");
    ClientHandler::saveUser(m_pPlayer);
}


void
EMScript::setObject() {
    FUNCLOG("EMScript::setObject");
    const char *syntax = "setobject [out]found,[in]itemId,[in]location";
    int *found;       // if the object was found
    int itemID;       // object id
    int location;     // object is in the room or the players inventory
    Variable *pvResult;

    m_exparse.get_token();
    pvResult = findVariableName(token, m_listVars);
    if (!pvResult)
        pvResult = createNewVariable(token);

    m_exparse.get_token();
    if (*token != ',')
        throwSmallBasicException("Syntax error - Expecting ',' in setObject");

    m_exparse.get_exp(&itemID);
    if (!itemID)
        throwSmallBasicException("Invalid item id for set object");

    m_exparse.get_token();
    if (*token != ',')
        throwSmallBasicException("Syntax error - Expecting ',' in setObject");

    m_exparse.get_exp(&location);
    if (!location)
        throwSmallBasicException("Invalid location set object");

    MudObject *pObject;
    if (1 == location) pObject = m_pPlayer->findItem(itemID);
    if (2 == location) pObject = m_pPlayer->room()->findItem(itemID);

    if (!pObject)
    {
        pvResult->setIntValue(0);
        Variable* pvErrorMsg = findVariableName("errormsg", m_listVars);
        if(!pvErrorMsg)
            pvErrorMsg = createNewVariable("errormsg");
        char msg[255];
        snprintf(msg,sizeof msg,"Player id %d | Room %d | Unable to find object %d in %s. Script %s:%d",m_pPlayer->id(),m_pPlayer->roomNum(),itemID,location==1?"player":"room",m_szFilename,source_line);
        NULL_TERMINATE(msg,sizeof msg);
        pvErrorMsg->setStringValue(msg);
    }
    else
    {
        pvResult->setIntValue(1);
        m_pMudObject = pObject;
    }
}


void
EMScript::setRoom()
{
    int roomID;
    m_exparse.get_exp(&roomID);
    if(!roomID)
        throwSmallBasicException("Invalid item id for set room");

    Room* pRoom = m_world.findRoom(roomID);
    if(!pRoom)
        throwSmallBasicException(FmtString("Room %d not found",roomID));

    m_pRoom = pRoom;
}


void
EMScript::help()
{
    list<ScriptCmd> listCmds;
    int iCount = m_world.copyCmdList(&listCmds);
    if(!iCount)
        outputString("None.");
    else {
        for (list<ScriptCmd>::iterator it = listCmds.begin();
             it != listCmds.end(); it++) {
            outputString((*it).cmd());
            outputString("      - ");
            outputString((*it).description());
            outputNewLine();
        }
    }
}

void
EMScript::showItems()
{
    if(m_pRoom)
    {
        list<MudObject*>* itemList = m_pRoom->itemList();
        list<MudObject*>::iterator it;
        unsigned32 nVisibleObjects=0;

        for(it=itemList->begin(); it!=itemList->end(); it++)
        {
            MudObject* o = *it;
            if(o->isVisible())
            {
                if(nVisibleObjects > 0)
                    outputString(", ");

                outputString((*it)->name());
                nVisibleObjects++;
            }
        }
    }
}

void
EMScript::showPlayersInventory()
{
    if(m_pPlayer)
    {
        list<MudObject*>* itemList = m_pPlayer->itemList();
        list<MudObject*>::iterator it;
        unsigned32 nVisibleObjects=0;

        for(it=itemList->begin(); it!=itemList->end(); it++)
        {
            if((*it)->isVisible())
            {
                if(nVisibleObjects > 0)
                    outputString(", ");
                outputString((*it)->name());
                nVisibleObjects++;
            }
        }
        if(!nVisibleObjects)
            outputString("Nothing.");
        outputNewLine();
    }
}

// checkInventory retval,objectID
// upon returning, retval will contain 1 if the item exists in the players inventory,
// or 0 if it does not.
void
EMScript::checkInventory()
{
    int itemID;
    Variable* pvResult;

    m_exparse.get_token();
    pvResult = findVariableName(token,m_listVars);
    if(!pvResult)
        pvResult = createNewVariable(token);
    m_exparse.get_token();
    if(*token != ',')
        throwSmallBasicException("Syntax error. checkInventory [out]inPlayerInventory,[int]objectID");
    m_exparse.get_exp(&itemID);
    if(m_pPlayer->findItem(itemID))
        pvResult->setIntValue(1);
    else
        pvResult->setIntValue(0);
}

//countRoomItems [out]playerCount,[out]itemCount,[in]roomID
//Upon returning, playerCount will contain # players including player, itemCount will contain the # objects in the room.
void
EMScript::countRoomItems()
{
    char* pszSyntaxError = "Syntax error. countRoomItems [out]playerCount,[out]itemCount,[in]roomID";
    int roomID;
    Variable* pvPlayerCount;
    Variable* pvItemCount;
    Variable* pvRoomID;

    m_exparse.get_token();
    pvItemCount=findVariableName(token,m_listVars);
    if(!pvItemCount)
        pvItemCount = createNewVariable(token);

    m_exparse.get_token();
    if(*token != ',')
        throwSmallBasicException(pszSyntaxError);

    m_exparse.get_token();
    pvPlayerCount=findVariableName(token,m_listVars);
    if(!pvPlayerCount)
        pvPlayerCount = createNewVariable(token);

    m_exparse.get_token();
    if(*token != ',')
        throwSmallBasicException(pszSyntaxError);

    m_exparse.get_token();
    pvRoomID=findVariableName(token,m_listVars);
    if(!pvRoomID)
        pvRoomID = createNewVariable(token);

    m_world.showPlayers(pvRoomID->intValue());
    Room* pRoom = m_world.findRoom(pvRoomID->intValue());
    pvItemCount->setIntValue(countVisibleRoomItems(pRoom));
    pvPlayerCount->setIntValue(m_world.numPlayersInRoom(pRoom->id()));
}

int
EMScript::countVisibleRoomItems(Room* pRoom) {
    int count = 0;
    list<MudObject*>::iterator it;
    for(it=pRoom->itemList()->begin(); it!=pRoom->itemList()->end(); it++) {
        Variable* v = (*it)->findVariableName("ov.invisible");
        if(v && v->intValue() == 0)
            count++;
    }
    return count;
}

void
EMScript::pickupItem()
{
    if(m_pRoom && m_pPlayer)
    {
        int nItemID;
        m_exparse.get_exp(&nItemID);
        if(!nItemID)
            throwSmallBasicException("Invalid item id for pickup item");

        MudObject* pObject = m_pRoom->findItem(nItemID);
        if(pObject && pObject->isVisible())
        {
            if(pObject->isRoomOnly())
            {
                outputString("You can't pick up this object.\r\n");
            }
            else
            {
                if(m_pRoom->removeItem(nItemID))
                    m_pPlayer->addItem(pObject);
                else
                    throwSmallBasicException("Unable to remove item from room");
            }
        }
        else
            runScript(NULL,m_pPlayer,NULL,"scripts/character/player.b","onObjectNotFound");
    }
}

void
EMScript::dropItem()
{
    if(m_pRoom && m_pPlayer)
    {
        int nItemID;
        m_exparse.get_exp(&nItemID);
        if(!nItemID)
            throwSmallBasicException("Invalid item id for pickup item");

        MudObject* pObject = m_pPlayer->removeItem(nItemID);
        if(!pObject)
            throwSmallBasicException("Object was not found in players inventory");
        m_pRoom->addItem(pObject);
    }
}

/**
 * Shows all other players in the same room as the current player. Doesn't show the current player.
 **/
void
EMScript::showPlayers()
{
    list<string>::iterator it;
    unsigned32 nVisibleObjects=0;
    list<string> playerList;

    m_world.getPlayerNames(playerList,m_pPlayer->roomNum(),m_pPlayer->id());
    for(it=playerList.begin(); it!=playerList.end(); it++)
    {
        string name = *it;
        if(nVisibleObjects > 0)
            outputString(", ");
        outputString(name.c_str());
        nVisibleObjects++;
    }
}


void
EMScript::reload()
{
    int module;
    m_exparse.get_exp(&module);
    switch(module)
    {
        case 1:  m_world.readCommands(); break;
        case 2:  m_world.readExtraWords(); break;
    }
}

//TODO: should lock the list, check for the existance, then unlock
Variable*
EMScript::createNewVariable(const char* token)
{
    FUNCLOG("EMScript::createNewVariable");
    string t(token);
    if(t.find("pv.")==0 && m_pPlayer)
    {
        return m_pPlayer->findOrCreateVariable(token+3);
    }
    else if(t.find("rv.")==0 && m_pRoom)
    {
        return m_pRoom->findOrCreateVariable(token+3);
    }
    else if(t.find("ov.")==0 && m_pMudObject)
    {
        return m_pMudObject->findOrCreateVariable(token+3);
    }
    return SmallBasic::createNewVariable(token);
}

//TODO: when I am not able to find a variable for non-script vars,
//TODO  an exception should be thrown instead of freely creating
//TODO  them.
Variable*
EMScript::findVariableName(const char* token,list<Variable*>& searchList)
{
    string t(token);
    if(t.find("pv.")==0 && m_pPlayer)
    {
        // player variable
        Variable* v = m_pPlayer->findOrCreateVariable(token+3);
        if(!v)
            throwSmallBasicException("Unable to find or create variable");
        return v;
    }
    else if(t.find("rv.")==0 && m_pRoom)
    {
        // room variable
        Variable* v = m_pRoom->findOrCreateVariable(token+3);
        if(!v)
            throwSmallBasicException("Unable to find or create variable");
        return v;
    }
    else if(t.find("ov.")==0 && m_pMudObject)
    {
        Variable* v = m_pMudObject->findOrCreateVariable(token+3);
        if(!v)
            throwSmallBasicException("Unable to find or create variable");
        return v;
    }

    // regular smallbasic variable
    return SmallBasic::findVariableName(token,searchList);
}

/**
   \throw SocketInstanceException
   \throw SmallBasicException
**/
void
EMScript::runScript(Room* pRoom,Player* pPlayer,MudObject* pItem,const char* pszPathname,const char* pszSub)
{
    EMScript script(m_world,pRoom,pPlayer,pItem,pszPathname);
    script.run(pszSub);
}
