// tests/test_MusicLibrary.cpp
// Unit tests for MusicLibrary class

#include "../include/MusicLibrary.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MusicLibraryTest : public ::testing::Test {
protected:
  MusicLibrary library;

  void SetUp() override {
    // Reset ID counter before each test
    Song::resetIdCounter();
  }

  void TearDown() override { library.clear(); }

  // Helper to create a test song
  Song createTestSong(const std::string &title, const std::string &artist,
                      const std::string &album) {
    Song song;
    song.title = title;
    song.artist = artist;
    song.album = album;
    song.duration = 180;
    song.filePath = "/test/path/" + title + ".mp3";
    return song;
  }
};

// ========================
// Test: addSong
// ========================

TEST_F(MusicLibraryTest, AddSong_SingleSong_IncreasesSize) {
  Song song = createTestSong("Test Song", "Test Artist", "Test Album");

  library.addSong(song);

  EXPECT_EQ(library.getSize(), 1);
}

TEST_F(MusicLibraryTest, AddSong_MultipleSongs_CorrectSize) {
  library.addSong(createTestSong("Song 1", "Artist 1", "Album 1"));
  library.addSong(createTestSong("Song 2", "Artist 2", "Album 2"));
  library.addSong(createTestSong("Song 3", "Artist 3", "Album 3"));

  EXPECT_EQ(library.getSize(), 3);
}

TEST_F(MusicLibraryTest, AddSong_AssignsUniqueIds) {
  library.addSong(createTestSong("Song 1", "Artist", "Album"));
  library.addSong(createTestSong("Song 2", "Artist", "Album"));

  const auto &songs = library.getAllSongs();
  EXPECT_NE(songs[0].id, songs[1].id);
}

TEST_F(MusicLibraryTest, AddSong_ReallocationCheck) {
  for (int i = 0; i < 1000; i++) {
    library.addSong(createTestSong("Mock Song", "Mock Artist", "Mock Album"));
  }
  // Check if reallocation happened
  bool reallocation_happened = library.addSong(
      createTestSong("Mock Song 1001", "Mock Artist", "Mock Album"));
  EXPECT_TRUE(reallocation_happened);
}
// ========================
// Test: findSongByID
// ========================

TEST_F(MusicLibraryTest, FindSongByID_ExistingID_ReturnsSong) {
  library.addSong(createTestSong("Target Song", "Artist", "Album"));

  const Song *found = library.findSongByID(0);

  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->title, "Target Song");
}

TEST_F(MusicLibraryTest, FindSongByID_NonExistingID_ReturnsNull) {
  library.addSong(createTestSong("Song", "Artist", "Album"));

  const Song *found = library.findSongByID(999);

  EXPECT_EQ(found, nullptr);
}

// ========================
// Test: findSongByTitle
// ========================

TEST_F(MusicLibraryTest, FindSongByTitle_ExactMatch_ReturnsSong) {
  library.addSong(createTestSong("Unique Title", "Artist", "Album"));

  const Song *found = library.findSongByTitle("Unique Title");

  ASSERT_NE(found, nullptr);
  EXPECT_EQ(found->title, "Unique Title");
}

TEST_F(MusicLibraryTest, FindSongByTitle_NoMatch_ReturnsNull) {
  library.addSong(createTestSong("Some Song", "Artist", "Album"));

  const Song *found = library.findSongByTitle("Nonexistent");

  EXPECT_EQ(found, nullptr);
}

// ========================
// Test: findSongByArtist
// ========================

TEST_F(MusicLibraryTest, FindSongByArtist_MultipleMatches_ReturnsAll) {
  library.addSong(createTestSong("Song 1", "Same Artist", "Album 1"));
  library.addSong(createTestSong("Song 2", "Same Artist", "Album 2"));
  library.addSong(createTestSong("Song 3", "Different Artist", "Album 3"));

  auto found = library.findSongByArtist("Same Artist");

  EXPECT_EQ(found.size(), 2);
}

TEST_F(MusicLibraryTest, FindSongByArtist_NoMatch_ReturnsEmpty) {
  library.addSong(createTestSong("Song", "Artist", "Album"));

  auto found = library.findSongByArtist("Unknown Artist");

  EXPECT_TRUE(found.empty());
}

// ========================
// Test: getArtistIndex
// ========================

TEST_F(MusicLibraryTest, GetArtistIndex_GroupsByArtist) {
  library.addSong(createTestSong("Song 1", "Artist A", "Album"));
  library.addSong(createTestSong("Song 2", "Artist A", "Album"));
  library.addSong(createTestSong("Song 3", "Artist B", "Album"));

  const auto &index = library.getArtistIndex();

  EXPECT_EQ(index.size(), 2);                // 2 artists
  EXPECT_EQ(index.at("Artist A").size(), 2); // 2 songs
  EXPECT_EQ(index.at("Artist B").size(), 1); // 1 song
}

// ========================
// Test: getAlbumIndex
// ========================

TEST_F(MusicLibraryTest, GetAlbumIndex_GroupsByAlbum) {
  library.addSong(createTestSong("Song 1", "Artist", "Album X"));
  library.addSong(createTestSong("Song 2", "Artist", "Album X"));
  library.addSong(createTestSong("Song 3", "Artist", "Album Y"));

  const auto &index = library.getAlbumIndex();

  EXPECT_EQ(index.size(), 2); // 2 albums
  EXPECT_EQ(index.at("Album X").size(), 2);
  EXPECT_EQ(index.at("Album Y").size(), 1);
}

// ========================
// Test: getSortedSongs
// ========================

TEST_F(MusicLibraryTest, GetSortedSongs_ReturnsSortedByTitle) {
  library.addSong(createTestSong("Zebra", "Artist", "Album"));
  library.addSong(createTestSong("Apple", "Artist", "Album"));
  library.addSong(createTestSong("Mango", "Artist", "Album"));

  auto sorted = library.getSortedSongs();

  ASSERT_EQ(sorted.size(), 3);
  EXPECT_EQ(sorted[0]->title, "Apple");
  EXPECT_EQ(sorted[1]->title, "Mango");
  EXPECT_EQ(sorted[2]->title, "Zebra");
}

// ========================
// Test: clear
// ========================

TEST_F(MusicLibraryTest, Clear_RemovesAllSongs) {
  library.addSong(createTestSong("Song 1", "Artist", "Album"));
  library.addSong(createTestSong("Song 2", "Artist", "Album"));

  library.clear();

  EXPECT_EQ(library.getSize(), 0);
}

TEST_F(MusicLibraryTest, Clear_ResetsIdCounter) {
  library.addSong(createTestSong("Song", "Artist", "Album"));
  library.clear();

  library.addSong(createTestSong("New Song", "Artist", "Album"));

  const auto &songs = library.getAllSongs();
  EXPECT_EQ(songs[0].id, 0); // ID should restart from 0
}

TEST_F(MusicLibraryTest, Clear_ClearsAllIndexes) {
  library.addSong(createTestSong("Song", "Artist", "Album"));
  library.clear();

  EXPECT_TRUE(library.getArtistIndex().empty());
  EXPECT_TRUE(library.getAlbumIndex().empty());
  EXPECT_TRUE(library.getSortedSongs().empty());
}

// ========================
// Test: getAllSongs
// ========================

TEST_F(MusicLibraryTest, GetAllSongs_ReturnsAllAddedSongs) {
  library.addSong(createTestSong("Song 1", "Artist 1", "Album 1"));
  library.addSong(createTestSong("Song 2", "Artist 2", "Album 2"));

  const auto &songs = library.getAllSongs();

  EXPECT_EQ(songs.size(), 2);
}

TEST_F(MusicLibraryTest, GetAllSongs_EmptyLibrary_ReturnsEmpty) {
  const auto &songs = library.getAllSongs();

  EXPECT_TRUE(songs.empty());
}
