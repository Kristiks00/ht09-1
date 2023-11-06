typedef struct RandomSourceOperations RandomSourceOperations;
typedef struct
{
    double value;
    int fd;
    RandomSourceOperations *op;
} RandomSource;
struct RandomSourceOperations
{
    RandomSource *(*destroy)(RandomSource *);
    double (*next)(RandomSource *);
};
