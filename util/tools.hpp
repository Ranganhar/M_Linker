#pragma once
#include <cassert>
#include <execinfo.h>
#include <iostream>
#include <set>
#include <string>
#include <vector>
class Context;
namespace fatal
{
void m_assert(bool cond, const char *errorMessage);
}; // namespace fatal

namespace parse
{
void parseArgs(int argc, char *argv[], Context *ctx);
}; // namespace parse