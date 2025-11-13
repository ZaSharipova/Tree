#include "TreeGraph.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "Enums.h"
#include "Structs.h"

// void PrintDotNode(FILE *file, const TreeNode_t *node) {
//     assert(file);
//     assert(node);

//     fprintf(file, "    \"%p\" [ label=\"{Parent: %p | Addr: %p | Data: " TREE_SPEC"", (void *)node, (void *)node->parent, (void *)node, node->data);
//     if (!(!node->left && !node->right)) {
//         fprintf(file, "?");
//     }
//     fprintf(file, " | {Left:  %p | Right: %p}}\"; shape=Mrecord; color=black];\n", node->left, node->right);


//     if (node->left) {
//         fprintf(file, "    \"%p\" -> \"%p\" \n [label=\"да\", fontsize=15, fontcolor=darkgreen, labeldistance=2.0, labelangle=45];", (void *)node, (void *)node->left);
//         PrintDotNode(file, node->left);
//     }

//     if (node->right) {
//         fprintf(file, "    \"%p\" -> \"%p\" [label=\"нет\", fontsize=15, fontcolor=darkred, labeldistance=2.0, labelangle=45];\n", (void *)node, (void *)node->right);
//         PrintDotNode(file, node->right);
//     }
// }


// static void DoSnprintf(DumpInfo *Info) {
//     assert(Info);

//     snprintf(Info->image_file, sizeof(Info->image_file), "Images/graph_%zu.svg", Info->graph_counter);
//     (Info->graph_counter)++;
//     char cmd[40] = {};
//     snprintf(cmd, sizeof(cmd), "dot " FILE_OUT " -T svg -o %s", Info->image_file);
    
//     system(cmd);
// }

// void DoTreeInGraphviz(const TreeNode_t *node, DumpInfo *Info) {
//     assert(node);
//     assert(Info);

//     FILE *file = fopen(Info->filename_to_write_graphviz, "w");
//     if (!file) {
//         perror("Cannot open file for writing.");
//         return;
//     }

//     fprintf(file, "digraph BinaryTree {\n");
//     fprintf(file, "    rankdir=TB;\n");
//     fprintf(file, "    node [shape=record, style=filled, fillcolor=lightblue];\n");
//     fprintf(file, "    edge [fontsize=10];\n\n");
//     fprintf(file, "    graph [fontname=\"Arial\"];\n");
//     fprintf(file, "    node [fontname=\"Arial\"];\n");
//     fprintf(file, "    edge [fontname=\"Arial\"];\n");      
// }

void PrintDotNode(FILE *file, const TreeNode_t *node, const TreeNode_t *node_colored, bool flag) {
    assert(file);
    assert(node);
    assert(node_colored);

    fprintf(file, "    \"%p\" [ label=\"{Parent: %p | Addr: %p | Data: " TREE_SPEC"", (void *)node, (void *)node->parent, (void *)node, node->data);
    if (!(!node->left && !node->right)) {
        fprintf(file, "?");
    }
    fprintf(file, " | {Left:  %p | Right: %p}}\"; shape=Mrecord; color=black\n", node->left, node->right);
    if ((node == node_colored || node->parent == node_colored) && !flag) {
        fprintf(file, "fillcolor=darkgoldenrod2");
    }
    fprintf(file, "];\n");


    if (node->left) {
        fprintf(file, "    \"%p\" -> \"%p\" \n [label=\"да\", fontsize=15, fontcolor=darkgreen, labeldistance=2.0, labelangle=45, color=darkolivegreen2];", (void *)node, (void *)node->left);
        PrintDotNode(file, node->left, node_colored, flag);
    }

    if (node->right) {
        fprintf(file, "    \"%p\" -> \"%p\" [label=\"нет\", fontsize=15, fontcolor=darkred, labeldistance=2.0, labelangle=45, color=coral1];\n", (void *)node, (void *)node->right);
        PrintDotNode(file, node->right, node_colored, flag);
    }
}


static void DoSnprintf(DumpInfo *Info) {
    assert(Info);

    snprintf(Info->image_file, sizeof(Info->image_file), "Images/graph_%zu.svg", Info->graph_counter);
    (Info->graph_counter)++;
    char cmd[50] = {};
    snprintf(cmd, sizeof(cmd), "dot " FILE_OUT " -T svg -o %s", Info->image_file);
    
    system(cmd);
}

void DoTreeInGraphviz(const TreeNode_t *node, DumpInfo *Info, const TreeNode_t *node_colored) {
    assert(node);
    assert(Info);
    assert(node_colored);

    FILE *file = fopen(Info->filename_to_write_graphviz, "w");
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

    if (node->parent == NULL) {
        fprintf(file, "    \"1\" [label=\"ROOT\", shape=rect, fillcolor=pink];\n");
        fprintf(file, "    \"1\" -> \"%p\";\n", (void *)node);
    } else {
        fprintf(file, "    // Empty tree\n");
    }

    PrintDotNode(file, node, node_colored, Info->flag_new);

    fprintf(file, "}\n");
    fclose(file);

    system("mkdir -p Images");
    DoSnprintf(Info);
}
