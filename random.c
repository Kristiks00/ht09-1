#include "random-source.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

RandomSource *
destroy_random(RandomSource *p)
{
    if (p == NULL) {
        return NULL;
    }
    free(p->op);
    if (close(p->fd) < 0) {
        fprintf(stderr, "In function destroy_random: Cannot close file\n");
        exit(1);
    };
    free(p);
    return NULL;
}

double
next_random(RandomSource *p)
{
    unsigned long long val = 0;
    if (read(p->fd, &val, sizeof(val)) < 0) {
        fprintf(stderr, "In function next_random: Cannot read from file\n");
        exit(1);
    }
    p->value = (double) val;
    p->value = (p->value < 0) ? -(p->value) : p->value;
    p->value /= ULLONG_MAX;
    return p->value;
}

RandomSource *
random_random_factory(char *buf)
{
    RandomSource *p = calloc(1, sizeof(*p));
    p->fd = open("/dev/urandom", O_RDONLY);
    if (p->fd < 0) {
        fprintf(stderr, "In function random_random_factory: Cannot open '/dev/urandom'\n");
        exit(1);
    }
    p->op = calloc(1, sizeof(*(p->op)));
    p->op->destroy = destroy_random;
    p->op->next = next_random;
    return p;
}
