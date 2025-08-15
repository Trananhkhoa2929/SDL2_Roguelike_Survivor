cmake_minimum_required(VERSION 3.16)
project(SDL2_Roguelike_Survivor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(Headers)
include_directories(Headers/Components)
include_directories(Headers/Components/Core)
include_directories(Headers/Components/Gameplay)
include_directories(Headers/Components/Items)
include_directories(Headers/Components/UI)
include_directories(Headers/Core)
include_directories(Headers/Events)
include_directories(Headers/Managers)
include_directories(Headers/States)
include_directories(Headers/Systems)
include_directories(Headers/Systems/AI)
include_directories(Headers/Systems/Core)
include_directories(Headers/Systems/Gameplay)
include_directories(Headers/Systems/UI)
include_directories(Headers/Utils)

# Platform-specific configuration
if(EMSCRIPTEN)
    # Emscripten-specific settings
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    
    # Emscripten compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s SDL2_IMAGE_FORMATS='[\"png\"]'")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s INITIAL_MEMORY=134217728")  # 128MB initial memory
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s MAXIMUM_MEMORY=268435456")  # 256MB maximum memory
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s STACK_SIZE=1048576")        # 1MB stack
    
    # Asset preloading - embed the Asset directory
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file Asset")
    
    # Web-specific optimizations
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
    endif()
    
    # HTML shell template
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --shell-file ${CMAKE_SOURCE_DIR}/web_template.html")
    
    message(STATUS "Configuring for Emscripten web build")
else()
    # Desktop build - find SDL2 libraries
    find_package(PkgConfig REQUIRED)
    
    # Try to find SDL2 using different methods
    if(EXISTS "${CMAKE_SOURCE_DIR}/lib/SDL2")
        # Use bundled SDL2 libraries
        set(SDL2_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/lib/SDL2/include")
        set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/SDL2/lib/libSDL2.a;${CMAKE_SOURCE_DIR}/lib/SDL2/lib/libSDL2main.a")
        set(SDL2_IMAGE_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/SDL2_image/lib/libSDL2_image.a")
        set(SDL2_TTF_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/lib/libSDL2_ttf.a")
        
        include_directories(${SDL2_INCLUDE_DIRS})
        include_directories("${CMAKE_SOURCE_DIR}/lib/SDL2_image/include")
        include_directories("${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/include")
        
        message(STATUS "Using bundled SDL2 libraries")
    else()
        # Try to find system SDL2
        pkg_check_modules(SDL2 REQUIRED sdl2)
        pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
        pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
        
        include_directories(${SDL2_INCLUDE_DIRS})
        message(STATUS "Using system SDL2 libraries")
    endif()
endif()

# Collect all source files
file(GLOB_RECURSE SOURCES 
    "Sources/*.cpp"
    "main.cpp"
)

# Collect all header files
file(GLOB_RECURSE HEADERS
    "Headers/*.h"
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
if(EMSCRIPTEN)
    # Emscripten handles SDL2 linking through flags
    target_compile_definitions(${PROJECT_NAME} PRIVATE __EMSCRIPTEN__)
else()
    # Desktop linking
    target_link_libraries(${PROJECT_NAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES} 
        ${SDL2_TTF_LIBRARIES}
    )
    
    # Platform-specific system libraries
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} opengl32 gdi32 winmm imm32 ole32 oleaut32 version uuid advapi32 setupapi shell32)
    elseif(APPLE)
        target_link_libraries(${PROJECT_NAME} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
    else()
        target_link_libraries(${PROJECT_NAME} GL dl pthread)
    endif()
endif()

# Copy assets to build directory for desktop builds
if(NOT EMSCRIPTEN)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Asset $<TARGET_FILE_DIR:${PROJECT_NAME}>/Asset
        COMMENT "Copying assets to build directory"
    )
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")