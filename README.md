# Phuong Tran Hoang Music Player 🎵

A modern C++ music player with a beautiful ImGui interface, featuring shuffle, queue management, and Vietnamese language support.

![C++](https://img.shields.io/badge/C++-17-blue) ![ImGui](https://img.shields.io/badge/UI-ImGui-orange) ![miniaudio](https://img.shields.io/badge/Audio-miniaudio-green)

## Features

- 🎶 **Audio Playback** - MP3/WAV support via miniaudio
- 🔀 **Shuffle Mode** - Smart shuffle with history tracking
- 📚 **Library Management** - Browse by Artists, Albums, or All Songs
- 🔍 **Search** - Find songs by title, artist, or album
- 🌏 **Vietnamese Support** - Full Unicode file path and metadata support
- 📋 **Queue System** - Add songs to queue, view upcoming tracks
- ⏮️⏯️⏭️ **Playback Controls** - Previous, Play/Pause, Next, Seek

## Prerequisites

- **MSYS2/MinGW** (Windows) or GCC (Linux)
- **CMake** 3.10+
- **GLFW3** - Window management
- **TagLib** - Audio metadata reading
- **OpenGL** - Rendering

## Build Instructions

### Windows (MSYS2/MinGW)

```bash
# Clone the repository
git clone https://github.com/phuong-tranhoang/MiniProject01.git
cd MiniProject01

# Create build directory
mkdir build && cd build

# Generate and build
cmake .. -G "MinGW Makefiles"
mingw32-make

# Run
./MusicPlayer.exe
```

### Linux

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt install libglfw3-dev libtag1-dev cmake g++

# Build
mkdir build && cd build
cmake ..
make

# Run
./MusicPlayer
```

## Usage

1. **Load Library**: Enter a folder path and click "Load Folder"
2. **Browse Music**: Use the Library, Albums, or Artists tabs
3. **Play Songs**: Click on a song to play, or add to queue
4. **Control Playback**: Use the bottom bar controls

## Project Structure

```
MiniProject01/
├── include/         # Header files
│   ├── AudioEngine.h
│   ├── MusicLibrary.h
│   ├── MusicPlayer.h
│   ├── Song.h
│   └── ...
├── src/             # Source files
│   ├── main.cpp
│   ├── AudioEngine.cpp
│   ├── MusicPlayer.cpp
│   └── ...
├── assets/          # Fonts and resources
├── lib/             # Third-party libraries
└── CMakeLists.txt
```

## Dependencies

| Library | Purpose | License |
|---------|---------|---------|
| [miniaudio](https://miniaud.io/) | Audio playback | Public Domain |
| [ImGui](https://github.com/ocornut/imgui) | User interface | MIT |
| [GLFW](https://www.glfw.org/) | Window/Input | Zlib |
| [TagLib](https://taglib.org/) | Audio metadata | LGPL/MPL |

## License

MIT License - See [LICENSE](LICENSE) for details.

## Author

**PhuongTH33** - FPT University
