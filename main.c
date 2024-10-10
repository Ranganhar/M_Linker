#include "./include/utils/utils.h"
#include <stdlib.h>

/* 默认为 true 代表 32 位, false 代表 64 位*/
static bool bit_flag = true;

// int main(int argc, char* argv[])
// {
//     FILE* file;
//     ELF32Header* elf32_header = new ELF32Header();
//     ELF64Header* elf64_header = new ELF64Header();
//     file = fopen(argv[1], "rb");
//     if(file == nullptr)
//     {
//         std::cerr << "\033[0;1;31mfatal:\033[0m fail to open file!" << std::endl;
//         exit(1);
//     }

//     if(!is_ELF_file(file))
//     {
//         std::cerr << "\033[0;1;31mfatal:\033[0m not a ELF file!" << std::endl;
//         exit(1);
//     }

//     if(!is_32_Bit_ELF(file))
//         bit_flag = false;
    
//     if(bit_flag)
//     {
        
//         // static Section32Header* section_header;
//         ReadELFHeader(file, elf32_header);
//     }
//     else 
//     {
//         // static Section64Header* section_header;
//         ReadELFHeader(file, elf64_header);
//     }
//     std::cout << "Hello World!" << std::endl;
//     fclose(file);

//     return 0;   
// }

int main(int argc, char* argv[])
{
    if(argc < 2)
        fatal("usage: %s <file>", argv[0]); 

          
}