#ifndef CONTEXT_H
#define CONTEXT_H

#include "../utils/utils.h"
#include <map>
struct ObjectFile;
struct MergedSection;
struct Output_ElfHeader;
struct Output_SectionHeader;
struct Output_Section;
struct Output_ProgramHeader;
struct Got_Section;
struct chunk;

typedef struct
{
    char* output;
    u_int8_t emulation;
    char** lib_path;
    int lib_path_count;
} ContextArgs;

typedef struct
{
    ContextArgs Args;

    struct ObjectFile** Objs;
    int ObjCount;

    struct MergedSection** MergedSections;
    int MergedSectionCount;

    struct chunk** chunk;
    int chunkCount;

    char* buffer;

    struct Output_ElfHeader* output_elf_header;
    struct Output_SectionHeader* output_section_header;
    struct Output_ProgramHeader* output_program_header;
    struct Got_Section* got_section;
    struct OutPut_Section** output_section;
    int output_section_count;

    uint64_t addr;    
} Context;

#endif
