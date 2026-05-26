#include <stdio.h>


/*
See the preprocessor compiled version of the c code : `gcc -E -P XMacro.c -o XMacro_preprocessed.c`
Compile : `gcc XMacro.c -o XMacro.elf`
*/

// Defining a macro ('X') within a macro
// with the values of colors.
#define COLORS     \
    X(RED, 0x30)   \
    X(BLACK, 0xFF) \
    X(WHITE, 0x01) \
    X(BLUE, (int)0x70)  // To see that preprocessor macro are just text replace, if we used the *toString function, it will return BLUE(int)0x70 but WHITE0x01

// Creating an enum of colors
// by macro expansion.
enum colors {
// Define the X macro used above
#define X(color, value) color, // We used 'color' here because we want to used the first argument of the X macro, Aka 'RED' etc..
    COLORS
#undef X
        COLORS_LIST
};

// A utility that takes the enum value
// and returns corresponding string value
char *toString(enum colors value) {
    switch (value) {
#define X(color, value) \
    case color:         \
        return #color#value;
        COLORS
#undef X
    }
}

int toValue(enum colors value) {
    switch (value) {
#define X(color, value) \
    case color:         \
        return value;
        COLORS
#undef X
    }
}

// driver program.
int main(void) {
    enum colors color = WHITE;
    int colorValue = toValue(color);
    printf("Color : %s, colorValue : %d\n", toString(color), colorValue);
    enum colors color2 = BLUE;
    int colorValue2 = toValue(color2);
    printf("Color2 : %s, colorValue2 : %d\n", toString(color2), colorValue2);
    return 0;
}