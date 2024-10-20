#include "tools.hpp"
#include "../include/context.hpp"

namespace fatal
{
void m_assert(bool cond, const char *errorMessage)
{
    if (!cond)
    {
        std::cerr << "linker: \033[0;1;31mfatal: \033[0m";
        std::cerr << errorMessage << std::endl;
        exit(1);
    }
}
}; // namespace fatal

namespace parse
{
void parseArgs(int argc, char *argv[], Context *ctx)
{
    fatal::m_assert(argc > 1, "wrong args!");
    std::vector<std::string> args(argv + 1, argv + argc);

    auto dashes = [&](std::string arg) -> std::set<std::string> {
        if (arg.size() == 1)
            return std::set<std::string>{"-" + arg};
        return std::set<std::string>{"-" + arg, "--" + arg};
    };

    auto read_flag = [&](std::string name) -> bool {
        if (dashes(name).count(args[0]))
        {
            return true;
        }
        return false;
    };

    std::string arg;
    auto read_arg = [&](std::string name) {
        std::set<std::string> name_dashes = dashes(name);
        if (name_dashes.count(args[0]))
        {
            fatal::m_assert(args[0].size() != 1, "option does not match any argument!");
            arg = args[1];
            return true;
        }
        for (auto dash : name_dashes)
        {
            if (!args[0].find(dash))
            {
                size_t pos = args[0].find(dash);
                if (dash.size() > 2)
                    dash += "=";
                arg = args[0].substr(pos + dash.size());
                return true;
            }
        }
        return false;
    };
    std::vector<std::string> wait_handle{};
    for (auto iter = args.begin(); iter != args.end();)
    {
        if (read_flag("help"))
        {
            std::cout << "usage: [options] file...\n";
            exit(0);
        }
        if (read_arg("o") || read_arg("output"))
        {
            ctx->args.output = arg;
            iter = args.erase(iter);
        }
        else if (read_flag("v") || read_flag("version"))
        {
            std::cout << "Version: 1.0.1\n";
            exit(0);
        }
        else if (read_arg("m"))
        {
            fatal::m_assert(arg == "elf64lriscv", "unknown -m attribute");
            ctx->args.emulation = Context::Args::MachineTypeRISCV64;
        }
        else if (read_arg("L"))
            ctx->args.lib_path.push_back(arg);
        else if (read_arg("l"))
            wait_handle.push_back("-l" + arg);
        else if (read_arg("plugin") || read_arg("plugin-opt") || read_arg("sysroot") ||read_flag("build-id") ||
        read_arg("hash-style") || read_flag("as-needed") || read_flag("static") || read_arg("z") || read_flag("start-group")
        || read_arg("end-group"))
        {

        }
        else
        {
            wait_handle.push_back(*iter);
        }
        iter = args.erase(iter);
    }

    auto parse_machineType_from_file = [&]()
    {
        for(size_t i = 0; i < wait_handle.size(); i++)
        {
            if(wait_handle[i][0] == '-')
                continue;
            {
                std::ifstream file(wait_handle[i]);
                fatal::m_assert(file.is_open(), "failed to open file!");
                file.seekg(16, std::ios::beg);
                fatal::m_assert(file.good(), "failed to seekg the file!");
                uint16_t type;
                file.read(reinterpret_cast<char*>(&type), sizeof(type));
                if(type != (uint16_t)ET_REL)
                    continue;
                file.seekg(18, std::ios::beg);
                uint16_t machine_type;
                file.read(reinterpret_cast<char*>(&machine_type), sizeof(machine_type));
                if(machine_type == (uint16_t)EM_RISCV)
                {
                    ctx->args.emulation = Context::Args::MachineTypeRISCV64;
                    return Context::Args::MachineTypeRISCV64;
                }
            }
        }
        return Context::Args::MachineTypeNone;
    };

    if(ctx->args.emulation != Context::Args::MachineTypeRISCV64)
    {
        if(parse_machineType_from_file() != Context::Args::MachineTypeRISCV64)
            fatal::m_assert(false, "no machine type specified!");
    }
};
} // namespace parse