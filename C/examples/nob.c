/*
nob.c
This file is used to compile example.
Usage : `gcc nob.c -o nob.elf`
Then : ./nob.elf     # This will build the examples

TODO: Since nob is shell but in c, we should be able to walk this dir and compile every .c
See : https://github.com/tsoding/nob.h/blob/main/how_to/015_walk_dirs/nob.c
*/
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"

#define UNDEFINED_BEHAVIOR_SRC    "Undefined_Behavior.c"
#define UNDEFINED_BEHAVIOR_ELF    "Undefined_Behavior.elf"
#define XMACRO_SRC                "XMacro.c"
#define XMACRO_ELF                "XMacro_preprocessed.c"
#define PTR_FUNCTION_APP_SRC      "PTR_Function/app.c"
#define PTR_FUNCTION_APPFLASH_SRC "PTR_Function/appFlash.c"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    Cmd cmd = {0};

    cmd_append(&cmd, "cc", UNDEFINED_BEHAVIOR_SRC, "-o", UNDEFINED_BEHAVIOR_ELF);
    if (!cmd_run(&cmd))
        return 1;

    // cmd_append(&cmd, "cc", "-E", "-P" , XMACRO_SRC, "-o", XMACRO_ELF);
    // if (!cmd_run(&cmd))
    //     return 1;

    cmd_append(&cmd, "cc", PTR_FUNCTION_APP_SRC, PTR_FUNCTION_APPFLASH_SRC, "-o", "app.elf");
    if (!cmd_run(&cmd))
        return 1;

    return 0;
}