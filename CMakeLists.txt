cmake_minimum_required(VERSION 3.10)
project(MusicPlayer)

set(CMAKE_CXX_STANDARD 17)

# --- 1. SEARCH PATHS (Crucial for MSYS2/Windows) ---
# Tell CMake to look in the UCRT64 and MinGW64 folders
set(MSYS_UCRT "C:/msys64/ucrt64")
set(MSYS_MINGW "C:/msys64/mingw64")

include_directories(include)
include_directories(include/imgui-master)
include_directories(include/imgui-master/backends)
include_directories(include/miniaudio-master)

# Include TagLib Headers
include_directories("${MSYS_UCRT}/include")
include_directories("${MSYS_UCRT}/include/taglib")
include_directories("${MSYS_MINGW}/include")
include_directories("${MSYS_MINGW}/include/taglib")

# --- 2. FIND LIBRARIES MANUALLY ---
# Find TagLib (Look for libtag.dll.a or libtag.a)
find_library(TAGLIB_LIB 
    NAMES tag libtag 
    PATHS 
    "${MSYS_UCRT}/lib" 
    "${MSYS_MINGW}/lib"
    NO_DEFAULT_PATH
)

# Find GLFW (Look for libglfw3.a)
# Note: We prioritize the one in your local lib/ folder if you put it there
find_library(GLFW_LIB 
    NAMES glfw3 libglfw3 
    PATHS 
    "${CMAKE_SOURCE_DIR}/lib"
    "${MSYS_UCRT}/lib"
    "${MSYS_MINGW}/lib"
)

if (NOT TAGLIB_LIB)
    message(FATAL_ERROR "Could not find TagLib! Did you run 'pacman -S mingw-w64-ucrt-x86_64-taglib'?")
endif()

if (NOT GLFW_LIB)
    message(FATAL_ERROR "Could not find GLFW library!")
endif()

message(STATUS "Found TagLib: ${TAGLIB_LIB}")
message(STATUS "Found GLFW: ${GLFW_LIB}")

# --- 3. SOURCE FILES ---
file(GLOB SOURCES "src/*.cpp")

# ImGui Sources
set(IMGUI_DIR "include/imgui-master")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# --- 4. EXECUTABLE & LINKING ---
add_executable(MusicPlayer ${SOURCES} ${IMGUI_SOURCES})

# Link libraries: TagLib + GLFW + OpenGL + System Libs
target_link_libraries(MusicPlayer 
    ${TAGLIB_LIB} 
    ${GLFW_LIB} 
    opengl32 
    winmm
)