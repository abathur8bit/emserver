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
       filename :  configparser.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

//#include <string>
#include <ssobjects/ssobjects.h>
#include <ssobjects/file.h>

//TODO: Would like to be able to have a comment at the end of the line
//TODO: Would like to be able to put a string in quotes

using namespace ssobjects;
using namespace std;

class ConfigParser
{
  protected:
    char* m_sFilename;
    File m_file;

  public:
    ///Constructs and opens the config file for reading
    ConfigParser(const char* pszFilename) : m_sFilename(),m_file(pszFilename) {};
    unsigned32 getString(const char* pszSectionName,const char* pszKeyName,const char* pszDefaultValue,char* pszReturnedString,const unsigned32 nReturnedSize);
    int getNumber(const char* pszSectionName,const char* pszKeyName,const int iDefaultValue,int* piReturnedNumber);
    bool getBool(const char* pszSectionName,const char* pszKeyName,const bool  bDefaultValue,bool* pbReturnValue);

  protected:
    bool parseKeyValue(char* key,char* value,char* buffer);
    bool parseSection(char* section,char* buffer);
    bool isWhiteSpace(const char c);
    char* trim(char* s);
};


#endif
