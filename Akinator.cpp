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

    char *answer = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    if (!answer) {
        fprintf(stderr, "Ошибка: не удалось выделить память для ответа.\n");
        return false;
    }

    printf("%s (да/нет): ", question);
    scanf("%29[^\n]", answer);
    getchar();

    bool result = (strncmp(answer, "да", 3) == 0);
    free(answer);

    return result;
}

static bool PlayAgain(void) {
    char *answer = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    if (!answer) {
        fprintf(stderr, "Ошибка: не удалось выделить память для ответа.\n");
        return false;
    }

    printf("\nЕсли хотите сыграть еще раз, введите CONT, иначе QUIT:\n");
    scanf("%29[^\n]", answer);
    getchar();

    bool result = (strncmp(answer, "CONT", sizeof("CONT") - 1) == 0);
    free(answer);

    return result;
}

static TreeErrors AddNewCharacter(TreeNode_t *node) {
    assert(node);

    char *name = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    char *new_question = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    if (!name || !new_question) {
        fprintf(stderr, "Ошибка: не удалось выделить память.\n");
        free(name);
        free(new_question);
        return kNoMemory;
    }

    printf("\nОтветьте тогда, кого Вы загадывали? Введите имя (инициалы, прозвище):\n");
    scanf("%29[^\n]", name);
    getchar();

    printf("\nОтлично! Чем \"%s\" отличается от \"%s\"? Он ... :\n", name, node->data);
    scanf("%29[^\n]", new_question);
    getchar();

    NodesInsertAtTheEnd(node, strdup(name), strdup(new_question));

    printf("\nХорошо, Акинатор переписан. Теперь в нем есть данный персонаж.\n");

    free(name);
    free(new_question);

    return kSuccess;
}

static TreeErrors HandleCorrectGuess(TreeNode_t *head) {
    assert(head);

    printf("\nОтлично, ответ угадан!\n");
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

    size_t len = strlen(node->data) + 3;
    char *question = (char *) calloc (len, sizeof(char));
    if (!question) {
        fprintf(stderr, "Ошибка: не удалось выделить память для вопроса.\n");
        return kNoMemory;
    }

    snprintf(question, len, "\nЭто %s?", node->data);

    bool yes = AskYesNo(question);
    free(question);

    if (!node->left && !node->right) {
        if (yes) {
            return HandleCorrectGuess(head);
        } else {
            return HandleWrongGuess(node, head);
        }
    } else {
        if (yes) {
            if (node->left)
                return Akinator(node->left);
            else
                fprintf(stderr, "Ошибка: нет левого узла для ответа 'да'.\n");
        } else {
            if (node->right)
                return Akinator(node->right);
            else
                fprintf(stderr, "Ошибка: нет правого узла для ответа 'нет'.\n");
        }
    }

    return kSuccess;
}

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, const char *name, char *question) {
    assert(node);
    assert(name);
    assert(question);

    TreeNode_t *new_node_left = NULL;
    NodeCtor(&new_node_left, &name);

    TreeNode_t *new_node_right = NULL;
    NodeCtor(&new_node_right, &(node->data));

    node->left  = new_node_left;
    node->right = new_node_right;

    size_t len = strlen(question);
    char *new_question = (char *) calloc (len + 2, sizeof(char));
    if (!new_question) return kNoMemory;

    strcpy(new_question, question);
    new_question[len] = '?';
    new_question[len + 1] = '\0';

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

    if (node->left)
        PrintAkinatorToFile(file, node->left);
    else
        fprintf(file, " nill");

    if (node->right)
        PrintAkinatorToFile(file, node->right);
    else
        fprintf(file, " nill");

    fprintf(file, " )");
}

TreeNode_t *FindAkinatorNodeAddress(TreeNode_t *node, const char *value) {
    assert(node);
    assert(value);

    if (strcmp(value, node->data) == 0) {
        return node;
    } else {
        if (!node->left) FindAkinatorNodeAddress(node->left, value);
        if (!node->right) FindAkinatorNodeAddress(node->right, value);
    }
    return NULL;
}

TreeErrors PrintDefinition(TreeNode_t *node, const char *value) {
    assert(node);
    assert(value);

    static TreeNode_t *head = node;
    TreeNode_t *address = FindAkinatorNodeAddress(node, value);
    if (!address) {
        return kNoSuchNode;
    }

    while (node != head) {
        printf(" %s,", node->data);
        node = node->parent;
    }
    return kSuccess;
}
