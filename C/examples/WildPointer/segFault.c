/* segFault.c — wild pointer non initialise */
#include <stdio.h>

int main(void) {
    int *p; /* p is not initialise, Undefined behavior */
    printf("p vaut %p\n", (void *)p);
    *p = 12; /* Dereferencing a wild pointer -> Undefined behavior */
    printf("écrit %d à l'adresse %p\n", 12, (void *)p);
    return 0;
}
