#include <stdio.h>

#include "TreeFunctions.h"
#include "Enums.h"

int main(void) {

    Tree_t tree = {};
    TreeCtor(&tree);
    InsertTree(&tree, 10);
    InsertTree(&tree, 5);
    InsertTree(&tree, 3);
    InsertTree(&tree, 7);
    InsertTree(&tree, 20);
    InsertTree(&tree, 15);
    InsertTree(&tree, 13);
    InsertTree(&tree, 16);
    InsertTree(&tree, 6);

    PrintSortedNode(tree.root);

    TreeDtor(&tree);
    return 0;
}