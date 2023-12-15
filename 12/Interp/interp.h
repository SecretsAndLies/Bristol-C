#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "../neillsimplescreen.h"

#define CURRENT_WORD p->words[p->curr_word]
#define FIRST_LETTER CURRENT_WORD[0]
#define strsame(A,B) (strcmp(A, B)==0)

#define MAXNUMTOKENS 1000
#define MAXTOKENSIZE 100

#define TURTLE "#"
#define TIME_TO_WAIT 1

#define SCREEN_HEIGHT 51
#define SCREEN_WIDTH 33
#define SCREEN_MID_ROW SCREEN_HEIGHT/2
#define SCREEN_MID_COL SCREEN_WIDTH/2

#define START_ANGLE 180

#define DEBUG //printf("%s %s %i\n",CURRENT_WORD, __func__, __LINE__);

#define NUM_VARIABLES 27 // this is 26 + a constant 1
#define CONST_VAR_INDEX 26 // 0-25 is A-Z

typedef enum {
  NONE,
  PS,
  TXT
} File_Type; 

typedef enum {
  INT,
  STRING
} Var_Type; 

typedef struct Variable{
   Var_Type var_type;
   char str_var[MAXTOKENSIZE];
   int int_var;
} Variable;

typedef struct Turtle
{
   double x;
   double y;
   char output[SCREEN_WIDTH][SCREEN_HEIGHT]; 
   double angle;
   neillcol col;
   Variable variables[NUM_VARIABLES];
} Turtle;

typedef struct Program{
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int curr_word;
   Turtle ttl;
} Program;

void* nfopen(char* fname, char* mode);
void on_error(const char* s);

Program * get_program(char * prog_name);

bool program(Program *p);
void test_program(void);

bool inslst(Program *p);

bool ins(Program *p);
void test_ins(void);

bool rgt(Program *p);
// tested in test_ins

bool fwd(Program *p);
// tested in test_ins 

bool num(Program *p);
void test_num(void);

void test(void);

bool col(Program *p);
void test_col(void);

bool loop(Program *p);
void test_loop(void);

bool set(Program *p);

bool check_ltr(char * s);
void test_check_ltr(void);

bool var(Program *p);
void test_var(void);

bool word(Program *p);
void test_word(void);

bool lst(Program *p);

bool items(Program *p);

bool item(Program *p);

bool varnum(Program * p);

bool is_operator(char c);
void test_is_operator(void);

bool op(Program *p);
void test_op(void);

void test_get_number(void);
bool get_number(char * str, double * num);



void cursor_goto(int row, int col);
// no testing needed.


