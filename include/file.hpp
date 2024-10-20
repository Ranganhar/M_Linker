#pragma once
#include "../util/tools.hpp"
#include "header.hpp"
#include <elf.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class File
{
  public:
    char *name;
    std::ifstream file;
    File(char *file_name) : name(file_name), file(file_name, std::ios::binary)
    {
        fatal::m_assert(file.is_open(), "Could not open this file when initializing class File!");
        check_ELF();
        check_size();
        read_elf_header();
        read_section_headers();
        read_Shstr_table();
    }

  public:
    Elf_Header *elf_header;
    std::vector<Shdr *> elf_sections;
    std::vector<uint8_t> Shstr_table;

  private:
    void check_ELF();
    void check_size();
    void read_elf_header();
    void read_section_headers();
    void read_Shstr_table();
    Shdr *read_section_header(size_t offset);

  public:
    std::vector<Shdr *> &get_elf_sections()
    {
        return elf_sections;
    }
    uint16_t get_ShstrIdx();
    std::vector<char> get_section_content(uint64_t index);
    std::vector<uint8_t> get_section_content_byte(uint64_t index);
    std::vector<uint8_t> get_bytes_from_Shdr(Shdr *shdr);
};

class ObjectFile : public File
{
  public:
    ObjectFile(char *file_name) : File(file_name)
    {
        symbol_table_section = get_section_from_type(this, (uint32_t)SHT_SYMTAB);
        get_first_global();
        read_elf_symbols();
        read_symbol_string_table();
    }
    uint64_t first_global_symbol;
    Shdr *symbol_table_section;
    std::vector<symbol *> elf_symbols;
    std::vector<uint8_t> symbol_string_table;

  private:
    void get_first_global();
    void read_elf_symbols();
    void read_symbol_string_table();
    symbol* read_single_symbol(std::vector<uint8_t> bytes, size_t offset);
};