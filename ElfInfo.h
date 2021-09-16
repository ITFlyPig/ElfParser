//
// Created by yuelin on 2021/9/15.
//

#ifndef ELFPARSER_ELFINFO_H
#define ELFPARSER_ELFINFO_H
#include <string>

/**
 * 存储Elf文件的相关信息
 */
class ElfInfo {
public:
    std::string path; // so文件的路径
    unsigned long size; // so文件的大小，字节为单位


};


#endif //ELFPARSER_ELFINFO_H
