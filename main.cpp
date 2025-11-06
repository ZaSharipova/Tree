#include <stdio.h>

#include "TreeFunctions.h"
#include "Enums.h"
#include "TreeGraph.h"

int main(void) {

    Tree_t tree = {};
    TreeCtor(&tree);

    // const char *number = "12";
    // INSERT_TO_TREE("10");
    // INSERT_TO_TREE("5");
    // INSERT_TO_TREE("3");
    // INSERT_TO_TREE("20");
    // INSERT_TO_TREE("7");
    // INSERT_TO_TREE("15");
    // INSERT_TO_TREE("13");
    // INSERT_TO_TREE("16");
    // INSERT_TO_TREE("6");

    int number = 0;
    INSERT_TO_TREE(10);
    INSERT_TO_TREE(5);
    INSERT_TO_TREE(3);
    INSERT_TO_TREE(7);
    INSERT_TO_TREE(20);
    INSERT_TO_TREE(15);
    INSERT_TO_TREE(13);
    INSERT_TO_TREE(16);
    INSERT_TO_TREE(6);


    // int number = 10;
    // InsertTree(&tree, &number);
    // number = 5;
    // InsertTree(&tree, &number);
    // number = 3;
    // InsertTree(&tree, &number);
    // number = 7;
    // InsertTree(&tree, &number);
    // number = 20;
    // InsertTree(&tree, &number);
    // number = 15;
    // InsertTree(&tree, &number);
    // number = 13;
    // InsertTree(&tree, &number);
    // number = 16;
    // InsertTree(&tree, &number);
    // number = 6;
    // InsertTree(&tree, &number);

    WriteTreeToDotFile(&tree, "output.txt");
    PrintSortedNode(tree.root);

    TreeDtor(&tree);
    return 0;
}