cmake_minimum_required(VERSION 3.10)
project(MusicPlayer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- HARDCODED PATHS FOR WINDOWS (MSYS2) ---
# We assume default MSYS2 installation path. 
# If you installed MSYS2 elsewhere, change "C:/msys64" to your path.

set(TAGLIB_INCLUDE_DIR "C:/msys64/mingw64/include/taglib")
set(TAGLIB_LIBRARY "C:/msys64/mingw64/lib/libtag.dll.a") 

# Check if the library file actually exists before trying to link
if(NOT EXISTS "${TAGLIB_LIBRARY}")
    message(FATAL_ERROR "Could not find TagLib at ${TAGLIB_LIBRARY}. Did you run 'pacman -S mingw-w64-x86_64-taglib'?")
endif()

# --- INCLUDES ---
include_directories(include)
include_directories("C:/msys64/mingw64/include") # Standard headers
include_directories(${TAGLIB_INCLUDE_DIR})       # TagLib headers

# --- SOURCES ---
file(GLOB SOURCES "src/*.cpp")

# --- EXECUTABLE ---
add_executable(MusicPlayer ${SOURCES})

# --- LINKING ---
# Link the specific library file we found
target_link_libraries(MusicPlayer "${TAGLIB_LIBRARY}")