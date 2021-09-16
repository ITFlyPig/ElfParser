//
// Created by yuelin on 2021/9/15.
//

#ifndef ELFPARSER_SECTIONHEADER_H
#define ELFPARSER_SECTIONHEADER_H

#include <stdlib.h>
#include <string>
#include "json.h"

/**
 * 节section的头部信息
 */
class SectionHeader {
public:
    // 段在段表中的索引
    uint16_t index;
    // section的名字
    uint32_t sh_name;
    std::string name = "";
    // section 的type
    uint32_t sh_type ;
    std::string type = "";
    // 段的标志
    uint32_t sh_flags;
    std::string flags = "";
    // 段的虚拟地址
    uint64_t sh_addr;
    std::string addr = "";
    // 段在文件中的偏移地址
    uint64_t sh_offset;
    std::string offset = "";
    // 段的长度
    uint64_t sh_size;
    // 段的链接信息
    uint32_t sh_link;
    uint32_t sh_info;
    // 段地址对齐
    uint64_t sh_addralign;
    // 项的长度
    uint64_t sh_entsize;

public:
    Json::Value to_json();

};


#endif //ELFPARSER_SECTIONHEADER_H
