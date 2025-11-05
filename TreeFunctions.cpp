#include "TreeFunctions.h"

#include <stdio.h>
#include <assert.h>

#include "Enums.h"

#define CHECK_ERROR_RETURN(cond) \
    err = cond;                  \
    if (err != kSuccess) {       \ 
        return err;              \
    }              

TreeErrors NodeCtor(TreeNode_t *node, TreeElem_t value) {
    //assert(node);

    TreeNode_t *node_ptr = (TreeNode_t *) calloc (1, sizeof(TreeNode_t));
    if (!node_ptr) {
        fprintf(stderr, "No memory to calloc NODE.\n");
        return kNoMemory;
    }

    node = node_ptr;
    node->data = value;
    node->left = NULL;
    node->right = NULL;

    return kSuccess;
}

TreeErrors NodeDtor(TreeNode_t *node) {
    assert(node);

    free(node); //??????

    return kSuccess;
}

void PrintNode(const TreeNode_t *node) {
    assert(node);

    printf("( ");

    printf("%d ", node->data);

    if (node->left) {
        PrintNode(node->left);
    }

    printf("%d ", node->data);

    if (node->right) {
        PrintNode(node->right);
    }

    printf(") ");

}

void PrintSortedNode(const TreeNode_t *node) {
    assert(node);

    if (node->left) {
        PrintSortedNode(node->left);
    }

    printf("%d ", node->data);

    if (node->right) {
        PrintSortedNode(node->right);
    }
}

TreeErrors InsertNode(TreeNode_t *parent_node, TreeElem_t value) {
    assert(parent_node);
    assert(value);

    TreeErrors err = kSuccess;

    if (parent_node->data <= value) {
        if (parent_node->left) {
            CHECK_ERROR_RETURN(InsertNode(parent_node->left, value));

        } else {
            TreeNode_t node_new = {};
            CHECK_ERROR_RETURN(NodeCtor(&node_new, value));
            parent_node->left = &node_new;
        }

    } else {
        if (parent_node->right) {
            CHECK_ERROR_RETURN(InsertNode(parent_node->right, value));

        } else {
            TreeNode_t node_new = {};
            CHECK_ERROR_RETURN(NodeCtor(&node_new, value));
            parent_node->right = &node_new;
        }
    }

    return kSuccess;
}

TreeErrors TreeVerify(const TreeNode_t *node) {
    assert(node);


}