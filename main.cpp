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

    size_t pos = 0;
    CHECK_ERROR_RETURN(AskAndDoFileRead(&tree, &Info, &FileInfo, file_in));

    int cnt = 0;
    CHECK_ERROR_RETURN(TreeVerify(tree.root, (int)tree.size, &cnt));
    
    // DoTreeInGraphviz(tree.root, &Info, tree.root);
    CHECK_ERROR_RETURN(DoDifferentAkinatorModes(&tree, &Info, (size_t)pos));

    DO_OPEN_FILE_RETURN(file_out, "akinator_out.txt", "w");
    PrintAkinatorToFile(file_out, tree.root);
    fclose(file_out);

    CHECK_ERROR_RETURN(TreeDtor(&tree, FileInfo.buf_ptr, pos));
    free(FileInfo.buf_ptr);
    fclose(Info.file);
    return 0;
}