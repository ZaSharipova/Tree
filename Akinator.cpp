#include "Akinator.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Enums.h"
#include "TreeFunctions.h"
#include "TreeGraph.h"
#include "TreeDump.h"

#define MAX_LINE_SIZE 30
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


static void ReadLine(char *answer) {
    assert(answer);

    scanf("%" MAX_LINE_SPECIFIER "[^\n]", answer);
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

static bool AskAndReturnYesNo(const char *type_of_answer, int size_of_line) {
    assert(type_of_answer);

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(answer, MAX_LINE_SIZE);
    ReadLine(answer);

    bool flag = false;
    flag = strncmp(answer, type_of_answer, (size_t)size_of_line) == 0;
    free(answer);

    return flag;
}

static bool AskYesNo(const char *question) {
    assert(question);

    printf(YELLOW "\nЭто %s? (" YES_ANSWER"/" NO_ANSWER"): " RESET, question);
    return AskAndReturnYesNo(YES_ANSWER, sizeof(YES_ANSWER));
}

static bool PlayAgain(void) {

    printf(GREEN "\nЕсли хотите сыграть еще раз, введите CONT, иначе QUIT:\n" RESET);
    return AskAndReturnYesNo(CONTINUE_ANSWER, sizeof(CONTINUE_ANSWER));
}

static TreeErrors AddNewCharacter(TreeNode_t *node) {
    assert(node);

    printf(BLUE "\nОтветьте тогда, кого Вы загадывали? Введите имя (инициалы, прозвище):\n" RESET);

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(name, MAX_LINE_SIZE);
    ReadLine(name);

    printf(BLUE "\nОтлично! Чем \"%s\" отличается от \"%s\"? Он ... :\n" RESET, name, node->data);

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(new_question, MAX_LINE_SIZE); 
    ReadLine(new_question);

    NodesInsertAtTheEnd(node, name, new_question);
    printf(MAGENTA "\nХорошо, Акинатор дополнен. Теперь в нем есть данный персонаж!\n" RESET);

    return kSuccess;
}

static TreeErrors DoCorrectGuess(TreeNode_t *head, DumpInfo *Info) {
    assert(head);

    printf(RED "\nОтлично, ответ угадан!\n" RESET);
    if (PlayAgain()) {
        return Akinator(head, Info);
    }

    return kSuccess;
}

static TreeErrors DoWrongGuess(TreeNode_t *node, TreeNode_t *head, DumpInfo *Info) {
    assert(node);
    assert(head);
    assert(Info);

   AddNewCharacter(node);

    snprintf(Info->message, sizeof(Info->message), "Added new character with question %s?", node->data);
    DoTreeInGraphviz((const TreeNode_t *)head, Info);
    DoDump(Info);
            
    if (PlayAgain()) {
        return Akinator(head, Info);
    }

    return kSuccess;
}


TreeErrors Akinator(TreeNode_t *node, DumpInfo *Info) {
    assert(node);

    TreeErrors err = kSuccess;
    CHECK_ERROR_RETURN(NodeVerify(node));

    static TreeNode_t *head = NULL;
    if (!head) head = node;

    const char *question = node->data; 
    bool yes = AskYesNo(question);

    if (!node->left && !node->right) {
        if (yes) {
            return DoCorrectGuess(head, Info);
        } else {
            return DoWrongGuess(node, head, Info);
        }
    } else {
        if (yes) {
            if (node->left) {
                return Akinator(node->left, Info);
            } else {
                fprintf(stderr, "Ошибка: нет левого узла для ответа 'да'.\n");
            }
        } else {
            if (node->right) {
                return Akinator(node->right, Info);
            } else {
                fprintf(stderr, "Ошибка: нет правого узла для ответа 'нет'.\n");
            }
        }
    }

    CHECK_ERROR_RETURN(NodeVerify(node));

    return kSuccess;
}

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, char *name, char *question) {
    assert(node);
    assert(name);
    assert(question);

    TreeNode_t *new_node_left = NULL;
    NodeCtor(&new_node_left, &name);

    TreeNode_t *new_node_right = NULL;
    NodeCtor(&new_node_right, &(node->data));

    node->left = new_node_left;
    node->right = new_node_right;
    new_node_left->parent = node;
    new_node_right->parent = node;

    size_t len = strlen(question) + 2;
    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(new_question, len);

    strcpy(new_question, question);
    node->data = new_question;

    
    return kSuccess;
}

void DoAskGeneralQuestion(TreeNode_t *node) {
    assert(node);

    printf("Это %s?", node->data);
}

void PrintAkinatorToFile(FILE *file, TreeNode_t *node) {
    assert(file);
    assert(node);

    fprintf(file, "( \"%s\"", node->data);

    if (node->left) {
        PrintAkinatorToFile(file, node->left);
    } else {
        fprintf(file, " nill");
    }

    if (node->right) {
        PrintAkinatorToFile(file, node->right);
    } else {
        fprintf(file, " nill");
    }

    fprintf(file, " )");

}

TreeErrors FindAkinatorNodeAddress(TreeNode_t *node, const char *value, TreeNode_t **address) {
    assert(node);
    assert(value);
    assert(address);

    if (strcmp(value, node->data) == 0) { //
        *address = node;
        return kSuccess;
    }

    if (node->left) {
        if (FindAkinatorNodeAddress(node->left, value, address) == kSuccess) return kSuccess;
    }
    if (node->right) {
        if (FindAkinatorNodeAddress(node->right, value, address) == kSuccess) return kSuccess;
    }
    return kFailure;
}


TreeErrors PrintDefinition(TreeNode_t *node, const char *value, size_t count) {
    assert(node);
    assert(value);

    TreeNode_t *address = NULL;
    FindAkinatorNodeAddress(node, value, &address);

    if (!address) {
        printf("%s address not found.", value);
        return kNoSuchNode;
    }
    printf("===========DEFINITION===========\n");
    printf("%s - address: %p\nDefinition: ", value, address);


    char **array_of_definitions = (char **) calloc (count * 2 + 1, sizeof(char *));
    if (!array_of_definitions) {
        fprintf(stderr, "No memory for calloc array of definitions.\n");
        return kNoMemory;
    }

    size_t pos = 0;
    TreeNode_t *current = address->parent;
    TreeNode_t *prev = address;
    while (current) {
        if (prev) {
            if (current->left == prev) {
                array_of_definitions[pos++] = current->data;

            } else if (current->right == prev) {
                size_t len = strlen(current->data) * 4 + 4;
                DO_CALLOC_AND_CHECK_PROBLEM_RETURN(negated, len);

                snprintf(negated, len, "не %s", current->data);
                array_of_definitions[pos++] = negated;
            }
        }

        prev = current;
        current = current->parent;
    }

    for (int i = (int)pos - 1; i >= 0; i--) {
        printf("%s", array_of_definitions[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    printf("================================");

    for (size_t i = 0; i < pos; ++i) {
        if (strncmp(array_of_definitions[i], "не ", sizeof("не ")) == 0) {
            free(array_of_definitions[i]);
        }
    }

    free(array_of_definitions);
    return kSuccess;
}

TreeErrors CompareResults(TreeNode_t *node, const char *value1, const char *value2, int count) {
    assert(node);
    assert(value1);
    assert(value2);

    static TreeNode_t *head = node;

    TreeNode_t *address1 = NULL;
    TreeNode_t *address2 = NULL;

    FindAkinatorNodeAddress(node, value1, &address1);
    if (!address1) {
        fprintf(stderr, "Address for '%s' not found.\n", value1);
        return kNoSuchNode;
    }

    FindAkinatorNodeAddress(node, value2, &address2);
    if (!address2) {
        fprintf(stderr, "Address for '%s' not found.\n", value2);
        return kNoSuchNode;
    }

    TreeNode_t **path1 = (TreeNode_t **) calloc (count, sizeof(TreeNode_t *));
    if (!path1) return kNoMemory;

    TreeNode_t **path2 = (TreeNode_t **) calloc (count, sizeof(TreeNode_t *));
    if (!path2) {
        free(path1);
        return kNoMemory;
    }
    int len1 = 0;
    int len2 = 0;

    for (TreeNode_t *cur = address1; cur != NULL; cur = cur->parent) {
        if (len1 >= count) break;
        path1[len1++] = cur;
    }
    for (TreeNode_t *cur = address2; cur != NULL; cur = cur->parent) {
        if (len2 >= count) break;
        path2[len2++] = cur;
    }

    for (int i = 0; i < len1 / 2; i++) {
        TreeNode_t *tmp = path1[i];
        path1[i] = path1[len1 - 1 - i];
        path1[len1 - 1 - i] = tmp;
    }
    for (int i = 0; i < len2 / 2; i++) {
        TreeNode_t *tmp = path2[i];
        path2[i] = path2[len2 - 1 - i];
        path2[len2 - 1 - i] = tmp;
    }

    int common_len = 0;
    int min_len = (len1 < len2) ? len1 : len2;
    for (int i = 0; i < min_len; i++) {
        if (strcmp(path1[i]->data, path2[i]->data) == 0) {
            common_len++;
        } else {
            break;
        }
    }

    printf("Общие предки:\n");
    for (int i = 0; i < common_len; i++) {
        printf("  %s\n", path1[i]->data);
    }
    printf("\n");

    printf("Уникальные предки для %s:\n", value1);
    for (int i = common_len; i < len1; i++) {
        printf("  %s\n", path1[i]->data);
    }
    printf("\n");

    printf("Уникальные предки для %s:\n", value2);
    for (int i = common_len; i < len2; i++) {
        printf("  %s\n", path2[i]->data);
    }

    free(path1);
    free(path2);
    return kSuccess;
}
