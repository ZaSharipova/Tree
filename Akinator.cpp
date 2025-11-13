#include "Akinator.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Enums.h"
#include "Structs.h"
#include "TreeFunctions.h"
#include "TreeGraph.h"
#include "TreeDump.h"
#include "StackFunctions.h"
#include "ReadTree.h"


#define MAX_LINE_SIZE 30
#define MAX_PHRASE_SIZE 130
#define MAX_LINE_SPECIFIER "29"
#define YES_ANSWER "да"
#define NO_ANSWER "нет"
#define CONTINUE_ANSWER "CONT"
#define QUIT_ANSWER "QUIT"

#define DO_CALLOC_AND_CHECK_PROBLEM_RETURN(ptr, size)                   \
    char *ptr = (char *) calloc (size, sizeof(char));                   \
    if (!ptr) {                                                         \
        fprintf(stderr, "No memory in calloc.\n");                      \
        return kNoMemory;                                               \
    }

const char *AND_PHRASES[] = {
    "и",
    "а также",
    "более того",
    "еще",
    "к тому же",
    "плюс",
};
#define AndPhrasesSize (sizeof(AND_PHRASES)/sizeof(AND_PHRASES[0]))

bool CheckRight(TreeNode_t *last, Stack_Info *path);

static void ReadLine(char *answer) {
    assert(answer);

    scanf("%" MAX_LINE_SPECIFIER "[^\n]", answer);
    int char_symbol = 0;
    while ((char_symbol = getchar()) != '\n' && char_symbol != EOF);
}

static TypeOfAnswer AskAndReturnYesNo(const char *type_of_answer_yes, const char *type_of_answer_no) {
    assert(type_of_answer_yes);
    assert(type_of_answer_no);

    char *answer = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    if (!answer) {
        fprintf(stderr, "No memory in calloc.\n");
        return kWrong;
    }
    //DO_CALLOC_AND_CHECK_PROBLEM_RETURN(answer, MAX_LINE_SIZE);
    ReadLine(answer);

    TypeOfAnswer flag = kWrong;
    if (strncmp(answer, type_of_answer_yes, strlen(type_of_answer_yes)) == 0) {
        flag = kDo;
    } else if (strncmp(answer, type_of_answer_no, strlen(type_of_answer_no)) == 0) {
        flag = kDoNot;
    }

    free(answer);

    return flag;
}

static TypeOfAnswer AskIfGuessed(const char *question) { //askifguessed
    assert(question);

    printf(YELLOW "\nЭто %s? (" YES_ANSWER"/" NO_ANSWER"):\n" RESET, question);
    return AskAndReturnYesNo(YES_ANSWER, NO_ANSWER);
}

static TypeOfAnswer PlayAgain(void) {

    printf(GREEN "\nЕсли хотите сыграть еще раз, введите CONT, иначе QUIT:\n" RESET);
    return AskAndReturnYesNo(CONTINUE_ANSWER, QUIT_ANSWER);
}

static TreeErrors AddNewCharacter(TreeNode_t *node, Tree_t *tree) {
    assert(node);
    assert(tree);

    printf(BLUE "\nОтветьте тогда, кого Вы загадывали? Введите имя (инициалы, прозвище):\n" RESET);

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(name, MAX_LINE_SIZE);
    ReadLine(name);

    printf(BLUE "\nОтлично! Чем \"%s\" отличается от \"%s\"? Он ... :\n" RESET, name, node->data);

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(new_question, MAX_LINE_SIZE); 
    ReadLine(new_question);

    NodesInsertAtTheEnd(node, name, new_question, tree);
    printf(MAGENTA "\nХорошо, Акинатор дополнен. Теперь в нем есть данный персонаж!\n" RESET);
    free(new_question);

    return kSuccess;
}

static TreeErrors DoCorrectGuess(Tree_t *tree, TreeNode_t **node, DumpInfo *Info, TypeOfAnswer *play_again_flag) {
    assert(tree);
    assert(node);
    assert(Info);
    assert(play_again_flag);

    printf(RED "\nОтлично, ответ угадан!\n" RESET);
    *play_again_flag = PlayAgain();
    if (*play_again_flag != kWrong) {
        *node = tree->root;
    }

    return kSuccess;
}

static TreeErrors DoWrongGuess(Tree_t *tree, TreeNode_t **node, DumpInfo *Info, TypeOfAnswer *play_again_flag) {
    assert(tree);
    assert(node);
    assert(Info);
    assert(play_again_flag);

    AddNewCharacter(*node, tree);

    Info->question = (*node)->data;
    Info->name = (*node)->left->data;
    DoTreeInGraphviz(tree->root, Info, *node);
    DoDump(Info);
            
    *play_again_flag = PlayAgain();
    if (*play_again_flag != kWrong) {
        *node = tree->root;
    }

    return kSuccess;
}

TreeErrors Akinator(Tree_t *tree, TreeNode_t *node, DumpInfo *Info) {
    assert(tree);
    assert(node);
    assert(Info);

    TreeErrors err = kSuccess;
    CHECK_ERROR_RETURN(NodeVerify(node));

    const char *question = NULL;
    TypeOfAnswer play_again_flag = kDo;
    TypeOfAnswer ask_if_guessed_flag = kWrong;

    while (node && play_again_flag == kDo) {
        question = node->data;
        ask_if_guessed_flag = AskIfGuessed(question);

        if (!node->left && !node->right) {
            if (ask_if_guessed_flag == kDo) {
                CHECK_ERROR_RETURN(DoCorrectGuess(tree, &node, Info, &play_again_flag));
            } else if (ask_if_guessed_flag == kDoNot) {
                CHECK_ERROR_RETURN(DoWrongGuess(tree, &node, Info, &play_again_flag));
            } else {
                return kFailure;
            }

        } else {
            if (ask_if_guessed_flag == kDo) {
                node = node->left;
            } else if (ask_if_guessed_flag == kDoNot) {
                node = node->right;
            } else {
                return kFailure;
            }
        }
    }

    CHECK_ERROR_RETURN(NodeVerify(node));
    return kSuccess;
}

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, char *name, char *question, Tree_t *tree) {
    assert(node);
    assert(name);
    assert(question);
    assert(tree);

    TreeNode_t *new_node_left = NULL;
    NodeCtor(&new_node_left, &name);

    TreeNode_t *new_node_right = NULL;
    NodeCtor(&new_node_right, &(node->data));

    node->left = new_node_left;
    node->right = new_node_right;
    new_node_left->parent = node;
    new_node_right->parent = node;
    (tree->size)++;

    // size_t len = strlen(question) + 2;
    // DO_CALLOC_AND_CHECK_PROBLEM_RETURN(new_question, len);

    // strcpy(new_question, question);
    // node->data = new_question;

    node->data = strdup(question);

    return kSuccess;
}

void DoAskGeneralQuestion(TreeNode_t *node) {
    assert(node);

    printf("Это %s?", node->data);
}

void PrintAkinatorToFile(FILE *file, TreeNode_t *node) {
    assert(file);
    assert(node);

    fprintf(file, "(\"%s\" ", node->data);

    if (node->left) {
        PrintAkinatorToFile(file, node->left);
    } else {
        fprintf(file, " nil");
    }

    if (node->right) {
        PrintAkinatorToFile(file, node->right);
    } else {
        fprintf(file, " nil");
    }

    fprintf(file, ")");

}

TreeErrors FindAkinatorNodeAddress(TreeNode_t *node, const char *value, TreeNode_t **address) {
    assert(node);
    assert(value);
    assert(address);

    if (strncmp(value, node->data, strlen(node->data)) == 0) {
        *address = node;
        return kSuccess;
    }

    if (node->left) {
        if (FindAkinatorNodeAddress(node->left, value, address) == kSuccess) return kSuccess;
    }
    if (node->right) {
        if (FindAkinatorNodeAddress(node->right, value, address) == kSuccess) return kSuccess;
    }

    // fprintf(stderr, "No such name in Tree.\n");
    return kFailure;
}

TreeErrors PrintDefinition(TreeNode_t *current, char *definition_str, size_t buffer_len) {
    assert(current);
    assert(definition_str);

    Stack_Info path = {};
    size_t pos_in_phrases = 0;
    StackCtor(&path, 4, stderr);
    StackPush(&path, current, stderr);
    TreeNode_t *cur = current;

    while (current->parent) {
        StackPush(&path, current->parent, stderr);
        current = current->parent;
    }
    TreeNode_t *prev_node = NULL;

    TreeNode_t *node = {};
    StackPop(&path, &prev_node, stderr);

    while (path.size >= 1) {
        StackPop(&path, &node, stderr);

        char buf[MAX_PHRASE_SIZE] = {};
        const char *phrase = AND_PHRASES[pos_in_phrases];
        pos_in_phrases = (pos_in_phrases + 1) % AndPhrasesSize;

        if (prev_node->left && prev_node->left == node) {
            strcpy(buf, phrase);
            strcat(buf, " ");
            strcat(buf, prev_node->data);
        } 
        else if (prev_node->right && prev_node->right == node) {
            strcpy(buf, phrase);
            strcat(buf, " не ");
            strcat(buf, prev_node->data);
        } 
        else {
            return kSuccess;
        }

        if (definition_str[0] != '\0' && (prev_node->left || prev_node->right)) {
            strcat(definition_str, ", ");
        }

        strcat(definition_str, buf);
        printf("%s", buf);

        if ((prev_node->left || prev_node->right) && (prev_node->left != cur && prev_node->right != cur)) {
            printf(", ");
        }

        prev_node = node;
    }

    return kSuccess;
}

void DoSystemCallForSayCommand(char *command, size_t size_of_command, const char *value, char *definition_str) {
    assert(command);
    assert(size_of_command);
    assert(value);
    assert(definition_str);

    snprintf(command, size_of_command, "say \"%s - это %s\"", value, definition_str);
    fprintf(stderr, "Running command: %s\n", command);
    int return_result = system(command);
    fprintf(stderr, "Return code: %d\n", return_result);
}

TreeErrors DoPrintDefinition(TreeNode_t *node, const char *value, size_t tree_size, size_t buffer_len) {
    assert(node);
    assert(value);

    TreeNode_t *address = NULL;
    FindAkinatorNodeAddress(node, value, &address);

    if (!address) {
        printf("%s address not found.\n", value);
        return kNoSuchNode;
    }

    printf("\n============DEFINITION============\n");
    printf("%s - address: %p\nDefinition: ", value, address);

    size_t size_of_command = (buffer_len + strlen(value) + strlen(" - это ") + tree_size * 15) * 4;
    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(definition_str, size_of_command);

    if (address->parent) {
        PrintDefinition(address, definition_str, size_of_command);
    }

    printf("\n==================================\n");

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(command, size_of_command);

    DoSystemCallForSayCommand(command, size_of_command, value, definition_str);

    free(definition_str);
    free(command);
    return kSuccess;
}

static int FindDepth(TreeNode_t *node) {
    assert(node);

    int depth = 0;
    while (node) {
        depth++;
        node = node->parent;
    }
    return depth;
}

TreeNode_t *FindClosestSameNode(TreeNode_t *node1, TreeNode_t *node2) {
    assert(node1);
    assert(node2);

    int depth1 = FindDepth(node1);
    int depth2 = FindDepth(node2);

    while (depth1 > depth2) { 
        node1 = node1->parent; 
        depth1--; 
    }
    while (depth2 > depth1) { 
        node2 = node2->parent; 
        depth2--; 
    }

    while (node1 != node2) {
        node1 = node1->parent;
        node2 = node2->parent;
    }

    return node1;
}

TreeErrors PrintSimpleDefinition(TreeNode_t *node) {
    assert(node);

    if (node->parent) {
       PrintSimpleDefinition(node->parent);
    }

    if (node->parent && node->parent->left == node) {
        printf("%s, ", node->data);
    } else if (node->parent && node->parent->right == node) {
        printf("не %s, ", node->data);
    }

    if (!node->parent) {
        printf("%s, ", node->data);
    }

    return kSuccess;
}

void FillPath(TreeNode_t *node, Stack_Info *path) {
    assert(node);
    assert(path);

    for (TreeNode_t *cur = node; cur; cur = cur->parent) {
        StackPush(path, cur, stderr);
    }
}

TreeErrors PrintSameCharacteristics(TreeNode_t *node1, TreeNode_t *node2, Stack_Info *path1, 
    Stack_Info *path2, TreeNode_t **cur1, TreeNode_t **cur2) {
    assert(node1);
    assert(node2);
    assert(path1);
    assert(path2);
    assert(cur1);
    assert(cur2);

    TreeNode_t *next1 = NULL;
    TreeNode_t *next2 = NULL;

    int cnt = 0;
    while (path1->size > 1 && path2->size > 1) {
        StackPop(path1, cur1, stderr);
        StackPop(path2, cur2, stderr);

        if (*cur1 == *cur2) {
            StackPop(path1, &next1, stderr);
            StackPop(path2, &next2, stderr);
            StackPush(path1, next1, stderr);
            StackPush(path2, next2, stderr);
            if (next1 == next2) {
                if (cnt++ < 1) {
                    printf("%s", (*cur1)->data);
                } else printf(", %s", (*cur1)->data);
            } else {
                return kSuccess;
            }
        } else {
            StackPush(path1, *cur1, stderr);
            StackPush(path2, *cur2, stderr);
            return kSuccess;
        }
    }

    return kSuccess;
}

bool CheckRight(TreeNode_t *last, Stack_Info *path) {
    assert(last);
    assert(path);

    TreeNode_t *node_following = NULL;
    StackPop(path, &node_following, stderr);
    StackPush(path, node_following, stderr);

    if (last->right == node_following) {
        return true;
    }
    return false;
}

TreeErrors PrintDifferentCharacteristics(TreeNode_t *cur, Stack_Info *path, const char *value) {
    assert(cur);
    assert(path);
    assert(value);

    TreeNode_t *last_common = cur;
    printf("\n%s — ", value);

    if (CheckRight(last_common, path)) printf("не ");
    printf("%s", last_common->data);
    if (path->size > 1) {
        printf(",");
    }

    while (path->size > 1) {
        last_common = cur;
        StackPop(path, &cur, stderr);
        if (CheckRight(cur, path)) {
            printf(" не");
        } 
        printf(" %s", cur->data);
        if (path->size > 1) {
            printf(",");
        }
    }

    return kSuccess;
}

TreeErrors CompareNames(TreeNode_t *head, const char *value1, const char *value2) {
    assert(head);
    assert(value1);
    assert(value2);

    TreeNode_t *node1 = NULL;
    TreeNode_t *node2 = NULL;

    if (FindAkinatorNodeAddress(head, value1, &node1) != kSuccess ||
        FindAkinatorNodeAddress(head, value2, &node2) != kSuccess) {
        fprintf(stderr, "Один из узлов не найден в дереве.\n");
        return kNoSuchNode;
    }

    Stack_Info path1 = {};
    Stack_Info path2 = {};
    if (StackCtor(&path1, 4, stderr) != kSuccessS || StackCtor(&path2, 4, stderr) != kSuccessS) {
        return kNoMemory;
    }
    FillPath(node1, &path1);
    FillPath(node2, &path2);

    TreeNode_t *cur1 = NULL;
    TreeNode_t *cur2 = NULL;

    printf("==================Names Comparison==============\n");
    printf("%s и %s похожи таким образом: ", value1, value2);

    PrintSameCharacteristics(node1, node2, &path1, &path2, &cur1, &cur2);

    printf(",\n\nPазличаются таким образом:\n");
    PrintDifferentCharacteristics(cur1, &path1, value1);
    PrintDifferentCharacteristics(cur2, &path2, value2);

    printf(".\n=============================================\n");

    StackDtor(&path1, stderr);
    StackDtor(&path2, stderr);

    return kSuccess;
}

TreeErrors AskAndDoFileRead(Tree_t *tree, DumpInfo *Info, FileInfo *FileInfo, FILE *file_in) {
    assert(tree);
    assert(FileInfo);
    assert(Info);
    assert(file_in);

    printf(YELLOW "Вы хотите воспользоваться старой базой данных? (да/нет): \n" RESET);

    TreeErrors err = kSuccess;
    TypeOfAnswer type_of_answer = AskAndReturnYesNo(YES_ANSWER, NO_ANSWER);
    switch (type_of_answer) {
    case (kDo): {
        DoBufRead(file_in, "akinator_out.txt", FileInfo);

        size_t pos = 0;
        DO_OPEN_FILE_RETURN(file_log, "logfile_for_read.txt", "w");
        TreeNode_t *new_node = NULL;
        CHECK_ERROR_RETURN(ReadNodeFromFile(tree, file_in, file_log, &pos, tree->root, FileInfo->buf_ptr, &new_node));
        tree->root = new_node;

        fclose(file_in);
        fclose(file_log);
        Info->flag_new = true;
        DoTreeInGraphviz(tree->root, Info, tree->root);
        DoDump(Info);
        Info->flag_new = false;
        return kSuccess;
    }
    case (kDoNot):{
        TreeElem_t value = "No One knows";

        InsertTree(tree, &value);
       //INSERT_TO_TREE(&value);
        return kSuccess;
    }
    case (kWrong):
        fprintf(stderr, "Error, no such answer.");
        return kFailure;
    default:
        fprintf(stderr, "No such mode.");
        return kFailure;
    }
}

TreeErrors DoDifferentAkinatorModes(Tree_t *tree, DumpInfo *Info, size_t pos) {
    assert(tree);
    assert(Info);

    TreeErrors err = kSuccess;
    printf(YELLOW "В какой режим акинатора вы хотите сыграть: \n1. Поиск объекта, 2. Математическое определение объекта, 3. Сравнение двух объектов\n" RESET);
    
    int number = 0;
    scanf("%d", &number);
    getchar();

    switch (number) {
    case (1):
        CHECK_ERROR_RETURN(Akinator(tree, tree->root, Info));
        return kSuccess;
    case (2): {
        printf("Введите название объекта: \n");
        DO_CALLOC_AND_CHECK_PROBLEM_RETURN(name, MAX_LINE_SIZE);
        ReadLine(name);
        CHECK_ERROR_RETURN(DoPrintDefinition(tree->root, name, tree->size, pos));
        free(name);
        return kSuccess;
    }
    case (3): {
        printf("Введите названия двух интересующих объектов:\n");
        DO_CALLOC_AND_CHECK_PROBLEM_RETURN(name1, MAX_LINE_SIZE);
        DO_CALLOC_AND_CHECK_PROBLEM_RETURN(name2, MAX_LINE_SIZE);
        int result = scanf("%s %s", name1, name2);
        getchar();
        if (result < 2) {
            return kFailure;
        }
        CHECK_ERROR_RETURN(CompareNames(tree->root, name1, name2));
        free(name1);
        free(name2);
        return kSuccess;
    } 
    default:
        fprintf(stderr, "No such mode.");
        return kFailure;
    }
}