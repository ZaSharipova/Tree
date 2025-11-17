#include "ShowGraphics/ShowImageAndGif.h"
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: gif_viewer <gifpath>\n");
        return 1;
    }

    ShowImage(argv[1]);
    return 0;
}
