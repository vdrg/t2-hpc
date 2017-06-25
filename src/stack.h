#include <stdlib.h>

typedef struct stack_node {
  int *path, // The cost will be the last int
  int size,
  struct stack_node *next,
} Node;

typedef struct stack {
  struct stack_node *top,
} Stack;

void push(Stack *stack, int *path, int size);

Node *pop(Stack *stack);

bool empty(Stack *stack);

void free_stack(Stack *stack);
