#include "../include/context.hpp"
#include <cstring>
void Context::check_ELF()
{
    char buffer[8] = {0};
    file.read(buffer, 8);
    file.seekg(0, std::ios::beg);
    fatal::m_assert(buffer[0] == 0x7f && buffer[1] == 'E' && buffer[2] == 'L' && buffer[3] == 'F',
                    "This is not an ELF file!");
}

bool Context::check_Arch()
{
    char buffer[8] = {0};
    file.read(buffer, 8);
    file.seekg(0, std::ios::beg);
    return strncmp(buffer, "<!arch>", 7) == 0;
}

void Context::check_file_type()
{
    fatal::m_assert(file.is_open(), "Could not open the file when check the file type!");
    if (file.seekg(0, std::ios::end).good())
    {
        fileType = Empty;
        return;
    }

    file.seekg(sizeof(uint8_t), std::ios::beg);
    fatal::m_assert(file.good(), "Failed to seek the specified offset when checking the file type!");

    uint16_t type;
    file.read(reinterpret_cast<char *>(&type), sizeof(uint16_t));
    if (type == ET_REL)
    {
        fileType = Object;
        get_machine_type();
    }
    else if (check_Arch())
    {
        fileType = Archive;
        return;
    }
    else
        fileType = Unknown;
}

void Context::get_machine_type()
{
    auto parse_machineType_from_file = [&]() {
        for (size_t i = 0; i < args.wait_handle.size(); i++)
        {
            if (args.wait_handle[i][0] == '-')
                continue;
            {
                std::ifstream file(args.wait_handle[i]);
                fatal::m_assert(file.is_open(), "failed to open file!");
                file.seekg(16, std::ios::beg);
                fatal::m_assert(file.good(), "failed to seekg the file!");
                uint16_t type;
                file.read(reinterpret_cast<char *>(&type), sizeof(type));
                if (type != (uint16_t)ET_REL)
                    continue;
                file.seekg(18, std::ios::beg);
                uint16_t machine_type;
                file.read(reinterpret_cast<char *>(&machine_type), sizeof(machine_type));
                if (machine_type == (uint16_t)EM_RISCV)
                {
                    args.emulation = Context::Args::MachineTypeRISCV64;
                    return Context::Args::MachineTypeRISCV64;
                }
            }
        }
        return Context::Args::MachineTypeNone;
    };

    if (args.emulation != Context::Args::MachineTypeRISCV64)
    {
        if (parse_machineType_from_file() != Context::Args::MachineTypeRISCV64)
            fatal::m_assert(false, "no machine type specified!");
    }
}

void Context::read_file()
{
    if (fileType == Context::Object)        
        object_files.push_back(new ObjectFile(std::move(file)));
    else if(fileType == Context::Archive)
    {
        auto objs = get_objs_from_arch();
        for(auto obj : objs)
            object_files.push_back(obj);
    }
    else
        fatal::m_assert(false, "Unknown file type!");
}

std::vector<ObjectFile*> Context::get_objs_from_arch()
{
    fatal::m_assert(fileType == Context::Archive, "This is not an archive file!");
    std::vector<ObjectFile *> objs;
    size_t pos = 8;
    file.seekg(pos, std::ios::beg);
    std::string string_table;
    while (!file.eof())
    {
        if (pos % 2 == 1)
            file.seekg(1, std::ios::cur);
        auto arhdr = new Arch_Header();
        file.read(reinterpret_cast<char *>(arhdr), Arch_Header_size);
        if (is_arch_symbol_table(arhdr))
            continue;
        else if (is_arch_string_table(arhdr))
        {
            string_table.resize(std::stoi(arhdr->size));
            file.read(string_table.data(), std::stoi(arhdr->size));
        }
        else
        {
            std::ifstream obj_stream;
            file.read(reinterpret_cast<char *>(&obj_stream), std::stoi(arhdr->size));
            auto obj = new ObjectFile(std::move(obj_stream), get_arch_name(arhdr, string_table), file_imp);
            objs.push_back(obj);
        }
    }
    return objs;
}