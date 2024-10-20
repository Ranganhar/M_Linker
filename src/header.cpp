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