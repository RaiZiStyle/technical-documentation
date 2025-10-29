/* segFault.c — wild pointer non initialisé */
#include <stdio.h>

int main(void) {
    int* p; /* p n'est pas initialisé -> valeur indéterminée */
    printf("p vaut %p\n", (void*)p);
    *p = 12; /* Déréférencement d'un wild pointer -> comportement indéfini */
    printf("écrit %d à l'adresse %p\n", 12, (void*)p);
    return 0;
}
