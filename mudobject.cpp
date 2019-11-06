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
       filename :  mudobject.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#include "mudobject.h"

int MudObject::m_iUniqueID = 1000;

MudObject::MudObject(const unsigned32 id,const char* pszName)
  : m_listVariables()
{
  m_pvID = new Variable("id",Variable::TYPE_INT);
  m_pvID->setIntValue(id);
  m_listVariables.push_back(m_pvID);

  m_pvName = new Variable("name$",Variable::TYPE_STRING);
  m_pvName->setStringValue(pszName);
  m_listVariables.push_back(m_pvName);

  m_pvInvisible = new Variable("invisible",Variable::TYPE_INT);
  m_listVariables.push_back(m_pvInvisible);

  m_pvRoomOnly = new Variable("roomonly",Variable::TYPE_INT);
  m_listVariables.push_back(m_pvRoomOnly);
}

MudObject::~MudObject()
{
  if(m_listVariables.size())
  {
#ifdef DEBUG
    printf("\n\n==============\n"
           "%s:%d Variables:\n",name(),id());
#endif
    list<Variable*>::iterator it;
    for(it = m_listVariables.begin(); it!=m_listVariables.end(); it++)
    {
#ifdef DEBUG
      printf("variable name: %s\n",(*it)->name());
#endif
      delete *it;
    }
  }
}


/**
    Searches our list of available variables.

    \param pszName The name of the variable you are searching for.

    \return The found variable, or NULL if it wasn't found.
**/
Variable*
MudObject::findVariableName(const char* pszName)
{
  AutoLock l(m_lock);
  list<Variable*>::iterator v;
  for(v = m_listVariables.begin(); v!=m_listVariables.end(); v++)
  {
    if(!strncasecmp((*v)->name(),pszName,strlen((*v)->name())))
      return *v;
  }
  return NULL;
}


/**
    Searches our list of available variables, and creates one if it isn't able
    to find one already created.

    \param pszName The name of the variable you are searching for.

    \return The found variable, or a newly created one.
**/
Variable*
MudObject::findOrCreateVariable(const char* pszName)
{
  AutoLock l(m_lock);
  list<Variable*>::iterator v;
  for(v = m_listVariables.begin(); v!=m_listVariables.end(); v++)
  {
    if(!strncasecmp((*v)->name(),pszName,strlen((*v)->name())))
      return *v;
  }

  // no variable found, so create one
  int iType = (pszName[strlen(pszName)-1] == '$') ? Variable::TYPE_STRING : Variable::TYPE_INT;
  Variable* pNewVar = new Variable(pszName,iType);
  m_listVariables.push_back(pNewVar);
  return pNewVar;
}


/**
    Searches our item list for the object by it's name.

    Thread safe.

    \return A pointer to the object if found, NULL if not.
**/
MudObject*
MudObject::lookupItem(const char* pszName)
{
  AutoLock l(m_lock);
  list<MudObject*>::iterator it;
  for(it=m_listItems.begin(); it != m_listItems.end(); it++)
    if(!strcasecmp(pszName,(*it)->name()))
      return *it;
  return NULL;
}

/**
    Searches our item list for the object by it's id.

    Thread safe.

    \return A pointer to the object if found, NULL if not.
**/
MudObject*
MudObject::findItem(const int iItemID)
{
  AutoLock l(m_lock);
  list<MudObject*>::iterator it;
  for(it=m_listItems.begin(); it != m_listItems.end(); it++)
    if(iItemID == (*it)->id())
      return *it;
  return NULL;
}


/**
   Removes the specified item from our item list. Does not delete the object.
   If the object was not in our list, NULL is returned.

   \return A pointer to the object that was removed, NULL if the object was not found.
**/
MudObject*
MudObject::removeItem(const MudObject* const pItem)
{
  AutoLock l(m_lock);
  list<MudObject*>::iterator it;
  for(it=m_listItems.begin(); it != m_listItems.end(); it++)
    if(*it == pItem)
    {
      MudObject* o = *it;
      m_listItems.erase(it);
      return o;
    }
  return NULL;
}


/**
   Removes the specified item from our item list. Does not delete the object.
   If the object was not in our list, NULL is returned.

   \return A pointer to the object that was removed, NULL if the object was not found.
**/
MudObject*
MudObject::removeItem(const int iItemID)
{
  AutoLock l(m_lock);
  list<MudObject*>::iterator it;
  for(it=m_listItems.begin(); it != m_listItems.end(); it++)
    if(iItemID == (*it)->id())
    {
      MudObject* o = *it;
      m_listItems.erase(it);
      return o;
    }
  return NULL;
}

void
MudObject::addItem(MudObject* const pItem)
{
  AutoLock l(m_lock);
  m_listItems.push_back(pItem);
}


const char*
MudObject::name()
{
  AutoLock l(m_lock);
  const char* n = m_pvName->stringValue();
  return n;
}


void
MudObject::setName(const char* pszName)
{
  AutoLock l(m_lock);
  m_pvName->setStringValue(pszName);
}


int
MudObject::id()
{
  AutoLock l(m_lock);
  int n = m_pvID->intValue();
  return n;
}
