/*
20
    (10
        (5(*)(*))
        (17(*)(*))
        )
        (30
        (21(*)(*))
        (*)
        )

20 - - - -30
|         |
10 -17    21
|
5

5(3(2(*)(*))(4(*)(*)))(7(6(*)(*))(*))



17(2(*)(3(*)(4(*)(*))))(6(8(*)(*))(*))
17 - - - -6
|         |
2 -3 -4   8


The tree has left branches vertically down the page and right branches
horizontally right.

 gcc visualize_b_tree.c -o visualize_b_tree
 */
#include <stdio.h>
#define STRSIZE 5000

struct node {
  int i;
  struct node *left;
  struct node *right;
};
typedef struct node Node;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  get_tree(argv[1], 0, (int)strlen(argv[1]));
  char * test = PrintTree();
  printf("%i", test);
  return 0;
}

void get_tree(char * line, int l, int r) {
    // otherwise, get the string inside the brackets, and run gettree.

}

Node *MakeNode(int i) {
  Node *n = (Node *)calloc(1, sizeof(Node));
  assert(n != NULL);
  n->i = i;
  return n;
}


char *PrintTree(Node *t)
{

   char *str;

   assert((str = calloc(STRSIZE, sizeof(char))) != NULL);
   if(t == NULL){
      strcpy(str, "*");
      return str;
   }
   sprintf(str, "%i (%s) (%s)", t->i, PrintTree(t->left), PrintTree(t->right));
   return str;

}
