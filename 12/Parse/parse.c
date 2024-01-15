#include "parse.h"

int main( int argc, char *argv[] )  
{
   test();
   check_args_valid(argc, argv);

   Program * prog = get_program(argv[1]);
   if(!parse_program(prog)){
      free(prog);
      on_error("Cannot parse.");
   }
   free(prog);
   exit(EXIT_SUCCESS);
}

void test(void)
{
   test_parse_num();
   test_parse_ins();
   test_check_ltr();
   test_parse_word();
   test_parse_var();
   test_parse_col();
   test_is_operator();
   test_parse_program();
   test_is_number();
   test_parse_loop();
   test_parse_pfix();   
   test_parse_set();
   test_parse_lst();
   test_parse_items();
   test_parse_varnum();
   test_parse_item();
   test_parse_rgt();
   test_parse_fwd();
   test_parse_inslst();
   test_parse_rgt();
}

void check_args_valid(int argc, char *argv[])
{
   if(argc!=MAX_ARGS){
      on_error("Wrong number of arguments");
   }
   int len = strlen(argv[1]);
   if(len<=MIN_FILENAME_LEN){
      on_error("Invalid filename: too short");
   }
   if(!strsame(&argv[1][len-MIN_FILENAME_LEN],".ttl")){
      on_error("Invalid filename: must end with .ttl");
   }
}

bool is_operator(char c)
{
   return c == '+' || c == '-' || c == '/' || c == '*';
}

void test_is_operator(void)
{
   assert(is_operator('+') == true);
   assert(is_operator('-') == true);
   assert(is_operator('*') == true);
   assert(is_operator('/') == true);
   assert(is_operator('a') == false);
   assert(is_operator('1') == false);
   assert(is_operator('&') == false);
   assert(is_operator(' ') == false);
}


// <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
bool parse_pfix(Program *p)
{
   if(strsame(CURRENT_WORD, ")")){
      p->curr_word++;
      return true;
   }
   if(!parse_op(p) && !parse_varnum(p)){
         return false;
      }
   if(!parse_pfix(p)){
      return false;
   }
   return true;
}

void test_parse_pfix(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // $A + ")"
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"+");
   strcpy(prog->words[2],")");
   assert(parse_pfix(prog));
   assert(prog->curr_word==3);
   prog->curr_word = 0;
   // invalid operator +a
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"+a");
   strcpy(prog->words[2],")");
   assert(!parse_pfix(prog));
   prog->curr_word = 0;
   // invalid varnum $AA + )
   strcpy(prog->words[0],"$AA");
   strcpy(prog->words[1],"+");
   strcpy(prog->words[2],")");
   assert(!parse_pfix(prog));
   prog->curr_word = 0;
   // -8 ")"
   strcpy(prog->words[0],"-8");
   strcpy(prog->words[1],")");
   assert(parse_pfix(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;
   free(prog);
}

// <OP> ::= + - / *
bool parse_op(Program *p)
{
   if(strlen(CURRENT_WORD)!=1){
      return false;
   }
   if(!is_operator(FIRST_LETTER)){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_parse_op(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // invalid too long
   strcpy(prog->words[0],"+1");
   assert(!parse_op(prog));
   prog->curr_word = 0;

   // too short
   strcpy(prog->words[0],"");
   assert(!parse_op(prog));
   prog->curr_word = 0;
   
   // valid 
   strcpy(prog->words[0],"+");
   strcpy(prog->words[1],"-");
   strcpy(prog->words[2],"/");
   strcpy(prog->words[3],"*");
   assert(parse_op(prog));
   assert(prog->curr_word==1);
   assert(parse_op(prog));
   assert(prog->curr_word==2);
   assert(parse_op(prog));
   assert(prog->curr_word==3);
   assert(parse_op(prog));
   assert(prog->curr_word==4);

   prog->curr_word = 0;

   free(prog);

}

// <SET> ::= "SET" <LTR> "(" <PFIX>
bool parse_set(Program *p)
{
   // go past set.
   if(!strsame(CURRENT_WORD,"SET")){
      return false;
   }
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   // go past the letter.
   p->curr_word++;
   if(!strsame(CURRENT_WORD, "(")){
      return false;
   }
   // go past the open bracket.
   p->curr_word++;
   if(!parse_pfix(p)){
      return false;
   }

   return true;
}

void test_parse_set(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // SET A ( -5.3 $A + )
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(parse_set(prog));
   assert(prog->curr_word==7);
   prog->curr_word = 0;
   // set misspelled.
   strcpy(prog->words[0],"SEET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(!parse_set(prog));
   prog->curr_word = 0;
   // not a letter.
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"a");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(!parse_set(prog));
   prog->curr_word = 0;
   // no opening bracket
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"/");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],")");
   assert(!parse_set(prog));
   prog->curr_word = 0;
   // no closing bracket
   strcpy(prog->words[0],"SET");
   strcpy(prog->words[1],"A");
   strcpy(prog->words[2],"(");
   strcpy(prog->words[3],"-5.3");
   strcpy(prog->words[4],"$A");
   strcpy(prog->words[5],"+");
   strcpy(prog->words[6],"+");
   assert(!parse_set(prog));
   prog->curr_word = 0;
   free(prog);
}

// <VAR> ::= $<LTR>
bool parse_var(Program *p)
{
   if(strlen(CURRENT_WORD)<2){
      return false;
   }
   if(FIRST_LETTER!='$'){
      return false;
   }
   if(!check_ltr(&CURRENT_WORD[1])){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_parse_var(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // invalid too long
   strcpy(prog->words[0],"START");
   assert(!parse_var(prog));
   prog->curr_word = 0;

   // too short
   strcpy(prog->words[0],"$");
   assert(!parse_var(prog));
   prog->curr_word = 0;
   
   // no dollar sign
   strcpy(prog->words[0],"10");
   assert(!parse_var(prog));
   prog->curr_word = 0;

   // not a letter.
   strcpy(prog->words[0],"$7");
   assert(!parse_var(prog));
   prog->curr_word = 0;

   // lower case 
   strcpy(prog->words[0],"$a");
   assert(!parse_var(prog));
   prog->curr_word = 0;

   // valid 
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"$Z");
   assert(parse_var(prog));
   assert(prog->curr_word==1);
   assert(parse_var(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   free(prog);
}

// <LST> ::= "{" <ITEMS>
bool parse_lst(Program *p)
{
   if(!strsame(CURRENT_WORD, "{")){
      return false;
   }
   // pass {
   p->curr_word++;
   if(!parse_items(p)){
      return false;
   }
   return true;
}

void test_parse_lst(void)
{
   Program* prog = calloc(1, sizeof(Program));
   // SET A ( -5.3 $A + )
   strcpy(prog->words[0],"{");
   strcpy(prog->words[1],"\"RED\"");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"}");
   assert(parse_lst(prog));
   assert(prog->curr_word==5);
   prog->curr_word = 0;

   // no opening {
   strcpy(prog->words[0],"10");
   strcpy(prog->words[1],"\"RED\"");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"}");
   assert(!parse_lst(prog));
   prog->curr_word = 0;

   // no closing {
   strcpy(prog->words[0],"{");
   strcpy(prog->words[1],"\"RED\"");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"-1");
   assert(!parse_lst(prog));
   prog->curr_word = 0;

   // weird list varialbe string vs "string"
   strcpy(prog->words[0],"{");
   strcpy(prog->words[1],"RED");
   strcpy(prog->words[2],"10");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"}");
   assert(!parse_lst(prog));
   prog->curr_word = 0;

   free(prog);

}

// <ITEMS> ::= "}" | <ITEM> <ITEMS>
bool parse_items(Program *p)
{
   if(strsame(CURRENT_WORD, "}")){
      p->curr_word++;
      return true;
   }
   if(!parse_item(p)){
      return false;
   }
   if(!parse_items(p)){
      return false;
   }
   return true;
}

void test_parse_items(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   
   strcpy(prog->words[0],"$A");
   strcpy(prog->words[1],"}");
   assert(parse_items(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"2");
   strcpy(prog->words[1],"}");
   assert(parse_items(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"}");
   assert(parse_items(prog));
   assert(prog->curr_word==2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"2");
   strcpy(prog->words[3],"3");
   strcpy(prog->words[4],"15");
   strcpy(prog->words[5],"}");
   assert(parse_items(prog));
   assert(prog->curr_word==6);
   prog->curr_word = 0;

   // random bad value in the middle of a list.
   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"2");
   strcpy(prog->words[3],"3asd");
   strcpy(prog->words[4],"15");
   strcpy(prog->words[5],"}");
   assert(!parse_items(prog));
   prog->curr_word = 0;

   // no closing paren.
   strcpy(prog->words[0],"\"RED\"");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"2");
   strcpy(prog->words[3],"3");
   strcpy(prog->words[4],"15");
   strcpy(prog->words[5],"12");
   assert(!parse_items(prog));
   prog->curr_word = 0;

   free(prog);
}

bool is_number(char * str)
{
   int str_index = 0;
   if(str[str_index] == '-'){
      str_index++;
   }
   if(!isdigit(str[str_index]) && str[str_index] != '.'){
      return false;
   }
   while(isdigit((str[str_index]))){
      str_index++;
   }
   if(str[str_index] == '.'){
      str_index++;
      if (!isdigit((str[str_index])) && str[str_index] != '\0'){
            return false;
      }
      while(isdigit((str[str_index]))){
            str_index++;
      }
   }
   return str[str_index] == '\0';
}

void test_is_number(void)
{
   assert(is_number("7"));
   assert(is_number("-17123212"));
   assert(is_number("-7.0"));
   assert(!is_number("t"));
   assert(!is_number("81f"));
   assert(!is_number("as8"));
}

// <VARNUM> ::= <VAR> | <NUM>
bool parse_varnum(Program * p)
{
   if(FIRST_LETTER=='$'){
      if(!parse_var(p)){
         return false;
      }
   }
   else {
      if(!parse_num(p)){
         return false;
      }
   }
   return true;
}

void test_parse_varnum(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   
   strcpy(prog->words[0],"$A");
   assert(parse_varnum(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"1");
   assert(parse_varnum(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"-1.123");
   assert(parse_varnum(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"a1");
   assert(!parse_varnum(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"$2");
   assert(!parse_varnum(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"RED\"");
   assert(!parse_varnum(prog));
   prog->curr_word = 0;

   free(prog);
}

// <ITEM> ::= <VARNUM> | <WORD>
bool parse_item(Program *p)
{
   if(FIRST_LETTER=='$' || is_number(CURRENT_WORD)){
      if(!parse_varnum(p)){
         return false;
      }
   }
   // otherwise it's a word.
   else {
      if(!parse_word(p)){
         return false;
      }
   }
   return true;
}

void test_parse_item(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   
   strcpy(prog->words[0],"$A");
   assert(parse_item(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"1");
   assert(parse_item(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"-1.123");
   assert(parse_item(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"RED\"");
   assert(parse_item(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"a1");
   assert(!parse_item(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"$2");
   assert(!parse_item(prog));
   prog->curr_word = 0;

   free(prog);
}

// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
bool parse_word(Program *p)
{
   int len = strlen(CURRENT_WORD);
   if(len<3){
      return false;
   }
   if(FIRST_LETTER != '"'){
      return false;
   }
   if(CURRENT_WORD[len-1] != '"'){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_parse_word(void)
{
  Program* prog = ncalloc(1, sizeof(Program));
   strcpy(prog->words[0],"\"RED\"");
   assert(parse_word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"BLUE\"");
   assert(parse_word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"HELLO!\"");
   assert(parse_word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   strcpy(prog->words[0],"\"178!\"");
   assert(parse_word(prog));
   assert(prog->curr_word==1);
   prog->curr_word = 0;

   // missing all quotes
   strcpy(prog->words[0],"178!");
   assert(!parse_word(prog));
   assert(prog->curr_word==0);

   // missing opening quotes
   strcpy(prog->words[0],"178!\"");
   assert(!parse_word(prog));
   assert(prog->curr_word==0);

   // missing closing quotes
   strcpy(prog->words[0],"\"178!");
   assert(!parse_word(prog));
   assert(prog->curr_word==0);

   free(prog);
}

// <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
bool parse_col(Program *p)
{
   if(!strsame(CURRENT_WORD,"COLOUR")){
      return false;
   }
   p->curr_word++;
   if(FIRST_LETTER=='$'){
      if(!parse_var(p)){
         return false;
      }
   }
   // otherwise, it's a word.
   else{
      if(!parse_word(p)){
         return false;
      }
   }

   return true;
}

void test_parse_col(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   // "COLOUR" <VAR> 
   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"$A");
   assert(parse_col(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   // "COLOUR" <WORD>
   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"\"HELLO!\"");
   assert(parse_col(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   // not used the word color
   strcpy(prog->words[0],"COLOU");
   strcpy(prog->words[1],"\"HELLO!\"");
   assert(!parse_col(prog));
   prog->curr_word = 0;

   // not used a word
   strcpy(prog->words[0],"COLOUR");
   strcpy(prog->words[1],"A");
   assert(!parse_col(prog));
   prog->curr_word = 0;

   free(prog);
}

bool check_ltr(char * s)
{
   int len = strlen(s);
   if(len>1){
      return false;
   }
   char letter = s[0];
   if(!isupper(letter)){
      return false;
   }
   return true;
}

void test_check_ltr(void)
{
   // too long
   assert(!check_ltr("START"));
   assert(!check_ltr("$START"));

   // not a letter
   assert(!check_ltr("1"));
   assert(!check_ltr("$1"));

   // lowercase
   assert(!check_ltr("a"));
   assert(!check_ltr("$a"));

   // dollar signs.
   assert(!check_ltr("$Z"));
   assert(!check_ltr("$A"));

   // valid 
   assert(check_ltr("A"));
   assert(check_ltr("Z"));

}

// <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
bool parse_loop(Program *p)
{
   // skip "LOOP"
   if(!strsame(CURRENT_WORD,"LOOP")){
      return false;
   }
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   // go past the letter.
   p->curr_word++;
   if(!strsame(CURRENT_WORD, "OVER")){
      return false;
   }
   // go past OVER
   p->curr_word++;
   if(!parse_lst(p)){
      return false;
   }
   if(!parse_inslst(p)){
      return false;
   }
   return true;
}

void test_parse_loop(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"\"RED\"");
   strcpy(prog->words[5],"\"GREEN\"");
   strcpy(prog->words[6],"\"YELLOW\"");
   strcpy(prog->words[7],"\"BLUE\"");
   strcpy(prog->words[8],"}");
   strcpy(prog->words[9],"COLOUR");
   strcpy(prog->words[10],"$C");
   strcpy(prog->words[11],"FORWARD");
   strcpy(prog->words[12],"$D");
   strcpy(prog->words[13],"RIGHT");
   strcpy(prog->words[14],"90");
   strcpy(prog->words[15],"END");
   assert(parse_loop(prog));
   assert(prog->curr_word==15);
   prog->curr_word=0;

   // doesn't include the word LOOP
   strcpy(prog->words[0],"loop");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   assert(!parse_loop(prog));
   prog->curr_word=0;

   // doesn't include OVER
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"over");
   strcpy(prog->words[3],"{");
   assert(!parse_loop(prog));
   prog->curr_word=0;

   // mangled instruction
   strcpy(prog->words[0],"LOOP");
   strcpy(prog->words[1],"C");
   strcpy(prog->words[2],"OVER");
   strcpy(prog->words[3],"{");
   strcpy(prog->words[4],"\"RED\"");
   strcpy(prog->words[5],"\"GREEN\"");
   strcpy(prog->words[6],"YELLOW\""); // MANGLED
   strcpy(prog->words[7],"\"BLUE\"");
   strcpy(prog->words[8],"}");
   strcpy(prog->words[9],"COLOUR");
   strcpy(prog->words[10],"$C");
   strcpy(prog->words[11],"FORWARD");
   strcpy(prog->words[12],"$D");
   strcpy(prog->words[13],"RIGHT");
   strcpy(prog->words[14],"90");
   strcpy(prog->words[15],"END");
   assert(!parse_loop(prog));
   prog->curr_word=0;

   free(prog);
}

// <NUM>    ::= 10 or -17.99 etc.
bool parse_num(Program *p)
{
   if(!is_number(CURRENT_WORD)){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_parse_num(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   strcpy(prog->words[0],"START");
   assert(!parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"1");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"10");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"-17.99");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"-8");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"0");
   assert(parse_num(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"11231230");
   assert(parse_num(prog));
   prog->curr_word = 0;
   free(prog);
}

// <FWD> ::= "FORWARD" <VARNUM>
bool parse_fwd(Program *p)
{
   // advance past FORWARD
   if(!strsame(CURRENT_WORD,"FORWARD")){
      return false;
   }
   p->curr_word++;
   return parse_varnum(p);
}

void test_parse_fwd(void)
 {
   Program* prog = ncalloc(1, sizeof(Program));

   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"$A");
   assert(parse_fwd(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"-123");
   assert(parse_fwd(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"3.0");
   assert(parse_fwd(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   // forward missing.
   strcpy(prog->words[0],"fads");
   strcpy(prog->words[1],"3.0");
   assert(!parse_fwd(prog));
   prog->curr_word = 0;

   free(prog);
 }


// <RGT> ::= "RIGHT" <VARNUM>
bool parse_rgt(Program *p)
{
   if(!strsame(CURRENT_WORD,"RIGHT")){
      return false;
   }
   p->curr_word++;
   return parse_varnum(p);
}

void test_parse_rgt(void)
{
   Program* prog = ncalloc(1, sizeof(Program));

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"$A");
   assert(parse_rgt(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"-123");
   assert(parse_rgt(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"3.0");
   assert(parse_rgt(prog));
   assert(prog->curr_word == 2);
   prog->curr_word = 0;

   // right missing.
   strcpy(prog->words[0],"fads");
   strcpy(prog->words[1],"3.0");
   assert(!parse_rgt(prog));
   prog->curr_word = 0;

   free(prog);
}

// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
bool parse_ins(Program *p)
{
   if(strsame(CURRENT_WORD, "FORWARD")){
      return parse_fwd(p);
   }
   else if(strsame(CURRENT_WORD, "RIGHT")){
      return parse_rgt(p);
   }
   else if(strsame(CURRENT_WORD, "COLOUR")){
      return parse_col(p);
   }
   else if(strsame(CURRENT_WORD, "LOOP")){
      return parse_loop(p);
   }
   else if(strsame(CURRENT_WORD, "SET")){
      return parse_set(p);
   }
   return false;
}

void test_parse_ins(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"123");
   assert(parse_ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"-82.43");
   assert(parse_ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"$Z");
   assert(parse_ins(prog));
   prog->curr_word = 0;

   // testing with random strings and non numbers.
   strcpy(prog->words[0],"MOW");
   strcpy(prog->words[1],"-82.43");
   assert(!parse_ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"far");
   assert(!parse_ins(prog));
   prog->curr_word = 0;

   free(prog);
}

// <INSLST> ::= "END" | <INS> <INSLST>
bool parse_inslst(Program *p)
{
   if(strsame(CURRENT_WORD, "END")){
      return true;
   }
   if(!parse_ins(p)){
      return false;
   }
   if(!parse_inslst(p)){
      return false;
   }
   return true;
}

void test_parse_inslst(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   strcpy(prog->words[0],"END");
   assert(parse_inslst(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"END");
   assert(parse_inslst(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"RIGHT");
   strcpy(prog->words[3],"$A");
   strcpy(prog->words[4],"SET");
   strcpy(prog->words[5],"A");
   strcpy(prog->words[6],"(");
   strcpy(prog->words[7],"-5.3");
   strcpy(prog->words[8],"$A");
   strcpy(prog->words[9],"+");
   strcpy(prog->words[10],")");
   strcpy(prog->words[11],"END");
   assert(parse_inslst(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"END!");
   assert(!parse_inslst(prog));
   prog->curr_word = 0;
   free(prog);
}

// <PROG>   ::= "START" <INSLST>
bool parse_program(Program *p)
{
   if(!strsame(CURRENT_WORD, "START")){
      return false;
   }
   p->curr_word++;
   return parse_inslst(p);
}

void test_parse_program(void)
{
   Program* prog = ncalloc(1, sizeof(Program));
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"END");
   assert(parse_program(prog));
   prog->curr_word = 0;

   // starting with END should fail
   strcpy(prog->words[0],"END");
   assert(!parse_program(prog));
   prog->curr_word = 0;

   // ending with START should fail
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"START");
   assert(!parse_program(prog));
   prog->curr_word = 0;

   // valid files should increment curr_word
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"FORWARD");
   strcpy(prog->words[2],"5");
   strcpy(prog->words[3],"RIGHT");
   strcpy(prog->words[4],"5.2");
   strcpy(prog->words[5],"FORWARD");
   strcpy(prog->words[6],"-2.2");
   strcpy(prog->words[7],"END");
   assert(parse_program(prog));
   assert(prog->curr_word==7);
   prog->curr_word = 0;

   // testing that a  recursive function suceeds.
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"FORWARD");
   strcpy(prog->words[2],"5");
   strcpy(prog->words[3],"LOOP");
   strcpy(prog->words[4],"C");
   strcpy(prog->words[5],"OVER");
   strcpy(prog->words[6],"{");
   strcpy(prog->words[7],"\"RED\"");
   strcpy(prog->words[8],"\"GREEN\"");
   strcpy(prog->words[9],"\"YELLOW\"");
   strcpy(prog->words[10],"\"BLUE\"");
   strcpy(prog->words[11],"}");
   strcpy(prog->words[12],"COLOUR");
   strcpy(prog->words[13],"$C");
   strcpy(prog->words[14],"FORWARD");
   strcpy(prog->words[15],"$D");
   strcpy(prog->words[16],"RIGHT");
   strcpy(prog->words[17],"90");
   strcpy(prog->words[18],"END");
   assert(parse_program(prog));
   assert(prog->curr_word==18);
   prog->curr_word = 0;

   // testing failure when one string is modified.
   strcpy(prog->words[0],"START");
   strcpy(prog->words[1],"FORWARD");
   strcpy(prog->words[2],"5");
   strcpy(prog->words[3],"LOOP");
   strcpy(prog->words[4],"C");
   strcpy(prog->words[5],"OVER");
   strcpy(prog->words[6],"{");
   strcpy(prog->words[7],"\"RED\"");
   strcpy(prog->words[8],"\"GREEN\"");
   strcpy(prog->words[9],"\"YELLOW\"");
   strcpy(prog->words[10],"\"BLUE"); // modified line.
   strcpy(prog->words[11],"}");
   strcpy(prog->words[12],"COLOUR");
   strcpy(prog->words[13],"$C");
   strcpy(prog->words[14],"FORWARD");
   strcpy(prog->words[15],"$D");
   strcpy(prog->words[16],"RIGHT");
   strcpy(prog->words[17],"90");
   strcpy(prog->words[18],"END");
   assert(!parse_program(prog));
   prog->curr_word = 0;

   free(prog);
}

Program * get_program(char * prog_name)
{
   FILE * f = nfopen(prog_name,"r");
   Program* prog = ncalloc(1, sizeof(Program));
   int i=0;
   while(fscanf(f, "%s", prog->words[i])==1 && i<MAXNUMTOKENS){
      if(strlen(prog->words[i])>=MAXTOKENSIZE-1){
         on_error("File contains words that are too long.");
      }
      i++;
   };
   if(i>=MAXNUMTOKENS){
      on_error("File is too long (too many words).");
   }
   fclose(f);
   return prog;
}
