// nob.c
// This file is used to compile example. 
// Usage : `gcc nob.c -o nob.elf`
// Then : ./nob.elf     # This will build the examples
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#define SRC_FOLDER   "WildPointer/"

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = {0};
    cmd_append(&cmd, "cc", "-Wall", "-Wextra", "-o", SRC_FOLDER"segFault.elf", SRC_FOLDER"segFault.c");
    if (!cmd_run(&cmd)) return 1;
    return 0;
}