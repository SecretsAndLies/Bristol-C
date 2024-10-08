#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>

#define STRSIZE 5000

struct node{
	char c;
	struct node *left;
	struct node *right;
};
typedef struct node Node;

Node *MakeNode(char c);
void InsertRandom(Node *t, Node *n);
char *PrintTree(Node *t);
int depth(Node *t, int d);
bool is_identical(Node * first, Node * second);

int main(void)
{

   char c;
   Node *head = MakeNode('A');
   Node *n;

   srand(time(NULL));
   for(c = 'B'; c < 'G'; c++){
      n = MakeNode(c);
      InsertRandom(head, n);
   }
   printf("%s\n", PrintTree(head));
   printf("depth: %i \n", depth(head, 0));

   char d;
   Node *head2 = MakeNode('A');
   Node *e;

   srand(time(NULL));
   for(d = 'B'; d < 'J'; d++){
      e = MakeNode(d);
      InsertRandom(head2, e);
   }
   printf("%s\n", PrintTree(head2));
   printf("depth: %i \n", depth(head2, 0));

   printf("identical: %i \n", is_identical(head,head));
   printf("non identical: %i \n", is_identical(head,head2));
   return 0;
}

bool is_identical(Node * first, Node * second)
{
    if(first==NULL && second==NULL){
        return true;
    }
    if(first == NULL || second==NULL){
        return false;
    }

    if(first->c!=second->c){
        return false;
    }

    bool left = is_identical(first->left, second->left);
    bool right = is_identical(first->right, second->right);
     
    return left & right; // if either are false, should be false.
}

Node *MakeNode(char c)
{

   Node *n = (Node *)calloc(1, sizeof(Node));
   assert(n != NULL);
   n->c = c;
   return n;

}

void InsertRandom(Node *t, Node *n)
{

   if((rand()%2) == 0){ /* Left */
      if(t->left == NULL){
         t->left = n;
      }
      else{
         InsertRandom(t->left, n);
      }
   }
   else{ /* Right */
      if(t->right == NULL){
         t->right = n;
      }
      else{
         InsertRandom(t->right, n);
      }
   }

}

int depth(Node *t, int d){
    
    if(t==NULL){
        return d;
    }

    int l = depth(t->left, d+1);
    int r = depth(t->right, d+1);

    return l>r?l:r; // gives max of left and right
}

char *PrintTree(Node *t)
{

   char *str;

   assert((str = calloc(STRSIZE, sizeof(char))) != NULL);
   if(t == NULL){
      strcpy(str, "*");
      return str;
   }
   sprintf(str, "%c (%s) (%s)", t->c, PrintTree(t->left), PrintTree(t->right));
   return str;

}
