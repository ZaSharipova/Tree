#include "ReadTree.h"

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "Enums.h"
#include "TreeFunctions.h"

long long SizeOfFile(const char *filename) {
    assert(filename);

    struct stat stbuf = {};

    int err = stat(filename, &stbuf);
    if (err != kSuccess) {
        perror("stat() failed");
        return kErrorStat;
    }

    return stbuf.st_size;
}

char *ReadToBuf(const char *filename, FILE *file, size_t filesize) {
    assert(filename);
    assert(file);

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

void DoBufRead(FILE *file, const char *filename, FileInfo *Info) {
    assert(file);
    assert(filename);
    assert(Info);

    Info->filesize = (size_t)SizeOfFile(filename);

    Info->buf_ptr = ReadToBuf(filename, file, Info->filesize);
    assert(Info->buf_ptr != NULL);
}

void SkipSpaces(TreeElem_t buffer, size_t *pos) {
    assert(buffer);
    assert(pos);

    while (buffer[*pos] == ' ') {
        (*pos)++;
    }
}

TreeElem_t ReadTitle(FILE *logfile, TreeElem_t buffer, size_t *pos) {
    assert(logfile);
    assert(buffer);
    assert(pos);
    
    SkipSpaces(buffer, pos);
    
    if (buffer[*pos] != '"') {
        fprintf(stderr, "Syntax error: Expected '\"' at position %zu\n", *pos);
        return NULL;
    }
    
    int cnt = 0;
    int result = sscanf(buffer + *pos, "\"%*[^\"]\"%n", &cnt);
    if (result < 0) {
        fprintf(stderr, "Syntax error: Failed to read quoted string at position %zu\n", *pos);
        return NULL;
    }
    fprintf(logfile, "%d %s", cnt, buffer + *pos);
    
    buffer[*pos + (size_t)cnt - 1] = '\0';
    TreeElem_t start_ptr = buffer + *pos + 1;
    *pos += (size_t)cnt;

    return start_ptr;
}

TreeErrors ReadNodeFromFile(FILE *file, FILE *logfile, size_t *pos, TreeNode_t *node, TreeElem_t buffer, TreeNode_t **node_to_add) {
    assert(file);
    assert(logfile);
    //assert(node);
    assert(pos);
    //assert(node_to_add);

    TreeErrors err = kSuccess;
    SkipSpaces(buffer, pos);

    if (buffer[*pos] == '(') {
        TreeNode_t *new_node = NULL;
        NodeCtor(&new_node, NULL);
        (*pos)++;
        new_node->data = ReadTitle(logfile, buffer, pos);
        new_node->parent = node;

        SkipSpaces(buffer, pos);
        TreeNode_t *left_child = NULL;
        CHECK_ERROR_RETURN(ReadNodeFromFile(file, logfile, pos, new_node, buffer, &left_child));
        new_node->left = left_child;

        SkipSpaces(buffer, pos);
        TreeNode_t *right_child = NULL;
        CHECK_ERROR_RETURN(ReadNodeFromFile(file, logfile, pos, new_node, buffer, &right_child));
        new_node->right = right_child;

        if (buffer[*pos] == ')') {
            buffer[*pos] = '\0';
            (*pos)++;
        } else {
            fprintf(stderr, "Syntax error: expected ')'\n");
            return kSyntaxError;
        }
        *node_to_add = new_node;
        return kSuccess;

    } else if (strncmp(buffer + *pos, "nil", sizeof("nil") - 1) == 0) {
        *pos += strlen("nil");
        *node_to_add = NULL;
        return kSuccess;

    } else {
        fprintf(stderr, "Syntax error in %zu %c", *pos, buffer[*pos]);
        return kSyntaxError;
    }

    return kSuccess;
}