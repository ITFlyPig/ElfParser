//
// Created by yuelin on 2021/9/15.
//

#ifndef ELFPARSER_ELFHEADER_H
#define ELFPARSER_ELFHEADER_H

#include <string>
#include "json.h"
/**
 * elf文件的头部信息
 */
class ElfHeader {
public:
    std::string magic = "";
    std::string calss_str = "";
    std::string version = "";
    std::string data = "";
    std::string abi = "";
    std::string type = "";
    std::string machine = "";
    // elf文件头的大小
    uint32_t elf_header_size = 0;
    uint64_t program_header_table_off = 0;
    uint64_t section_header_table_off = 0;
    uint32_t program_header_entry_num = 0;
    uint32_t section_header_entry_num = 0;
    uint32_t program_header_entry_size = 0;
    uint32_t section_header_entry_size = 0;
    // 段表字符串表在段表中的下标
    uint16_t shstrndx = -1;

public:
    /**
     * 将对象转为json字符串
     * @return
     */
    std::string toJson();

    bool is_64bit; // 标志是否是64位的
};


#endif //ELFPARSER_ELFHEADER_H
