#include "./util/tools.hpp"
#include "./include/file.hpp"
#include "./include/context.hpp"
#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char *argv[])
{

    Context* ctx = new Context(argv[1]);
    parse::parseArgs(argc, argv, ctx);
    ctx->init();
    // if(!file)
    // {
    //     std::cerr << "Failed to create file object!" << std::endl;
    //     exit(1);
    // }

    // auto index_content = file->get_section_content_byte(file->get_ShstrIdx());

    // auto name = get_section_name(index_content, 1);
    // std::cout << name <<std::endl;

    // for(auto test : file->get_elf_sections())
    // {
    //     std::cout << get_section_name(index_content, test->name) << std::endl;
    // }

    // std::cout << file->first_global_symbol <<std::endl;
    return 0;
}