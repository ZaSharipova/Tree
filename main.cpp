#include <stdio.h>

#include "TreeFunctions.h"
#include "Enums.h"
#include "TreeGraph.h"

int main(void) {

    Tree_t tree = {};
    TreeCtor(&tree);

    // char *number = "10";
    // InsertTree(&tree, &number);
    // number = "5";
    // InsertTree(&tree, &number);
    // number = "3";
    // InsertTree(&tree, &number);
    // number = "20";
    // InsertTree(&tree, &number);
    // number = "7";
    // InsertTree(&tree, &number);
    // number = "15";
    // InsertTree(&tree, &number);
    // number = "13";
    // InsertTree(&tree, &number);
    // number = "16";
    // InsertTree(&tree, &number);
    // number = "6";
    // InsertTree(&tree, &number);

    int number = 10;
    InsertTree(&tree, &number);
    number = 5;
    InsertTree(&tree, &number);
    number = 3;
    InsertTree(&tree, &number);
    number = 7;
    InsertTree(&tree, &number);
    number = 20;
    InsertTree(&tree, &number);
    number = 15;
    InsertTree(&tree, &number);
    number = 13;
    InsertTree(&tree, &number);
    number = 16;
    InsertTree(&tree, &number);
    number = 6;
    InsertTree(&tree, &number);

    WriteTreeToDotFile(&tree, "output.txt");
    PrintSortedNode(tree.root);

    TreeDtor(&tree);
    return 0;
}