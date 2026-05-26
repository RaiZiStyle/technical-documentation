#include <stdarg.h>
#include <stdio.h>

int somme(int nb_args, ...) {
    va_list args;
    va_start(args, nb_args);

    int total = 0;

    // Loop thought the argument. We assume the argument are int, we don't really check
    for (int i = 0; i < nb_args; i++) {
        total += va_arg(args, int);
    }

    va_end(args);

    return total;
}

int main(void) {
    int r = somme(4, 10, 20, 30, 40);

    printf("Resultat = %d\n", r);

    return 0;
}