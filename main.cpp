#include <iostream>
#include <cstdlib>
#include <cmath>
#include <map>
#include "string"
#include "fstream"

using namespace std;

map<int, string> section_type = {
        {0,  "NULL"},
        {1,  "PROGBITS"},
        {2,  "SYMTAB"},
        {3,  "STRTAB"},
        {4,  "RELA"},
        {5,  "HASH"},
        {6,  "DYNAMIC"},
        {7,  "NOTE"},
        {8,  "NOBITS"},
        {9,  "REL"},
        {10, "SHLIB"},
        {11, "DNYSYM"},

};

typedef uint32_t Elf32_Word; // 32位版本无符号整形
typedef uint32_t Elf32_Off;  // 32位的版本偏移地址
typedef uint32_t Elf32_Addr; // 32位版本程序地址
typedef uint16_t Elf32_Half;

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
void slice(unsigned char *origin, unsigned char *out, int start, int size) {
    int end = start + size;
    int index = 0;
    for (int i = start; i < end; ++i) {
        out[index] = origin[i];
        index++;
    }
}

Elf32_Word get_int(unsigned char *origin, int index, int size) {
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
    return section_type.at(sh_type);
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
    string class_str = class_arr;
    return class_str;
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
   return to_string((int)magic[6]);
}

/**
 * 重写read方法，使他支持读取到unsigned char的数组中
 * @param in
 * @param out
 * @param size
 */
void read(ifstream &in,unsigned char *out, int size) {
    char arr[size];
    in.read(arr, size);
    for (int i = 0; i < size; ++i) {
        out[i] = arr[i];
    }
}

Elf32_Half read_elf32_half(ifstream &in){
    int size = sizeof(Elf32_Half);
   unsigned char arr[size];
    read(in, arr, size);
    return to_int(arr, size);
}

Elf32_Word read_elf32_word(ifstream &in){
    int size = sizeof(Elf32_Word);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int(arr, size);
}

Elf32_Addr read_elf32_addr(ifstream &in){
    int size = sizeof(Elf32_Addr);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int(arr, size);
}

Elf32_Off read_elf32_off(ifstream &in){
    int size = sizeof(Elf32_Off);
    unsigned char arr[size];
    read(in, arr, size);
    return to_int(arr, size);
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
    cout << "class:" << get_magic_class(header.e_ident) << " Data:" << get_magic_data(header.e_ident) << " Version:" << get_magic_version(header.e_ident) << endl;
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
    Elf32_Ehdr elf32_ehdr;
    parse_header(elf32_ehdr, in);

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
