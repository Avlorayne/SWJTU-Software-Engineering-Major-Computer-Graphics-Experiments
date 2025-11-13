cmake_minimum_required(VERSION 3.30)
project(GDItest)

set(CMAKE_CXX_STANDARD 17)

# 设置Windows子系统
if (WIN32)
    if (MINGW)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    endif()
    add_compile_definitions(UNICODE _UNICODE)
endif()

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加所有源文件（修正路径）
add_executable(GDItest
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${CMAKE_SOURCE_DIR}/src/appcore/GraphicsApp.cpp  # 修正为 appcore
        ${CMAKE_SOURCE_DIR}/src/ui/TabManager.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/UIComponents.cpp
        ${CMAKE_SOURCE_DIR}/src/graphics/ExistingGraphics.cpp
        ${CMAKE_SOURCE_DIR}/src/graphics/InteractiveGraphics.cpp
        ${CMAKE_SOURCE_DIR}/src/graphics/DrawingAlgorithms.cpp
        src/graphics/GraphicsManager.cpp
        src/graphics/GraphicsManager.h
)

# 链接必要的Windows库和数学库
target_link_libraries(GDItest kernel32 user32 gdi32 comctl32)

# 设置输出目录
set_target_properties(GDItest PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)