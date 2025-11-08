#ifndef ENUMS_H_
#define ENUMS_H_

typedef const char* TreeElem_t;
#define TREE_SPEC "%s"

#define FILE_OUT "output.txt"

struct TreeNode_t {
    TreeElem_t data;
    TreeNode_t *parent;
    TreeNode_t *left;
    TreeNode_t *right;
};

struct Tree_t {
    TreeNode_t *root;
    size_t size;
};

enum TreeErrors{
    kSuccess,
    kNoMemory,
    kNodeNullPointer,
    kBadTree,
    kNoSuchNode,
};

#endif //ENUMS_H_