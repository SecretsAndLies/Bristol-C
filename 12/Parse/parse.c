#include "parse.h"
// <PROG> ::= "START" <INSLST>
// <INSLST> ::= "END" | <INS> <INSLST>
// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
// <FWD> ::= "FORWARD" <VARNUM>
// <RGT> ::= "RIGHT" <VARNUM>
// <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
// <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
// <SET> ::= "SET" <LTR> "(" <PFIX>
// <VARNUM> ::= <VAR> | <NUM>
// % Variables e.g. $A, $B, $Z etc.
// <VAR> ::= $<LTR>
// % One Uppercase letter
// <LTR> ::= A, B ... Z
// % Any valid double (as defined by scanf("%lf"...)
// <NUM> ::= 10 or -17.99 etc.
// % A single word (as defined by scanf("%s"...) with double-quotes around it
// % Valid colours include "BLACK", "RED", "GREEN", "BLUE",
// % "YELLOW", "CYAN", "MAGENTA", "WHITE"
// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
// <LST> ::= "{" <ITEMS>
// <ITEMS> ::= "}" | <ITEM> <ITEMS>
// <ITEM> ::= <VARNUM> | <WORD>
// <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
// % A single mathematical operation character
// <OP> ::= + - / *


// https://docs.google.com/spreadsheets/d/1jdvzG2hEDDXJ83oQDXqG4Nl4RXmUWUHoT9VOMphs4Qk/edit#gid=0
int main( int argc, char *argv[] )  
{
   test();
   if(argc!=2){
      on_error("Wrong number of arguments.");
   }
   Program * prog = get_program(argv[1]);
   if(parse_program(prog)){
      puts("PARSED!"); // TODO REMOVE.
   }
   else{
      free(prog);
      on_error("Cannot parse.");
   }
   free(prog);
   return 0;
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
   DEBUG
   if(strsame(CURRENT_WORD, ")")){
      p->curr_word++;
      return true;
   }
   if(is_operator(FIRST_LETTER))
   {
      if(!parse_op(p)){
         return false;
      }
   }
   else {
      if(!parse_varnum(p)){
         return false;
      }
   }

   // in both cases you need to parse_prefix.
   if(!parse_pfix(p)){
      return false;
   }

   return true;
}

// <OP> ::= + - / *
bool parse_op(Program *p)
{
   DEBUG
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
   Program* prog = calloc(1, sizeof(Program));
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
   DEBUG
   // go past set.
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   // go past the letter.
   p->curr_word++;
   if(!strsame(CURRENT_WORD, "(")){
      DEBUG
      return false;
   }
   // go past the open bracket.
   p->curr_word++;
   if(!parse_pfix(p)){
      return false;
   }

   return true;
}

// <VAR> ::= $<LTR>
bool parse_var(Program *p)
{
   DEBUG
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
   Program* prog = calloc(1, sizeof(Program));
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
   DEBUG
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

// <ITEMS> ::= "}" | <ITEM> <ITEMS>
bool parse_items(Program *p)
{
   DEBUG
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

bool is_number(char * str)
{
   double a;
   if(sscanf(str,"%lf", &a)!=1){
      return false;
   }
   a=0; // switching off warnings :-(
   return true;
}

void test_is_number(void)
{
   assert(is_number("7"));
   assert(is_number("-7.0"));
   assert(!is_number("t"));
}

// <VARNUM> ::= <VAR> | <NUM>
bool parse_varnum(Program * p)
{
   DEBUG
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

// <ITEM> ::= <VARNUM> | <WORD>
bool parse_item(Program *p)
{
   DEBUG
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

// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
bool parse_word(Program *p)
{
   DEBUG
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
  Program* prog = calloc(1, sizeof(Program));
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
   DEBUG
   // skip past COLOUR
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
   Program* prog = calloc(1, sizeof(Program));
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
   prog->curr_word = 2;

   // TODO test invalid var and words? or are we happy that they're tested in their functions?

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
   DEBUG
   // skip "LOOP"
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
   Program* prog = calloc(1, sizeof(Program));
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
   free(prog);
}

// <NUM>    ::= 10 or -17.99 etc.
bool parse_num(Program *p)
{
   DEBUG
   if(!is_number(CURRENT_WORD)){
      return false;
   }
   p->curr_word++;
   return true;
}

void test_parse_num(void)
{
   Program* prog = calloc(1, sizeof(Program));
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
   DEBUG
   // advance past FORWARD
   p->curr_word++;
   return parse_varnum(p);
}


// <RGT> ::= "RIGHT" <VARNUM>
bool parse_rgt(Program *p)
{
   DEBUG
   // advance past RIGHT
   p->curr_word++;
   return parse_varnum(p);
}

// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
bool parse_ins(Program *p)
{
   DEBUG
   // todo switch?
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
   // if ins contains none of those words, 
   // it's an invalid instruction.
   return false;
}

void test_parse_ins(void)
{
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"123");
   assert(parse_ins(prog));
   prog->curr_word = 0;
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"-82.43");
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
   DEBUG
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
   Program* prog = calloc(1, sizeof(Program));
   strcpy(prog->words[0],"END");
   assert(parse_inslst(prog));
   prog->curr_word = 0;

   strcpy(prog->words[0],"s");
   strcpy(prog->words[1],"START");
   strcpy(prog->words[2],"START");
   strcpy(prog->words[3],"START");
   strcpy(prog->words[4],"END");
   assert(parse_inslst(prog));
   prog->curr_word = 0;

   free(prog);}

// <PROG>   ::= "START" <INSLST>
bool parse_program(Program *p)
{
   DEBUG
   if(!strsame(CURRENT_WORD, "START")){
      return false;
   }
   p->curr_word++;
   return parse_inslst(p);
}

void test_parse_program(void)
{
   Program* prog = calloc(1, sizeof(Program));
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

   free(prog);
}

Program * get_program(char * prog_name)
{
   FILE * f = nfopen(prog_name,"r");
   Program* prog = calloc(1, sizeof(Program));
   int i=0;
   while(fscanf(f, "%s", prog->words[i++])==1 && i<MAXNUMTOKENS);
   if(i>=MAXNUMTOKENS){
      on_error("File contains too many tokens to parse.");
   }
   fclose(f);
   return prog;
}

void* nfopen(char* fname, char* mode)
{
   FILE* fp;
   fp = fopen(fname, mode);
   if(!fp){
      on_error("Cannot open file");
   }
   return fp;
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}