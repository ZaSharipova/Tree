#include "TreeFunctions.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Enums.h"             

TreeErrors TreeRootCtor(Tree_t *tree) {
    assert(tree);

    tree->root = NULL;
    tree->size = 0;

    return kSuccess;
}

TreeErrors NodeCtor(TreeNode_t **node, TreeElem_t *value) {
    assert(node);
    //assert(value);

    *node = (TreeNode_t *) calloc(1, sizeof(TreeNode_t));
    if (!*node) {
        fprintf(stderr, "No memory to calloc NODE.\n");
        return kNoMemory;
    }

    if (value && *value) {
        (*node)->data = *value; 
    } else {
        (*node)->data = NULL;
    }
    (*node)->left =  NULL;
    (*node)->right =  NULL;

    return kSuccess;
}

TreeErrors NodeDtor(TreeNode_t *node) {
    assert(node);

    free(node); 
    node =  NULL;

    return kSuccess;
}

TreeErrors TreeDtor(Tree_t *tree) {
    assert(tree);

    DeleteNode(tree->root);
    tree->root =  NULL;
    tree->size = NULL;

    return kSuccess;
}

void PrintNodePreOrder(const TreeNode_t *node) {
    if (!node) {
        return;
    }

    printf("( ");

    printf("" TREE_SPEC " ", node->data);

    PrintNodePreOrder(node->left);
    PrintNodePreOrder(node->right);

    printf(") ");
}

void PrintNodePostOrder(const TreeNode_t *node) {
    if (!node) {
        return;
    }

    printf("( ");

    PrintNodePostOrder(node->left);
    PrintNodePostOrder(node->right);
    printf("" TREE_SPEC " ", node->data);

    printf(") ");
}

void PrintNodeInOrder(const TreeNode_t *node) {
    if (!node) {
        return;
    }

    printf("( ");

    PrintNodeInOrder(node->left);
    printf("" TREE_SPEC " ", node->data);
    PrintNodeInOrder(node->right);

    printf(") ");
}

void PrintSortedNode(const TreeNode_t *node) {
    assert(node);

    printf("( ");

    if (node->left) {
        PrintSortedNode(node->left);
    }

    printf("" TREE_SPEC" ", node->data);


    if (node->right) {
        PrintSortedNode(node->right);
    }

    printf(") ");
}

void SortNodeToArray(const TreeNode_t *node, TreeElem_t *arr_after) {
    assert(node);
    assert(arr_after);

    static int i = 0;

    if (node->left) {
        SortNodeToArray(node->left, arr_after);
    }
    arr_after[i++] = node->data;

    if (node->right) {
        SortNodeToArray(node->right, arr_after);
    }
}

TreeErrors InsertTree(Tree_t *tree, TreeElem_t *value) {
    assert(tree);
    assert(value);
    
    if (tree->root ==  NULL) {
        TreeNode_t *new_node =  NULL;
        TreeErrors err = NodeCtor(&new_node, value);
        if (err != kSuccess) {
            return err;
        }
        
        tree->root = new_node;
        tree->size++;

    } else {
        return InsertNode(tree->root, value);
    }
    
    return kSuccess;
}

bool CompareNodes(TreeElem_t parent_value, TreeElem_t children_value) {
    // assert(parent_value);
    // assert(children_value);

    if (strcmp(TREE_SPEC, "%d") == 0) {
        return (children_value - parent_value > 0);
    }

    return (atoi((const char *)children_value) - atoi((const char *)parent_value) > 0);
}

TreeErrors InsertNode(TreeNode_t *parent_node, TreeElem_t *value) {
    assert(parent_node);
    assert(value);
    
    TreeNode_t *new_node =  NULL;
    TreeErrors err = NodeCtor(&new_node, value);
    if (err != kSuccess) return err;
    
    if (CompareNodes(parent_node->data, *value)) {
        if (parent_node->right) {
            return InsertNode(parent_node->right, value);

        } else {
            parent_node->right = new_node;
        }
    } else {
        if (parent_node->left) {
            return InsertNode(parent_node->left, value);
        } else {
            parent_node->left = new_node;
        }
    }
    
    return kSuccess;
}

TreeErrors TreeVerify(const TreeNode_t *head, int size) {
    assert(head);

    static int nodes_cnt = 1;
    if (head->left) {
        nodes_cnt++;
        TreeVerify(head->left, size);
    }

    if (head->right) {
        nodes_cnt++;
        TreeVerify(head->right, size);
    }

    if (nodes_cnt < size) {
        return kBadTree;
    }
    return kSuccess;
}

TreeErrors NodeVerify(const TreeNode_t *node) {
    assert(node);

    if (node ==  NULL) {
        return kNodeNullPointer;
    }



    return kSuccess;
}

TreeErrors DeleteNode(TreeNode_t *node) {
    if (!node)
        return kSuccess;

    DeleteNode(node->left);
    DeleteNode(node->right);

    free(node);

    return kSuccess;
}

static int CompareElemsQsort(const void *a, const void *b) {
    const TreeElem_t *res_a = (const TreeElem_t *)a;
    const TreeElem_t *res_b = (const TreeElem_t *)b;

    if (*res_a > *res_b) return 1;
    else if (*res_a < *res_b) return -1;
    else return 0;
}

int CheckSorting(TreeElem_t *arr_to_check_with, TreeNode_t *head, TreeElem_t *arr_before, int count) {
    assert(arr_to_check_with);
    assert(head);
    assert(arr_before);

    SortNodeToArray(head, arr_to_check_with);
    qsort(arr_before, (size_t)count, sizeof(TreeElem_t), CompareElemsQsort);
    
    for (int i = 0; i < count; i++) {
        if (arr_before[i] != arr_to_check_with[i]) {
            printf("AAAAAAA sorting doesn't work.");
            return -1;
        }
    }
    printf("\nSorting works PERFECT!!!!");
    return 0;
}

TreeNode_t *FindNodeAddress(TreeNode_t *head, TreeElem_t *value) {
    assert(head);
    assert(value);

    if (head->data > *value) {
        FindNodeAddress(head->left, value);
    } else if (head->data < *value) {
        FindNodeAddress(head->right, value);
    }
    return head;
}