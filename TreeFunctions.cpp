#include "TreeFunctions.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "Enums.h"

#define CHECK_ERROR_RETURN(cond) \
    err = cond;                  \
    if (err != kSuccess) {       \
        return err;              \
    }              

TreeErrors TreeCtor(Tree_t *tree) {
    assert(tree);

    tree->root = NULL;
    tree->size = 0;

    return kSuccess;
}

TreeErrors NodeCtor(TreeNode_t **node, TreeElem_t *value) {
    assert(node);

    *node = (TreeNode_t *) calloc(1, sizeof(TreeNode_t));
    if (!*node) {
        fprintf(stderr, "No memory to calloc NODE.\n");
        return kNoMemory;
    }

    (*node)->data = *value;
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

void PrintNode(const TreeNode_t *node) {
    assert(node);

    printf("( ");

    printf("" TREE_SPEC " ", node->data);

    if (node->left) {
        PrintNode(node->left);
    }

    if (node->right) {
        PrintNode(node->right);
    }

    printf(") ");

}

void PrintSortedNode(const TreeNode_t *node) {
    assert(node);

    printf("( ");

    if (node->left) {
        PrintSortedNode(node->left);
    }

    printf(" " TREE_SPEC" ", node->data);

    if (node->right) {
        PrintSortedNode(node->right);
    }

    printf(") ");
}

TreeErrors InsertTree(Tree_t *tree, TreeElem_t *value) {
    assert(tree);
    
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
    assert(parent_value);
    assert(children_value);

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

// TreeErrors InsertNode(TreeNode_t *parent_node, TreeElem_t value) {
//     assert(parent_node);
    
//     TreeNode_t *new_node = NULL;
//     TreeErrors err = NodeCtor(&new_node, value);
//     if (err != kSuccess) return err;
    
//     if (value > parent_node->data) {
//         if (parent_node->right) {
//             return InsertNode(parent_node->right, value);

//         } else {
//             parent_node->right = new_node;
//         }
//     } else {
//         if (parent_node->left) {
//             return InsertNode(parent_node->left, value);
//         } else {
//             parent_node->left = new_node;
//         }
//     }
    
//     return kSuccess;
// }

TreeErrors TreeVerify(const TreeNode_t *node) {
    assert(node);

    if (node ==  NULL) {
        return kNodeNullPointer;
    }

    return kSuccess;
}

TreeErrors DeleteNode(TreeNode_t *node) {
    if (!node)
        return kSuccess;

    if (node->left) {
        DeleteNode(node->left);
    }

    if (node->right) {
        DeleteNode(node->right);
    }

    free(node);

    return kSuccess;
}