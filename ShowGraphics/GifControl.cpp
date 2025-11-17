#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "GifControl.h"

static pid_t gif_pid = 0;

void StopGif() {
    // printf("StopGif\n");
    // fflush(stdout);

    if (gif_pid != 0) {
        kill(gif_pid, SIGTERM);
        gif_pid = 0;
    }
}

void PlayGif(const char *filename) {
    // printf("PlayGif: %s\n", filename);
    // fflush(stdout);

    StopGif();

    if (!filename || filename[0] == '\0') {
        fprintf(stderr, "PlayGif ERROR: пустой путь GIF!\n");
        return;
    }
    pid_t pid = fork();

    if (pid == 0) {
        execl("./gif_viewer", "./gif_viewer", filename, NULL);

        perror("execl");
        //exit(1); //
    }

    gif_pid = pid;
}
