cmake_minimum_required(VERSION 3.20)

project(nOs LANGUAGES CXX)

set(CMAKE_SUBPROJECT_TITLE "nOs")
set(CMAKE_GENERATOR_PLATFORM x64)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OUTPUT_DIR_NAME "$<CONFIG>-x64")

set(LOCAL_NOS_BIN   "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR_NAME}/nOs")
set(LOCAL_NOS_INT   "${CMAKE_SOURCE_DIR}/bin-int/${OUTPUT_DIR_NAME}/nOs")

set(LOCAL_BOOST_BIN "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR_NAME}/Boost")
set(LOCAL_BOOST_INT "${CMAKE_SOURCE_DIR}/bin-int/${OUTPUT_DIR_NAME}/Boost")

add_library(nOs SHARED)

target_sources(nOs PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log/Logger.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log/Logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/ApplicationEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/Event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/KeyEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/MouseEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/EntryPoint.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Log.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Layer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Layer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/LayerStack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/LayerStack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/Windows/WindowsWindow.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/Windows/WindowsWindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nospch.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nospch.cpp"
)

source_group("Log" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log/Logger.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log/Logger.cpp"
)

source_group("Core\\Events" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/ApplicationEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/Event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/KeyEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/MouseEvent.h"
)

source_group("Core" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/EntryPoint.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Log.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Layer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Layer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/LayerStack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/LayerStack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Window.h"
)

source_group("Platform\\Win32" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/Windows/WindowsWindow.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/Windows/WindowsWindow.cpp"
)

source_group("" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nospch.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nospch.cpp"
)

target_precompile_headers(nOs PRIVATE 
    "nOs/src/nospch.h"
)

target_include_directories(nOs PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src"
)

set_target_properties(nOs PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${LOCAL_NOS_BIN}"
    LIBRARY_OUTPUT_DIRECTORY "${LOCAL_NOS_BIN}"
    RUNTIME_OUTPUT_DIRECTORY "${LOCAL_NOS_BIN}"
    OBJECT_OUTPUT_DIRECTORY  "${LOCAL_NOS_INT}"
    MSVC_RUNTIME_LIBRARY     "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_link_libraries(nOs PRIVATE d3d11 dxgi)

if(WIN32)
    target_compile_definitions(nOs PRIVATE 
        NOS_PLATFORM_WINDOWS
        NOS_BUILD_DLL
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(nOs PRIVATE NOS_ENABLE_ASSERTS)
endif()

target_compile_definitions(nOs PRIVATE
    $<$<CONFIG:Debug>:NOS_ENABLE_ASSERTS>
    $<$<CONFIG:Release>:NOS_RELEASE>
    $<$<CONFIG:Dist>:NOS_DIST>
)

add_executable(Boost)

target_sources(Boost PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Boost/src/Boost.cpp"
)

source_group("" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Boost/src/Boost.cpp"
)

target_include_directories(Boost PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src"
    MSVC_RUNTIME_LIBRARY     "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

set_target_properties(Boost PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${LOCAL_BOOST_BIN}"
    MSVC_RUNTIME_LIBRARY     "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)

target_link_libraries(Boost PRIVATE nOs)

if(WIN32)
    target_compile_definitions(Boost PRIVATE 
        NOS_PLATFORM_WINDOWS
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Boost PRIVATE NOS_ENABLE_ASSERTS)
endif()

target_compile_definitions(Boost PRIVATE
    $<$<CONFIG:Debug>:NOS_ENABLE_ASSERTS>
    $<$<CONFIG:Release>:NOS_RELEASE>
    $<$<CONFIG:Dist>:NOS_DIST>
)

add_custom_command(TARGET Boost POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:nOs>"
        "${LOCAL_BOOST_BIN}/$<TARGET_FILE_NAME:nOs>"
    COMMENT "Copying nOs.dll next to Boost.exe inside the local bin directory..."
)

set(CMAKE_VS_STARTUP_PROJECT "Boost")
