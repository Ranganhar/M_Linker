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
    std::ifstream file;
    std::string name;
    File *parent;
    File(std::ifstream &&f) : file(std::move(f)), name(""), parent(nullptr)
    {
        init();
    }
    File(std::ifstream &&f, std::string n) : file(std::move(f)), name(n), parent(nullptr)
    {
        init();
    }
    File(std::ifstream &&f, std::string n, File *parent) : file(std::move(f)), name(n), parent(parent)
    {
        init();
    }

  public:
    Elf_Header *elf_header;
    std::vector<Shdr *> elf_sections;
    std::vector<uint8_t> Shstr_table;

  private:
    void init();
    void check_size();
    void read_elf_header();
    void read_section_headers();
    void read_Shstr_table();
    void check_file_type();
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
    ObjectFile(std::ifstream &&f) : File(std::move(f))
    {
        Obj_init();
    }
    ObjectFile(std::ifstream &&f, std::string n) : File(std::move(f), n)
    {
        Obj_init();
    }
    ObjectFile(std::ifstream &&f, std::string n, File *parent) : File(std::move(f), n, parent)
    {
        Obj_init();
    }
    uint64_t first_global_symbol;
    Shdr *symbol_table_section;
    std::vector<symbol *> elf_symbols;
    std::vector<uint8_t> symbol_string_table;

  private:
    void Obj_init();
    void get_first_global();
    void read_elf_symbols();
    void read_symbol_string_table();
    symbol *read_single_symbol(std::vector<uint8_t> bytes, size_t offset);
};
