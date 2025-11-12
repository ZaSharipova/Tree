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

    int cnt = 0;
    
    SkipSpaces(buffer, pos);
    
    if (buffer[*pos] != '"') {
        fprintf(stderr, "Syntax error: Expected '\"' at position %zu\n", *pos);
        return NULL;
    }
    
    int result = sscanf(buffer + *pos, "\"%*[^\"]\"%n", &cnt);
    fprintf(logfile, "%d %s", cnt, buffer + *pos);
    if (result < 0) {
        fprintf(stderr, "Syntax error: Failed to read quoted string at position %zu\n", *pos);
        return NULL;
    }
    
    buffer[*pos + (size_t)cnt - 1] = '\0';
    TreeElem_t start_ptr = buffer + *pos + 1;
    *pos += (size_t)cnt;

    return start_ptr;
}

TreeNode_t *ReadNodeFromFile(FILE *file, FILE *logfile, size_t *pos, TreeNode_t *node, TreeElem_t buffer, int *error) {
    assert(file);
    assert(logfile);
    //assert(node);
    assert(pos);
    assert(error);

    *error = 0;
    SkipSpaces(buffer, pos);

    if (buffer[*pos] == '(') {
        TreeNode_t *new_node = NULL;
        NodeCtor(&new_node, NULL);
        (*pos)++;
        new_node->data = ReadTitle(logfile, buffer, pos);
        new_node->parent = node;

        SkipSpaces(buffer, pos);
        new_node->left = ReadNodeFromFile(file, logfile, pos, new_node, buffer, error);
        if (*error) return NULL;

        SkipSpaces(buffer, pos);
        new_node->right = ReadNodeFromFile(file, logfile, pos, new_node, buffer, error);
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
        fprintf(stderr, "Syntax error in %zu %c", *pos, buffer[*pos]);
        *error = 1;
        return NULL;
    }
}