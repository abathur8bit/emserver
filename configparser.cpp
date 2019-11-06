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

       created  :  November 2001
       filename :  configparser.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#include "configparser.h"

#include <algorithm>    // std::max
#include <string>


//TODO: Would like to be able to have a comment at the end of the line
//TODO: Would like to be able to put a string in quotes

using namespace ssobjects;
using namespace std;

bool
ConfigParser::isWhiteSpace(const char c)
{
  if(' '==c || '\t'==c)
    return true;
  return false;
}

char*
ConfigParser::trim(char* s)
{
  char* start = s;
  char* end;
  if(strlen(s))
  {
    end = start+strlen(s);
    while(isWhiteSpace(*start) && strlen(start))
    {
      memmove(start,start+1,strlen(start)-1);
      *(--end) = '\0';
    }
    if(strlen(start))
    {
      end = s+strlen(s);
      end--;
      while(isWhiteSpace(*end) && end > start)
      {
        *end = '\0';
        end--;
      }
    }
  }

  return start;
}

bool
ConfigParser::parseKeyValue(char* key,char* value,char* buffer)
{
  if('#' != buffer[0] && strlen(buffer) > 0)
  {
    char* start = strchr(buffer,'=');
    *start = '\0';
    strcpy(key,buffer);
    *start = '=';
    start++;
    memset(value,'\0',strlen(buffer));
    size_t n = strlen(buffer)-(start-buffer);
    memmove(value,start,n);
    return true;
  }
  return false;
}

bool
ConfigParser::parseSection(char* section,char* buffer)
{
  if('#' != buffer[0])
  {
    char* start = strchr(buffer,'[');
    char* end = strchr(buffer,']');
    if(NULL != start && NULL != end)
    {
      start++;
      memmove(section,start,end-start);	//copy less the "[]" chars
      section[end-start]='\0';
      trim(section);
      return true;
    }
  }
  return false;
}

unsigned32
ConfigParser::getString(
        const char* pszSectionName,
        const char* pszKeyName,
        const char* pszDefaultValue,
        char* pszReturnedString,
        const unsigned32 nReturnedSize)
{
#     ifdef _WIN32

  int er = GetPrivateProfileString(pszSectionName,pszKeyName,pszDefaultValue,pszReturnedString,nReturnedSize,m_sFilename);
  return er;

#     else

  char buffer[1000];
  char section[1000];
  char key[1000];
  char value[1000];
  bool bFound = false;

  m_file.rewind();
  do
  {
    m_file.fgetsTrimNL(buffer,sizeof buffer);
    trim(buffer);
    if(parseSection(section,buffer))
    {
      if(strcasecmp(section,pszSectionName)==0)
        bFound=true;
    }
  } while(!bFound);

  bFound = false;
  do
  {
    m_file.fgetsTrimNL(buffer,sizeof buffer);
    trim(buffer);

    if(strlen(buffer) < 3)   //must have at least "a=b" string
      continue;

    if('[' == buffer[0])
      break;  //we ended up at the next section

    if(parseKeyValue(key,value,buffer))
    {
      trim(key);
      if(strcasecmp(key,pszKeyName)==0)
      {
        bFound=true;
        trim(value);
        strncpy(pszReturnedString,value,min((int)strlen(value),(int)nReturnedSize));
        NULL_TERMINATE(pszReturnedString,min((int)strlen(value)+1,(int)nReturnedSize));
        return strlen(pszReturnedString);
      }
    }
  } while(!bFound);

  // If we got here, we didn't find any matching key.
  // We'll just return the default value
  strncpy(pszReturnedString,pszDefaultValue,min((int)strlen(pszDefaultValue),(int)nReturnedSize));
  NULL_TERMINATE(pszReturnedString,nReturnedSize);
  return strlen(pszReturnedString);

#     endif
}

int
ConfigParser::getNumber(
        const char* pszSectionName,
        const char* pszKeyName,
        const int   iDefaultValue,
        int*        piReturnedNumber)
{
# ifdef _WIN32

  int number = GetPrivateProfileInt(pszSectionName,pszKeyName,iDefaultValue,m_sFilename);
  *piReturnedNumber = number;
  return number;

# else

  char szNumber[256];
  char szDefault[256];
  sprintf(szDefault,"%d",iDefaultValue);
  getString(pszSectionName,pszKeyName,szDefault,szNumber,sizeof szNumber);
  *piReturnedNumber = atoi(szNumber);
  return *piReturnedNumber;

# endif
}

bool
ConfigParser::getBool(
        const char* pszSectionName,
        const char* pszKeyName,
        const bool  bDefaultValue,
        bool* pbReturnValue)
{
  char buffer[6];
  getString(pszSectionName,
            pszKeyName,
            bDefaultValue?"true":"false",
            buffer,
            sizeof buffer);

  if(!strcasecmp(buffer,"true"))
    *pbReturnValue = true;
  else if(!strcasecmp(buffer,"false"))
    *pbReturnValue = false;
  else
  {
    int iValue = atoi(buffer);
    *pbReturnValue = iValue?true:false;
  }

  return *pbReturnValue;
}
