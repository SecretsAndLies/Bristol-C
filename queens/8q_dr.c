#include "8q.h"

int main(int argc, char* argv[]) 
{
    test();
    validate_arg_count(argc);
    bool is_verbose = false;
    int size;
    parse_args(argc, argv, &size, &is_verbose);
    Board solutions[SMALL_LIST];
    int solution_count = get_solutions(size, solutions);
    if(is_verbose){
        for(int i=0; i<solution_count; i++){
            print_board_string(&solutions[i]);
        }
    }
    printf("%i solutions \n", solution_count);
    return 0;
}
