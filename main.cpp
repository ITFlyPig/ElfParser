#include <iostream>
#include <cstdlib>
#include <cmath>
#include <map>
#include "string"
#include "fstream"
#include "json.h"
#include "ElfHeader.h"
#include "SectionHeader.h"

using namespace std;

static const int ERROR = -1;
static const int SUCCESS = 0;

#define EI_OSABI 7
#define UNKNOW "unknow";

map<int, string> abi_map = {
        {0x00, "System V"},
        {0x01, "HP-UX"},
        {0x02, "NetBSD"},
        {0x03, "Linux"},
        {0x04, "GNU Hurd"},
        {0x06, "Solaris"},
        {0x07, "AIX"},
        {0x09, "FreeBSD"},
        {0x0A, "Tru64"},
        {0x0B, "Novell Modesto"},
        {0x0C, "OpenBSD"},
        {0x0D, "OpenVMS"},
        {0x0E, "NonStop Kernel"},
        {0x0F, "AROS"},
        {0x10, "Fenix OS"},
        {0x11, "CloudABI"},
        {0x12, "Stratus Technologies OpenVOS"}
};



map<uint32_t, string> machine_map = {
        {0x00,  "No specific instruction set"},
        {0x01,  "AT&T WE 32100"},
        {0x02,  "SPARC"},
        {0x03,  "x86"},
        {0x04,  "Motorola 68000 (M68k)"},
        {0x05,  "Motorola 88000 (M88k)"},
        {0x06,  "Intel MCU"},
        {0x07,  "Intel 80860"},
        {0x08,  "MIPS"},
        {0x09,  "IBM_System/370"},
        {0x0A,  "MIPS RS3000 Little-endian"},
        {0x0E,  "Hewlett-Packard PA-RISC"},
        {0x13,  "Intel 80960"},
        {0x14,  "PowerPC"},
        {0x15,  "PowerPC (64-bit)"},
        {0x16,  "S390, including S390x"},
        {0x17,  "IBM SPU/SPC"},
        {0x24,  "NEC V800"},
        {0x25,  "Fujitsu FR20"},
        {0x26,  "TRW RH-32"},
        {0x27,  "Motorola RCE"},
        {0x28,  "ARM (up to ARMv7/Aarch32)"},
        {0x29,  "Digital Alpha"},
        {0x2A,  "SuperH"},
        {0x2B,  "SPARC Version 9"},
        {0x2C,  "Siemens TriCore embedded processor"},
        {0x2D,  "Argonaut RISC Core"},
        {0x2E,  "Hitachi H8/300"},
        {0x2F,  "Hitachi H8/300H"},
        {0x30,  "Hitachi H8S"},
        {0x32,  "IA-64"},
        {0x31,  "Hitachi H8/500"},
        {0x33,  "Stanford MIPS-X"},
        {0x34,  "Motorola ColdFire"},
        {0x35,  "Motorola M68HC12"},
        {0x36,  "Fujitsu MMA Multimedia Accelerator"},
        {0x37,  "Siemens PCP"},
        {0x38,  "Sony nCPU embedded RISC processor"},
        {0x39,  "Denso NDR1 microprocessor"},
        {0x3A,  "Motorola Star*Core processor"},
        {0x3B,  "Toyota ME16 processor"},
        {0x3C,  "STMicroelectronics ST100 processor"},
        {0x3D,  "Advanced Logic Corp. TinyJ embedded processor family"},
        {0x3E,  "AMD x86-64"},
        {0x8C,  "TMS320C6000 Family"},
        {0xAF,  "MCST Elbrus e2k"},
        {0xB7,  "ARM 64-bits (ARMv8/Aarch64)"},
        {0xF3,  "RISC-V"},
        {0xF7,  "Berkeley Packet Filter"},
        {0x101, "WDC 65C816"}
};

map<uint32_t, string> section_type_map = {
        {0x0, "SHT_NULL"},
        {0x1, "SHT_PROGBITS"},
        {0x2, "SHT_SYMTAB"},
        {0x3, "SHT_STRTAB"},
        {0x4, "SHT_RELA"},
        {0x5, "SHT_HASH"},
        {0x6, "SHT_DYNAMIC"},
        {0x7, "SHT_NOTE"},
        {0x8, "SHT_NOBITS"},
        {0x9, "SHT_REL"},
        {0x0A, "SHT_SHLIB"},
        {0x0B, "SHT_DYNSYM"},
        {0x0E, "SHT_INIT_ARRAY"},
        {0x0F, "SHT_FINI_ARRAY"},
        {0x10, "SHT_PREINIT_ARRAY"},
        {0x11, "SHT_GROUP"},
        {0x12, "SHT_SYMTAB_SHNDX"},
        {0x13, "SHT_NUM"},
        {0x60000000, "SHT_LOOS"}
};


typedef uint32_t Elf32_Word; // 32位版本无符号整形
typedef uint32_t Elf32_Off;  // 32位的版本偏移地址
typedef uint32_t Elf32_Addr; // 32位版本程序地址
typedef uint16_t Elf32_Half;
typedef uint64_t Elf64_Off; // 64位的地址偏移

// section 结构体
typedef struct elf32_shdr {
    // 段名是个字符串,它位于一个叫做".shstrtab"的字符串表。 sh_name是段名字符串在“.shstrtab”中的偏移
    Elf32_Word sh_name;
    // 段的类型
    Elf32_Word sh_type;
    // 段的标志位
    Elf32_Word sh_flags;
    // 如果该段可以被加载，则 sh_addr为该段被加載后在进程地址空间中的虛拟地址;否则 sh_addr为0
    Elf32_Addr sh_addr;
    // 如果该段存在于文件中,则表示该段在文件中的偏移;否则无意义。比如sh_ofet对于BSS段来说就没有意义
    Elf32_Off sh_offset;
    // 段的长度
    Elf32_Word sh_size;
    // 段链接信息
    Elf32_Word sh_link;
    // 段链接信息
    Elf32_Word sh_info;
    // 段地址对齐
    Elf32_Word sh_addralign;
    // 项的长度
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
        printf("%02x ", c);
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
Elf32_Word to_int32(unsigned char *arr, int len) {
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

uint64_t to_int64(unsigned char *arr, int len) {
    uint64_t result = 0;
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
void slice(unsigned char *origin, unsigned char *out, int start, int size) {
    int end = start + size;
    int index = 0;
    for (int i = start; i < end; ++i) {
        out[index] = origin[i];
        index++;
    }
}

Elf32_Word get_int32(unsigned char *origin, int index, int size) {
    unsigned char arr[size];
    slice(origin, arr, index, size);
    return to_int32(arr, size);
}

uint64_t get_int64(unsigned char *origin, int index, int size) {
    unsigned char arr[size];
    slice(origin, arr, index, size);
    return to_int64(arr, size);
}

//Elf64_Off get_int32(unsigned char *origin, int index, int size) {
//    unsigned char arr[size];
//    slice(origin, arr, index, size);
//    return to_int32(arr, size);
//}

void fill(Elf32_Shdr *p_shdr, unsigned char *one_section_arr, int len) {
    if (len != sizeof(Elf32_Shdr)) {
        printf("fill 错误，无法将数组填充到Elf32_Shdr：数据长度不相等\n");
        return;
    }
    // 获取name
    int index = 0;

    p_shdr->sh_name = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取type
    p_shdr->sh_type = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取flags
    p_shdr->sh_flags = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取addr
    p_shdr->sh_addr = get_int32(one_section_arr, index, sizeof(Elf32_Addr));
    index += sizeof(Elf32_Addr);
    // 获取offset
    p_shdr->sh_offset = get_int32(one_section_arr, index, sizeof(Elf32_Off));
    index += sizeof(Elf32_Off);
    // 获取size
    p_shdr->sh_size = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取link
    p_shdr->sh_link = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取info
    p_shdr->sh_info = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取addralign
    p_shdr->sh_addralign = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // 获取entsize
    p_shdr->sh_entsize = get_int32(one_section_arr, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);

}

void parse_one_section_header(Elf32_Shdr *shdr, unsigned char *one_section_arr, int len) {
    if (len != sizeof(Elf32_Shdr)) {
        printf("parse_one_section_header 传入的参数不合法，该数组的长度应该为：%d \n", sizeof(Elf32_Shdr));
        return;
    }
    // 按Elf32_Shdr的结构来解析该数组
    // 将数据填充到结构体
    fill(shdr, one_section_arr, len);
}

/**
 * 获取section的type
 * @param sh_type
 * @return
 */
std::string get_type(Elf32_Word sh_type) {
    return section_type_map.at(sh_type);
}

/**
 * 打印一个section表的item信息
 * @param shdr
 */
void print_elf32_shdr(Elf32_Shdr *shdr) {
    if (shdr == nullptr) return;
    printf("========start=========== \n");
    printf("在段表字符串表中的下标：%d \n", shdr->sh_name);

    try {
        string type_str = get_type(shdr->sh_type);
        printf("段的类型：%s (%d) \n", type_str.c_str(), shdr->sh_type);
    } catch (...) {
        cout << "catch (...)" << endl;
    }
    printf("段在文件中的偏移量：0x%02X \n", shdr->sh_offset);
    printf("段的长度：0x%02X \n", shdr->sh_size);

    printf("========end=========== \n");


}

void get_shstrtab(ifstream &in, char *shstrtab_p, Elf32_Half size, Elf32_Off off) {
    in.seekg(off);
    streamoff pos = in.tellg();
    in.read(shstrtab_p, size);
    cout << "读取到的段表字符串：" << endl;
    for (Elf32_Half i = 0; i < size; ++i) {
        cout << shstrtab_p[i];
    }
}

/**
 * 获取magic的class数据:3字节（ELF）+ 2字节（64/32位）+ 1字节\0
 * @param magic
 * @return
 */
string get_magic_class(unsigned char *magic) {
    char class_arr[6];
    class_arr[0] = magic[1];
    class_arr[1] = magic[2];
    class_arr[2] = magic[3];
    if (magic[4] == 1) { // 32位
        class_arr[3] = '3';
        class_arr[4] = '2';
    } else { // 64位
        class_arr[3] = '6';
        class_arr[4] = '4';
    }
    class_arr[5] = '\0';
    return class_arr;
}

/**
 * 获取魔数中的data，在第6个字节，表示是大端还是小端
 * @param magic
 * @return
 */
string get_magic_data(unsigned char *magic) {
    return magic[5] == 1 ? "little endian" : "big endian";
}

/**
 * 第7个字节，规定ELF文件的主版本号
 * @param magic
 * @return
 */
string get_magic_version(unsigned char *magic) {
    return to_string((int) magic[6]);
}

/**
 * 重写read方法，使他支持读取到unsigned char的数组中
 * @param in
 * @param out
 * @param size
 */
void read(ifstream &in, unsigned char *out, int size) {
    char arr[size];
    in.read(arr, size);
    for (int i = 0; i < size; ++i) {
        out[i] = arr[i];
    }
}

Elf32_Half read_elf32_half(ifstream &in) {
    int size = sizeof(Elf32_Half);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int32(arr, size);
}

Elf32_Word read_elf32_word(ifstream &in) {
    int size = sizeof(Elf32_Word);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int32(arr, size);
}

Elf64_Off read_elf64_off(ifstream &in) {
    int size = sizeof(Elf64_Off);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int32(arr, size);
}

Elf32_Addr read_elf32_addr(ifstream &in) {
    int size = sizeof(Elf32_Addr);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int32(arr, size);
}

Elf32_Off read_elf32_off(ifstream &in) {
    int size = sizeof(Elf32_Off);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int32(arr, size);
}

/**
 * 解析文件头
 * @param header
 * @param in
 */
void parse_header(Elf32_Ehdr &header, ifstream &in) {
    in.seekg(0, ios::beg);
    // 读取魔数
    char magic[EI_NIDENT];
    in.read(magic, EI_NIDENT);
    // 将读取到的数据保存到结构体
    for (int i = 0; i < EI_NIDENT; ++i) {
        header.e_ident[i] = magic[i];
    }
    cout << "class:" << get_magic_class(header.e_ident) << " Data:" << get_magic_data(header.e_ident) << " Version:"
         << get_magic_version(header.e_ident) << endl;
    // 读取type
    header.e_type = read_elf32_half(in);
    // 读取machine
    header.e_machine = read_elf32_half(in);
    header.e_version = read_elf32_word(in);
    header.e_entry = read_elf32_addr(in);
    header.e_phoff = read_elf32_off(in);
    header.e_shoff = read_elf32_off(in);
    header.e_flags = read_elf32_word(in);
    header.e_ehsize = read_elf32_half(in);
    header.e_phentsize = read_elf32_half(in);
    header.e_phnum = read_elf32_half(in);
    header.e_shentsize = read_elf32_half(in);
    header.e_shnum = read_elf32_half(in);
    header.e_shstrndx = read_elf32_half(in);
}


string get_magic_abi(unsigned char *magic) {
    try {
        string abi = abi_map.at(magic[EI_OSABI]);
        return abi;
    } catch (std::exception &e) {
        cout << "获取abi异常:" << e.what() << endl;
    }
    return UNKNOW;


}

/**
 * 解析得到header的type类型
 * @param type
 * @return
 */
string get_header_type(Elf32_Half type) {
    try {
        return section_type_map.at(type);
    } catch (std::exception &e) {
        cout << "解析type异常：" << e.what() << endl;
    }
    return UNKNOW;
}

string get_header_machine(Elf32_Half machine) {
    try {
        return machine_map.at(machine);
    } catch (std::exception &e) {
        cout << "解析machine异常：" << e.what() << endl;
    }
    return UNKNOW;
}

void parse_elf_header(ElfHeader &elfHeader, ifstream &in) {
    in.seekg(0, ios::beg);
    // 读取魔数
    char magic[EI_NIDENT];
    in.read(magic, EI_NIDENT);
    // 将魔数字符化
    for (int i = 0; i < EI_NIDENT; ++i) {
        char temp[4];
        sprintf(temp, "%02X ", magic[i]);
        elfHeader.magic.append(temp);
    }
    // 获取class
    elfHeader.calss_str = get_magic_class(reinterpret_cast<unsigned char *>(magic));
    elfHeader.is_64bit = elfHeader.calss_str.find("64") != elfHeader.calss_str.npos;
    // 获取abi
    elfHeader.abi = get_magic_abi(reinterpret_cast<unsigned char *>(magic));
    //获取Version
    elfHeader.version = get_magic_version(reinterpret_cast<unsigned char *>(magic));
    // 获取data
    elfHeader.data = get_magic_data(reinterpret_cast<unsigned char *>(magic));
    // 解析type 32位
    elfHeader.type = get_header_type(read_elf32_half(in));
    // 解析machine 32位
    elfHeader.machine = get_header_machine(read_elf32_half(in));
    //4字节存储e_version 32位
    read_elf32_word(in);
    // e_entry：elf入口虚拟地址 32位-4字节；64位-8字节
    if (elfHeader.is_64bit) {
        read_elf64_off(in);
    } else {
        read_elf32_word(in);
    }
    //e_phoff：Points to the start of the program header table.
    // 32位-4字节；64位-8字节
    if (elfHeader.is_64bit) {
        elfHeader.program_header_table_off = read_elf64_off(in);
    } else {
        elfHeader.program_header_table_off = read_elf32_off(in);
    }
    // e_shoff：Points to the start of the section header table.
    // 32位-4字节；64位-8字节
    if (elfHeader.is_64bit) {
        elfHeader.section_header_table_off = read_elf64_off(in);
    } else {
        elfHeader.section_header_table_off = read_elf32_off(in);
    }
    // e_flags：用来标志一些ELF文件平台相关的属性
    // 32位
    read_elf32_word(in);
    // e_ehsize: elf头文件本身的大小
    // 32位
    elfHeader.elf_header_size = read_elf32_half(in);
    // e_phentsize:  the size of a program header table entry.
    // 32位
    elfHeader.program_header_entry_size = read_elf32_half(in);
    // e_phnum: the number of entries in the program header table.
    // 32位
    elfHeader.program_header_entry_num = read_elf32_half(in);
    // e_shentsize: the size of a section header table entry.
    // 32位
    elfHeader.section_header_entry_size = read_elf32_half(in);
    // e_shnum: the number of entries in the section header table.
    // 32位
    elfHeader.section_header_entry_num = read_elf32_half(in);
    // e_shstrndx: 段表字符串表在段表中的下标

    cout << elfHeader.toJson() << endl;
}


/**
 * 解析段表，得到段头信息
 * @param in
 * @param file_header
 * @param section_headers
 * @return
 */
int parse_section_headers(ifstream &in, Elf32_Ehdr &file_header, Elf32_Shdr *section_headers) {
    // 检查参数
    if (file_header.e_shoff <= 0 || file_header.e_shnum <= 0) {
        return ERROR;
    }

    in.seekg(file_header.e_shoff);

    int section_header_num = file_header.e_shnum;
    int section_header_size = file_header.e_shentsize;

    for (int i = 0; i < section_header_num; ++i) {
        char shent_arr[section_header_size];
        in.read(shent_arr, section_header_size);
        streamoff pos = in.tellg();
        cout << "当前偏移量" << pos << endl;
        // 解析每一个section 描述
        print_char_arr(shent_arr, section_header_size);
        parse_one_section_header(&section_headers[i], reinterpret_cast<unsigned char *>(shent_arr),
                                 section_header_size);
        print_elf32_shdr(&section_headers[i]);
    }

}

void cur_stream_pos(ifstream &in) {
    streampos pos = in.tellg();
    printf("当前偏移位置%X \n", pos);
}

/**
 * 从段表字符串表中获取段的名称
 * @param off
 * @param shstrtab_content
 * @return
 */
void get_section_name(char *section_name, Elf32_Word off, char *shstrtab_content) {
    Elf32_Word index = 0;
    Elf32_Word offset = off;
    while (shstrtab_content[offset] != '\0') {
        section_name[index] = shstrtab_content[offset];
        index++;
        offset++;
    }
    section_name[index] = '\0';
}

string get_section_type(uint32_t type) {
    try {
        return section_type_map.at(type);
    } catch (std::exception& e) {
        cout <<  "获取section对应的type失败：type=" << type << endl;
    }
    return "";
}

/**
 * 解析一个段的数据
 * @param section_header
 * @param buff
 * @param size
 */
void parse_one_section_header(SectionHeader *section_header, bool is64bit, unsigned char *buff, uint32_t size) {
    if (section_header == nullptr || buff == nullptr || size <= 0) return;

    int index = 0;
    // name：段的名字在段表字符串表中的偏移量 4字节
    section_header->sh_name = get_int32(buff, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // type：段类型 4字节
    section_header->sh_type = get_int32(buff, index, sizeof(Elf32_Word));
    section_header->type = get_section_type(section_header->sh_type);
    index += sizeof(Elf32_Word);
    // flags：标志 32位4字节；64位8字节
    if (is64bit) {
        section_header->sh_flags = get_int64(buff, index, sizeof(Elf64_Off));
        index += sizeof(Elf64_Off);
    } else{
        section_header->sh_flags = get_int32(buff, index, sizeof(Elf32_Word));
        index += sizeof(Elf32_Word);
    }
    // addr：段加载到内存中的虚拟地址 32位4字节；64位8字节
    if (is64bit) {
        section_header->sh_addr = get_int64(buff, index, sizeof(Elf64_Off));
        index += sizeof(Elf64_Off);
    } else {
        section_header->sh_addr = get_int32(buff, index, sizeof(Elf32_Word));
        index += sizeof(Elf32_Word);
    }
    // offset: 段在文件中的偏移地址  32位4字节；64位8字节
    if (is64bit) {
        section_header->sh_offset = get_int64(buff, index, sizeof(Elf64_Off));
        index += sizeof(Elf64_Off);
    } else {
        section_header->sh_offset = get_int32(buff, index, sizeof(Elf32_Word));
        index += sizeof(Elf32_Word);
    }
    char temp[10];
    sprintf(temp, "0X%X", section_header->sh_offset);
    section_header->offset = temp;
    // size: 段的长度 32位4字节；64位8字节
    if (is64bit) {
        section_header->sh_size = get_int64(buff, index, sizeof(Elf64_Off));
        index += sizeof(Elf64_Off);
    } else {
        section_header->sh_size = get_int32(buff, index, sizeof(Elf32_Word));
        index += sizeof(Elf32_Word);
    }
    // link  4字节
    section_header->sh_link = get_int32(buff, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // info 4字节
    section_header->sh_info = get_int32(buff, index, sizeof(Elf32_Word));
    index += sizeof(Elf32_Word);
    // addralign：对齐 32位4字节；64位8字节
    if (is64bit) {
        section_header->sh_addralign = get_int64(buff, index, sizeof(Elf64_Off));
        index += sizeof(Elf64_Off);
    } else {
        section_header->sh_addralign = get_int32(buff, index, sizeof(Elf32_Word));
        index += sizeof(Elf32_Word);
    }
    // entsize 32位4字节；64位8字节
    if (is64bit) {
        section_header->sh_entsize = get_int64(buff, index, sizeof(Elf64_Off));
        index += sizeof(Elf64_Off);
    } else {
        section_header->sh_entsize = get_int32(buff, index, sizeof(Elf32_Word));
        index += sizeof(Elf32_Word);
    }
}

/**
 * 解析段头表
 * @param vector
 * @param in
 */
void parse_section_header_table(const ElfHeader &elf_header, vector<SectionHeader *> &headers, ifstream &in) {
    // 参数检查
    if (elf_header.section_header_table_off <= 0 || elf_header.section_header_entry_num <= 0) return;

    in.seekg(elf_header.section_header_table_off, ios::beg);

    for (int i = 0; i < elf_header.section_header_entry_num; ++i) {
        // 读取一个段header的数据
        char buff[elf_header.section_header_entry_size];
        in.read(buff, elf_header.section_header_entry_size);
        // 解析数据
        SectionHeader *section_header = new SectionHeader();
        section_header->index = i;
        parse_one_section_header(section_header, elf_header.is_64bit, reinterpret_cast<unsigned char *>(buff), elf_header.section_header_entry_size);
        cout << "段相关的数据：" << section_header->to_json().toStyledString() << endl;
        headers.push_back(section_header);
    }
}

int main() {
//    unsigned char c = 152;
//    printf("%02x ", c);

    std::string elf_path = "/Users/yuelin/Documents/Android优化/so结构/libnative-lib.so";
    Json::Value root;
    root["name"] = "wangyuelin";

    ifstream in;
    in.open(elf_path, ios::in);
    if (!in.is_open()) {
        cout << "文件打开失败" << endl;
        return -1;
    }
    ElfHeader elfHeader;
    parse_elf_header(elfHeader, in);
    // 解析文件头
    Elf32_Ehdr file_header;
    parse_header(file_header, in);

    // 解析section header table -> section headers
    vector<SectionHeader *> section_headers;
    parse_section_header_table(elfHeader, section_headers, in);




    /*

    // 解析文件的段表，得到所有的段头信息
    cout << "段的数量：" << file_header.e_shnum << " 每个段的大小：" << file_header.e_shentsize << " 段表在文件中的偏移："
         << file_header.e_shoff << endl;
    Elf32_Shdr section_headers[file_header.e_shnum];
    parse_section_headers(in, file_header, section_headers);

    // 获取段表字符串表的数据
    Elf32_Half shstrtab_index = file_header.e_shstrndx;
    Elf32_Shdr shstrtab_header = section_headers[shstrtab_index];
    char shstrtab_content[shstrtab_header.sh_size];
    printf("段表在文件中的偏移量：%02x, 大小：%02x \n", shstrtab_header.sh_offset, shstrtab_header.sh_size);
    if (shstrtab_header.sh_offset > 0 && shstrtab_header.sh_size > 0) {
        // 将数据读到内存中
        in.seekg(shstrtab_header.sh_offset, ios::beg);
//        cur_stream_pos(in);

        in.read(shstrtab_content, sizeof(shstrtab_content));
        print_char_arr(shstrtab_content, sizeof(shstrtab_content));

        printf("段表字符串表：\n");
        // 输出字符串
        for (int i = 0; i < sizeof(shstrtab_content); ++i) {
            printf("%c", shstrtab_content[i]);
        }
        printf("\n");

    }

    // 为段头的名字给定具体的字符串
    for (int i = 0; i < file_header.e_shnum; ++i) {
        Elf32_Shdr sercion_header = section_headers[i];
        char section_name[20];
        get_section_name(section_name, sercion_header.sh_name, shstrtab_content);
        printf("段的索引：%d 段的名字：%s  段在文件中的偏移：%02x 段的大小：%02x \n", i, section_name, sercion_header.sh_offset,
               sercion_header.sh_size);
    }

*/



    // 开始解析得到段表字符串表

//    printf("一个char占 %d 个字节 \n", sizeof(char));
//    Elf32_Half size = 0x194;
//    Elf32_Off off = 0x318b3;
//    char shstrtab[size];
//    get_shstrtab(in, shstrtab, size, off);

//
//
//    int section_num = 38;
//    // 保存段表
//    Elf32_Shdr section_tab[section_num];
//
//    // .shstrtab（段表字符串表）在段表中的下标
//    Elf32_Half e_shstrndx = 37;
//
//    // 一个section 描述占用40个字节，总的有38个描述
//    int shent_len = 40;
//
//    for (int i = 0; i < section_num; ++i) {
//
//        char shent_arr[shent_len];
//        in.read(shent_arr, shent_len);
//        streamoff pos= in.tellg();
//        cout << "当前偏移量" << pos << endl;
//        // 解析每一个section 描述
//        print_char_arr(shent_arr, shent_len);
//        parse_one_section_header(&section_tab[i], reinterpret_cast<unsigned char *>(shent_arr), shent_len);
////        print_elf32_shdr(&section_tab[i]);
//    }

    // 读取段表字符串表到内存中
//    Elf32_Shdr shstrtab_shdr = section_tab[e_shstrndx];
//    printf("段表在文件中的偏移量：%02x, 大小：%02x", shstrtab_shdr.sh_offset, shstrtab_shdr.sh_size);
//    char shstrtab[shstrtab_shdr.sh_size + 1];
//    in.seekg(shstrtab_shdr.sh_offset);
//    in.read(shstrtab, shstrtab_shdr.sh_size + 1);
//
//    printf("开始输出段表字符串表\n %s", shstrtab);







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


