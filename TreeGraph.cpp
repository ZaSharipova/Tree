#include "TreeGraph.h"

#include <stdio.h>
#include <assert.h>

#include "Enums.h"

void PrintDotNode(FILE *file, const TreeNode_t *node) {
    assert(file);
    assert(node);

    static int cnt = 0;
    fprintf(file, "    \"1\" [label = \"ROOT\"; shape=rect; fillcolor=pink]");
    fprintf(file, "    \"%p\" [ label=\"{Parent: %p | Addr: %p | Data: " TREE_SPEC" | {Left:  %p | Right: %p}}\"; shape=Mrecord; color=black];\n", 
            (void *)node, (void *)node->parent, (void *)node, node->data, 
            node->left, 
            node->right);
    
    if (cnt < 1) {
        fprintf(file, " \"1\" -> \"%p\" ", (void *)node);
        cnt ++;
    }

    if (node->left) {
        fprintf(file, "    \"%p\" -> \"%p\" \n [label=\"да\"бfontsize=20, fontcolor=darkgreen, labeldistance=2.0, labelangle=45];", (void *)node, (void *)node->left);
        PrintDotNode(file, node->left);
    }

    if (node->right) {
        fprintf(file, "    \"%p\" -> \"%p\" [label=\"нет\"]\n", (void *)node, (void *)node->right);
        PrintDotNode(file, node->right);
    }
}

void WriteTreeToDotFile(const Tree_t *tree, const char *filename) {
    assert(tree);
    assert(filename);

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Cannot open file for writing.");
        return;
    }

    fprintf(file, "digraph BinaryTree {\n");
    fprintf(file, "    rankdir=TB;\n");
    fprintf(file, "    node [shape=record, style=filled, fillcolor=lightblue];\n");
    fprintf(file, "    edge [fontsize=10];\n\n");
    fprintf(file, "    graph [fontname=\"Arial\"];\n");
    fprintf(file, "    node [fontname=\"Arial\"];\n");
    fprintf(file, "    edge [fontname=\"Arial\"];\n");      

    if (tree->root) {
        PrintDotNode(file, tree->root);
    } else {
        fprintf(file, "    // Empty tree\n");
    }

    fprintf(file, "}\n");

    fclose(file);

    system("dot " FILE_OUT" -T svg -o image.svg ");
}