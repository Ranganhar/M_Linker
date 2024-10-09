#include "judge.hpp"

bool is_ELF_file(FILE *file)
{
    uint8_t magic[4];
    if (fread(magic, 1, 4, file) != 4)
        return false;
    return magic[0] == 0x7F && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F';
}

bool is_32_Bit_ELF(FILE* file)
{
    uint8_t classType;
    fseek(file, 4, SEEK_SET);
    fread(&classType, 1, 1, file);
    return classType == 1;       // 1 表示 32 位, 2 表示 64 位
}