#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "TreeFunctions.h"
#include "Enums.h"
#include "Structs.h"
#include "TreeGraph.h"
#include "TreeDump.h"
#include "Akinator.h"
#include "ReadTree.h"

int main(void) {

    Tree_t tree = {};
    TreeErrors err = kSuccess;
    CHECK_ERROR_RETURN(TreeRootCtor(&tree));

    DumpInfo Info = {};
    Info.filename_to_write_dump = "alldump.html";
    Info.filename_to_write_graphviz = FILE_OUT;
    Info.file = fopen(Info.filename_to_write_dump, "w");
    if (Info.file == NULL) {
        perror("fopen() failed.");
        return kErrorOpeningFile;
    }
    Info.tree = &tree;

    DO_OPEN_FILE_RETURN(file_in, "akinator_out.txt", "r");
    
    FileInfo FileInfo = {};
    // DoBufRead(file_in, "akinator_in.txt", &FileInfo);

    size_t pos = 0;
    // DO_OPEN_FILE_RETURN(file_log, "logfile_for_read.txt", "w");
    // TreeNode_t *new_node = NULL;
    // CHECK_ERROR_RETURN(ReadNodeFromFile(&tree, file_in, file_log, &pos, tree.root, FileInfo.buf_ptr, &new_node));
    // tree.root = new_node;

    // fclose(file_in);
    // fclose(file_log);

    CHECK_ERROR_RETURN(AskAndDoFileRead(&tree, &Info, &FileInfo, file_in));
    // CHECK_ERROR_RETURN(Akinator(&tree, tree.root, &Info));

    int cnt = 0;
    //printf("%s", FileInfo.buf_ptr);
    CHECK_ERROR_RETURN(TreeVerify(tree.root, (int)tree.size, &cnt));
    
    // DoTreeInGraphviz(tree.root, &Info, tree.root);
    CHECK_ERROR_RETURN(DoDifferentAkinatorModes(&tree, &Info, (size_t)pos));
    // CHECK_ERROR_RETURN(DoPrintDefinition(tree.root, "Зарина", tree.size, pos));
    // CHECK_ERROR_RETURN(CompareNames(tree.root, "Зарина", "Исами"));

    DO_OPEN_FILE_RETURN(file_out, "akinator_out.txt", "w");
    PrintAkinatorToFile(file_out, tree.root);
    fclose(file_out);

    CHECK_ERROR_RETURN(TreeDtor(&tree, FileInfo.buf_ptr, pos));
    free(FileInfo.buf_ptr);
    fclose(Info.file);
    return 0;
}