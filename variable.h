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

       created  :  May 10, 2001
       filename :  variable.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

       purpose  :  Class to hold a variable

       notes    :
*********************************************************************/

#ifndef VARIABLE_H
#define VARIABLE_H

#include <ssobjects/ssobjects.h>

class Variable
{
  public:
    enum {TYPE_INT,TYPE_STRING};
	enum {MAX_LENGTH=1024};
  protected:
    char   m_sName[MAX_LENGTH];
    int    m_iValue;
    char   m_sValue[MAX_LENGTH];
    int    m_iType;

  public:
    Variable(const char* pszVariableName,const int iType)
      : m_sName(),m_iValue(0),m_sValue(),m_iType(iType)
    {
    	#ifdef DEBUG
        	printf("created variable [%s] of type %s\n",pszVariableName,(iType==TYPE_INT)?"int":"string");
        #endif
        strncpy(m_sName,pszVariableName,sizeof(m_sName));
        NULL_TERMINATE(m_sName,sizeof(m_sName));

        int length = strlen(m_sName);
        while('\r'==m_sName[length-1] || '\n'==m_sName[length-1])
        {
            m_sName[length-1]='\0';
            length = strlen(m_sName);
        }
    }

    virtual ~Variable(){}

    void setStringValue(const char* pszValue) {strncpy(m_sValue,pszValue,sizeof(m_sValue)); NULL_TERMINATE(m_sValue,sizeof(m_sValue));}
    void setIntValue(const int iValue) {m_iValue = iValue;}
    int intValue() const {return m_iValue;}
    const char* stringValue() const {return m_sValue;}
    const char* name() const {return m_sName;}
    int type() const {return m_iType;}
    Variable& operator ++ (int)
    {
      if(type() == TYPE_INT)
        m_iValue++;
      return *this;
    }
    Variable& operator -- (int)
    {
      if(type() == TYPE_INT)
        m_iValue--;
      return *this;
    }
};

#endif
