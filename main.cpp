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

       created  :  March 18, 2001
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/


#include <stdio.h>
#include <iostream>

#include <ssobjects/ssobjects.h>
#include <ssobjects/socketinstance.h>
#include <ssobjects/logs.h>
#include <ssobjects/threadutils.h>

#include "emserver.h"
#include "scriptcmd.h"
#include "world.h"

using namespace std;
using namespace ssobjects;

void
help()
{
  puts("Usage:   em [-p <port>]\n");
  puts("  Options:");
  puts("   -p,--port    = Server listsns on port <port>");

  exit(0);
}

//bool
//isWhiteSpace(const char c)
//{
//  if(' '==c || '\t'==c)
//    return true;
//  return false;
//}
//
//char*
//trim(char* s)
//{
//  char* start = s;
//  char* end;
//  if(strlen(s))
//  {
//    end = start+strlen(s);
//    int i=0;
//    while(isWhiteSpace(*start) && strlen(start))
//    {
//      memmove(start,start+1,strlen(start)-1);
//      *(--end) = '\0';
//    }
//    if(strlen(start))
//    {
//      end = s+strlen(s);
//      end--;
//      while(isWhiteSpace(*end) && end > start)
//      {
//        *end = '\0';
//        end--;
//      }
//    }
//  }
//
//  return start;
//}
//
//int
//parseKeyValue(char* key,char* value,char* buffer)
//{
//  if('#' != buffer[0] && strlen(buffer) > 0)
//  {
//    //0123456789
//    //a = bleh
//    char* start = strchr(buffer,'=');
//    *start = '\0';
//    strcpy(key,buffer);
//    *start = '=';
//    start++;
//    memset(value,'\0',strlen(buffer));
//    size_t n = strlen(buffer)-(start-buffer);
//    memmove(value,start,n);
//    return 1;
//  }
//  return 0;
//}
//
//int
//parseSection(char* section,char* buffer)
//{
//  if('#' != buffer[0])
//  {
//    char* start = strchr(buffer,'[');
//    char* end = strchr(buffer,']');
//    if(NULL != start && NULL != end)
//    {
//      start++;
//      memmove(section,start,end-start);	//copy less the "[]" chars
//      section[end-start]='\0';
//      trim(section);
//      return 1;
//    }
//  }
//  return 0;
//}
//
//int
//getString(
//  const char* pszSectionName,
//  const char* pszKeyName,
//  const char* pszDefaultValue,
//  char* pszReturnedString,
//  const unsigned32 nReturnedSize)
//{
//  char buffer[1000];
//  char section[1000];
//  char key[1000];
//  char value[1000];
//  bool bFound = false;
//  ssobjects::File f("config.ini");
//
//  f.rewind();
//  do
//  {
//    f.fgetsTrimNL(buffer,sizeof buffer);
//    trim(buffer);
//    //printf("%s\n",buffer);
//    if(parseSection(section,buffer))
//    {
//    	if(strcasecmp(section,pszSectionName)==0)
//	      bFound=true;
//    }
//  } while(!bFound);
//
//  bFound = false;
//  do
//  {
//    f.fgetsTrimNL(buffer,sizeof buffer);
//    trim(buffer);
//
//    if(strlen(buffer) < 3)   //must have at least "a=b" string
//      continue;
//
//    if('[' == buffer[0])
//      break;  //we ended up at the next section
//
//    if(parseKeyValue(key,value,buffer))
//    {
//      trim(key);
//      if(strcasecmp(key,pszKeyName)==0)
//      {
//        bFound=true;
//        trim(value);
//        strncpy(pszReturnedString,value,min(strlen(value),nReturnedSize));
//        NULL_TERMINATE(pszReturnedString,nReturnedSize);
//        return strlen(pszReturnedString);
//      }
//    }
//  } while(!bFound);
//
//  // If we got here, we didn't find any matching key.
//  // We'll just return the default value
//  strncpy(pszReturnedString,pszDefaultValue,min(strlen(pszDefaultValue),nReturnedSize));
//  NULL_TERMINATE(pszReturnedString,nReturnedSize);
//  return strlen(pszReturnedString);
//}

//void test()
//{
//	char value[1000] = "";
//  getString("gardenmaze","name","default value",value,sizeof value);
//  printf("name=[%s]\n",value);
//  exit(0);
//}

int
main(int argc,char* argv[])
{
    UNUSED(argv);

//    list<string> slist;
//    char test[4] = "123";
//    string s(test);
//    test[0] = 'a';
//    test[1] = 'b';
//    test[2] = 'c';
//    printf("test=%s s=%s\n",test,s.c_str());
//    slist.push_back(s);
//    s.clear();
//    printf("test=%s s=%s\n",test,s.c_str());
//    s = slist.back();
//    printf("test=%s s=%s\n",test,s.c_str());
//    return 0;

  logs::init("server");
  logs::enable();

  unsigned16 wPort = 4001;
  for(int i=1; i<argc; i++)
  {
    if(!strcmp(argv[i],"--port") || !strcmp(argv[i],"-p"))
    {
      if(argc<i+1) help();
      wPort = atoi(argv[i+1]);
    }
  }

  puts("Easye Mud Server");
  puts("Copyright (c) 2009, Ant Works Software");
  puts("Author: Lee Patterson (http://easyemud.sourceforget.net)");
  printf("Server running on port %u\n\n",wPort);

  try
  {
    SockAddr saBind((unsigned long)INADDR_ANY,wPort);
    EMServer server(saBind);
    server.startServer();
  }
  catch(GeneralException& e)
  {
    printf("%s\n",e.getErrorMsg());
  }



  return 0;
}
