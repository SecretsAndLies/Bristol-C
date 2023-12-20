#include "interp.h"

// TODO delete debug
int main( int argc, char *argv[] )  
{
   test();
   Program * prog = init_program(argv, argc);
   if(!run_program(prog)){
      free_prog(prog);
      on_error("Error - cannot interpret program.");
   }
   if(prog->output_location==TXT){
      print_arr_to_txt_file(prog);
   }
   free_prog(prog);
   exit(EXIT_SUCCESS);
}

void test(void)
{
   test_run_fwd();
   test_eval_args();
   test_add_to_angle();
   test_run_pfix();
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
   test_run_op();
}

// todo this is similar the other print arr thing. Refactor?
void print_arr_to_txt_file(Program * p)
{
   FILE *fp = fopen(p->output_file_name, "w");
   for(int r=0; r<SCREEN_HEIGHT; r++){
      for(int c=0; c<SCREEN_WIDTH; c++){
         char letter = p->output[r][c];
         fprintf(fp, "%c", letter);
      }  
      fprintf(fp, "\n");
   }  
   fprintf(fp, "\n"); 
}

void free_prog(Program * p)
{
   if(p->output_location==SCREEN){
      neillreset(); 
      printf("\n"); // you need this or reset doesn't work.
   }
   stack_free(p->stck);
   free(p);
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
   DEBUG
   Variable * v = &p->variables[var-'A'];
   v->var_type=NUM;
   v->num_var=num;
}

void set_variable_to_string(char var, Program * p, char * str)
{
   DEBUG
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

// todo this is still a tiny bit too long.
void eval_args(int argc, char *argv[], File_Type * ft)
{
   if(argc>MAX_ARGS){
      on_error("Wrong number of arguments.");
   }
   if(argc==MAX_ARGS){
      if(!is_valid_filename(argv[INPUT_FILE_INDEX], ".ttl")){
         on_error("Invalid filename for arg 1, expected a .ttl file");
      }
      bool is_txt = is_valid_filename(argv[OUTPUT_FILE_INDEX], ".txt");
      bool is_ps = is_valid_filename(argv[OUTPUT_FILE_INDEX], ".ps");
      if(!is_ps && !is_txt){
         on_error("Invalid filename for arg 2, expected a .ps or .txt file");
      }
      if(is_ps){
         *ft = PS;
         return;
      }
      if(is_txt){
         *ft = TXT;
         return;
      }
   }
   *ft = SCREEN;
}

void test_eval_args(void)
{   
   char * argv3[] = {"./interp_s", "TTLs/octagon1.ttl"};
   File_Type ft;
   eval_args(2, argv3, &ft);
   assert(ft==SCREEN);

   char * argv[] = {"./interp_s", "TTLs/octagon1.ttl", "out_octagon1.txt"};
   eval_args(3, argv, &ft);
   assert(ft==TXT);

   char * argv2[] = {"./interp_s", "TTLs/octagon1.ttl", "out_octagon1.ps"};
   eval_args(3, argv2, &ft);
   assert(ft==PS);
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
   assert(is_operator('+'));
   assert(is_operator('-'));
   assert(is_operator('/'));
   assert(is_operator('*'));
   assert(!is_operator('a'));
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
   }
   else {
      if(!run_varnum(p)){
         return false;
      }
      // after varnum runs, current var is filled. (potentially it could do the stack directly?)
      stack_push(p->stck, p->curr_var.num_var);
   }

   // in both cases you need to prefix.
   if(!run_pfix(p)){
      return false;
   }

   return true;
}


void test_run_pfix(void)
{
   Program* prog = calloc(1, sizeof(Program));
   prog->stck = stack_init();

   double res;
   strcpy(prog->words[0],"45");
   strcpy(prog->words[1],")");
   assert(run_pfix(prog));
   stack_pop(prog->stck, &res);
   assert(fequal(res,45));
   prog->curr_word=0;

   strcpy(prog->words[0],"45");
   strcpy(prog->words[1],"1");
   strcpy(prog->words[2],"+");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,46));
   prog->curr_word=0;

   strcpy(prog->words[0],"22");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"/");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,11));
   prog->curr_word=0;

   strcpy(prog->words[0],"22");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"*");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,44));
   prog->curr_word=0;

   strcpy(prog->words[0],"22");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"-");
   strcpy(prog->words[3],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,20));
   prog->curr_word=0;

   strcpy(prog->words[0],"2");
   strcpy(prog->words[1],"2");
   strcpy(prog->words[2],"-"); //0
   strcpy(prog->words[3],"1");
   strcpy(prog->words[4],"+"); // 1
   strcpy(prog->words[5],"4");
   strcpy(prog->words[6],"*"); // 4
   strcpy(prog->words[7],"2");
   strcpy(prog->words[8],"/"); // 2
   strcpy(prog->words[9],")");
   run_pfix(prog);
   stack_pop(prog->stck, &res);
   assert(fequal(res,2));
   prog->curr_word=0;

   stack_free(prog->stck);
   free(prog);
}

// <OP> ::= + - / *
// takes two numbers on the stack and performs the op.
bool run_op(Program *p)
{
   DEBUG
   if(strlen(CURRENT_WORD)!=1){
      return false;
   }
   // this check I believe is redundant. why? todo?
   if(!is_operator(FIRST_LETTER)){
      return false;
   }
   
   if(!eval_operator(FIRST_LETTER, p)){
      return false;
   }
   
   p->curr_word++;
   return true;
}

bool eval_operator(char op, Program * p)
{
   double second_num;
   double first_num;
   stack_pop(p->stck, &second_num); 
   stack_pop(p->stck, &first_num); 
   if(op=='+'){
      stack_push(p->stck, first_num+second_num);
   }
   if(op=='-'){
      stack_push(p->stck, first_num-second_num);
   }
   if(op=='*'){
      stack_push(p->stck, first_num*second_num);
   }
   if(op=='/'){
      // catch attempts to divide by zero.
      if(fequal(second_num,0)){
         return false;
      }
      stack_push(p->stck, first_num/second_num);
   }
   return true;
}

void test_run_op(void)
{
   Program* prog = calloc(1, sizeof(Program));
   prog->stck = stack_init();
   stack_push(prog->stck, 2);
   stack_push(prog->stck, 2);
   strcpy(prog->words[0],"+");

   double res;
   assert(run_op(prog));
   stack_pop(prog->stck, &res);
   assert(fequal(res,4));
   prog->curr_word=0;


   stack_push(prog->stck, 2);
   stack_push(prog->stck, 0);
   strcpy(prog->words[0],"/");
   assert(!run_op(prog));
   // two numbers are left untouched on the stack, removing.
   stack_pop(prog->stck, &res);
   stack_pop(prog->stck, &res);

   prog->curr_word=0;

   stack_free(prog->stck);
   free(prog);
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
   char letter = FIRST_LETTER;
   // go past the letter.
   p->curr_word++;
   if(!strsame(CURRENT_WORD, "(")){
      return false;
   }
   // go past the open bracket.
   p->curr_word++;
   if(!run_pfix(p)){
      return false;
   }
   // after pfix is run, a number is left on the stack.
   double num;
   stack_pop(p->stck, &num);
   set_variable_to_num(letter,p,num);
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
   if(!get_var_from_variables(CURRENT_WORD[1], p)){
      return false;
   }

   p->curr_word++;
   return true;
}

void test_run_var(void)
{
   // todo check for the unused variable beheviour.
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
   double num;
   // todo change your run item in the other program to be like this.
   // a var num is a var OR A number.
   if(FIRST_LETTER=='$' || get_number(CURRENT_WORD, &num)){
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
   int end_index = len-1;
   // todo magic number.
   if(len<3){
      return false;
   }
   if(FIRST_LETTER != '"'){
      return false;
   }
   if(CURRENT_WORD[end_index] != '"'){
      return false;
   }
   p->curr_var.var_type=STRING;

   // copy the word (eg: "RED" WITH quotes into the current variable.)
   strcpy(p->curr_var.str_var, CURRENT_WORD);
   p->curr_word++;
   return true;
}

void test_run_word(void)
{
   Program* prog = calloc(1, sizeof(Program));

   strcpy(prog->words[0],"\"RED\"");
   run_word(prog);
   assert(prog->curr_var.var_type==STRING);
   assert(strsame(prog->curr_var.str_var,"\"RED\""));

   prog->curr_word=0;

   free(prog);
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
   // at this point, current var should be set to a string.
   // so we can update ttl->col to be this colour.
   // todo add error checks if no valid color is found?
   if(!set_col(p)){
      return false;
   }

   return true;
}

bool set_col(Program * p)
{
   if(strsame(p->curr_var.str_var,"\"BLACK\"")){
      p->ttl.col=black;
   }
   else if(strsame(p->curr_var.str_var,"\"RED\"")){
      p->ttl.col=red;
   }
   else if(strsame(p->curr_var.str_var,"\"GREEN\"")){
      p->ttl.col=green;
   }
   else if(strsame(p->curr_var.str_var,"\"BLUE\"")){
      p->ttl.col=blue;
   }
   else if(strsame(p->curr_var.str_var,"\"YELLOW\"")){
      p->ttl.col=yellow;
   }
   else if(strsame(p->curr_var.str_var,"\"CYAN\"")){
      p->ttl.col=cyan;
   }
   else if(strsame(p->curr_var.str_var,"\"MAGENTA\"")){
      p->ttl.col=magenta;
   }
   else if(strsame(p->curr_var.str_var,"\"WHITE\"")){
      p->ttl.col=white;
   }
   else {
      return false;
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
   p->curr_word++;
   if(!check_ltr(CURRENT_WORD)){
      return false;
   }
   char letter = FIRST_LETTER;
   p->curr_word++;
   if(!strsame(CURRENT_WORD, "OVER")){
      return false;
   }
   p->curr_word++;
   if(!execute_loop(p,letter)){
      return false;
   }
   // now we run inslist again - this could be just the END of the prog, or lots more.
   p->curr_word++;
   if(!run_inslst(p)){
      return false;
   }

   return true;
}

bool execute_loop(Program * p, char letter)
{
   int first_item_index = p->curr_word+1;
   if(!run_lst(p)){
      return false;
   }
   int last_item_index = p->curr_word-LOOP_BACK_OFFSET;
   int num_items_in_loop = (last_item_index-first_item_index)+1;
   int start_of_ins_list = p->curr_word;
   for (int i=0; i<num_items_in_loop; i++)
   {
      int current_item_index = first_item_index+i;
      double num;
      if(get_number(p->words[current_item_index], &num)){
         set_variable_to_num(letter,p,num);
      }
      else{
         set_variable_to_string(letter,p,p->words[current_item_index]);
      }
      if(!run_inslst(p)){
         return false;
      }
      if(i!=num_items_in_loop-1){
         p->curr_word = start_of_ins_list;
      }
   }
   return true;
}

void test_run_loop(void)
{
   // write a a test that covers downarrow 
   // (ie the thing where the loop is defined at the top of the function.)
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

void test_run_fwd(void)
{
   Program* prog = calloc(1, sizeof(Program));
   // out of bounds error.
   strcpy(prog->words[0],"FORWARD");
   strcpy(prog->words[1],"17");
   assert(!run_fwd(prog));
   free(prog);
}

// todo too long and repetitive.
bool go_fwd(Program * p)
{
   // first we write to the location we're already in.
   int c = round(p->ttl.x);
   int r = round(p->ttl.y); 
   if(!write_turtle_to_arr(p,r,c)){
      return false;
   }
   double angle_in_radians = DEGREES_TO_RADIANS(p->ttl.angle);
   double x_move = cos(angle_in_radians);
   double y_move = sin(angle_in_radians);
   // convert X,Y into actual r c
   for (int i=0; i<p->ttl.distance; i++){
      // note that we have to - here because of array index weirdness
      p->ttl.x -= x_move;
      p->ttl.y -= y_move;
      int c = round(p->ttl.x);
      int r = round(p->ttl.y); 
      if(!write_turtle_to_arr(p,r,c)){
         return false;
      }
   }
   if(p->output_location==SCREEN){
      print_arr_to_screen(p);
   }
   // finally we round the coordinates
   p->ttl.x=round(p->ttl.x);
   p->ttl.y=round(p->ttl.y);
   return true;
}

bool is_out_of_bounds(int r, int c)
{
   if (r < 0 || r >= SCREEN_HEIGHT || c < 0 || c >= SCREEN_WIDTH) {
      puts("OUT OF BOUNDS"); // todo remove? Or just print to stderr
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
   if(letter == 'K'){
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
   // neillclrscrn(); // todo add back in.
   for(int r=0; r<SCREEN_HEIGHT; r++){
      for(int c=0; c<SCREEN_WIDTH; c++){
         char letter = p->output[r][c];
         print_letter_w_colour_code(letter);
      }  
      printf("\n");
   }  
   printf("\n");
   // neillbusywait(1);
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
   assert(fequal(add_to_angle(350.0, 10.0), 0.0));
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

Program * init_program(char * argv[], int argc)
{
   File_Type ft;
   eval_args(argc, argv, &ft);
   Program* prog = calloc(1, sizeof(Program));
   get_prog_from_file(argv[INPUT_FILE_INDEX], prog);
   init_prog_variables(prog);
   prog->output_location = ft; 
   if(prog->output_location!=SCREEN){
      strcpy(prog->output_file_name, argv[OUTPUT_FILE_INDEX]);
   }
   return prog;
}

void init_prog_variables(Program * prog)
{
   set_prog_output_to_spaces(prog);
   prog->ttl.angle=START_ANGLE;
   prog->ttl.x = START_X;
   prog->ttl.y = START_Y;
   prog->ttl.col=white;
   prog->stck = stack_init();
}

void set_prog_output_to_spaces(Program * p)
{
   for(int r=0; r<SCREEN_HEIGHT; r++){
         for(int c=0; c<SCREEN_WIDTH; c++){
            p->output[r][c]=' '; 
         }  
      }  
}

void get_prog_from_file(char * filename, Program * prog)
{
   FILE * f = nfopen(filename,"r");
   int i=0;
   while(fscanf(f, "%s", prog->words[i++])==1 && i<MAXNUMTOKENS);
   if(i>=MAXNUMTOKENS){
      on_error("File contains too many tokens to parse.");
   }
   fclose(f);
   prog->output_location=SCREEN;
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
