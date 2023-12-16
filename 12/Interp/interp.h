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
#define MAX_ARGS 3
#define INPUT_FILE_INDEX 1
#define OUTPUT_FILE_INDEX 2

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
  NUM,
  STRING
} Var_Type; 

typedef struct Variable{
   Var_Type var_type;
   char str_var[MAXTOKENSIZE];
   double num_var; 
   bool is_set;
} Variable;

typedef struct Turtle
{
   double x;
   double y;
   char output[SCREEN_WIDTH][SCREEN_HEIGHT]; 
   double angle;
   neillcol col;
} Turtle;

typedef struct Program{
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int curr_word;
   Turtle ttl;
   Variable variables[NUM_VARIABLES];
   Variable curr_var;
} Program;

void* nfopen(char* fname, char* mode);
void on_error(const char* s);

Program * get_program(char * prog_name);

bool run_program(Program *p);
void test_run_program(void);

bool run_inslst(Program *p);
void test_run_inslst(void);

bool run_ins(Program *p);
void test_run_ins(void);

bool run_rgt(Program *p);
// tested in test_ins

bool run_fwd(Program *p);
// tested in test_ins 

bool run_num(Program *p);
void test_run_num(void);

void test(void);

bool run_col(Program *p);
void test_run_col(void);

bool run_loop(Program *p);
void test_run_loop(void);

bool run_set(Program *p);

bool check_ltr(char * s);
void test_check_ltr(void);

bool run_var(Program *p);
void test_run_var(void);

bool run_word(Program *p);
void test_run_word(void);

bool run_lst(Program *p);

bool run_items(Program *p);

bool run_item(Program *p);

bool run_varnum(Program * p);

bool is_operator(char c);
void test_is_operator(void);

bool run_op(Program *p);
void test_run_op(void);

void test_get_number(void);
bool get_number(char * str, double * num);



void cursor_goto(int row, int col);
// no testing needed. pretty sure i can delte.

void check_args_valid(int argc, char *argv[]);

bool is_valid_filename(char * filename, char * ext);
void test_is_valid_filename(void);

bool get_number_from_variable(char var, double * num, Program * prog);
void test_get_number_from_variable(void);











