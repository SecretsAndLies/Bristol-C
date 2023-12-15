#include "interp.h"

// TODO delete debug
int main( int argc, char *argv[] )  
{
   test();
   if(argc!=2){
      on_error("Wrong number of arguments.");
   }
   // todo validate what the arg1 is.
   Program * prog = get_program(argv[1]);
   if(!program(prog)){
      free(prog);
      on_error("Cannot parse.");
   }
   free(prog);
   exit(EXIT_SUCCESS);
}

void test(void)
{
   test_num();
   test_ins();
   test_check_ltr();
   test_word();
   test_var();
   test_col();
   test_is_operator();
   test_program();
   test_get_number();
   test_loop();
}

bool is_operator(char c)
{
   return c == '+' || c == '-' || c == '/' || c == '*';
}

void test_is_operator(void)
{
}


// <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
bool pfix(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, ")")){
      p->curr_word++;
      return true;
   }
   if(is_operator(FIRST_LETTER))
   {
      if(!op(p)){
         return false;
      }
   }
   else {
      if(!varnum(p)){
         return false;
      }
   }

   // in both cases you need to prefix.
   if(!pfix(p)){
      return false;
   }

   return true;
}

// <OP> ::= + - / *
bool op(Program *p)
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

void test_op(void)
{

}

// <SET> ::= "SET" <LTR> "(" <PFIX>
bool set(Program *p)
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
   if(!pfix(p)){
      return false;
   }

   return true;
}

// <VAR> ::= $<LTR>
bool var(Program *p)
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

void test_var(void)
{
}

// <LST> ::= "{" <ITEMS>
bool lst(Program *p)
{
   DEBUG
   if(!strsame(CURRENT_WORD, "{")){
      return false;
   }
   // pass {
   p->curr_word++;
   if(!items(p)){
      return false;
   }
   return true;
}

// <ITEMS> ::= "}" | <ITEM> <ITEMS>
bool items(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, "}")){
      p->curr_word++;
      return true;
   }
   if(!item(p)){
      return false;
   }
   if(!items(p)){
      return false;
   }
   return true;
}

// todo this code is identifcal to get number but returns a number.
bool get_number(char * str, double * num)
{
   if(sscanf(str,"%lf", num)!=1){
      return false;
   }
   return true;
}

void test_get_number(void)
{
}

// <VARNUM> ::= <VAR> | <NUM>
bool varnum(Program * p)
{
   DEBUG
   if(FIRST_LETTER=='$'){
      if(!var(p)){
         return false;
      }
   }
   else {
      if(!num(p)){
         return false;
      }
   }
   return true;
}

// <ITEM> ::= <VARNUM> | <WORD>
bool item(Program *p)
{
   DEBUG
   double num;
   if(FIRST_LETTER=='$' || get_number(CURRENT_WORD,&num)){
      if(!varnum(p)){
         return false;
      }
   }
   // otherwise it's a word.
   else {
      if(!word(p)){
         return false;
      }
   }
   return true;
}

// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
bool word(Program *p)
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

void test_word(void)
{
}

// <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
bool col(Program *p)
{
   DEBUG
   // skip past COLOUR
   p->curr_word++;
   if(FIRST_LETTER=='$'){
      if(!var(p)){
         return false;
      }
   }
   // otherwise, it's a word.
   else{
      if(!word(p)){
         return false;
      }
   }

   return true;
}

void test_col(void)
{

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
}

// <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
bool loop(Program *p)
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
   if(!lst(p)){
      return false;
   }
   if(!inslst(p)){
      return false;
   }
   return true;
}

void test_loop(void)
{
}

// <NUM>    ::= 10 or -17.99 etc.
bool num(Program *p)
{
   // TODO repretitive use of num.
   DEBUG
   double num;
   if(!get_number(CURRENT_WORD, &num)){
      return false;
   }
   p->curr_word++;
   p->ttl.variables[CONST_VAR_INDEX].var_type=INT;
   p->ttl.variables[CONST_VAR_INDEX].int_var=num;

   return true;
}

void test_num(void)
{
}

// <FWD> ::= "FORWARD" <VARNUM>
bool fwd(Program *p)
{
   DEBUG

   // advance past FORWARD
   p->curr_word++;

   // TODO you need to get the number out of var num
   return varnum(p);
}


// <RGT> ::= "RIGHT" <VARNUM>
bool rgt(Program *p)
{
   DEBUG
   // advance past RIGHT
   p->curr_word++;
   return varnum(p);
}

// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
bool ins(Program *p)
{
   DEBUG
   // todo switch?
   if(strsame(CURRENT_WORD, "FORWARD")){
      return fwd(p);
   }
   else if(strsame(CURRENT_WORD, "RIGHT")){
      return rgt(p);
   }
   else if(strsame(CURRENT_WORD, "COLOUR")){
      return col(p);
   }
   else if(strsame(CURRENT_WORD, "LOOP")){
      return loop(p);
   }
   else if(strsame(CURRENT_WORD, "SET")){
      return set(p);
   }
   // if ins contains none of those words, 
   // it's an invalid instruction.
   return false;
}

void test_ins(void)
{

}

// <INSLST> ::= "END" | <INS> <INSLST>
bool inslst(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, "END")){
      return true;
   }
   if(!ins(p)){
      return false;
   }
   if(!inslst(p)){
      return false;
   }
   return true;
}

void test_inslst(void)
{

}

// <PROG>   ::= "START" <INSLST>
bool program(Program *p)
{
   DEBUG
   if(!strsame(CURRENT_WORD, "START")){
      return false;
   }
   p->curr_word++;
   return inslst(p);
}

void test_program(void)
{

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

/* Issue ANSI Codes to move cursor to the given position */
void cursor_goto(int row, int col)
{
   printf("\033[%i;%iH",row,col);
}
