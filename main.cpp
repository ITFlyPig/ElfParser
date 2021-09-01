#include <iostream>
#include <cstdlib>
#include <cmath>
#include "string"
#include "fstream"

using namespace std;

typedef uint32_t Elf32_Word; // 32位版本无符号整形
typedef uint32_t Elf32_Off;  // 32位的版本偏移地址
typedef uint32_t Elf32_Addr; // 32位版本程序地址
typedef uint16_t Elf32_Half;

// section 结构体
typedef struct elf32_shdr {
    Elf32_Word sh_name;
    Elf32_Word sh_type;
    Elf32_Word sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off sh_offset;
    Elf32_Word sh_size;
    Elf32_Word sh_link;
    Elf32_Word sh_info;
    Elf32_Word sh_addralign;
    Elf32_Word sh_entsize;
} Elf32_Shdr;

// elf文件头
#define EI_NIDENT 16
typedef struct elf32_hdr {
    unsigned char e_ident[EI_NIDENT];
    Elf32_Half e_type;
    Elf32_Half e_machine;
    Elf32_Word e_version;
    Elf32_Addr e_entry;
    Elf32_Off e_phoff;
    Elf32_Off e_shoff;  // 段表的偏移地址
    Elf32_Word e_flags;
    Elf32_Half e_ehsize;
    Elf32_Half e_phentsize;
    Elf32_Half e_phnum;
    Elf32_Half e_shentsize;// 段表描述符的大小
    Elf32_Half e_shnum;
    Elf32_Half e_shstrndx;
} Elf32_Ehdr;

/**
 * char数组转整数
 * @param arr
 * @param size
 * @return
 */
uint32_t char_arr_to(char *arr, int size) {
    uint32_t result = 0;
    for (int i = size - 1; i >= 0; ++i) {
        result += (arr[i] * pow(8, i));
    }
    return result;
}

/**
 * 打印char数组
 * @param arr
 * @param size
 */
void print_char_arr(char *arr, int size) {
    if (size <= 0) return;
    for (int i = 0; i < size; ++i) {
        unsigned char c = arr[i];
        printf("%02x ",c);
        if (i >= (size - 1)) {
            printf("\n");
        }
    }
}

void print_pos(ifstream &in) {
    streampos pos = in.tellg();
//    printf("pos=%d \n", pos);
    cout << "pos=" << pos << endl;

}

void read_and_print_by_bytes(ifstream &in, int total_bytes) {
    int count = 0;
    in.seekg(0);
    for (int i = 0; i < total_bytes; ++i) {
        char arr[1];
        in.read(arr, 1);
        printf("%02X ", arr[0]);
        count++;
        if (count >= 16) {
            count = 0;
            cout << endl;
        }
    }
}

void read_and_print_one_byte(ifstream &in) {
    char arr[1];
    in.read(arr, 1);
    printf("%02X ", arr[0]);

}


/**
 * 解析段表
 * @param in
 */
void parse_section_table(ifstream &in, Elf32_Off off, Elf32_Half size) {


}

/**
 * 将char数组转为有符号的整数
 * @param arr
 * @param len
 * @return
 */
Elf32_Word to_int(unsigned char *arr, int len) {
    Elf32_Word result = 0;
    for (int i = 0; i < len; ++i) {
        if (i == 0) {
            result = arr[i];
        } else {
            result |= (arr[i] << 8 * i);
        }
    }
    return result;
}

/**
 * 截取部分数组
 * @param origin
 * @param out
 * @param start
 * @param size
 */
void slice(unsigned char *origin,unsigned char *out, int start, int size) {
    int end = start + size;
    int index = 0;
    for (int i = start; i < end; ++i) {
        out[index] = origin[i];
        index++;
    }
}

Elf32_Word get_int(unsigned char *origin,int index, int size) {
    unsigned char arr[size];
    slice(origin, arr, index, size);
    return to_int(arr, size);
}

void fill(Elf32_Shdr *p_shdr, unsigned char *one_section_arr, int len) {
    if (len != sizeof(Elf32_Shdr)) {
        printf("fill 错误，无法将数组填充到Elf32_Shdr：数据长度不相等\n");
        return;
    }
    // 获取name
    int index = 0;

    p_shdr->sh_name = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取type
    p_shdr->sh_type = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取flags
    p_shdr->sh_flags = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取addr
    p_shdr->sh_addr = get_int(one_section_arr, index, sizeof(Elf32_Addr));
    index += sizeof(Elf32_Addr);
    // 获取offset
    p_shdr->sh_offset = get_int(one_section_arr, index, sizeof(Elf32_Off));
    index += sizeof(Elf32_Off);
    // 获取size
    p_shdr->sh_size = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取link
    p_shdr->sh_link = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取info
    p_shdr->sh_info = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取addralign
    p_shdr->sh_addralign = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取entsize
    p_shdr->sh_entsize = get_int(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);

}

void parse_one_section_header(unsigned char *one_section_arr, int len) {
    if (len != sizeof(Elf32_Shdr)) {
        printf("parse_one_section_header 传入的参数不合法，该数组的长度应该为：%d \n", sizeof(Elf32_Shdr));
        return;
    }
    // 按Elf32_Shdr的结构来解析该数组

    Elf32_Shdr shdr;
    // 将数据填充到结构体
    fill(&shdr, one_section_arr, len);

}

int main() {
//    unsigned char c = 152;
//    printf("%02x ", c);

    std::string elf_path = "/Users/yuelin/Documents/Android优化/so结构/libnative-lib.so";

    ifstream in;
    in.open(elf_path, ios::in);
    if (!in.is_open()) {
        cout << "文件打开失败" << endl;
        return -1;
    }
    printf("一个char占 %d 个字节 \n", sizeof(char));

    // 开始读取段表描述符
    in.seekg(203336);


    // 一个section 描述占用40个字节，总的有38个描述

    for (int i = 0; i < 2; ++i) {
        int shent_len = 40;
        char shent_arr[shent_len];
        in.read(shent_arr, shent_len);
//        print_char_arr(shent_arr, shent_len);
        // 解析每一个section 描述
        print_char_arr(shent_arr, shent_len);
        parse_one_section_header(reinterpret_cast<unsigned char *>(shent_arr), shent_len);

    }






    /*
    read_and_print_by_bytes(in, 52);
    print_pos(in);

    // 读取type
    in.seekg(0);
    print_pos(in);
    in.seekg(16);
    print_pos(in);
//    // 读取type
    char type[2];
    in.read(type, 2);
    print_char_arr(type, 2);
//    // 读取machine
//    read_and_print_one_byte(in);
//    print_pos(in);
//    read_and_print_one_byte(in);
//    print_pos(in);
//    read_and_print_one_byte(in);
//    print_pos(in);
//    read_and_print_one_byte(in);
//    read_and_print_one_byte(in);
//    read_and_print_one_byte(in);
//    read_and_print_one_byte(in);
//    read_and_print_one_byte(in);
//    read_and_print_one_byte(in);
//    read_and_print_one_byte(in);
    char machine[2];
    in.read(machine, 2);
    print_char_arr(machine, 2);



//    // Magic 16字节
//    char magic[16];
//    printf("Magic：");
//    in.get(magic, 16);
//    for (int i = 0; i < 16; ++i) {
//        printf("%02X ", magic[i]);
//        if (i == 15) {
//            printf("\n");
//        }
//    }
//
//    // 读取2个字节的type
//    cout << endl;
//    cout << "开始读取type" << endl;
//    print_pos(in);
//    char type_arr[2];
//    in.get(type_arr, 2);
//    print_char_arr(type_arr, 2);



//    // 读取头文件中的type
//    in.seekg(0);
//    Elf32_Off type_pos = EI_NIDENT;
//    in.seekg(type_pos);
//    int type_size = sizeof(Elf32_Half);
//    printf("sizeof(Elf32_Off): %d \n", type_size);
//    char type[type_size];
//    in.get(type, type_size);
//    printf("Elf 文件的type：%d \n", char_arr_to(type, type_size));
//    print_char_arr(type, type_size);
//
//    // 读取头文件中的machine
//    int machine_size = sizeof(Elf32_Half);
//    char machine[machine_size];
//    in.get(machine, machine_size);
//    printf("读取到的machine：%d  \n", char_arr_to(type, machine_size));
//    print_char_arr(machine, machine_size);
//
//    int version_size = sizeof(Elf32_Word);
//    char version[version_size];
//    in.get(version, version_size);
//
//    printf("读取到的version：%d  \n", char_arr_to(version, version_size));
//
//
//
//    // 计算段表偏移在elf中的位置
    in.seekg(0);
    Elf32_Off shoff_pos =
            EI_NIDENT + sizeof(Elf32_Half) + sizeof(Elf32_Half) + sizeof(Elf32_Word) + sizeof(Elf32_Addr) +
            sizeof(Elf32_Off);
    in.seekg(shoff_pos);
    printf("段表偏移在elf文件中的位置：%X \n", shoff_pos);
    // 读取段表的位置
    int shoff_size = sizeof(Elf32_Off);
    char shoff_arr[shoff_size];
//    streampos pos = in.tellg();
//    cout << "当前偏移位置=" << pos << endl;
    in.read(shoff_arr, shoff_size);

    // 将四字节转为int，因为是小端存储，所以，低位存的也就是数据的低位
    int a = int((unsigned char)(shoff_arr[3]) << 24 |
                (unsigned char)(shoff_arr[2]) << 16 |
                (unsigned char)(shoff_arr[1]) << 8 |
                (unsigned char)(shoff_arr[0]));

    printf("段表的偏移量为：%d \n", a);

    // 读取段表描述符的大小

    Elf32_Half shentsize_off = 0x2E;
    in.seekg(0);
    in.seekg(shentsize_off);
    char shentsize_arr[sizeof(Elf32_Half)];
    in.read(shentsize_arr, sizeof(Elf32_Half));
    print_char_arr(shentsize_arr, sizeof(Elf32_Half));
    int shentsize = int((unsigned char)(shentsize_arr[1]) << 8 |
                        (unsigned char)(shentsize_arr[0]));
    printf("段表描述符的大小：%d", shentsize);

    // 开始读取段表描述符
    in.seekg(0);
    in.seekg(a);
    char section_header_arr[shentsize];
    in.read(section_header_arr, shentsize);
    print_char_arr(section_header_arr, shentsize);
//
//    // 读取段表偏移量
//    in.seekg(shoff_pos);
//    char shoff[sizeof(Elf32_Off)];
//    in.get(shoff, sizeof(Elf32_Off));
//    printf("段表的偏移量：%d \n", char_arr_to(shoff, sizeof(Elf32_Off)));
     */


    return 0;
}
