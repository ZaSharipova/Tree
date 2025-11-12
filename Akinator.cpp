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

const char *AndPhrases[] = {
    "и",
    "а также",
    "более того",
    "еще",
    "к тому же",
    "плюс",
};
#define AndPhasesSize sizeof(AndPhrases)/sizeof(AndPhrases[0])

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
        flag = kYes;
    } else if (strncmp(answer, type_of_answer_no, strlen(type_of_answer_no)) == 0) {
        flag = kNo;
    }

    free(answer);

    return flag;
}

static TypeOfAnswer AskYesNo(const char *question) {
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

static TreeErrors DoCorrectGuess(Tree_t *tree, TreeNode_t *node, DumpInfo *Info) {
    assert(tree);
    assert(node);
    assert(Info);

    printf(RED "\nОтлично, ответ угадан!\n" RESET);
    if (PlayAgain() == kYes) {
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
            
    if (PlayAgain() == kYes) {
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
            if (yes_flag == kYes) {
                return DoCorrectGuess(tree, node, Info);
            } else if (yes_flag == kNo){
                return DoWrongGuess(tree, node, Info);
            } else {
                return kFailure;
            }

        } else {
            if (yes_flag == kYes) {
                if (node->left) {
                    node = node->left;
                } else {
                    fprintf(stderr, "Ошибка: нет левого узла для ответа 'да'.\n");
                    return kNoPossibleNode;
                }
            } else if (yes_flag == kNo) {
                if (node->right) {
                    node = node->right;
                } else {
                    fprintf(stderr, "Ошибка: нет правого узла для ответа 'нет'.\n");
                    return kNoPossibleNode;
                }
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

TreeErrors PrintDefinition(TreeNode_t *current, TreeNode_t *prev, char *definition_str, size_t buffer_len, size_t *pos_in_phrases) {
    assert(current);
    assert(prev);
    assert(definition_str);
    assert(pos_in_phrases);

    static int pos = 1;

    if (current->parent) {
        PrintDefinition(current->parent, current, definition_str, buffer_len, pos_in_phrases);
    }

    char *text = NULL;
    char buf[MAX_LINE_SIZE] = {};
    
    if (prev == current->left) {
        pos = (pos + 1) % AndPhasesSize;
        snprintf(buf, sizeof(buf), "%s %s", AndPhrases[pos], current->data);
        text = buf;

    } else if (prev == current->right) {
        pos = (pos + 1) % AndPhasesSize;
        snprintf(buf, sizeof(buf), "%s не %s", AndPhrases[pos], current->data);
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

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(definition_str, buffer_len);

    size_t pos = 0;
    if (address->parent) {
        PrintDefinition(address->parent, address, definition_str, buffer_len, &pos);
    }

    printf("\n==================================\n");

    size_t size_of_command = buffer_len + strlen(value) + strlen(" - это ") + tree_size * 11;
    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(command, size_of_command);

    snprintf(command, size_of_command, "say \"%s - это %s\"", value, definition_str);
    fprintf(stderr, "Running command: %s\n", command);
    int return_result = system(command);
    fprintf(stderr, "Return code: %d\n", return_result);

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

TreeErrors CompareNames(TreeNode_t *head, const char *value1, const char *value2) {
    assert(head);
    assert(value1);
    assert(value2);

    TreeErrors err = kSuccess;
    TreeNode_t *node1 = NULL;
    CHECK_ERROR_RETURN(FindAkinatorNodeAddress(head, value1, &node1));

    TreeNode_t *node2 = NULL;
    CHECK_ERROR_RETURN(FindAkinatorNodeAddress(head, value2, &node2));

    TreeNode_t *same_parent = FindClosestSameNode(node1, node2);

    TreeNode_t *node = same_parent;
    printf("==================Names=Comparison==============\n");
    printf("%s и %s похожи в: ", value1, value2);
    if (node->parent) {
        PrintSimpleDefinition(node->parent);
    }
    // if (node->parent || node == head) {
    //     while (node->parent && node != same_parent) {
    //         node = node->parent;
    //         printf("%s, ", node->data);
    //     }
    // }


    printf("\nНо %s — ", value1);

    if (node1->parent == same_parent->parent) {
        printf("%s", node1 == same_parent->left ? same_parent->left->data : node1->data);
    } else if (node1 == same_parent->right) {
        printf("не %s", same_parent->data);
    } else {
        printf("%s", node1->data);
    }

    printf(", а %s — ", value2);
    if (node2 == same_parent->right->parent) {
        printf("не %s", same_parent->data);
    } else
        printf("%s", node2->data);

    printf(".\n==================================\n");
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