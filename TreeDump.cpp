#include "TreeDump.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "Enums.h"
#include "TreeGraph.h"

#define MAX_STRING_SIZE 60

static void PrintCurrentTime(FILE *file, const char *label);

void DoDump(DumpInfo *Info) {
    assert(Info);

    if (!Info->file) {
        fprintf(stderr, "Failed to open file %s.\n", Info->filename_to_write_dump);
    }
    FILE *file = Info->file;

    fprintf(file, "<pre> <h2>DUMP <font color = orange> %s </font> </h2>\n", Info->message);
    PrintCurrentTime(file, "made");
    fprintf(file, "<img src = %s>", Info->image_file);
}

static void PrintCurrentTime(FILE *file, const char *label) {
    assert(file);
    assert(label);

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);

    char buf_time[MAX_STRING_SIZE] = {};
    strftime(buf_time, sizeof(buf_time), "%Y-%m-%d %H:%M:%S\n", tm_now);

    fprintf(file, "<h4 style=\"margin: 3px 0;\">%s: %s</h4>\n", label, buf_time);
}