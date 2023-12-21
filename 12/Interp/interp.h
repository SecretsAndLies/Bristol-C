#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include "../neillsimplescreen.h"
#include "stack.h"
#include "../General/general.h"

#define CURRENT_WORD p->words[p->curr_word]
#define FIRST_LETTER CURRENT_WORD[0]
#define strsame(A,B) (strcmp(A, B)==0)

#define MAXNUMTOKENS 10000
#define MAXTOKENSIZE 1000
#define MAX_ARGS 3
#define INPUT_FILE_INDEX 1
#define OUTPUT_FILE_INDEX 2
#define OUTPUTCOMMANDLEN 250
#define MEDIUMSTR 500
#define SMALLSTR 100

#define TIME_TO_WAIT 1

#define SCREEN_HEIGHT 33
#define SCREEN_WIDTH 51
#define START_Y SCREEN_HEIGHT/2
#define START_X SCREEN_WIDTH/2

#define A4_HEIGHT 80
#define A4_WIDTH 60

#define START_Y_PS A4_HEIGHT/2
#define START_X_PS A4_WIDTH/2

#define FLOAT_PRECISION 0.0000001

#define START_ANGLE 90

#define DEBUG //printf("%s %s %i\n",CURRENT_WORD, __func__, __LINE__);

#define NUM_VARIABLES 26 //num letters in alpabet

#define PI 3.14159265358979
#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0 / PI))
#define DEGREES_TO_RADIANS(degrees) ((degrees) * (PI / 180.0))

#define MAX_DEGREES 360.0

#define LOOP_BACK_OFFSET 2

typedef enum {
  SCREEN,
  PS,
  TXT
} File_Type; 

typedef enum {
   NOT_SET,
   NUM,
   STRING
} Var_Type; 

typedef struct Variable{
   Var_Type var_type;
   char str_var[MAXTOKENSIZE];
   double num_var; 
} Variable;

typedef struct Turtle
{
   double x;
   double y;
   double angle;
   double distance;
   neillcol col;
} Turtle;

typedef struct Program{
   char output[SCREEN_HEIGHT][SCREEN_WIDTH]; 
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int curr_word;
   Turtle ttl;
   Variable variables[NUM_VARIABLES];
   Variable curr_var;
   File_Type output_location;
   char output_file_name[SMALLSTR];
   stack * stck;
   char ps_output[MAXNUMTOKENS][MAXTOKENSIZE];
   int ps_num_words;
} Program;

Program * init_program(char * argv[], int argc);

void get_prog_from_file(char * filename, Program * prog);
void init_prog_variables(Program * prog);
void set_prog_output_to_spaces(Program * p);

bool run_program(Program *p);
void test_run_program(void);

bool run_inslst(Program *p);
void test_run_inslst(void);

bool run_ins(Program *p);
void test_run_ins(void);

bool run_rgt(Program *p);
// tested in test_ins

bool run_fwd(Program *p);
void test_run_fwd(void);

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

bool eval_operator(char op, Program * p);

void test_get_number(void);
bool get_number(char * str, double * num);

void eval_args(int argc, char *argv[], File_Type * ft);
void test_eval_args(void);

bool is_valid_filename(char * filename, char * ext);
void test_is_valid_filename(void);


void set_variable_to_string(char var, Program * p, char * str);
void set_variable_to_num(char var, Program * p, double num);
bool get_var_from_variables(char var, Program * prog);
void test_get_and_set_variables(void);

bool fequal(double a, double b);
void test_fequal(void);

bool go_fwd(Program * p);

void print_arr_to_screen(Program * p);
// no tests needed.

bool write_turtle_to_arr(Program * p, int r, int c);

bool is_out_of_bounds(Program * p);

void print_letter_w_colour_code(char letter);

void test_add_to_angle(void);
double add_to_angle(double current_angle, double add_value);

void test_run_set(void);

void free_prog(Program * p);

bool run_pfix(Program *p);
void test_run_pfix(void);

void print_arr_to_txt_file(Program * p);

bool set_col(Program * p);

bool execute_loop(Program * p, char letter);

void append_to_file(char * filename, char * content);
void write_start_text_to_ps(char * filename);

void write_prog_text_to_ps(Program * prog);

void handle_ps_output(Program * prog);