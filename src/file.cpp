#include "../include/file.hpp"
#include "../include/context.hpp"
#include "../util/tools.hpp"
#include <cstring>
#include <elf.h>

void File::init()
{
    fatal::m_assert(file.is_open(), "Could not open this file when initializing class File!");
    check_size();
    read_elf_header();
    read_section_headers();
    read_Shstr_table();
}

void File::check_size()
{
    fatal::m_assert(file.is_open(), "File is not open!");

    file.seekg(0, std::ios::end);
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    fatal::m_assert((static_cast<size_t>(file_size) > Elf_Header_size), "File too small!");
}

template <typename T> T read_little_endian(std::ifstream &file)
{
    T value;
    file.read(reinterpret_cast<char *>(&value), sizeof(T));
    return value;
}

void File::read_elf_header()
{
    fatal::m_assert(file.is_open(), "Could not open the file when reading elf header!");
    elf_header = new Elf_Header();
    file.read(reinterpret_cast<char *>(elf_header->ident), sizeof(elf_header->ident));
    elf_header->type = read_little_endian<uint16_t>(file);
    elf_header->machine = read_little_endian<uint16_t>(file);
    elf_header->version = read_little_endian<uint32_t>(file);
    elf_header->entry = read_little_endian<uint64_t>(file);
    elf_header->Phoff = read_little_endian<uint64_t>(file);
    elf_header->Shoff = read_little_endian<uint64_t>(file);
    elf_header->flags = read_little_endian<uint32_t>(file);
    elf_header->Ehsize = read_little_endian<uint16_t>(file);
    elf_header->Phentsize = read_little_endian<uint16_t>(file);
    elf_header->Phnum = read_little_endian<uint16_t>(file);
    elf_header->Shentsize = read_little_endian<uint16_t>(file);
    elf_header->Shnum = read_little_endian<uint16_t>(file);
    elf_header->ShstrIdx = read_little_endian<uint16_t>(file);
}

void File::read_section_headers()
{
    fatal::m_assert(file.is_open(), "Could not open the file when reading section hearders!");
    elf_sections.push_back(read_section_header(elf_header->Shoff));

    uint64_t shdr_num = elf_header->Shnum ? elf_header->Shnum : elf_sections.front()->size;
    for (size_t i = 1; i < shdr_num; i++)
    {
        size_t offset = elf_header->Shoff + i * Section_Header_size;
        elf_sections.push_back(read_section_header(offset));
    }
}

Shdr *File::read_section_header(size_t offset)
{
    fatal::m_assert(file.is_open(), "Could not open the file when reading section header!");
    file.seekg(offset, std::ios::beg);
    fatal::m_assert(file.good(), "Failed to seek to the specified offset when reading section header!");
    Shdr *shdr = new Shdr();
    shdr->name = read_little_endian<uint32_t>(file);
    shdr->type = read_little_endian<uint32_t>(file);
    shdr->flags = read_little_endian<uint64_t>(file);
    shdr->addr = read_little_endian<uint64_t>(file);
    shdr->offset = read_little_endian<uint64_t>(file);
    shdr->size = read_little_endian<uint64_t>(file);
    shdr->link = read_little_endian<uint32_t>(file);
    shdr->info = read_little_endian<uint32_t>(file);
    shdr->addr_align = read_little_endian<uint64_t>(file);
    shdr->entsize = read_little_endian<uint64_t>(file);
    return shdr;
}

uint16_t File::get_ShstrIdx()
{
    return (uint16_t)((elf_header->ShstrIdx == SHN_XINDEX) ? elf_sections.front()->link : elf_header->ShstrIdx);
}

std::vector<char> File::get_section_content(uint64_t index)
{
    fatal::m_assert(file.is_open(), "Could not open the file when getting section content!");
    file.seekg(elf_sections[index]->offset, std::ios::beg);
    fatal::m_assert(file.good(), "Failed to seek to the specified offset when getting section content!");

    std::vector<char> res(elf_sections[index]->size);
    file.read(res.data(), elf_sections[index]->size);
    fatal::m_assert(file.good(), "Could not read the file when getting section content!");

    return res;
}

std::vector<uint8_t> File::get_section_content_byte(uint64_t index)
{
    fatal::m_assert(file.is_open(), "Could not open the file when getting section content byte!");
    file.seekg(elf_sections[index]->offset, std::ios::beg);
    fatal::m_assert(file.good(), "Failed to seek to the specified offset when getting section content byte!");

    std::vector<uint8_t> res(elf_sections[index]->size);
    file.read(reinterpret_cast<char *>(res.data()), elf_sections[index]->size);
    fatal::m_assert(file.good(), "Could not read the file when getting section content byte!");

    return res;
}

std::vector<uint8_t> File::get_bytes_from_Shdr(Shdr *shdr)
{
    fatal::m_assert(file.is_open(), "Could not open the file when getting bytes from shdr!");
    file.seekg(shdr->offset, std::ios::beg);
    fatal::m_assert(file.good(), "Failed to seek to the specified offset when getting bytes from shdr!");

    std::vector<uint8_t> res(shdr->size);
    file.read(reinterpret_cast<char *>(res.data()), shdr->size);
    fatal::m_assert(file.good(), "Could not read the file when getting bytes from shdr!");

    return res;
}

void File::read_Shstr_table()
{
    Shstr_table = get_section_content_byte((uint64_t)get_ShstrIdx());
}

void ObjectFile::read_elf_symbols()
{
    std::vector<uint8_t> bytes = get_bytes_from_Shdr(symbol_table_section);
    size_t nums = bytes.size() / symbol_size;

    for (size_t i = 0; i < nums; i++)
        elf_symbols.push_back(read_single_symbol(bytes, i * symbol_size));
}

void ObjectFile::get_first_global()
{
    if (symbol_table_section)
        first_global_symbol = (uint64_t)symbol_table_section->info;
}

symbol *ObjectFile::read_single_symbol(std::vector<uint8_t> bytes, size_t offset)
{
    fatal::m_assert(bytes.size() >= symbol_size + offset, "No bytes input!");
    return reinterpret_cast<symbol *>(const_cast<uint8_t *>(bytes.data() + offset));
}

void ObjectFile::read_symbol_string_table()
{
    symbol_string_table = get_section_content_byte((uint64_t)symbol_table_section->link);
    for (size_t i = 0; i < elf_symbols.size(); i++)
        std::cout << get_section_name(symbol_string_table, elf_symbols[i]->name) << std::endl;
}

void ObjectFile::Obj_init()
{
    symbol_table_section = get_section_from_type(this, (uint32_t)SHT_SYMTAB);
    get_first_global();
    read_elf_symbols();
    read_symbol_string_table();
}