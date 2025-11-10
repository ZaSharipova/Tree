#include "Akinator.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Enums.h"
#include "TreeFunctions.h"

#define MAX_LINE_SIZE 30


static bool AskYesNo(const char *question) {
    assert(question);

    bool flag = false;
    char *answer = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    //char answer[MAX_LINE_SIZE] = {0};
    printf(YELLOW "\nЭто %s ? (да/нет): " RESET, question);
    scanf("%29[^\n]", answer);
    getchar();

    flag = strncmp(answer, "да", 3) == 0;
    free(answer);

    return flag;
}

static bool PlayAgain(void) {

    bool flag = false;
    char *answer = (char *) calloc (MAX_LINE_SIZE, sizeof(answer));
    //char answer[MAX_LINE_SIZE] = {0};
    printf(GREEN "\nЕсли хотите сыграть еще раз, введите CONT, иначе QUIT:\n" RESET);
    scanf("%29[^\n]", answer);
    getchar();
    flag = strncmp(answer, "CONT", sizeof("CONT")) == 0;
    free(answer);

    return flag;
}

static TreeErrors AddNewCharacter(TreeNode_t *node) {
    assert(node);

    char *name = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    char *new_question = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    // char name[MAX_LINE_SIZE] = {0};
    // char new_question[MAX_LINE_SIZE] = {0};

    printf(BLUE "\nОтветьте тогда, кого Вы загадывали? Введите имя (инициалы, прозвище):\n" RESET);
    scanf("%29[^\n]", name);
    getchar();

    printf(BLUE "\nОтлично! Чем \"%s\" отличается от \"%s\"? Он ... :\n" RESET, name, node->data);
    scanf("%29[^\n]", new_question);
    getchar();

    NodesInsertAtTheEnd(node, strdup(name), strdup(new_question));
    //free(node->data);
    // new_question[strlen(new_question)] = '?';
    // node->data = strdup(new_question);

    free(name);
    free(new_question);

    printf("\nХорошо, Акинатор переписан. Теперь в нем есть данный персонаж.\n");

    return kSuccess;
}

static TreeErrors HandleCorrectGuess(TreeNode_t *head) {
    assert(head);

    printf(RED "\nОтлично, ответ угадан!\n" RESET);
    if (PlayAgain()) {
        return Akinator(head);
    }
    return kSuccess;
}

static TreeErrors HandleWrongGuess(TreeNode_t *node, TreeNode_t *head) {
    assert(node);
    assert(head);

    AddNewCharacter(node);
    if (PlayAgain()) {
        return Akinator(head);
    }
    return kSuccess;
}


TreeErrors Akinator(TreeNode_t *node) {
    assert(node);

    static TreeNode_t *head = NULL;
    if (!head) head = node;

    char *question = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    //char question[MAX_LINE_SIZE] = {0};
    strcpy(question, node->data);
    //snprintf(question, MAX_LINE_SIZE, "%s", node->data);

    bool yes = AskYesNo(question);

    if (!node->left && !node->right) {
        if (yes) {
            return HandleCorrectGuess(head);
        } else {
            return HandleWrongGuess(node, head);
        }

    } else {
        if (yes) {
            if (node->left) {
                return Akinator(node->left);
            } else {
                fprintf(stderr, "Ошибка: нет левого узла для ответа 'да'.\n");
            }
        } else {
            if (node->right) {
                return Akinator(node->right);
            } else {
                fprintf(stderr, "Ошибка: нет правого узла для ответа 'нет'.\n");
            }
        }
    }
    free(question);

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

    size_t len = strlen(question);
    char *new_question = (char *) calloc (len + 2, sizeof(char));
    if (!new_question) return kNoMemory;

    strcpy(new_question, question);

    node->data = new_question;
    free(new_question);

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
    if (!node) return kFailure;
    if (strcmp(value, node->data) == 0) {
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


TreeErrors PrintDefinition(TreeNode_t *node, const char *value, int count) {
    assert(node);
    assert(value);

    static TreeNode_t *head = node;
    TreeNode_t *address = NULL;
    FindAkinatorNodeAddress(node, value, &address);

    if (!address) {
        printf("%s address not found.", value);
        return kNoSuchNode;
    }
    printf("===========DEFINITION===========\n");
    printf("%s - address: %p\nDefinition: ", value, address);


    while (node != head) {
        printf("%s, ", node->data);
        node = node->parent;
    }
    printf("%s\n", node->data);
    printf("================================");
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
        fprintf(stderr, "No address have found.");
    }

    FindAkinatorNodeAddress(node, value2, &address2);
    if (!address2) {
        fprintf(stderr, "No address have found.");
    }
    
    node = head;
    printf("Схожи в: ");
    while (node != address1) {
        printf("%s, ", node->data);
        node = node->parent;
    }

    // printf("Различаются в: ");
    // do {
    //     printf() ...
    // } while (node);
    return kSuccess;

}
