cmake_minimum_required(VERSION 3.20)

project(nOs LANGUAGES CXX)

set(CMAKE_SUBPROJECT_TITLE "nOs")
set(CMAKE_GENERATOR_PLATFORM x64)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set(OUTPUT_DIR_NAME "$<CONFIG>-x64")
set(LOCAL_NOS_BIN   "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR_NAME}/nOs")
set(LOCAL_NOS_INT   "${CMAKE_SOURCE_DIR}/bin-int/${OUTPUT_DIR_NAME}/nOs")
set(LOCAL_BOOST_BIN "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_DIR_NAME}/Boost")
set(LOCAL_BOOST_INT "${CMAKE_SOURCE_DIR}/bin-int/${OUTPUT_DIR_NAME}/Boost")

set(NOS_COMPILE_DEFS
    $<$<CONFIG:Debug>:NOS_ENABLE_ASSERTS>
    $<$<CONFIG:Release>:NOS_RELEASE>
    $<$<CONFIG:Dist>:NOS_DIST>
)

if(WIN32)
    list(APPEND NOS_COMPILE_DEFS NOS_PLATFORM_WINDOWS)
endif()

add_library(nOs STATIC)

target_sources(nOs PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log/Logger.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log/Logger.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/ApplicationEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/Event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/KeyEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Events/MouseEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Buffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/GraphicsContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RenderCommand.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RenderCommand.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Renderer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RendererAPI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RendererAPI.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Shader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/PixelShader.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/VertexShader.hlsl"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXContext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXRendererAPI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXRendererAPI.cpp"
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

source_group("Core\\Renderer" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Buffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/GraphicsContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RenderCommand.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RenderCommand.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Renderer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RendererAPI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/RendererAPI.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Shader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/Shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/PixelShader.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/VertexShader.hlsl"
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

source_group("Platform\\Windows" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/Windows/WindowsWindow.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/Windows/WindowsWindow.cpp"
)

source_group("Platform\\DirectX" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXContext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXRendererAPI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/Platform/DirectX/DirectXRendererAPI.cpp"
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
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/gui"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/log"
    "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src"
)

set_target_properties(nOs PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${LOCAL_NOS_BIN}"
    LIBRARY_OUTPUT_DIRECTORY "${LOCAL_NOS_BIN}"
    RUNTIME_OUTPUT_DIRECTORY "${LOCAL_NOS_BIN}"
)

target_link_libraries(nOs PRIVATE 
    d3d11
    dxgi
    d3dcompiler
)

target_compile_definitions(nOs PRIVATE ${NOS_COMPILE_DEFS})

add_executable(Boost)

target_sources(Boost PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Boost/src/Boost.cpp"
)

source_group("" FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/Boost/src/Boost.cpp"
)


set_target_properties(Boost PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${LOCAL_BOOST_BIN}"
)

target_link_libraries(Boost PRIVATE nOs)

target_compile_definitions(Boost PRIVATE ${NOS_COMPILE_DEFS})

set(CMAKE_VS_STARTUP_PROJECT "Boost")

add_custom_command(TARGET Boost POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/VertexShader.hlsl"
        "$<TARGET_FILE_DIR:Boost>/VertexShader.hlsl"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/nOs/src/nOs/Renderer/PixelShader.hlsl"
        "$<TARGET_FILE_DIR:Boost>/PixelShader.hlsl"
)