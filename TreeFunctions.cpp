#include "TreeFunctions.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Enums.h"  
#include "Structs.h"

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
    (*node)->parent = NULL;

    return kSuccess;
}

TreeErrors NodeDtor(TreeNode_t *node) {
    assert(node);

    free(node); 
    node =  NULL;

    return kSuccess;
}

TreeErrors TreeDtor(Tree_t *tree, TreeElem_t buffer, size_t pos) {
    assert(tree);
    assert(buffer);

    TreeErrors err = kSuccess;
    CHECK_ERROR_RETURN(DeleteNode(tree->root, buffer, pos));

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

void SortNodeToArray(const TreeNode_t *node, TreeElem_t *arr_after, int *pos) {
    assert(node);
    assert(arr_after);
    assert(pos);

    if (node->left) {
        SortNodeToArray(node->left, arr_after, pos);
    }
    arr_after[(*pos)++] = node->data;

    if (node->right) {
        SortNodeToArray(node->right, arr_after, pos);
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
            new_node->parent = parent_node;
        }
    } else {
        if (parent_node->left) {
            return InsertNode(parent_node->left, value);
        } else {
            parent_node->left = new_node;
            new_node->parent = parent_node;
        }
    }
    
    return kSuccess;
}

TreeErrors TreeVerify(const TreeNode_t *head, int size, int *cnt) {
    assert(head);
    assert(cnt);

    TreeErrors err = kSuccess;
    unsigned int error = 0;
    CHECK_ERROR_RETURN(NodeVerify(head));

    if (head->left) {
        (*cnt)++;
        CHECK_ERROR_RETURN(TreeVerify(head->left, size, cnt));
    }

    if (head->right) {
        (*cnt)++;
        CHECK_ERROR_RETURN(TreeVerify(head->right, size, cnt));
    }

    if (*cnt + 1 < size && head->parent == NULL) {
        return kBadTree;
    }

    return kSuccess;
}

TreeErrors NodeVerify(const TreeNode_t *node) {
    assert(node);

    unsigned int error = 0;

    if (node == NULL) {
        error |= kNodeNullPointer;
        return (TreeErrors) error;
    }

    if (node->data == NULL) {
        error |= kNodeInvalidData;
    }
    
    if (node->parent == node || node->left == node || node->right == node) {
        error |= kNodeSelfReference;
    }
    
    if (node->parent != NULL) {
        if (node->parent->left != node && node->parent->right != node) {
            error |= kNodeParentChildMismatch;
        }
    }
    
    if (node->left != NULL) {
        if (node->left->parent != node) {
            error |= kNodeChildParentMismatch;
        }
    }
    if (node->right != NULL) {
        if (node->right->parent != node) {
            error |= kNodeChildParentMismatch;
        }
    }
    if ((node->right && !node->left) || (!node->right && node->left)) {
        error |= kNotRightTree;
    }

    return (TreeErrors) error;
}



TreeErrors DeleteNode(TreeNode_t *node, TreeElem_t buffer, size_t pos) {
    assert(buffer);
    if (!node)
        return kSuccess;

    if (node->left) {
        if (node->left->data >= buffer && node->left->data < buffer + pos) {
            DeleteNode(node->left, buffer, pos);
        }
        node->left = NULL;
    }

    if (node->right) {
        if (node->right->data >= buffer && node->right->data < buffer + pos) {
            DeleteNode(node->right, buffer, pos);
        }
        node->right = NULL;
    }
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

    int pos = 0;
    SortNodeToArray(head, arr_to_check_with, &pos);
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