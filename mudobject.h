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
       filename :  mudobject.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

*********************************************************************/

#ifndef MUDOBJECT_H
#define MUDOBJECT_H

#include <list>
#include <ssobjects/ssobjects.h>
#include "variable.h"
#include "lockableobject.h"

using namespace std;
using namespace ssobjects;

class MudObject;

class MudObject : public LockableObject
{
  private:
    static int m_iUniqueID;
  protected:
    int uniqueID() {return ++m_iUniqueID;}

  protected:
//    unsigned32 m_id;
//    string m_name;
    Variable* m_pvID;
    Variable* m_pvName;
    Variable* m_pvInvisible;
    Variable* m_pvRoomOnly;             ///< Flag if this object is aloud to be picked up by player
    list<Variable*> m_listVariables;    ///< Item variables.
    list<MudObject*> m_listItems;       ///< Lists of objects this object can contain.

  public:
    MudObject(const unsigned32 id=0,const char* pszName="");
    ~MudObject();

  public:
//    list<Variable*>& variables() const {return m_listVariables;};
    const char* name() ;
    void setName(const char* name);
    int  id();
    bool isVisible() {return m_pvInvisible->intValue()?false:true;};  // reversed, you are asking if the object is visable, stored as "invisible"
    bool isRoomOnly() {return m_pvRoomOnly->intValue()?true:false;};

    Variable* createNewVariable(const char* pszName,const int iType);
    Variable* findVariableName(const char* pszName);
    Variable* findOrCreateVariable(const char* pszName);

    void       addItem(MudObject* pItem);           ///< Add an item to our object list.
    MudObject* findItem(const int iItemID);       ///< Find an object by ID.
    MudObject* lookupItem(const char* pszName);   ///< Find an object by it's name.
    MudObject* removeItem(const int iItemID);    ///< Find and remove the item from our item list.
    MudObject* removeItem(const MudObject* const pItem);    ///< Find and remove the item from our item list.

    int copyItemList(list<MudObject>* plistCopyTo);     ///< Copies our list of items. //TODO: impliment
    list<MudObject*>* itemList() {return &m_listItems;} //TODO: HACK! remove this and use copy list method
};

#endif //MUDOBJECT_H
