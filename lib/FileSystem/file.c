#include "../../include/FileSystem/file.h"
#include "../../include/utils/utils.h"

// 将文件的全部内容读入动态分配的缓冲区
char* ReadFile(const char* name, uint64_t* len)
{
    FILE* file = fopen(name, "rb");
    if(!file)
        return NULL;
    
    char* buffer;
    size_t size;

    // Find the size of the file
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    buffer = (char*)malloc(size + 1);
    if(!buffer)
    {
        perror("failed to allocate memory!");
        fclose(file);
        return NULL;
    }

    if(fread(buffer, size, 1, file) != 1)
    {
        perror("failed to read file!");
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[size] = '\0';
    *len = size;

    fclose(file);
    return buffer;
}

File* NewFile(const char* name)
{
    File* file = (File*)malloc(sizeof(File));
    char* contents = ReadFile(name, &file->contents_len);
    if(!contents)
    {
        free(file);
        return NULL;
    }

    file->name = strdup(name);
    file->contents = contents;

    return file;
}

