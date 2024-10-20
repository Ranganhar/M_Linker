#pragma once
#include "../util/tools.hpp"
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

class File;

typedef struct Elf_Header
{
    uint8_t ident[16];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint64_t entry;
    uint64_t Phoff;
    uint64_t Shoff;
    uint32_t flags;
    uint16_t Ehsize;
    uint16_t Phentsize;
    uint16_t Phnum;
    uint16_t Shentsize;
    uint16_t Shnum;
    uint16_t ShstrIdx;
} Ehdr;

constexpr size_t Elf_Header_size = sizeof(Ehdr);

typedef struct Section_Header_Table
{
    uint32_t name;
    uint32_t type;
    uint64_t flags;
    uint64_t addr;
    uint64_t offset;
    uint64_t size;
    uint32_t link;
    uint32_t info;
    uint64_t addr_align;
    uint64_t entsize;
} Shdr;

constexpr size_t Section_Header_size = sizeof(Shdr);

std::string get_section_name(std::vector<uint8_t> content, uint32_t offset);

Shdr* get_section_from_type(File* file, uint32_t type);

/* Symbol */

typedef struct Symbol
{
    uint32_t name;
    uint8_t info;
    uint8_t other;
    uint16_t Shidx;
    uint64_t val;
    uint64_t size;
} symbol;

constexpr size_t symbol_size = sizeof(symbol);