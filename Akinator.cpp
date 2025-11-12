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
    int char_symbol = 0;
    while ((char_symbol = getchar()) != '\n' && char_symbol != EOF);
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

    printf(YELLOW "\nЭто %s? (" YES_ANSWER"/" NO_ANSWER"):\n" RESET, question);
    return AskAndReturnYesNo(YES_ANSWER, sizeof(YES_ANSWER));
}

static bool PlayAgain(void) {

    printf(GREEN "\nЕсли хотите сыграть еще раз, введите CONT, иначе QUIT:\n" RESET);
    return AskAndReturnYesNo(CONTINUE_ANSWER, sizeof(CONTINUE_ANSWER));
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

static TreeErrors DoCorrectGuess(Tree_t *tree, TreeNode_t *node, DumpInfo *Info) {
    assert(tree);
    assert(node);
    assert(Info);

    printf(RED "\nОтлично, ответ угадан!\n" RESET);
    if (PlayAgain()) {
        return Akinator(tree, tree->root, Info);
    }

    return kSuccess;
}

static TreeErrors DoWrongGuess(Tree_t *tree, TreeNode_t *node, DumpInfo *Info) {
    assert(tree);
    assert(node);
    assert(Info);

    AddNewCharacter(node, tree);

    Info->question = node->data;
    Info->name = node->left->data;
    DoTreeInGraphviz((const TreeNode_t *)tree->root, Info);
    DoDump(Info);
            
    if (PlayAgain()) {
        return Akinator(tree, tree->root, Info);
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
    bool yes_flag = false;

    while (node) {
        question = node->data;
        yes_flag = AskYesNo(question);

        if (!node->left && !node->right) {
            if (yes_flag) {
                return DoCorrectGuess(tree, node, Info);
            } else {
                return DoWrongGuess(tree, node, Info);
            }

        } else {
            if (yes_flag) {
                if (node->left) {
                    node = node->left;
                } else {
                    fprintf(stderr, "Ошибка: нет левого узла для ответа 'да'.\n");
                    return kNoPossibleNode;
                }
            } else {
                if (node->right) {
                    node = node->right;
                } else {
                    fprintf(stderr, "Ошибка: нет правого узла для ответа 'нет'.\n");
                    return kNoPossibleNode;
                }
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
    tree->size ++;

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

    fprintf(file, "(\"%s\"", node->data);

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
    return kFailure;
}

TreeErrors PrintDefinition(TreeNode_t *current, TreeNode_t *prev, char *definition_str, size_t buffer_len) {
    assert(current);
    assert(prev);
    assert(definition_str);

    if (current->parent) {
        PrintDefinition(current->parent, current, definition_str, buffer_len);
    }

    const char *text = NULL;
    
    if (prev == current->left) {
        text = current->data;

    } else if (prev == current->right) {
        static char buf[MAX_LINE_SIZE] = {};
        snprintf(buf, sizeof(buf), "не %s", current->data);
        text = buf;
    }

    if (text) {
        if (definition_str[0] != '\0' && (current->left || current->right)) {
            strncat(definition_str, ", ", buffer_len - strlen(definition_str) - 1);
        }
        strncat(definition_str, text, buffer_len - strlen(definition_str) - 1);
        printf("%s", text);
        if ((current->left || current->right) && current->left->left && current->left->left) printf(", ");
    }

    return kSuccess;
}


TreeErrors DoPrintDefinition(TreeNode_t *node, const char *value, size_t buffer_len) {
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

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(definition_str, buffer_len);

    if (address->parent) {
        PrintDefinition(address->parent, address, definition_str, buffer_len);
    }

    printf("\n==================================\n");

    size_t size_of_command = buffer_len + strlen(value) + strlen(" - это ");
    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(command, size_of_command);

    snprintf(command, size_of_command, "say \"%s - это %s\"", value, definition_str);
    fprintf(stderr, "Running command: %s\n", command);
    int return_result = system(command);
    fprintf(stderr, "Return code: %d\n", return_result);

    free(definition_str);
    free(command);
    return kSuccess;
}



// TreeErrors CompareResults(TreeNode_t *node, const char *value1, const char *value2, int count) {
//     assert(node);
//     assert(value1);
//     assert(value2);

//     // static TreeNode_t *head = node;

//     TreeNode_t *address1 = NULL;
//     TreeNode_t *address2 = NULL;

//     FindAkinatorNodeAddress(node, value1, &address1);
//     if (!address1) {
//         fprintf(stderr, "Address for '%s' not found.\n", value1);
//         return kNoSuchNode;
//     }

//     FindAkinatorNodeAddress(node, value2, &address2);
//     if (!address2) {
//         fprintf(stderr, "Address for '%s' not found.\n", value2);
//         return kNoSuchNode;
//     }

//     TreeNode_t **path1 = (TreeNode_t **) calloc (count, sizeof(TreeNode_t *));
//     if (!path1) return kNoMemory;

//     TreeNode_t **path2 = (TreeNode_t **) calloc (count, sizeof(TreeNode_t *));
//     if (!path2) {
//         free(path1);
//         return kNoMemory;
//     }
//     int len1 = 0;
//     int len2 = 0;

//     for (TreeNode_t *cur = address1; cur != NULL; cur = cur->parent) {
//         if (len1 >= count) break;
//         path1[len1++] = cur;
//     }
//     for (TreeNode_t *cur = address2; cur != NULL; cur = cur->parent) {
//         if (len2 >= count) break;
//         path2[len2++] = cur;
//     }

//     for (int i = 0; i < len1 / 2; i++) {
//         TreeNode_t *tmp = path1[i];
//         path1[i] = path1[len1 - 1 - i];
//         path1[len1 - 1 - i] = tmp;
//     }
//     for (int i = 0; i < len2 / 2; i++) {
//         TreeNode_t *tmp = path2[i];
//         path2[i] = path2[len2 - 1 - i];
//         path2[len2 - 1 - i] = tmp;
//     }

//     int common_len = 0;
//     int min_len = (len1 < len2) ? len1 : len2;
//     for (int i = 0; i < min_len; i++) {
//         if (strcmp(path1[i]->data, path2[i]->data) == 0) {
//             common_len++;
//         } else {
//             break;
//         }
//     }

//     printf("Общие предки:\n");
//     for (int i = 0; i < common_len; i++) {
//         printf("  %s\n", path1[i]->data);
//     }
//     printf("\n");

//     printf("Уникальные предки для %s:\n", value1);
//     for (int i = common_len; i < len1; i++) {
//         printf("  %s\n", path1[i]->data);
//     }
//     printf("\n");

//     printf("Уникальные предки для %s:\n", value2);
//     for (int i = common_len; i < len2; i++) {
//         printf("  %s\n", path2[i]->data);
//     }

//     free(path1);
//     free(path2);
//     return kSuccess;
// }