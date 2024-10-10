#ifndef FILE_H
#define FILE_H

#include "../utils/utils.h"

typedef struct File
{
    char *name;
    char *contents;
    uint64_t contents_len;
    struct File *parent;
} File;

char* ReadFile(const char* name, uint64_t* len);
File* NewFile(const char* name);

#endif