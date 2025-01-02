cmake_minimum_required(VERSION 2.8)

# 定义App和Server名称
set(APP_NAME ZTraderProxy)
set(SERVER_NAME ZTraderProxyServer)

project(${APP_NAME}-${SERVER_NAME})

# 设置 C++ 标准
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_SYSTEM_VERSION 8.1)
set(CMAKE_GENERATOR_PLATFORM win32)
set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 8.1)
set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION_MAXIMUM OFF)

#set(RXCPP_DIR $ENV{RXCPP_DIR})
#set(rxcpp_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../vcpkg/installed/x86-windows/share/rxcpp")

#find_package(rxcpp CONFIG)

gen_server(${APP_NAME} ${SERVER_NAME})

# 禁用 /RTC1
#target_compile_options(${SERVER_NAME} PRIVATE /RTC1-) 

#target_link_libraries(mysqlclient ssl crypto nghttp2-static)

# 获取当前链接库列表
#get_target_property(current_libraries ProxyServer INTERFACE_LINK_LIBRARIES)

# 包含目录
include_directories(
    $ENV{BOOST_HOME}
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/Base/
    C:/workspace/dev/git-workspace/zinvest/money/trunk/3rd/
    C:/workspace/dev/git-workspace/zinvest/money/trunk/APIs/
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/
    C:/workspace/dev/git-workspace/zinvest/money/trunk/3rd/bson
)

# 添加预处理器定义
add_definitions(
    -D_MBCS
    -DZJ_Base_Common_DYNAMIC
    -DWIN32
    -D_CRT_SECURE_NO_WARNINGS
    -D_SCL_SECURE_NO_WARNINGS
    -D_WIN32_WINNT=0x0A00           #_WIN32_WINNT_WIN10
)

# 设置编译器选项
add_compile_options(/utf-8)

# 设置警告等级和优化级别
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")

# 源文件
set(SOURCE_FILES
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderApi.cpp
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderSvrImpl.cpp
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderUtils.cpp
)

# 头文件（仅用于 IDE 提示）
set(HEADER_FILES
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/enumDefine.h
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderApi.h
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderCli.h
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderDataType.h
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderStruct.h
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderSvrImpl.h
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Include/ZTraderCommon/ZTraderUtils.h
)

# 添加修改可执行目标
target_sources(${SERVER_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# 为目标设置工具架构
set_target_properties(${SERVER_NAME} PROPERTIES
    # 设置首选工具架构为 x86
    VS_GLOBAL_PreferredToolArchitecture "x86"
    # 设置 Windows 目标平台版本为 8.1
    VS_TARGET_PLATFORM_VERSION "8.1"
    # 设置平台工具集为 v140
    VS_PLATFORM_TOOLSET "v140"
)

# 清空所有链接库
set_target_properties(${SERVER_NAME} PROPERTIES LINK_LIBRARIES "")

# 链接库
#[[
target_link_libraries(${SERVER_NAME} 
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Common.lib
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Rpc.lib
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Adapter.lib
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Adapter_CTP.lib
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/log4cxx.lib
    c:/tars/cpp/lib32/\$\(Configuration\)/tarsservant.lib
    c:/tars/cpp/lib32/\$\(Configuration\)/tarsutil.lib
)]]

# 链接库目录
#[[
target_link_directories(${SERVER_NAME} PRIVATE
    $ENV{BOOST_HOME}/stage/lib
    C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)
    c:/tars/cpp/lib32/\$\(Configuration\)/
)]]

# 设置生成调试信息
set_target_properties(${SERVER_NAME} PROPERTIES
    COMPILE_PDB_NAME ${SERVER_NAME}
    LINK_FLAGS "/DEBUG"
)

# 设置库目录
target_link_directories(${SERVER_NAME} PRIVATE 
    $<$<CONFIG:Debug>:$ENV{BOOST_HOME}/stage/lib>
    $<$<CONFIG:Debug>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32>
    $<$<CONFIG:Debug>:c:/tars/cpp/lib32>
    $<$<CONFIG:Release>:$ENV{BOOST_HOME}/stage/lib>
    $<$<CONFIG:Release>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32>
    $<$<CONFIG:Release>:c:/tars/cpp/lib32>
)

# 设置库文件
target_link_libraries(${SERVER_NAME} PRIVATE 
    $<$<CONFIG:Debug>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Common.lib>
    $<$<CONFIG:Debug>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Rpc.lib>
    $<$<CONFIG:Debug>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Adapter.lib>
    $<$<CONFIG:Debug>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/log4cxx.lib>
    $<$<CONFIG:Debug>:c:/tars/cpp/lib32/\$\(Configuration\)/tarsservant.lib>
    $<$<CONFIG:Debug>:c:/tars/cpp/lib32/\$\(Configuration\)/tarsutil.lib>
    $<$<CONFIG:Release>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Common.lib>
    $<$<CONFIG:Release>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Rpc.lib>
    $<$<CONFIG:Release>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32/\$\(Configuration\)/MY_Adapter.lib>
    $<$<CONFIG:Release>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/log4cxx.lib>
    $<$<CONFIG:Release>:c:/tars/cpp/lib32/\$\(Configuration\)/tarsservant.lib>
    $<$<CONFIG:Release>:c:/tars/cpp/lib32/\$\(Configuration\)/tarsutil.lib>
)

# IDE 项目组织
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Source Files" FILES ${SOURCE_FILES})

# 移除 /RTC1
#string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}") 
#string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}") 

# 定义自定义调试配置文件路径
set(CUSTOM_DEBUG_PROPS "${CMAKE_CURRENT_SOURCE_DIR}/custom_debug.props")

# 检查文件是否存在
if(NOT EXISTS "${CUSTOM_DEBUG_PROPS}")
    message(FATAL_ERROR "Custom debug props file not found: ${CUSTOM_DEBUG_PROPS}")
endif()

# 为目标设置 VS_USER_PROPS 属性
set_target_properties(${SERVER_NAME} PROPERTIES
    VS_USER_PROPS "${CUSTOM_DEBUG_PROPS}"
)

if(WIN32)
    include (C:\\workspace\\dev\\git-workspace\\RxCpp\\projects\\CMake\\shared.cmake)
endif()