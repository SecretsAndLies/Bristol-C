#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include "../General/general.h"

#define CURRENT_WORD p->words[p->curr_word]
#define FIRST_LETTER CURRENT_WORD[0]
#define STRSAME(A,B) (strcmp(A, B)==0)

#define MAXNUMTOKENS 10000
#define MAXTOKENSIZE 1000
#define MAX_ARGS 2
#define MIN_FILENAME_LEN 4

typedef struct Program{
   char words[MAXNUMTOKENS][MAXTOKENSIZE];
   int curr_word;
} Program;

Program * get_program(char * prog_name);

bool parse_program(Program *p);
void test_parse_program(void);

bool parse_inslst(Program *p);
void test_parse_inslst(void);

bool parse_ins(Program *p);
void test_parse_ins(void);

bool parse_rgt(Program *p);
void test_parse_rgt(void);

bool parse_fwd(Program *p);
void test_parse_fwd(void);

bool parse_num(Program *p);
void test_parse_num(void);

void test(void);

bool parse_col(Program *p);
void test_parse_col(void);

bool parse_loop(Program *p);
void test_parse_loop(void);

bool parse_set(Program *p);
void test_parse_set(void);

bool check_ltr(char * s);
void test_check_ltr(void);

bool parse_var(Program *p);
void test_parse_var(void);

bool parse_word(Program *p);
void test_parse_word(void);

bool parse_lst(Program *p);
void test_parse_lst(void);

bool parse_items(Program *p);
void test_parse_items(void);

bool parse_item(Program *p);
void test_parse_item(void);

bool parse_varnum(Program * p);
void test_parse_varnum(void);

bool is_operator(char c);
void test_is_operator(void);

bool parse_op(Program *p);
void test_parse_op(void);

bool is_number(char * str);
void test_is_number(void);

// tested in the bash script.
void check_args_valid(int argc, char *argv[]);

void test_parse_pfix(void);
bool parse_pfix(Program *p);
