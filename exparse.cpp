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

/**********************************************************/

/**********************************************************/
/* SMALL BASIC Interpeter (sbasic.c) V0.2                 */
/* Copyright (C) 1994 James J. Bielman                    */
/* Adapted from _C Power User's Guide_ by Herbert Schildt */
/**********************************************************/

#include <ssobjects/logs.h>

#include "smallbasic.h"
#include "exparse.h"

//  extern char *prog;	/* Expression to be analyzed */
//  extern int *variables;
//  extern int source_line;

extern struct commands table[];

Exparse::Exparse(SmallBasic* pbasic) : m_pbasic(pbasic)
{
  DLOG("exp: constructed at address %p",this);
}


/* Main entry point into parser */
int
Exparse::get_exp(int *piResult,char* pszResult)
{
  int type;
  get_token();

  if(!*m_pbasic->token)
  {
    serror(2);  //throws
    return 0;
  }
  if('$' == m_pbasic->token[strlen(m_pbasic->token)-1] ||
     m_pbasic->token_type == QUOTE)
  {
    if(m_pbasic->token_type == QUOTE)
    {
      //TODO: need to have a max length for result to keep rogue scripts at bay
      if(pszResult)
        strncpy(pszResult,m_pbasic->token,MAX_STRING_LENGTH);
    }
    else
    {
      Variable* v = find_string_var(m_pbasic->token);
      strcpy(pszResult,v->stringValue());
    }
    type = Variable::TYPE_STRING;
  }
  else
  {
    level2(piResult);
    putback();
    type = Variable::TYPE_INT;
  }
  return type;
}

/* +- */
void
Exparse::level2(int *result)
{
  char op;
  int hold;

  level3(result);
  while((op = *m_pbasic->token) == '+' || op == '-')
  {
    get_token();
    level3(&hold);
    arith(op, result, &hold);
  }
}

/* /%* */
void
Exparse::level3(int *result)
{
  char op;
  int hold;

  level4(result);
  while((op = *m_pbasic->token) == '*' || op == '/' || op == '%')
  {
    get_token();
    level4(&hold);
    arith(op, result, &hold);
  }
}

void
Exparse::level4(int *result)
{
  int hold;

  level5(result);
  while(*m_pbasic->token == '^')
  {
    get_token();
    level4(&hold);
    arith('^', result, &hold);
  }
}

void
Exparse::level5(int *result)
{
  char op;

  op = 0;
  if((m_pbasic->token_type == DELIMITER) && *m_pbasic->token == '+' || *m_pbasic->token == '-')
  {
    op = *m_pbasic->token;
    get_token();
  }
  level6(result);
  if(op) unary(op, result);
}

void
Exparse::level6(int *result)
{
  if((*m_pbasic->token == '(') && (m_pbasic->token_type == DELIMITER))
  {
    get_token();
    level2(result);
    if(*m_pbasic->token != ')')
      serror(1);
    get_token();
  }
  else primitive(result);
}

void
Exparse::primitive(int *result)
{
  switch(m_pbasic->token_type)
  {
    case VARIABLE:
      *result = find_var(m_pbasic->token);
      get_token();
      return;

    case NUMBER:
      *result = atoi(m_pbasic->token);
      get_token();
      return;

    default:
      serror(0);
  }
}

void
Exparse::arith(char o, int *r, int *h)
{
  int t, ex;

  switch(o)
  {
    case '-':
      *r = *r - *h;
      break;

    case '+':
      *r = *r + *h;
      break;

    case '*':
      *r = *r * *h;
      break;

    case '/':
      *r = (*r) / (*h);
      break;

    case '%':
      t = (*r) / (*h);
      *r = *r - (t * (*h));
      break;

    case '^':
      ex = *r;
      if(*h == 0)
      {
        *r = 1;
        break;
      }
      for(t = *h-1; t > 0; --t) *r = (*r) * ex;
      break;
  }
}

void
Exparse::unary(char o, int *r)
{
  if(o == '-') *r = -(*r);
}

int
Exparse::find_var(char *s)
{
  if(!isalpha(*s))
  {
    serror(4);
    return 0;
  }

  Variable* pVar = m_pbasic->findVariableName(s,m_pbasic->m_listVars);
  if(!pVar)
    pVar = m_pbasic->createNewVariable(const_cast<const char*>(s));

  return pVar->intValue();
}


Variable*
Exparse::find_string_var(char* s)
{
  if(!isalpha(*s))
  {
    serror(4);
    return 0;
  }

  Variable* pVar = m_pbasic->findVariableName(s,m_pbasic->m_listVars);
  if(!pVar)
    pVar = m_pbasic->createNewVariable(const_cast<const char*>(s));

  return pVar;
}


void
Exparse::serror(int error)
{
  static char *e[] = {
  "Syntax error",               //0
  "Unbalanced parens",          //1
  "No expression present",      //2
  "Expected: =",                //3
  "Not a variable",             //4
  "Lable table full",           //5
  "Duplicate label",            //6
  "Undefined label",            //7
  "Expected: THEN",             //8
  "Expected: TO",               //9
  "Too many nested FOR loops",  //10
  "NEXT without FOR",           //11
  "Too many nested GOSUBs",     //12
  "RETURN without GOSUB",       //13
  "File not found",             //14
  "Undefined variable",         //15
  };

  throwSmallBasicException(FmtString("Line %d: %s",m_pbasic->source_line,e[error]));
}

//TODO: fox crash when return is at the end of an if statement
//TODO: fix crash when there is spaces at the last empty line of source code

int
Exparse::get_token()
{
  char *temp;

  m_pbasic->token_type = m_pbasic->tok = 0;
  temp = m_pbasic->token;
  if(*m_pbasic->m_program == EOF)
  {
    *m_pbasic->token = 0;
    m_pbasic->tok = SB_FINISHED;
    return(m_pbasic->token_type = DELIMITER);
  }

  while(iswhite(*m_pbasic->m_program)) m_pbasic->m_program++;

  // check for "#include"
  if(*m_pbasic->m_program == '#')
  {
    // When the token comes back, it is thought to be a variable until we do this check
    m_pbasic->m_program++;
    if(get_token() != VARIABLE || strcasecmp(m_pbasic->token,"include"))
      serror(0);
    if(get_token() != QUOTE)
      serror(0);
    return m_pbasic->token_type = INCLUDE;
  }

  // check for a comment
  if(*m_pbasic->m_program == '\'')
  {
    while(*m_pbasic->m_program!='\n' && *m_pbasic->m_program!='\r')
      m_pbasic->m_program++;
    if(*m_pbasic->m_program=='\r')  //skip over \n\r
        m_pbasic->m_program++;
    m_pbasic->source_line++;
    m_pbasic->tok=SB_EOL;
    *m_pbasic->token = '\n';
    m_pbasic->token[1] = 0;
    return(m_pbasic->token_type = DELIMITER);
  }

  if(*m_pbasic->m_program == ':')
  {
    m_pbasic->m_program++;
    *m_pbasic->token = ':';
    m_pbasic->token[1] = 0;
    return(m_pbasic->token_type = DELIMITER);
  }

  if(*m_pbasic->m_program == '\n' || *m_pbasic->m_program == '\r')
  {
    m_pbasic->m_program++;
    m_pbasic->source_line++;
    m_pbasic->tok = SB_EOL;
    *m_pbasic->token = '\n';
    m_pbasic->token[1] = 0;
    return(m_pbasic->token_type = DELIMITER);
  }

  if(strchr("+-*^/%=;(),<>", *m_pbasic->m_program))
  {
    *temp = *m_pbasic->m_program;
    m_pbasic->m_program++;
    temp++;
    *temp = 0;
    return(m_pbasic->token_type = DELIMITER);
  }

  if(*m_pbasic->m_program == '"')
  {
    m_pbasic->m_program++;
    while(*m_pbasic->m_program != '"' && *m_pbasic->m_program != '\r')
      *temp++ = *m_pbasic->m_program++;
    if(*m_pbasic->m_program == '\n') serror(1);
    m_pbasic->m_program++;
    *temp = 0;
    return(m_pbasic->token_type = QUOTE);
  }

  if(isdigit(*m_pbasic->m_program))
  {
    while(!isdelim(*m_pbasic->m_program)) *temp++ = *m_pbasic->m_program++;
    *temp = 0;
    return(m_pbasic->token_type = NUMBER);
  }

  if(isalpha(*m_pbasic->m_program))
  {
    while(!isdelim(*m_pbasic->m_program))
      *temp++ = *m_pbasic->m_program++;

    // check if this string is in fact a label ("line number")
    if(*(temp-1) == ':')
    {
      *(temp-1) = 0;
      m_pbasic->token_type = LABEL;
    }
    else
      m_pbasic->token_type = STRING;
  }

  *temp = '\0';

  if(m_pbasic->token_type == STRING)
  {
    m_pbasic->tok = look_up(m_pbasic->token);
    if(!m_pbasic->tok)
    {
      m_pbasic->token_type = VARIABLE;
    }
    else
      m_pbasic->token_type = COMMAND;
  }
  return m_pbasic->token_type;
}

void
Exparse::putback()
{
  char *t;

  if(m_pbasic->tok == SB_EOL) m_pbasic->source_line--;

  t = m_pbasic->token;
  for(; *t; t++) m_pbasic->m_program--;
}

int
Exparse::look_up(char *s)
{
  int i;
  char *p;
  p = s;

  while(*p) { *p = tolower(*p); p++; }
  for(i = 0; *table[i].command; i++)
    if(!strcmp(table[i].command, s))
    {
#ifdef DEBUG
//      printf("found cmd %s\n",s);
#endif
      return table[i].tok;
    }
  return 0;
}

int
Exparse::isdelim(char c)
{
  if(strchr(" ;,+-<>/*%^=()", c) || c == 9 || c == '\n'|| c == '\r' || c == 0)
    return 1;
  return 0;
}

int
Exparse::iswhite(char c)
{
  if(c == ' ' || c == '\t') return 1;
  else return 0;
}
