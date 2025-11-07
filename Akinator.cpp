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

    char answer[MAX_LINE_SIZE] = {0};
    printf("%s (да/нет): ", question);
    scanf("%29[^\n]", answer);
    getchar();
    return strncmp(answer, "да", 3) == 0;
}

static bool PlayAgain(void) {

    char answer[MAX_LINE_SIZE] = {0};
    printf("\nЕсли хотите сыграть еще раз, введите CONT, иначе QUIT:\n");
    scanf("%29[^\n]", answer);
    getchar();

    return strncmp(answer, "CONT", 5) == 0;
}

static TreeErrors AddNewCharacter(TreeNode_t *node) {
    assert(node);

    char name[MAX_LINE_SIZE] = {0};
    char new_question[MAX_LINE_SIZE] = {0};

    printf("\nОтветьте тогда, кого Вы загадывали? Введите имя (инициалы, прозвище):\n");
    scanf("%29[^\n]", name);
    getchar();

    printf("\nОтлично! Чем \"%s\" отличается от \"%s\"? Он ... :\n", name, node->data);
    scanf("%29[^\n]", new_question);
    getchar();

    NodesInsertAtTheEnd(node, strdup(name));
    //free(node->data);
    new_question[strlen(new_question)] = '?';
    node->data = strdup(new_question);

    printf("\nХорошо, Акинатор переписан. Теперь в нем есть данный персонаж.\n");

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

    char question[MAX_LINE_SIZE] = {0};
    snprintf(question, sizeof(question), "\nЭто %s?", node->data);

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

    return kSuccess;
}

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, const char *name) {
    assert(node);
    assert(name);

    TreeNode_t *new_node_left = NULL;
    NodeCtor(&new_node_left, &name);

    TreeNode_t *new_node_right = NULL;
    NodeCtor(&new_node_right, &(node->data));

    node->left = new_node_left;
    node->right = new_node_right;

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
