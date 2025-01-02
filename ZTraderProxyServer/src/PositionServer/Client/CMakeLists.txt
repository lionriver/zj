# CMake 最低版本要求
cmake_minimum_required(VERSION 2.8)

# 定义App和Server名称
set(APP_NAME ZTraderProxy)
set(CLIENT_NAME ZTraderProxyClient)
set(SERVER_NAME ZTraderProxyServer)

project(${APP_NAME}-${CLIENT_NAME})

# 设置 C++ 标准
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_SYSTEM_VERSION 8.1)
set(CMAKE_GENERATOR_PLATFORM win32)
set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION 8.1)
set(CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION_MAXIMUM OFF)

gen_server(${APP_NAME} ${CLIENT_NAME})

#[[
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
target_sources(${CLIENT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
]]

# 为目标设置工具架构
set_target_properties(${CLIENT_NAME} PROPERTIES
    # 设置首选工具架构为 x86
    VS_GLOBAL_PreferredToolArchitecture "x86"
    # 设置 Windows 目标平台版本为 8.1
    VS_TARGET_PLATFORM_VERSION "8.1"
    # 设置平台工具集为 v140
    VS_PLATFORM_TOOLSET "v140"
)

# 清空所有链接库
set_target_properties(${CLIENT_NAME} PROPERTIES LINK_LIBRARIES "")

# 设置库目录
target_link_directories(${CLIENT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:$ENV{BOOST_HOME}/stage/lib>
    $<$<CONFIG:Debug>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32>
    $<$<CONFIG:Debug>:c:/tars/cpp/lib32>
    $<$<CONFIG:Release>:$ENV{BOOST_HOME}/stage/lib>
    $<$<CONFIG:Release>:C:/workspace/dev/git-workspace/zinvest/money/trunk/Libs/win32>
    $<$<CONFIG:Release>:c:/tars/cpp/lib32>
)

# 设置库文件
target_link_libraries(${CLIENT_NAME} PRIVATE 
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

add_dependencies(${CLIENT_NAME} ${SERVER_NAME})