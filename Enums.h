#ifndef ENUMS_H_
#define ENUMS_H_

typedef int TreeElem_t;

struct TreeNode_t {
    TreeNode_t *root;
    TreeElem_t data;
    TreeNode_t *left;
    TreeNode_t *right;
};

enum TreeErrors {
    kSuccess,
    kNoMemory,
};

#endif //ENUMS_H_