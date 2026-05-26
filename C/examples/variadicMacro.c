#include <stdio.h>

#define LOG(fmt, ...) printf("[LOG] " fmt "\n", __VA_ARGS__)

int main(void) {
    LOG("x = %d", 42);
    LOG("nom = %s age = %d", "Arthur", 25);

    return 0;
}