#pragma once
#include <cassert>
#include <execinfo.h>
#include <iostream>
#include <set>
#include <string>
#include <vector>
class Context;
class ObjectFile;
class File;
namespace fatal
{
void m_assert(bool cond, const char *errorMessage);
}; // namespace fatal

namespace parse
{
void parseArgs(int argc, char *argv[], Context *ctx);
}; // namespace parse
namespace fileTool
{
bool check_elf(std::ifstream *file);
bool check_arch(std::ifstream *file);
int check_file_type(std::ifstream *file);
int get_machine_type(std::ifstream *file, std::vector<std::string> wait_handle);
ObjectFile *get_single_obj_from_file(std::ifstream *file);
std::vector<ObjectFile *> get_objs_from_arch(std::ifstream *file, File *parent);
}; // namespace fileTool