#include "TreeGraph.h"

#include <stdio.h>
#include <assert.h>

#include "Enums.h"

#include <stdio.h>
#include <assert.h>

void PrintDotNode(FILE *file, const TreeNode_t *node) {
    assert(file);
    assert(node);

    fprintf(file, "    \"%p\" [label=\"%d\"];\n", (void *)node, node->data);

    if (node->left) {
        fprintf(file, "    \"%p\" -> \"%p\";\n", (void *)node, (void *)node->left);
        PrintDotNode(file, node->left);
    }

    if (node->right) {
        fprintf(file, "    \"%p\" -> \"%p\";\n", (void *)node, (void *)node->right);
        PrintDotNode(file, node->right);
    }
}

void WriteTreeToDotFile(const Tree_t *tree, const char *filename) {
    assert(tree);
    assert(filename);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Cannot open file for writing.");
        return; //
    }

    fprintf(file, "digraph BinaryTree {\n");
    fprintf(file, "    node [shape=circle];\n");

    if (tree->root) {
        PrintDotNode(file, tree->root);
    } else {
        fprintf(file, "\n");
    }

    fprintf(file, "}\n");

    fclose(file);
}
