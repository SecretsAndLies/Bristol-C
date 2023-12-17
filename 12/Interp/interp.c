#include "interp.h"

// TODO delete debug
int main( int argc, char *argv[] )  
{
   test();
   check_args_valid(argc, argv);
   Program * prog = get_program(argv[1]);
   if(!run_program(prog)){
      free(prog);
      on_error("Cannot parse.");
   }
   free(prog);
   neillreset(); // todo intergrate with a free function.
   exit(EXIT_SUCCESS);
}

void test(void)
{
   test_add_to_angle();
   test_fequal();
   test_is_valid_filename();
   test_get_and_set_variables();
   test_run_num();
   test_run_ins();
   test_check_ltr();
   test_run_word();
   test_run_var();
   test_run_col();
   test_is_operator();
   test_run_program();
   test_get_number();
   test_run_loop();
}

void test_get_and_set_variables(void)
{
   Program* prog = calloc(1, sizeof(Program));
   // intially no variables are set.
   assert(!get_var_from_variables('A', prog));
   assert(prog->curr_var.var_type==NOT_SET);

   // now we set a to 2.0
   set_variable_to_num('A', prog, 2.0);
   assert(get_var_from_variables('A', prog));
   assert(fequal(prog->curr_var.num_var,2.0));
   assert(prog->curr_var.var_type==NUM);

   // test Z with a string
   set_variable_to_string('Z',prog,"TEST2");
   assert(get_var_from_variables('Z', prog));
   assert(prog->curr_var.var_type==STRING);
   assert(strsame(prog->curr_var.str_var,"TEST2"));
   
   // reset A to a different type (overwting)
   set_variable_to_string('A',prog,"TEST");
   assert(get_var_from_variables('A', prog));
   assert(prog->curr_var.var_type==STRING);
   assert(strsame(prog->curr_var.str_var,"TEST"));


   free(prog);
}

void set_variable_to_num(char var, Program * p, double num)
{
   Variable * v = &p->variables[var-'A'];
   v->var_type=NUM;
   v->num_var=num;
}

void set_variable_to_string(char var, Program * p, char * str)
{
   Variable * v = &p->variables[var-'A'];   
   v->var_type=STRING;
   strcpy(v->str_var, str);
}

// Looks up var and sets the current variable to it
// returns false if that variable is unset.
// you'll need to check elsewhere that it's a type you're expecting.
bool get_var_from_variables(char var, Program * prog)
{
   Variable v = prog->variables[var-'A'];
   if(v.var_type==NOT_SET){
      return false;
   }
   prog->curr_var = v;
   return true;
}

void check_args_valid(int argc, char *argv[])
{
   if(argc!=MAX_ARGS){
      on_error("Wrong number of arguments.");
   }
   if(!is_valid_filename(argv[INPUT_FILE_INDEX], ".ttl")){
      on_error("Invalid filename for arg 1, expected a .ttl file");
   }
   bool is_not_txt = !is_valid_filename(argv[OUTPUT_FILE_INDEX], ".txt");
   bool is_not_ps = !is_valid_filename(argv[OUTPUT_FILE_INDEX], ".ps");

   if(is_not_ps && is_not_txt){
      on_error("Invalid filename for arg 2, expected a .ps or .txt file");
   }

}

bool is_valid_filename(char * filename, char * ext)
{
   int len = strlen(filename);
   int ext_len = strlen(ext);
   // we add 1 because a filename must be letter.extension
   if(len<ext_len+1){
      return false;
   }
   if(!strsame(&filename[len-ext_len],ext)){
      return false;
   }
   return true;
}

void test_is_valid_filename(void)
{
   assert(is_valid_filename("a.ttl",".ttl"));
   assert(is_valid_filename("asdasdas.ttl",".ttl"));
   // too short.
   assert(!is_valid_filename(".ttl",".ttl"));
   assert(is_valid_filename("v.ps",".ps"));
   assert(is_valid_filename("v.txt",".txt"));
   //wrong ext
   assert(!is_valid_filename("v.txt",".ttl"));

}

bool is_operator(char c)
{
   return c == '+' || c == '-' || c == '/' || c == '*';
}

void test_is_operator(void)
{
}


// <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
bool run_pfix(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, ")")){
      p->curr_word++;
      return true;
   }
   if(is_operator(FIRST_LETTER))
   {
      if(!run_op(p)){
         return false;
      }
      // if you're here, you know it's an operator,
      // and you can do that to the stack.
   }
   else {
      if(!run_varnum(p)){
         return false;
      }
      // add the number to the stack.
   }

   // in both cases you need to prefix.
   if(!run_pfix(p)){
      return false;
   }

   return true;
}

void test_run_pfix(void)
{
   // test that various expressions add the correct result to the stack.
}

// <OP> ::= + - / *
bool run_op(Program *p)
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
bool run_set(Program *p)
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
   if(!run_pfix(p)){
      return false;
   }

   // TODO
   // after pfix is run, there should be a result at the top
   // of the stack. pop it off and set the var in variables to that number.

   return true;
}

void test_run_set(void)
{
   //  SET A ( 45 )

   // test this with arithmetic expressions.
}

// <VAR> ::= $<LTR>
bool run_var(Program *p)
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
   get_var_from_variables(CURRENT_WORD[1], p);

   p->curr_word++;
   return true;
}

void test_run_var(void)
{
}

// <LST> ::= "{" <ITEMS>
bool run_lst(Program *p)
{
   DEBUG
   if(!strsame(CURRENT_WORD, "{")){
      return false;
   }
   // pass {
   p->curr_word++;
   if(!run_items(p)){
      return false;
   }
   return true;
}

// <ITEMS> ::= "}" | <ITEM> <ITEMS>
bool run_items(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, "}")){
      p->curr_word++;
      return true;
   }
   if(!run_item(p)){
      return false;
   }
   if(!run_items(p)){
      return false;
   }
   return true;
}

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
bool run_varnum(Program * p)
{
   DEBUG
   if(FIRST_LETTER=='$'){
      if(!run_var(p)){
         return false;
      }
   }
   else {
      if(!run_num(p)){
         return false;
      }
   }

   return true;
}

// <ITEM> ::= <VARNUM> | <WORD>
bool run_item(Program *p)
{
   DEBUG
   if(FIRST_LETTER=='$'){
      if(!run_varnum(p)){
         return false;
      }
   }
   // otherwise it's a word.
   else {
      if(!run_word(p)){
         return false;
      }
   }
   return true;
}

// <WORD> ::= "RED", "BLUE", "HELLO!" or "178"
bool run_word(Program *p)
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

void test_run_word(void)
{
}

// <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
bool run_col(Program *p)
{
   DEBUG
   // skip past COLOUR
   p->curr_word++;
   if(FIRST_LETTER=='$'){
      if(!run_var(p)){
         return false;
      }
   }
   // otherwise, it's a word.
   else{
      if(!run_word(p)){
         return false;
      }
   }

   return true;
}

void test_run_col(void)
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
bool run_loop(Program *p)
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
   if(!run_lst(p)){
      return false;
   }
   if(!run_inslst(p)){
      return false;
   }
   return true;
}

void test_run_loop(void)
{
}

// <NUM>    ::= 10 or -17.99 etc.
// populates num_var and returns true if suceeded.
bool run_num(Program *p)
{
   DEBUG
   double num;
   if(!get_number(CURRENT_WORD, &num)){
      return false;
   }
   // todo make these lines a func?
   p->curr_var.var_type=NUM;
   p->curr_var.num_var=num;

   p->curr_word++;

   return true;
}

void test_run_num(void)
{
}

// <FWD> ::= "FORWARD" <VARNUM>
bool run_fwd(Program *p)
{
   DEBUG

   // advance past FORWARD
   p->curr_word++;

   if(!run_varnum(p)){
      return false;
   }

   // check we've got a number we can use.
   if(p->curr_var.var_type!=NUM){
      return false;
   }

   p->ttl.distance=p->curr_var.num_var;

   if(!go_fwd(p)){
      return false;
   }
   
   return true;

}

// this is the MAGIC. Takes the x, y, angle and distance
// and writes to the array.
bool go_fwd(Program * p)
{

   // calculate ending cord.
   double angle_in_radians = DEGREES_TO_RADIANS(p->ttl.angle);

   // convert X,Y into actual r c
   for (int i=1; i<=p->ttl.distance; i++){
      // note that we have to - here because of array index weirdness
      p->ttl.x -= cos(angle_in_radians);
      p->ttl.y -= sin(angle_in_radians);

      int c = round(p->ttl.x);
      int r = round(p->ttl.y); 

      if(!write_turtle_to_arr(p,r,c)){
         return false;
      }
   }
   
   if(p->output_location==SCREEN){
      // neillclrscrn(); // TODO add back in.
      print_arr_to_screen(p);
      // neillbusywait(1);
   }

   if(p->output_location==TXT){
      // TODO print to the txt file.
   }

   return true;
}

bool is_out_of_bounds(int r, int c)
{
   if (r < 0 || r >= SCREEN_HEIGHT || c < 0 || c >= SCREEN_WIDTH) {
      return true;
   }
   return false;

}

bool write_turtle_to_arr(Program * p, int r, int c)
{
   if(is_out_of_bounds(r,c)){
      return false;
   }
   else if(p->ttl.col==white){
      p->output[r][c]='W';
   }
   else if(p->ttl.col==black){
      p->output[r][c]='K';
   }
   else if(p->ttl.col==red){
      p->output[r][c]='R';
   }
   else if(p->ttl.col==green){
      p->output[r][c]='G';
   }
   else if(p->ttl.col==yellow){
      p->output[r][c]='Y';
   }
   else if(p->ttl.col==blue){
      p->output[r][c]='B';
   }
   else if(p->ttl.col==magenta){
      p->output[r][c]='M';
   }
   else if(p->ttl.col==cyan){
      p->output[r][c]='C';
   }
   else{
      return false;
   }

   return true;
}

void print_letter_w_colour_code(char letter)
{
   if(letter == 'W'){
      neillfgcol(white);
   }
   if(letter == 'B'){
      neillfgcol(black);
   }
   if(letter == 'R'){
      neillfgcol(red);
   }
   if(letter == 'G'){
      neillfgcol(green);
   }
   if(letter == 'Y'){
      neillfgcol(yellow);
   }
   if(letter == 'B'){
      neillfgcol(blue);
   }
   if(letter == 'M'){
      neillfgcol(magenta);
   }
   if(letter == 'C'){
      neillfgcol(cyan);
   }


   printf("%c",letter);
}

void print_arr_to_screen(Program * p)
{
   for(int r=0; r<SCREEN_HEIGHT; r++){
      for(int c=0; c<SCREEN_WIDTH; c++){
         char letter = p->output[r][c];
         print_letter_w_colour_code(letter);
      }  
      printf("\n");
   }  
   printf("\n");
}

// <RGT> ::= "RIGHT" <VARNUM>
bool run_rgt(Program *p)
{
   DEBUG
   // advance past RIGHT
   p->curr_word++;
   if(!run_varnum(p)){
      return false;
   }
   p->ttl.angle = add_to_angle(p->ttl.angle, p->curr_var.num_var);
   return true;
}

void test_add_to_angle(void) 
{
   assert(fequal(add_to_angle(30.0, 40.0), 70.0));
   assert(fequal(add_to_angle(350.0, 20.0), 10.0));
   assert(fequal(add_to_angle(10.0, -20.0), 350.0));
   assert(fequal(add_to_angle(0.0, 720.0), 0.0));
   assert(fequal(add_to_angle(-30.0, 60.0), 30.0));
}

double add_to_angle(double current_angle, double add_value)
{
   double new_angle = fmod(current_angle + add_value, 360.0);

    // Handling negative angles
    if (new_angle < 0) {
      new_angle += 360.0;
      }
   return new_angle;
}

void test_run_rgt(void)
{
   Program* prog = calloc(1, sizeof(Program));

   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"45");
   run_rgt(prog);
   assert(fequal(prog->ttl.angle, 45));
   prog->curr_word=0;
   set_variable_to_num('A', prog, 45);
   strcpy(prog->words[0],"RIGHT");
   strcpy(prog->words[1],"$A");
   run_rgt(prog);
   assert(fequal(prog->ttl.angle, 45));

   free(prog);

}

// <INS> ::= <FWD> | <RGT> | <COL> | <LOOP> | <SET>
bool run_ins(Program *p)
{
   DEBUG
   // todo switch?
   if(strsame(CURRENT_WORD, "FORWARD")){
      return run_fwd(p);
   }
   else if(strsame(CURRENT_WORD, "RIGHT")){
      return run_rgt(p);
   }
   else if(strsame(CURRENT_WORD, "COLOUR")){
      return run_col(p);
   }
   else if(strsame(CURRENT_WORD, "LOOP")){
      return run_loop(p);
   }
   else if(strsame(CURRENT_WORD, "SET")){
      return run_set(p);
   }
   // if ins contains none of those words, 
   // it's an invalid instruction.
   return false;
}

void test_run_ins(void)
{

}

// <INSLST> ::= "END" | <INS> <INSLST>
bool run_inslst(Program *p)
{
   DEBUG
   if(strsame(CURRENT_WORD, "END")){
      return true;
   }
   if(!run_ins(p)){
      return false;
   }
   if(!run_inslst(p)){
      return false;
   }
   return true;
}

void test_run_inslst(void)
{

}

// <PROG>   ::= "START" <INSLST>
bool run_program(Program *p)
{
   DEBUG
   if(!strsame(CURRENT_WORD, "START")){
      return false;
   }
   p->curr_word++;
   return run_inslst(p);
}

void test_run_program(void)
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
   prog->output_location=SCREEN; // TODO take in the other args.
   
   // set the output to spaces,
   for(int r=0; r<SCREEN_HEIGHT; r++){
      for(int c=0; c<SCREEN_WIDTH; c++){
          prog->output[r][c]='_'; // TODO make this a space.
      }  
   }  

   // start angle
   prog->ttl.angle=START_ANGLE;
   prog->ttl.x = START_X;
   prog->ttl.y = START_Y;
   prog->ttl.col=white;


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


bool fequal(double a, double b)
{
    return fabs(a-b) < FLOAT_PRECISION;
}

void test_fequal(void)
{
   assert(fequal(0.01,0.01));
   assert(!fequal(0.02,0.01));
}
