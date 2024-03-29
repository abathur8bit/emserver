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
       filename :  scriptcmd.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef SCRIPTCMD_H
#define SCRIPTCMD_H


#include <string>
#include <ssobjects/ssobjects.h>

using namespace ssobjects;

using namespace std;

class ScriptCmd
{
  public:
    enum
      {
      ST_ROOM   = 1,
      ST_OBJECT = 2,
      ST_PLAYER = 3
      };

  protected:
    string m_sCmd;
    string m_sHandler;
    string m_sDescription;
    unsigned32 m_nType;

  public:
    ScriptCmd() {};
    ScriptCmd(const char* pszCmd,const char* pszHandler,const char* pszDescription,const unsigned32 nType)
      : m_sCmd(pszCmd),m_sHandler(pszHandler),m_sDescription(pszDescription),m_nType(nType) {};
    CONSTCHAR* cmd() {return m_sCmd.c_str();};
    CONSTCHAR* handler() {return m_sHandler.c_str();};
    CONSTCHAR* description() {return m_sDescription.c_str();};
    unsigned32 type() const {return m_nType;};
};

#endif //SCRIPTCMD_H
