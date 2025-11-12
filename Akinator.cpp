#include "Akinator.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

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

static TreeErrors AddNewCharacter(TreeNode_t *node, size_t *size) {
    assert(node);
    assert(size);

    printf(BLUE "\nОтветьте тогда, кого Вы загадывали? Введите имя (инициалы, прозвище):\n" RESET);

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(name, MAX_LINE_SIZE);
    ReadLine(name);

    printf(BLUE "\nОтлично! Чем \"%s\" отличается от \"%s\"? Он ... :\n" RESET, name, node->data);

    DO_CALLOC_AND_CHECK_PROBLEM_RETURN(new_question, MAX_LINE_SIZE); 
    ReadLine(new_question);

    NodesInsertAtTheEnd(node, name, new_question, size);
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

    AddNewCharacter(node, &tree->size);

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
    bool yes = false;

    while (node) {
        question = node->data;
        yes = AskYesNo(question);
        if (!node->left && !node->right) {
            if (yes) {
                return DoCorrectGuess(tree, node, Info);
            } else {
                return DoWrongGuess(tree, node, Info);
            }
        } else {
            if (yes) {
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

TreeErrors NodesInsertAtTheEnd(TreeNode_t *node, char *name, char *question, size_t *size) {
    assert(node);
    assert(name);
    assert(question);
    assert(size);

    TreeNode_t *new_node_left = NULL;
    NodeCtor(&new_node_left, &name);

    TreeNode_t *new_node_right = NULL;
    NodeCtor(&new_node_right, &(node->data));

    node->left = new_node_left;
    node->right = new_node_right;
    new_node_left->parent = node;
    new_node_right->parent = node;
    (*size)++;

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

TreeErrors PrintDefinition(TreeNode_t *current, TreeNode_t *prev,
                           char *definition_str, size_t buffer_len) {
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
        static char buf[256];
        snprintf(buf, sizeof(buf), "не %s", current->data);
        text = buf;
    }

    if (text) {
        if (definition_str[0] != '\0' && (current->left || current->right)) {
            strncat(definition_str, ", ", buffer_len - strlen(definition_str) - 1);
        }
        strncat(definition_str, text, buffer_len - strlen(definition_str) - 1);
        printf("%s", text);
        if ((current->left || current->right) && current->parent) printf(", ");
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

    printf("===========DEFINITION===========\n");
    printf("%s - address: %p\nDefinition: ", value, address);

    char *definition_str = (char *) calloc (buffer_len, sizeof(char));
    if (!definition_str) {
        fprintf(stderr, "No memory for definition string\n");
        return kNoMemory;
    }

    if (address->parent) {
        PrintDefinition(address->parent, address, definition_str, buffer_len);
    } else {
        strcpy(definition_str, value);
    }

    printf("\n================================\n");

    size_t size_of_command = buffer_len + strlen(value) + 5;
    char *command = (char *) calloc(size_of_command, sizeof(char));
    snprintf(command, size_of_command, "say \"%s - это %s\"", value, definition_str);
    fprintf(stderr, "Running command: %s\n", command);
    int ret = system(command);
    fprintf(stderr, "Return code: %d\n", ret);

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
long long size_of_file(const char *filename) {
    assert(filename != NULL);

    struct stat stbuf = {};

    int err = stat(filename, &stbuf);
    if (err != kSuccess) {
        perror("stat() failed");
        return kErrorStat;
    }

    return stbuf.st_size;
}

int is_blank_line(const char *str, size_t line_size) {
    assert(str != NULL);

    for (size_t i = 0; i < line_size; i++) {
        if (!isspace((unsigned char)str[i]))
            return 0;
    }
    return 1;
}

size_t count_lines(char *buf_ptr) {
    assert(buf_ptr != NULL);

    size_t counter = 0;
    char *line_start = buf_ptr;
    char *ptr = buf_ptr;

    while (*ptr != '\0') {
        if (*ptr == '\n') {
            size_t line_len = (size_t)(ptr - line_start);

            int has_alpha = 0;
            for (size_t i = 0; i < line_len; i++) {
                if (isalpha((unsigned char)line_start[i])) {
                    has_alpha = 1;
                    break;
                }
            }

            if (!is_blank_line(line_start, line_len) && has_alpha) {
                counter++;
            }

            line_start = ptr + 1;
        }
        ptr++;
    }

    return counter;
}

char *read_to_buf(const char *filename, FILE *file, size_t filesize) {
    assert(filename != NULL);
    assert(file     != NULL);

    char *buf_in = (char *) calloc(filesize + 2, sizeof(char));
    assert(buf_in != NULL);

    size_t bytes_read = fread(buf_in, sizeof(buf_in[0]), filesize, file);
    if (bytes_read == 0) {
        buf_in[0] = '\n';
        buf_in[1] = '\0';

    } else {
        if (buf_in[bytes_read - 1] != '\n') {
            buf_in[bytes_read] = '\n';
            bytes_read++;
        }

        buf_in[bytes_read] = '\0';
    }

    return buf_in;
}

void parse_buf(FileInfo *file_info) {
    assert(file_info != NULL);

    size_t line_idx = 0;
    char *line_start = file_info->buf_ptr;
    char *alpha_start = NULL;
    char *alpha_end = NULL;
    size_t bufsize = file_info->filesize + 1;

    for (size_t i = 0; i <= bufsize; i++) {
        char c = file_info->buf_ptr[i];
        int end_of_buffer = (i == bufsize);
        int end_of_line = (c == '\n' || end_of_buffer);

        if (!end_of_line) {
            if (isalpha((unsigned char)c)) {
                if (alpha_start == NULL) {
                    alpha_start = &(file_info->buf_ptr[i]);
                }

                alpha_end = &(file_info->buf_ptr[i]);
            }
        }

        if (end_of_line) {
            long line_len = &(file_info->buf_ptr[i]) - line_start;
            if (!is_blank_line(line_start, (size_t)line_len) && alpha_start != NULL) {

                file_info->text_ptr[line_idx].start_ptr = line_start;
                file_info->text_ptr[line_idx].end_ptr = &(file_info->buf_ptr[i - 1]);

                file_info->text_ptr[line_idx].start_ptr_alpha = alpha_start;
                file_info->text_ptr[line_idx].end_ptr_alpha = alpha_end;

                file_info->text_ptr[line_idx].size = 
                (size_t)(&(file_info->buf_ptr[i - 1]) - line_start) + 1;

                line_idx++;
            }

            line_start = &(file_info->buf_ptr[i + 1]);
            alpha_start = NULL;
            alpha_end = NULL;
        }
    }
}

TreeErrors DoBufRead(FILE *file, const char *filename, FileInfo *Info) {
    assert(file);
    assert(filename);
    assert(Info);

    // FILE *file = fopen(filename, "r");
    // if (file == NULL) {
    //     perror("fopen() failed.");
    //     return kErrorOpeningFile;
    // }

    Info->filesize = (size_t)size_of_file(filename);

    Info->buf_ptr = read_to_buf(filename, file, Info->filesize);
    assert(Info->buf_ptr != NULL);

    Info->count_lines = (int)count_lines(Info->buf_ptr);

    Info->text_ptr = (LineInfo *) calloc((size_t)Info->count_lines + 1, sizeof(LineInfo));
    assert(Info->text_ptr != NULL);

    parse_buf(Info);

    return kSuccess;
}

void SkipSpaces(TreeElem_t buffer, size_t *pos) {
    assert(buffer);
    assert(pos);

    while (buffer[*pos] == ' ') {
        (*pos)++;
    }
}

TreeElem_t ReadTitle(TreeElem_t buffer, size_t *pos) {
    assert(buffer);
    assert(pos);

    char tmp[256] = {};
    int cnt = 0;
        size_t pos_prev = *pos;
    
    SkipSpaces(buffer, pos);
    
    if (buffer[*pos] != '"') {
        fprintf(stderr, "Syntax error: Expected '\"' at position %zu\n", *pos);
        return NULL;
    }
    
    int result = sscanf(buffer + *pos, "\"%*[^\"]\"%n", &cnt);
    
    printf("%s %d ", buffer + *pos, cnt);
    if (result < 0) {
        fprintf(stderr, "Syntax error: Failed to read quoted string at position %zu\n", *pos);
        return NULL;
    }
    
    buffer[*pos + cnt - 1] = '\0';
    TreeElem_t start_ptr = buffer + *pos + 1;
    *pos += cnt;

    // printf("\nRead title: '%s' at positions %zu-%zu (length: %d)\n", 
    //        tmp, pos_prev, *pos, cnt);

    return start_ptr;
}

TreeNode_t *ReadNodeFromFile(FILE *file, size_t *pos, TreeNode_t *node, TreeElem_t buffer, int *error) {
    assert(file);
    //assert(node);
    assert(pos);
    assert(error);

    *error = 0;

    SkipSpaces(buffer, pos);
    // printf("-----%c ", buffer[*pos]);
    // printf("%s ", buffer + *pos);
    if (buffer[*pos] == '(') {
        TreeNode_t *new_node = NULL;
        NodeCtor(&new_node, NULL);
        (*pos)++;
        new_node->data = ReadTitle(buffer, pos);
        new_node->parent = node;
        //*pos += strlen(new_node->data);

        SkipSpaces(buffer, pos);
        new_node->left = ReadNodeFromFile(file, pos, new_node, buffer, error);
        if (*error) return NULL;

        SkipSpaces(buffer, pos);
        new_node->right = ReadNodeFromFile(file, pos, new_node, buffer, error);
        if (*error) return NULL;

        if (buffer[*pos] == ')') {
            buffer[*pos] = '\0';
            (*pos)++;
        } else {
            fprintf(stderr, "Syntax error: expected ')'\n");
            *error = 1;
            return NULL;
        }

        return new_node;

    } else if (strncmp(buffer + *pos, "nil", sizeof("nil") - 1) == 0) {
        *pos += strlen("nil");
        return NULL;
    } else {
        fprintf(stderr, "Syntax error in %d %c", *pos, buffer[*pos]);
        *error = 1;
        return NULL;
    }
}