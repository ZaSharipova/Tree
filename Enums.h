#ifndef ENUMS_H_
#define ENUMS_H_

typedef char* TreeElem_t;
#define TREE_SPEC "%s"

#define FILE_OUT "output.txt"

#define MAX_IMAGE_SIZE 100

enum TreeErrors{
    kSuccess,
    kNoMemory,
    kNodeNullPointer,
    kBadTree,
    kNoSuchNode,
    kNodeInvalidData,
    kNodeSelfReference,
    kNodeParentChildMismatch,
    kNodeChildParentMismatch,
    kFailure,
    kNoPossibleNode,
    kErrorOpeningFile,
    kErrorClosingFile,
    kErrorStat,
    kSyntaxError,
};

enum TypeOfAnswer {
    kYes,
    kNo,
    kWrong,
};

#endif //ENUMS_H_