#include "../include/context.hpp"
#include <cstring>

void Context::init()
{
    fileTool::check_elf(&file);
    switch (fileTool::check_file_type(&file))
    {
    case Context::Object:
        fileType = Object;
        file_imp = new ObjectFile(std::move(file));
        object_files.push_back(fileTool::get_single_obj_from_file(&file));
        if (fileTool::get_machine_type(&file, args.wait_handle))
            args.emulation = Context::Args::MachineTypeRISCV64;
        else
            args.emulation = Context::Args::MachineTypeNone;
        break;
    case Context::Archive:
        fileType = Archive;
        file_imp = new File(std::move(file));
        for (auto obj : fileTool::get_objs_from_arch(&file, file_imp))
            object_files.push_back(obj);
        break;
    case Context::Empty:
        fileType = Empty;
        break;
    default:
        fileType = Unknown;
        fatal::m_assert(false, "Unknown file type!");
    }
}