#pragma once
#include "../util/tools.hpp"
#include "file.hpp"
#include <stdint.h>
#include <string>

class Context
{
  public:
    char *name;
    std::ifstream file;
    Context(char *file_name)
        : name(file_name), file(file_name, std::ios::binary), args({Args::MachineTypeNone, "a.out", {}, {}})
    {
        check_ELF();
        check_file_type();
    }

    enum
    {
        Unknown,
        Empty,
        Object,
        Archive
    } fileType;
    struct Args
    {
        enum
        {
            MachineTypeNone,
            MachineTypeRISCV64
        } emulation;
        std::string output;
        std::vector<std::string> lib_path;
        std::vector<std::string> wait_handle;
    } args;
    File *file_imp;
    std::vector<ObjectFile*> object_files;
  private:
    void check_ELF();
    bool check_Arch();
    void read_file();
    void check_file_type();
    void get_machine_type();
    std::vector<ObjectFile*> get_objs_from_arch();
};