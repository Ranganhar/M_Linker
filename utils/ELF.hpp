#pragma once
#include <cstdint>
#include <cstring>
#include <vector>

/* ELF Header */
template <typename T>
struct ELFHeader
{
    struct ELF_Ident
    {
        uint32_t magic_num;
        uint8_t  class_type;
        uint8_t  data_encoding;
        uint8_t  elf_version;
    }ident;
	uint16_t	type;
	uint16_t	machine;
	uint32_t	version;
    T        entry;         /*32位4字节, 64位8字节*/
    T        phoff;         /*32位4字节, 64位8字节*/
    T        shoff;         /*32位4字节, 64位8字节*/

	uint16_t	ehsize;		/* ELF Header size */
	uint16_t	phentsize;	/* Section Header size */
	uint16_t	phnum;
	uint16_t	shentsize;	/* Program Header size */
	uint16_t	shnum;
	uint16_t	shstrndx;
};

using ELF32Header = ELFHeader<uint32_t>;
using ELF64Header = ELFHeader<uint64_t>;

constexpr std::size_t ELF32HeaderSize = sizeof(ELFHeader<uint32_t>);
constexpr std::size_t ELF64HeaderSize = sizeof(ELFHeader<uint64_t>);



/* Section Header */
template <typename T>
struct SectionHeader
{
    T name;
    T type;
    T flags;
    T addr;
    T offset;
    T size;
    T link;
    T info;
    T addralign;
    T entsize;
};

using Section32Header = SectionHeader<uint32_t>;
using Section64Header = SectionHeader<uint64_t>;

constexpr std::size_t Section32HeaderSize = sizeof(SectionHeader<uint32_t>);
constexpr std::size_t Section64HeaderSize = sizeof(SectionHeader<uint64_t>);



/* Function Method */
template <typename T>
void ReadELFHeader(FILE* file, ELFHeader<T>* header)
{
    uint8_t buffer[sizeof(ELFHeader<T>)];
    uint8_t* sp;
    
    sp = buffer;

    if(fseek(file, 0, SEEK_SET) < 0)
    {
        std::cerr << "\033[0;1;31mfatal:\033[0m fail to seek file!" << std::endl;
        exit(1);
    }

    // if(fread(buffer, header->ehsize, 1, file) != 1)
    // {
    //     std::cerr << "\033[0;1;31mfatal:\033[0m fail to read file!" << std::endl;
    //     exit(1);
    // }
    memmove(&header->ident, sp, sizeof(header->ident));
    sp += sizeof(header->ident);
    memmove(&header->type, sp, sizeof(header->type));
    sp += sizeof(header->type);
    memmove(&header->machine, sp, sizeof(header->machine));
    sp += sizeof(header->machine);
    memmove(&header->version, sp, sizeof(header->version));
    sp += sizeof(header->version);
    memmove(&header->entry, sp, sizeof(header->entry));
    sp += sizeof(header->entry);
    memmove(&header->phoff, sp, sizeof(header->phoff));
    sp += sizeof(header->phoff);
    memmove(&header->shoff, sp, sizeof(header->shoff));
    sp += sizeof(header->shoff);
    memmove(&header->ehsize, sp, sizeof(header->ehsize));
    sp += sizeof(header->ehsize);
    memmove(&header->phentsize, sp, sizeof(header->phentsize));
    sp += sizeof(header->phentsize);
    memmove(&header->phnum, sp, sizeof(header->phnum));
    sp += sizeof(header->phnum);
    memmove(&header->shentsize, sp, sizeof(header->shentsize));
    sp += sizeof(header->shentsize);
    memmove(&header->shnum, sp, sizeof(header->shnum));
    sp += sizeof(header->shnum);
    memmove(&header->shstrndx, sp, sizeof(header->shstrndx));
}

template <typename T>
void ReadSectionsHeader(FILE* file, ELFHeader<T>* header)
{
    std::vector<SectionHeader<typename T::AddrType>> sections_Headers(header.shnum);
    
}