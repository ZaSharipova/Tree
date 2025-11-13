#ifndef STACK_FUNCTIONS_H_
#define STACK_FUNCTIONS_H_

#include <stdio.h>

#include "Enums.h"
#include "Structs.h"

StackErr_t StackCtor(Stack_Info *stk, ssize_t capacity, FILE *open_log_file);
StackErr_t StackPush(Stack_Info *stk, TreeNode_t *value, FILE *open_log_file);
StackErr_t StackPop(Stack_Info *stk, TreeNode_t **value, FILE *open_log_file);
StackErr_t StackRealloc(Stack_Info *stk, FILE *open_log_file, Realloc_Mode realloc_type);
StackErr_t StackDtor(Stack_Info *stk, FILE *open_log_file);

#endif //STACK_FUNCTIONS_H_