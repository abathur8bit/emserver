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

       created  :  April 23, 2001
       filename :  smallbasic.h
       author   :  Lee Patterson (workerant@users.sourceforge.net)

       purpose  :

       notes    :  Adopted from:
                   SMALL BASIC Interpeter (sbasic.c) V0.2
                   Copyright (C) 1994 James J. Bielman
                   Adapted from _C Power User's Guide_ by Herbert Schildt

*********************************************************************/


#ifndef SMALLBASIC_H
#define SMALLBASIC_H

#include <stdio.h>
#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <ssobjects/generalexception.h>
#include <ssobjects/linkedlist.h>
#include <stack>
#include <list>

#include "variable.h"
#include "exparse.h"

using namespace ssobjects;
using namespace std;

#define NUM_LAB           50
#define LAB_LEN           100
#define FOR_NEST          25
#define SUB_NEST          25
#define PROG_SIZE         0xFFFF
#define MAX_COMMANDS      100
#define MAX_STRING_LENGTH 1024

#define DELIMITER       1
#define VARIABLE        2
#define NUMBER          3
#define COMMAND         4
#define STRING          5
#define QUOTE           6
#define LABEL           7
#define INCLUDE         8

#define PRINT           1
#define INPUT           2
#define IF              3
#define THEN            4
#define ELSE	       	  5
#define FOR             6
#define TO              7
#define NEXT            8
#define GOTO            9
#define SB_EOL          10
#define SB_FINISHED     11
#define GOSUB           12
#define RETURN          13
#define END             14
#define CUSTOM          15

struct label
{
  char name[LAB_LEN];
  char *p;
};

//  struct for_stack
//  {
//  	Variable* var;
//  	int target;
//  	char *loc;
//  };


class ForStack
{
  public:
    Variable* m_pVar;       ///< Contains the variable we are using to count with
    int m_iTarget;          ///< We are counting up to this
    char* m_pSourceLoc;     ///< Target in source file

  public:
    ForStack() : m_pVar(NULL),m_iTarget(0),m_pSourceLoc(NULL) {}
    ForStack(Variable* pVar,const int iTarget,char* pSrcLoc)
      : m_pVar(pVar),m_iTarget(iTarget),m_pSourceLoc(pSrcLoc) {}

    // equals operator
    ForStack& operator=(const ForStack& s)
    {
      m_pVar = s.m_pVar;
      m_iTarget = s.m_iTarget;
      m_pSourceLoc = s.m_pSourceLoc;
      return *this;
    }
    // copy constructor
    ForStack(const ForStack& s)
      : m_pVar(s.m_pVar),m_iTarget(s.m_iTarget),m_pSourceLoc(s.m_pSourceLoc) {}
};


struct commands
{ // keyword lookup table
  char command[20];
  char tok;
};

#define throwSmallBasicException(m) (throw SmallBasicException(m,__FILE__,__LINE__))

class SmallBasicException : public GeneralException
{
  public: // Constructor/destructor
    SmallBasicException(char* pchMessage,const char* pFname,const int iLine)
      : GeneralException(pchMessage,pFname,iLine){};
};

class Exparse;

class SmallBasic
{
  friend class Exparse;

  protected:
    Exparse m_exparse;
    char    m_szFilename[_MAX_PATH];
    int     source_line;
    char*   m_program;          ///< the program that has been loaded
    char*   m_programStart;     ///< keep track of the original pointer
    int     m_iProgramSize;     ///< how large the program is
    char*   m_pmain;            ///< When set, the program should start at this point
    char    token[80];
    char    token_type,tok;
    struct label label_table[NUM_LAB];
    list<Variable*> m_listVars;
    stack<ForStack> m_forStack;
    bool m_bStartedSubroutine;
    int m_iNumCustomCommands;

    char *gstack[SUB_NEST];

    int ftos;                   ///< index to top of FOR stack
    int gtos;                   ///< index to top of GOTO stack

  public:
    SmallBasic(const char* filename);
    virtual ~SmallBasic();
    virtual bool run(const char* pszStartSubroutine=NULL);           ///< Runs the script.
    virtual Variable* createNewVariable(const char* pVariableName);

  protected:
    virtual int outputStringf(const char* fmt,...);    ///< outputs like printf, calls outputString to output formated string
    virtual int outputString(const char* pszOutput);   ///< display text
    virtual void outputNewLine();                       ///< move cursor to next line, left side of screen
    virtual void inputNumber(int* i);                   ///< get input and store in i
    virtual void inputString(char* s,const unsigned32 nMaxLength);                ///< Get string input and store in s.
    virtual Variable* findVariableName(const char* token,list<Variable*>& searchList);
    virtual void removeVariable(Variable* const pVar);
    virtual bool execAddonCommand(const int) {return true;};
    virtual bool exec_command();
    virtual bool input();                               ///< Reads text from the user.

    int addCommand(char* pszCommand);                   ///< Add a new command to Small Basic's existing command set.
    void insertFile(const char* pszPathName);           ///< Insert file at current program position.

    void assignment();
    void exec_for();
    void exec_goto();
    void exec_if();
    void find_eol();
    char* find_label(char *s);
    ForStack fpop();
    void fpush(ForStack i);
    int  get_next_label(char *s);
    void gosub();
    char* gpop();
    void gpush(char *s);
    bool greturn();
    void label_init();
    int load_program(char *p, const char *fname);
    void next();
    void print();
    void scan_labels();

    virtual void showKnownCommands();
};
#endif
