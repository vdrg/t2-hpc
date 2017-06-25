#include <stdbool.h>

#include "stack.h"

void push(Stack *stack, int *path, int size) {
  struct stack_node *node = calloc(1, sizeof(struct stack_node));
  node->path = path;
  node->size = size;

  if (stack->top != NULL) node->next = stack->top;

  // Set the new node as the top node in the stack
  stack->top = node;
}

Node *pop(Stack *stack) {
  if (empty(stack)) return NULL;

  Node *top = stack->top;

  stack->top = top->next;

  return top;
}

bool empty(Stack *stack) {
  return stack == NULL || stack->top == NULL;

}

void free_stack(Stack *stack) {
  

}

