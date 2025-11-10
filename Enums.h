#ifndef ENUMS_H_
#define ENUMS_H_

typedef char* TreeElem_t;
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
    kFailure,
};

#define RESET   "\033[0m"
#define WHITE   "\033[1;30m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"

#endif //ENUMS_H_