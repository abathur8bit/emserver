//       1         2         3         4         5         6         7         8         9         0         1         2         3         4         5         6         7         8         9
//34567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

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

       created  :  May 4, 2001
       filename :  smallbasic.cpp
       author   :  Lee Patterson (workerant@users.sourceforge.net)

       purpose  :  Basic interpreter object. Will interprete scripts
                   written in basic.

                   You will be able to extend and customize the scripting
                   language by adding your own commands, and overriding various
                   methods.

       notes    :
*********************************************************************/


/**********************************************************/
/* SMALL BASIC Interpeter (sbasic.c) V0.2                 */
/* Copyright (C) 1994 James J. Bielman                    */
/* Adapted from _C Power User's Guide_ by Herbert Schildt */
/**********************************************************/

#include <assert.h>
#include <string.h>
#include <string>

#include <ssobjects/cstr.h>
#include <ssobjects/file.h>
#include <ssobjects/logs.h>

#include "smallbasic.h"

//TODO: need AND, OR

struct commands table[MAX_COMMANDS] =
        {
                {  "print",  PRINT},
                {  "input",  INPUT},
                {  "if",     IF},
                {  "then",   THEN},
                {  "else",   ELSE},
                {  "goto",   GOTO},
                {  "for",    FOR},
                {  "next",   NEXT},
                {  "to",     TO},
                {  "gosub",  GOSUB},
                {  "return", RETURN},
                {  "end",    END},      // if you add new commands to small basic, add before "end"
                {  "",       END},
        };

struct commands*
commandTable()
{
  return table;
}


SmallBasic::SmallBasic(const char* script)
        : m_exparse(this),
          m_pmain(NULL),
          m_listVars(),
          m_bStartedSubroutine(false),
          m_iNumCustomCommands(0)
{
  FUNCLOG("SmallBasic::SmallBasic");
  DLOG("sb: address = %p",this);


  char *p_buf;

  source_line = 1;

  /* Allocating memory for the program */
  if(!(p_buf = (char *)calloc(PROG_SIZE,2)))
    throwSmallBasicException("memory allocation failier");

  /* Load the program */
  sprintf(m_szFilename,"%s",script);
  if(!load_program(p_buf, m_szFilename))
  {
    DLOG("sb: unable to load %s",m_szFilename);
    throwSmallBasicException(FmtString("Unable to load %s",m_szFilename));
  }

  m_programStart = p_buf;

  //reset the table
  int i=0;
  strcpy(table[i].command,"print");  table[i++].tok  =  PRINT;
  strcpy(table[i].command,"input");  table[i++].tok  =  INPUT;
  strcpy(table[i].command,"if"   );  table[i++].tok  =  IF;
  strcpy(table[i].command,"then" );  table[i++].tok  =  THEN;
  strcpy(table[i].command,"else" );  table[i++].tok  =  ELSE;
  strcpy(table[i].command,"goto" );  table[i++].tok  =  GOTO;
  strcpy(table[i].command,"for"  );  table[i++].tok  =  FOR;
  strcpy(table[i].command,"next" );  table[i++].tok  =  NEXT;
  strcpy(table[i].command,"to"   );  table[i++].tok  =  TO;
  strcpy(table[i].command,"gosub");  table[i++].tok  =  GOSUB;
  strcpy(table[i].command,"return"); table[i++].tok  =  RETURN;
  strcpy(table[i].command,"end"  );  table[i++].tok  =  END;      // if you add new commands to small basic, add before "end"
  strcpy(table[i].command,""     );  table[i++].tok  =  END;
}

SmallBasic::~SmallBasic()
{
  if(m_programStart)
    free(m_programStart);

  if(m_listVars.size())
  {
#ifdef DEBUG
    printf("SmallBasic Variables:\n");
#endif
    list<Variable*>::iterator it;
    for(it = m_listVars.begin(); it!=m_listVars.end(); it++)
    {
#ifdef DEBUG
      printf("variable name: %s\n",(*it)->name());
#endif
      delete *it;
    }
  }
}


bool
SmallBasic::run(const char* pszStartSubroutine)
{
  DLOG("SmallBasic::run [%s:%s]",m_szFilename,pszStartSubroutine?pszStartSubroutine:"main");
  DLOG("sb: address = %p",this);
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

  return false;
}


/* Modified 4.11.94 JJB */
bool
SmallBasic::exec_command()
{
  FUNCLOG("SmallBasic::exec_command()");
  DLOG("sb: address = %p",this);
  if(token_type == VARIABLE)
  {
    m_exparse.putback();
    assignment();
  }
  else
    switch(tok)
    {
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
        input();
        break;

      case GOSUB:
        gosub();
        break;

      case RETURN:
        if(!greturn())
          return false;
        break;

      case END:
      case SB_FINISHED:
        return false;
        break;

      default:
        if(tok>END)
          return execAddonCommand(tok);
        break;
    }

  return true;
}

int
SmallBasic::load_program(char *p, const char *fname)
{
  FILE *fp;
  int i = 0;

  if(!(fp = fopen(fname, "r"))) return 0;

  i = 0;
  do
  {
    *p = getc(fp);
    p++; i++;

  } while(!feof(fp) && i < PROG_SIZE);
  fclose(fp);
  m_iProgramSize = i;
  return 1;
}

void
SmallBasic::assignment()
{
  int value;
  Variable* pVar = NULL;

  /* Get the variable name */
  m_exparse.get_token();
  if(!isalpha(*token))
  {
    m_exparse.serror(4);
    return;
  }

  pVar = findVariableName(token,m_listVars);
  if(!pVar)
    pVar = createNewVariable(token);

  /* get the = */
  m_exparse.get_token();
  if(*token != '=')
  {
    m_exparse.serror(3);
    return;
  }

  /* get the value */
  if(pVar->type() == Variable::TYPE_INT)
  {
    m_exparse.get_exp(&value);
    pVar->setIntValue(value);
  }
  else
  {
    m_exparse.get_token();
    if(VARIABLE != token_type)
      pVar->setStringValue(token);
    else
    {
      Variable* v = findVariableName(token,m_listVars);
      if(!v)
        v = createNewVariable(token);
      pVar->setStringValue(v->stringValue());
    }
  }
}


//TODO: need a way to remove variables
//TODO: need a FAST way to look up variable names
Variable*
SmallBasic::findVariableName(const char* token,list<Variable*>& searchList)
{
  list<Variable*>::iterator v;
  for(v = searchList.begin(); v!=searchList.end(); v++)
  {
    if(!strncasecmp((*v)->name(),token,strlen((*v)->name())))
      return *v;
  }
  return NULL;
}

Variable*
SmallBasic::createNewVariable(const char* pVarName)
{
  int type = (pVarName[strlen(pVarName)-1] == '$') ? Variable::TYPE_STRING : Variable::TYPE_INT;
  Variable* pnewVar = new Variable(pVarName,type);
  m_listVars.push_back(pnewVar);
  return pnewVar;
}


void
SmallBasic::removeVariable(Variable* const pVar)
{
  if(!pVar) return;
  list<Variable*>::iterator v;
  for(v = m_listVars.begin(); v!=m_listVars.end(); v++)
  {
    if(pVar == *v)
    {
      DELETE_NULL(*v);
      m_listVars.erase(v);
      return;
    }
  }
}

void
SmallBasic::print()
{
  int answer;
  int len=0, spaces;
  char last_delim;

  do
  {
    m_exparse.get_token();	/* get next list item */

    if(*token=='\n') break;	//LBP (07/03/99): check if this is a ';' or ',' at the end of a line

    if(token_type == QUOTE)  /* is string */
    {
      outputString(token);
      len += strlen(token);
      m_exparse.get_token();
    }
    else /* is expression */
    {
      char szStringValue[MAX_STRING_LENGTH];
      m_exparse.putback();
      int type = m_exparse.get_exp(&answer,szStringValue);   //TODO: provide a string and a number variable
      m_exparse.get_token();
      if(Variable::TYPE_INT == type)
        len += outputStringf("%d", answer);
      else
        len += outputStringf("%s",szStringValue);
    }

    last_delim = *token;

    if(*token == ',')
    {
      spaces = 8 - (len % 8);
      len += spaces;
      while(spaces)
      {
        outputString(" ");
        spaces--;
      }
    }
    else if(*token == ';') /* nothin */ ;
    else break;

  } while(*token == ';' || *token == ',');

  if(last_delim != ';' && last_delim != ',') outputNewLine();
}


int
SmallBasic::outputStringf(const char* fmt,...)
{
  char buffer[1024];
  va_list marker;

  va_start(marker,fmt);
  vsnprintf(buffer,sizeof buffer,fmt,marker);
  NULL_TERMINATE(buffer,sizeof buffer);
  return outputString(buffer);
}

int
SmallBasic::outputString(const char* pszOutput)
{
  return printf("%s",pszOutput);
}


void
SmallBasic::outputNewLine()
{
  printf("\n");
}


void
SmallBasic::scan_labels()
{
  int addr;
  char *temp;

  label_init();
  temp = m_program;

  m_exparse.get_token();
  if(token_type ==  NUMBER || token_type == LABEL)
  {
    if(!strcasecmp(token,"main"))
      m_pmain = m_program;

    strcpy(label_table[0].name, token);
    label_table[0].p = m_program;
  }
  if(token_type == INCLUDE)
    insertFile(token);
  else
    find_eol();

  do
  {
    m_exparse.get_token();
    if(token_type == NUMBER || token_type == LABEL)
    {
      if(!strcasecmp(token,"main"))
        m_pmain = m_program;

      addr = get_next_label(token);
      if(addr == -1 || addr == -2)
        (addr == -1) ? m_exparse.serror(5) : m_exparse.serror(6);

      strcpy(label_table[addr].name, token);
      label_table[addr].p = m_program;
    }
    else if(token_type == INCLUDE)
      insertFile(token);

    if(tok != SB_EOL) find_eol();
  } while(tok != SB_FINISHED);

  if(m_pmain)
    m_program = m_pmain;
  else
    m_program = temp;

//  #ifdef DEBUG
//      for(int i=0; i<NUM_LAB; i++)
//        printf("%d - %s\n",i,label_table[i].name);
//  #endif

}

void
SmallBasic::insertFile(const char* pszPathName)
{
  try
  {
    File f(pszPathName,"r");
    f.load();
    memmove(m_program+f.getSize(),m_program,m_iProgramSize);
    memcpy(m_program,f.getDataPtr(),f.getSize());
    m_iProgramSize+=f.getSize();
  }
  catch(FileException&)
  {
    m_exparse.serror(14);   // file not found
  }
}

void
SmallBasic::find_eol()
{
  if((m_program - m_programStart) < m_iProgramSize)
    while(*m_program != '\n' && *m_program != '\r' && *m_program != '\0')
      ++m_program;
  if(*m_program == '\r')  //move past \r combo if exists
    ++m_program;

/*	if(*m_program) m_program++;  */
}

int
SmallBasic::get_next_label(char *s)
{
  int t;

  for(t = 0; t < NUM_LAB; ++t)
  {
    if(label_table[t].name[0] == 0) return t;
    if(!strcmp(label_table[t].name, s)) return t;
  }

  return -1;
}

char *
SmallBasic::find_label(char *s)
{
  int t;

  for(t = 0; t < NUM_LAB; ++t)
    if(!strcasecmp(label_table[t].name, s)) return label_table[t].p;
  return NULL;
}

void
SmallBasic::exec_goto()
{
  char *loc;

  m_exparse.get_token();

  loc = find_label(token);
  if(loc == NULL) m_exparse.serror(7);
  else m_program = loc;
}

void
SmallBasic::label_init()
{
  int t;

  for(t = 0; t < NUM_LAB; ++t) label_table[t].name[0] = '\0';
}

//TODO: add ability to compare: ["aaa" = a$] and ["aaa" = "bbb"]
//TODO  do so by adding get_token and a check for a quote or string before
//TODO  for get_exp call.
void
SmallBasic::exec_if()
{
  int x, y, cond;
  char szX[MAX_STRING_LENGTH];
  char szY[MAX_STRING_LENGTH];
  char op;
  bool bNotEqual = false;
  int typeX,typeY;

  typeX = m_exparse.get_exp(&x,szX);

  m_exparse.get_token();
  if(!strchr("=<>", *token))
  {
    m_exparse.serror(0);
    return;
  }
  op = *token;

  m_exparse.get_token();
  if('>' == *token)
  {
    bNotEqual = true;
    typeY = m_exparse.get_exp(&y,szY);
  }
  else
  {
    if(QUOTE == token_type)
    {
      // this is a string, not a variable
      strncpy(szY,token,sizeof szY);
      NULL_TERMINATE(szY,sizeof szY);
      typeY = Variable::TYPE_STRING;
    }
    else
    {
      m_exparse.putback();
      typeY = m_exparse.get_exp(&y,szY);
    }
  }

  cond = 0;
  switch(op)
  {
    case '<':
      if(bNotEqual)
      {
        if(Variable::TYPE_INT == typeX)
        {
          if(x != y) cond = 1;
        }
        else
        {
          //make sure that the second variable is a string
          if(Variable::TYPE_STRING != typeY)
            throwSmallBasicException("Syntax Error in not equal compair");
          if(strcmp(szX,szY))
            cond = 1;
        }
      }
      else
      {
        if(Variable::TYPE_INT == typeX)
        {
          if(x < y) cond = 1;
        }
        else
        {
          //make sure that the second variable is a string
          if(typeY != Variable::TYPE_STRING)
            throwSmallBasicException("Syntax Error in less then compair");
          if(strcmp(szX,szY) < 0)
            cond = 1;
        }
      }
      break;

    case '>':
      if(Variable::TYPE_INT == typeX)
      {
        if(x > y) cond = 1;
      }
      else
      {
        //make sure that the second variable is a string
        if(typeY != Variable::TYPE_STRING)
          throwSmallBasicException("Syntax Error in greater then compair");
        if(strcmp(szX,szY) > 0)
          cond = 1;
      }
      break;

    case '=':
      if(Variable::TYPE_INT == typeX)
      {
        if(x == y) cond = 1;
      }
      else
      {
        //make sure that the second variable is a string
        if(typeY != Variable::TYPE_STRING)
          throwSmallBasicException("Syntax Error in equal compair");
        if(!strcmp(szX,szY))
          cond = 1;
      }
      break;
  }

  m_exparse.get_token();

  /* Check for THEN statement */
  if(tok != THEN)
  {
    m_exparse.serror(8);
    return;
  }

  if(cond)
  {
    /* Execute until ELSE or end-of-line */
    do
    {
      m_exparse.get_token();
      exec_command();
    } while(tok != ELSE && tok != SB_EOL && tok != SB_FINISHED);

    if(tok == ELSE) find_eol();
  }
  else
  {
    /* Skip ahead until ELSE */
    while(tok != ELSE && tok != SB_EOL)
      m_exparse.get_token();
  }
}

void
SmallBasic::fpush(ForStack i)
{
  m_forStack.push(i);
}

ForStack
SmallBasic::fpop()
{
  if(!m_forStack.size())
    m_exparse.serror(11);     // throws

  ForStack s = m_forStack.top();
  m_forStack.pop();
  return s;
}

void
SmallBasic::exec_for()
{
  Variable* pNewVar = NULL;
  int value;

  m_exparse.get_token(); /* get control var */
  if(!isalpha(*token))
    m_exparse.serror(4);        //throws

  Variable* pVar = findVariableName(token,m_listVars);
  if(!pVar)
  {
    pNewVar = createNewVariable(token);
    pVar = pNewVar;
  }

  m_exparse.get_token();
  if(*token != '=')
  {
    removeVariable(pNewVar);
    m_exparse.serror(3);        //throws
  }
  m_exparse.get_exp(&value);
  pVar->setIntValue(value);

  m_exparse.get_token();
  if(tok != TO)
  {
    removeVariable(pNewVar);
    m_exparse.serror(9);        //throws
  }

  int target;
  m_exparse.get_exp(&target);
  if(target > value)
  {
    ForStack i(pVar,target,m_program);
    fpush(i);
  }
  else
  {
    // skip ahead to "NEXT" token as the for loop is something like
    // "for a=9 to 0" that we don't handle.
    while(tok != NEXT) m_exparse.get_token();
  }
}

void
SmallBasic::next()
{
  ForStack i = fpop();
  (*i.m_pVar)++;
  if(i.m_pVar->intValue() <= i.m_iTarget)
  {
    // Not at the end of the loop. Put this for loop back on the stack,
    // and put the source pointer to the beginning of the loop
    fpush(i);
    m_program = i.m_pSourceLoc;
  }
}

bool
SmallBasic::input()
{
  FUNCLOG("SmallBasic::input()");
  DLOG("sb: address = %p",this);
  m_exparse.get_token();
  if(token_type == QUOTE)
  {
    outputStringf("%s",token);
    m_exparse.get_token();
    if(*token != ',' && *token != ';') m_exparse.serror(1);
    m_exparse.get_token();
  }
  else outputString("? ");

  Variable* pVar;
  Variable* pNewVar = NULL;
  pVar = findVariableName(token,m_listVars);
  if(!pVar)
  {
    pNewVar = createNewVariable(token);
    pVar = pNewVar;
  }

  if(pVar->type() == Variable::TYPE_INT)
  {
    int i;
    inputNumber(&i);
    pVar->setIntValue(i);
  }
  else
  {
    char buf[MAX_STRING_LENGTH];
    inputString(buf,sizeof buf);
    pVar->setStringValue(buf);
  }
  return false;
}


void
SmallBasic::inputNumber(int* i)
{
  char buf[MAX_STRING_LENGTH];
  scanf("%s",buf);
  *i = atoi(buf);
}

void
SmallBasic::inputString(char* s,const unsigned32)
{
  scanf("%s",s);
}


void
SmallBasic::gpush(char *s)
{
  gtos++;

  if(gtos == SUB_NEST)
  {
    m_exparse.serror(12);
    return;
  }

  gstack[gtos] = s;
}

char *
SmallBasic::gpop()
{
  if(gtos == 0)
  {
    // a return without a gosub when a subroutine is called from the run
    // method is the same as END
    if(m_bStartedSubroutine)
      return 0;

    m_exparse.serror(13);
    return 0;
  }
  return(gstack[gtos--]);
}

void
SmallBasic::gosub()
{
  char *loc;

  m_exparse.get_token();

  loc = find_label(token);
  if(loc == NULL)
    m_exparse.serror(7);
  else
  {
    gpush(m_program);
    m_program = loc;
  }
}

bool
SmallBasic::greturn()
{
  return (m_program = gpop()) ? true:false;
}


int
SmallBasic::addCommand(char* pszCommand)
{
  int i;
  int tok;
  char szCmd[MAX_STRING_LENGTH];
  strncpy(szCmd,pszCommand,sizeof szCmd);
  NULL_TERMINATE(szCmd,sizeof szCmd);
  //make sure command is all lower case
  for(i=0; i < szCmd[i]; i++)
    szCmd[i] = tolower(szCmd[i]);

  for(i=0; i<MAX_COMMANDS; i++)
  {
    if(*table[i].command == '\0')
    {
      strcpy(table[i].command,szCmd);
      tok = CUSTOM+m_iNumCustomCommands;
      m_iNumCustomCommands++;
      table[i].tok = tok;

      //fix up last item
      i++;
      table[i].command[0] = '\0';
      table[i].tok = END;

      return tok;
    }
  }
  return 0;
}


void
SmallBasic::showKnownCommands()
{

  FUNCLOG("Exparse::showKnownCommands");
  DLOG("exp: addr %p %d commands addr %p",this,m_iNumCustomCommands,table);
  char buf[2024];
  std::list<std::string> commandNameList;
  for(int i = 0; i<MAX_COMMANDS; i++) {
    if(strlen(table[i].command) > 0) {
      snprintf(buf,sizeof(buf),"exp: cmd [%p] [%-20s] tok [%04d]\n", &table[i], table[i].command, table[i].tok);
      std::string s(table[i].command);
      commandNameList.push_back(s);
    }
  }


  commandNameList.sort();
  for(list<string>::iterator it = commandNameList.begin(); it != commandNameList.end(); it++) {
    if(it != commandNameList.begin())
      outputString(", ");
    outputStringf("%s",(*it).c_str());
  }
}
