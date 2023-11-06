#include <stdio.h>
#include <stdlib.h>
#include "random-source.h"

enum
{
    A = 1103515245,
    C = 12345,
    BASE = 10
};
const unsigned B = (unsigned) (1 << 31) - 1;

RandomSource *
destroy_linear(RandomSource *p)
{
    if (p == NULL) {
        return p;
    }
    free(p->op);
    free(p);
    return NULL;
}

double
next_linear(RandomSource *p)
{
    long long val = (long long) (p->value * B);
    val &= B;
    val = (A * val + C) & B; // new value in range[0, 2^31 - 1]
    double retval = (double) val;
    retval /= B; // new value in range[0, 1]
    p->value = retval;
    return retval;
}

RandomSource *
random_linear_factory(char *buf)
{
    char *ptr = NULL;
    long long val = strtoll(buf, &ptr, BASE);
    if (ptr == NULL || *ptr) {
        fprintf(stderr, "In function random_linear_factory: Cannot read seed\n");
        exit(1);
    }
    val &= B; // lower 31 bits
    double ans = (double) val;
    ans /= B; // value in range[0, 1]
    RandomSource *p = calloc(1, sizeof(*p));
    p->value = ans;
    p->op = calloc(1, sizeof(*(p->op)));
    p->op->destroy = destroy_linear;
    p->op->next = next_linear;
    return p;
}
