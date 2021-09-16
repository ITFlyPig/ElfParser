cmake_minimum_required(VERSION 3.19)
project(ElfParser)

set(CMAKE_CXX_STANDARD 14)
set(root_dir ${cmake_source_dir})

# 添加第三方静态库

## 指定头文件搜索路劲
include_directories(${PROJECT_SOURCE_DIR}/include)

## 指定引用外部库搜索路劲
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 运行的程序
add_executable(ElfParser main.cpp ElfInfo.cpp ElfInfo.h SectionHeader.cpp SectionHeader.h ElfHeader.cpp ElfHeader.h ProgramHeader.cpp ProgramHeader.h)

target_link_libraries(${PROJECT_NAME} libjsoncpp.a)

# 动态库
#add_library(
#        ElfParser_dy
#        SHARED
#        main.cpp)