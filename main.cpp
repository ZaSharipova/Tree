#include <stdio.h>

#include "TreeFunctions.h"
#include "Enums.h"

int main(void) {

    TreeNode_t node0 = {};
    NodeCtor(&node0, NULL);

    TreeNode_t node1 = {};
    NodeCtor(&node1, 10);

    TreeNode_t node2 = {};
    NodeCtor(&node2, 20);

    TreeNode_t node3 = {};
    NodeCtor(&node3, 15);

    TreeNode_t node4 = {};
    NodeCtor(&node4, 5);

    
    return 0;
}