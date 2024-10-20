#pragma once
#include "../util/tools.hpp"
#include "file.hpp"
#include <stdint.h>
#include <string>

class Context
{
  public:
    Context() : args({Args::MachineTypeNone, "a.out",{}})
    {
    }
    struct Args
    {
        enum
        {
            MachineTypeNone,
            MachineTypeRISCV64
        } emulation;
        std::string output;
        std::vector<std::string> lib_path;
    } args;
};