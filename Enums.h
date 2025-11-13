#ifndef ENUMS_H_
#define ENUMS_H_

typedef char* TreeElem_t;
#define TREE_SPEC "%s"

#define FILE_OUT "output.txt"

#define MAX_IMAGE_SIZE 100

enum TreeErrors {
    kNoMemory                  = 1 << 0,
    kNodeNullPointer           = 1 << 1,
    kBadTree                   = 1 << 2,
    kNoSuchNode                = 1 << 3,
    kNodeInvalidData           = 1 << 4,
    kNodeSelfReference         = 1 << 5,
    kNodeParentChildMismatch   = 1 << 6,
    kNodeChildParentMismatch   = 1 << 7,
    kNotRightTree              = 1 << 8,
    kFailure                   = 1 << 9,
    kNoPossibleNode            = 1 << 10,
    kErrorOpeningFile          = 1 << 11,
    kErrorClosingFile          = 1 << 12,
    kErrorStat                 = 1 << 13,
    kSyntaxError               = 1 << 14,
    kSuccess                   = 0,
};


enum TypeOfAnswer {
    kDo,
    kDoNot,
    kWrong,
};


enum StackErr_t {
    kErrorEmptyStack       = 1 << 0,
    kErrorStackNullPointer = 1 << 1,
    kSizeError             = 1 << 2,
    kNegativeCapacity      = 1 << 3,
    kNegativeSize          = 1 << 4,
    kWrongCanaryLeft       = 1 << 5,
    kWrongCanaryRight      = 1 << 6,
    kWrongHash             = 1 << 7,
    kNoCallocMemory        = 1 << 8,

    kNoMemoryS             = 1 << 9,
    kZeroNumber            = 1 << 10,
    kNumberNotWritten      = 1 << 11,
    kNoCommandFound        = 1 << 12,
    kSuccessS              = 0,
};

enum Realloc_Mode {
    kIncrease,
    kDecrease,
    kNoChange,
    kIncreaseZero,
};

#endif //ENUMS_H_