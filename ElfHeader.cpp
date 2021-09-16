//
// Created by yuelin on 2021/9/15.
//

#include "ElfHeader.h"

std::string ElfHeader::toJson() {
    Json::Value jsonObj;
    jsonObj["magic"] = magic;
    jsonObj["calss_str"] = calss_str;
    jsonObj["data"] = data;
    jsonObj["version"] = version;
    jsonObj["abi"] = abi;
    jsonObj["type"] = type;
    jsonObj["machine"] = machine;
    jsonObj["elf_header_size"] = elf_header_size;
    jsonObj["program_header_table_off"] = program_header_table_off;
    jsonObj["section_header_table_off"] = section_header_table_off;
    jsonObj["program_header_entry_num"] = program_header_entry_num;
    jsonObj["section_header_entry_num"] = section_header_entry_num;
    jsonObj["program_header_entry_size"] = program_header_entry_size;
    jsonObj["section_header_entry_size"] = section_header_entry_size;
    return jsonObj.toStyledString();
}
