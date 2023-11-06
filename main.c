#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "random-source.h"

enum
{
    ARGC = 3,
    BASE = 10,
    NUMB = 2,
    BUF = 3,
    LIBSIZE = 15,
    FUNCSIZE = 22
};

int
main(int argc, char *argv[])
{
    if (argc <= ARGC) {
        fprintf(stderr, "In function main: Not enought arguments\n");
        exit(1);
    }
    char *ptr = NULL;
    long count = strtol(argv[NUMB], &ptr, BASE);
    if (ptr == NULL || *ptr) {
        fprintf(stderr, "In function main: Cannot read count\n");
        exit(1);
    }
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "In function main: Cannot make handle\n");
        exit(1);
    }
    char funcname[FUNCSIZE];
    snprintf(funcname, sizeof(funcname), "random_%s_factory", argv[1]);
    void *fabric = dlsym(handle, funcname);
    if (fabric == NULL) {
        fprintf(stderr, "In function main: Cannot find function %s\n", funcname);
        exit(1);
    }
    RandomSource *p = ((RandomSource * (*) (char *) ) fabric)(argv[BUF]);
    for (int i = 0; i < count; ++i) {
        printf("%.10g\n", p->op->next(p));
    }
    p->op->destroy(p);
    dlclose(handle);
    return 0;
}