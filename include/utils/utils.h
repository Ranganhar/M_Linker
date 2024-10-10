#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>

void fatal(const char* format, ...);

#endif