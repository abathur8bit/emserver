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

********************************************************************/

#ifndef EXPARSE_H
#define EXPARSE_H

#include "smallbasic.h"

#include <setjmp.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
//#include <string.h>

#include "variable.h"

//#define DELIMITER	1
//#define VARIABLE	2
//#define NUMBER		3
//#define COMMAND		4
//#define STRING		5
//#define QUOTE		6
//#define SB_EOL		9
//#define SB_FINISHED	10

class SmallBasic;

class Exparse
{
    friend class SmallBasic;

  protected:
    SmallBasic* m_pbasic;

  public:
    Exparse(SmallBasic* psbasic);

    int  get_exp(int *piResult,char* pszResult=NULL);
    void level2(int *result);
    void level3(int *result);
    void level4(int *result);
    void level5(int *result);
    void level6(int *result);
    void primitive(int *result);
    void arith(char o, int *r, int *h);
    void unary(char o, int *r);
    int  find_var(char *s);
    Variable* find_string_var(char *s);
    void serror(int error);
    int  get_token();
    void putback();
    int  look_up(char *s);
    int  isdelim(char c);
    int  iswhite(char c);
};

#endif
