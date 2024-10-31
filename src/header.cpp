#include "../include/header.hpp"
#include "../include/file.hpp"

std::string get_section_name(std::vector<uint8_t> content, uint32_t offset)
{
    std::string name;
    for (size_t i = offset; i < content.size(); i++)
    {
        if (content[i] == '\0')
            break;
        name.push_back(content[i]);
    }
    return name;
}

Shdr *get_section_from_type(File *file, uint32_t type)
{
    std::vector<Shdr *> sections = file->get_elf_sections();
    for (size_t i = 0; i < sections.size(); i++)
    {
        if (sections[i]->type == type)
            return sections[i];
    }
    return nullptr;
}

bool is_arch_string_table(Arch_Header *header)
{
    std::string name(header->name, 10);
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    return name.substr(0, 3) == "// ";
}

bool is_arch_symbol_table(Arch_Header *header)
{
    std::string name(header->name, 10);
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    return (name.substr(0, 2) == "/ ") || (name.substr(0, 6) == "/SYM/ ");
}

std::string get_arch_name(Arch_Header *header, std::string string_table)
{
    std::string name(header->name, 10);
    name.erase(std::remove(name.begin(), name.end(), ' '), name.end());
    if (name.substr(0, 1) == "/")
    {
        name.erase(name.begin());
        auto start = string_table.find(name);
        auto end = string_table.find("/\n", start);
        return string_table.substr(start, end - start);
    }

    size_t pos = name.find('/');
    if (pos != std::string::npos)
        return name.substr(0, pos);
    return name;
}