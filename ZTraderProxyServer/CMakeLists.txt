cmake_minimum_required(VERSION 2.8)

# 定义App和Server名称
set(APP_NAME ZTraderProxy)

project(${APP_NAME})

option(TARS_MYSQL "option for mysql" ON)
option(TARS_SSL "option for ssl" OFF)
option(TARS_HTTP2 "option for http2" OFF)

if(WIN32)
    include (c:\\tars\\cpp\\makefile\\tars-tools-x86.cmake)
    # include (C:\\workspace\\dev\\git-workspace\\RxCpp\\projects\\CMake\\shared.cmake)
else()
    include (/usr/local/tars/cpp/makefile/tars-tools.cmake)
endif()

####you can: cd build; cmake .. -DTARS_WEB_HOST={your web host}
set(TARS_WEB_HOST "" CACHE STRING "set web host")
IF (TARS_WEB_HOST STREQUAL "")
	set(TARS_WEB_HOST "http://tars.test.com")
ENDIF ()

if(WIN32)
    include_directories(c:\\tars\\cpp\\thirdparty\\include)
    include_directories($ENV{RXCPP_DIR}/Rx/v2/src)
else()
    include_directories(/usr/local/tars/cpp/thirdparty/include)
    include_directories($ENV{RXCPP_DIR}/Rx/v2/src)
endif()

#include_directories(/home/tarsprotol/App/OtherServer)
#link_directories(/usr/local/tars/cpp/thirdparty/lib)
#target_link_libraries(mysqlclient ssl crypto nghttp2-static)


#include_directories(PositionServer/Server)
add_subdirectory(src)
add_subdirectory(unit-test)

#include_directories(HelloServer/Server)
#add_subdirectory(HelloServer)
#add_subdirectory(ProxyServer)

# set(rxcpp_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../vcpkg/installed/x86-windows/share/rxcpp")