#include "../include/utils/utils.h"

void fatal(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    perror("fatal: ");
    vfprintf(stderr, format, args);
    perror("\n");
    
    va_end(args);
    exit(1);
}
