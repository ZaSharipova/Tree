#include "Akinator.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "Enums.h"
#include "TreeFunctions.h"

#define MAX_LINE_SIZE 30
#define YES_ANSWER "да"
#define NO_ANSWER "нет"
#define CONTINUE_ANSWER "CONT"
#define QUIT_ANSWER "QUIT"

static bool AskYesNo(const char *question) {
    assert(question);

    bool flag = false;
    char *answer = (char *) calloc (MAX_LINE_SIZE, sizeof(char));

    printf(YELLOW "\nЭто %s ? (" YES_ANSWER"/" NO_ANSWER"): " RESET, question);
    scanf("%29[^\n]", answer);
    getchar();

    flag = strncmp(answer, YES_ANSWER, sizeof(YES_ANSWER)) == 0;
    free(answer);

    return flag;
}

static bool PlayAgain(void) {

    bool flag = false;
    char *answer = (char *) calloc (MAX_LINE_SIZE, sizeof(char));

    printf(GREEN "\nЕсли хотите сыграть еще раз, введите CONT, иначе QUIT:\n" RESET);
    scanf("%29[^\n]", answer);
    getchar();

    flag = strncmp(answer, CONTINUE_ANSWER, sizeof(CONTINUE_ANSWER)) == 0;
    free(answer);

    return flag;
}

static TreeErrors AddNewCharacter(TreeNode_t *node) {
    assert(node);

    char *name = (char *) calloc (MAX_LINE_SIZE, sizeof(char));
    char *new_question = (char *) calloc (MAX_LINE_SIZE, sizeof(char));

    printf(BLUE "\nОтветьте тогда, кого Вы загадывали? Введите имя (инициалы, прозвище):\n" RESET);
    scanf("%29[^\n]", name);
    getchar();

    printf(BLUE "\nОтлично! Чем \"%s\" отличается от \"%s\"? Он ... :\n" RESET, name, node->data);
    scanf("%29[^\n]", new_question);
    getchar();

    NodesInsertAtTheEnd(node, name, new_question);

    // free(name);
    // free(new_question);

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

    const char *question = node->data; 
    //char question[MAX_LINE_SIZE] = {0};
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

    size_t len = strlen(question);
    char *new_question = (char *) calloc (len + 2, sizeof(char));
    if (!new_question) return kNoMemory;

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
                size_t len = strlen(current->data) + 5;
                char *negated = (char *) calloc (len, sizeof(char));
                if (!negated) {
                    fprintf(stderr, "No memory for calloc array of definitions.\n");
                    return kNoMemory;
                }

                snprintf(negated, len, "не %s", current->data);
                array_of_definitions[pos++] = negated;
            }
        }
        prev = current;
        //printf("%s, ", node->data);
        current = current->parent;
    }

    for (int i = (int)pos - 1; i >= 0; --i) {
        printf("%s", array_of_definitions[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    //printf("%s\n", node->data);
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