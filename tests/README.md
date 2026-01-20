# Unit Testing Setup Guide

## 1. Install Dependencies (MSYS2 UCRT64)

Open **MSYS2 UCRT64** terminal and run:

```bash
# Install Google Test
pacman -S mingw-w64-ucrt-x86_64-gtest

# Install LCOV for coverage reports
pacman -S mingw-w64-ucrt-x86_64-lcov
```

## 2. Build Tests

```bash
# Navigate to project root
cd /d/PHUONG/FPT/CPP/MiniProject/MiniProject01

# Create test build directory
mkdir build_test
cd build_test

# Configure with tests enabled
cmake .. -G "MinGW Makefiles" -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug

# Build
mingw32-make
```

## 3. Run Tests

```bash
# Run all tests
./MusicPlayerTests.exe

# Or use CTest
ctest --output-on-failure
```

## 4. Generate Coverage Report

```bash
# Run tests first to generate coverage data
./MusicPlayerTests.exe

# Generate HTML report
mingw32-make coverage

# Open report in browser
start coverage/index.html
```

## Test Structure

```
tests/
├── test_main.cpp           # GTest main entry
├── test_MusicLibrary.cpp   # MusicLibrary tests (21 tests)
└── test_MusicPlayer.cpp    # MusicPlayer tests (25 tests)
```

## Test Cases Summary

### MusicLibrary Tests
- `addSong` - Single/multiple songs, unique IDs
- `findSongByID` - Existing/non-existing IDs
- `findSongByTitle` - Exact match, no match
- `findSongByArtist` - Multiple matches, no match
- `getArtistIndex` - Grouping by artist
- `getAlbumIndex` - Grouping by album
- `getSortedSongs` - Alphabetical sorting
- `clear` - Remove all, reset ID counter

### MusicPlayer Tests
- Initial state (empty library, queue, null current)
- Queue management (add, remove, clear)
- Shuffle mode (enable, disable)
- Choose and play song
- Select by ID/title
- Add album/artist to queue
- Play next/previous
- History tracking
- Clear library
- Smart playlist generation

## Troubleshooting

### GTest Not Found
```bash
# Check if installed
pacman -Qs gtest

# Reinstall if needed
pacman -S mingw-w64-ucrt-x86_64-gtest
```

### Coverage Not Working
```bash
# Check LCOV version
lcov --version

# Make sure you're in build_test directory
cd build_test
```
