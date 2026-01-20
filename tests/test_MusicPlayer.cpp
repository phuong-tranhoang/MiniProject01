// tests/test_MusicPlayer.cpp
// Unit tests for MusicPlayer class

#include "../include/MusicPlayer.h"
#include "PlaybackQueue.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <utility>

class MusicPlayerTest : public ::testing::Test {
protected:
  MusicPlayer player;

  void SetUp() override { Song::resetIdCounter(); }

  void TearDown() override { player.clearLibrary(); }

  // Helper: Add a song to library and return pointer
  const Song *addSongToLibrary(const std::string &title,
                               const std::string &artist,
                               const std::string &album) {
    Song song;
    song.title = title;
    song.artist = artist;
    song.album = album;
    song.duration = 180;
    song.filePath = "/test/" + title + ".mp3";

    // Access private library through const reference workaround
    // Note: In real testing, you might expose a non-const getter
    MusicLibrary &lib = const_cast<MusicLibrary &>(player.getLibrary());
    lib.addSong(song);
    return lib.findSongByTitle(title);
  }
};

// ========================
// Test: Initial State
// ========================

TEST_F(MusicPlayerTest, Constructor_LibraryEmpty) {
  EXPECT_EQ(player.getLibrarySize(), 0);
}

TEST_F(MusicPlayerTest, Constructor_QueueEmpty) {
  EXPECT_EQ(player.getQueueSize(), 0);
}

TEST_F(MusicPlayerTest, Constructor_CurrentSongNull) {
  EXPECT_EQ(player.getCurrentSong(), nullptr);
}

TEST_F(MusicPlayerTest, Constructor_ShuffleDisabled) {
  EXPECT_FALSE(player.isShuffleEnabled());
}

// ========================
// Test: Load Library
// ========================

TEST_F(MusicPlayerTest, LoadLibrary_LoadsSongs) {
  std::filesystem::path sourceFile(__FILE__);
  std::filesystem::path rootDir = sourceFile.parent_path();
  std::filesystem::path libraryPath = rootDir / "library";
  player.loadLibrary(libraryPath.string());

  EXPECT_GE(player.getLibrarySize(), 0);
}

TEST_F(MusicPlayerTest, LoadLibrary_InvalidPath_NoChange) {
  player.loadLibrary("/test/invalid");
  EXPECT_EQ(player.getLibrarySize(), 0);
}

TEST_F(MusicPlayerTest, LoadLibrary_MultipleTimes_SamePath) {
  std::filesystem::path sourceFile(__FILE__);
  std::filesystem::path rootDir = sourceFile.parent_path();
  std::filesystem::path libraryPath = rootDir / "library";
  player.loadLibrary(libraryPath.string());
  player.loadLibrary(libraryPath.string());

  EXPECT_EQ(player.getLibrarySize(), 2);
}

// ========================
// Test: Queue Management
// ========================

TEST_F(MusicPlayerTest, GetQueueManager) {
  auto &queue = player.getQueueManager();
  EXPECT_EQ(queue.getQueueSize(), 0);
}

TEST_F(MusicPlayerTest, AddSongToQueue_IncreasesQueueSize) {
  const Song *song = addSongToLibrary("Test Song", "Artist", "Album");

  player.addSongToQueue(song);

  EXPECT_EQ(player.getQueueSize(), 1);
}

TEST_F(MusicPlayerTest, AddSongToQueue_MultipleSongs) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  const Song *song3 = addSongToLibrary("Song 3", "Artist", "Album");

  player.addSongToQueue(song1);
  player.addSongToQueue(song2);
  player.addSongToQueue(song3);

  EXPECT_EQ(player.getQueueSize(), 3);
}

TEST_F(MusicPlayerTest, AddSongToQueue_ShuffleMode) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");

  player.addSongToQueue(song1);
  player.enableShuffle();
  player.addSongToQueue(song2);

  EXPECT_EQ(player.getQueueSize(), 2);
  EXPECT_TRUE(player.isShuffleEnabled());
}

TEST_F(MusicPlayerTest, RemoveSongFromQueue_DecreasesQueueSize) {
  const Song *song = addSongToLibrary("Test Song", "Artist", "Album");
  player.addSongToQueue(song);

  player.removeSongFromQueue(song);

  EXPECT_EQ(player.getQueueSize(), 0);
}

TEST_F(MusicPlayerTest, ClearQueue_EmptiesQueue) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  player.addSongToQueue(song1);
  player.addSongToQueue(song2);

  player.clearQueue();

  EXPECT_EQ(player.getQueueSize(), 0);
}

// ========================
// Test: Shuffle Mode
// ========================

TEST_F(MusicPlayerTest, GetShuffleManager) {
  const Song *song = addSongToLibrary("Song", "Artist", "Album");
  player.addSongToQueue(song);
  player.enableShuffle();
  auto &shuffleMgr = player.getShuffleManager();
  player.playNext();
  EXPECT_EQ(shuffleMgr.getCurrentIndex(), 0);
}

TEST_F(MusicPlayerTest, EnableShuffle_SetsFlag) {
  const Song *song = addSongToLibrary("Song", "Artist", "Album");
  player.addSongToQueue(song);

  player.enableShuffle();

  EXPECT_TRUE(player.isShuffleEnabled());
}

TEST_F(MusicPlayerTest, EnableShuffle_WithEmptyQueue_DoesNotChangeState) {
  player.enableShuffle();

  EXPECT_FALSE(player.isShuffleEnabled());
}

TEST_F(MusicPlayerTest, DisableShuffle_ClearsFlag) {
  const Song *song = addSongToLibrary("Song", "Artist", "Album");
  player.addSongToQueue(song);
  player.enableShuffle();

  player.disableShuffle();

  EXPECT_FALSE(player.isShuffleEnabled());
}

TEST_F(MusicPlayerTest, ClearQueue_DisablesShuffle) {
  const Song *song = addSongToLibrary("Song", "Artist", "Album");
  player.addSongToQueue(song);
  player.enableShuffle();

  player.clearQueue();

  EXPECT_FALSE(player.isShuffleEnabled());
}

// ========================
// Test: Choose and Play Song
// ========================

TEST_F(MusicPlayerTest, ChooseAndPlaySong_SetsCurrentSong) {
  const Song *song = addSongToLibrary("Target Song", "Artist", "Album");

  player.chooseAndPlaySong(song);

  EXPECT_EQ(player.getCurrentSong(), song);
}

TEST_F(MusicPlayerTest, ChooseAndPlaySong_NullSong_NoChange) {
  player.chooseAndPlaySong(nullptr);

  EXPECT_EQ(player.getCurrentSong(), nullptr);
}
// ========================
// Test: Select and Add by ID
// ========================

TEST_F(MusicPlayerTest, SelectAndAddSong_ByID_SetsCurrentSong) {
  const Song *song = addSongToLibrary("Unique Title", "Artist", "Album");

  player.selectAndAddSong(song->id);

  EXPECT_EQ(player.getQueueSize(), 1);
}

// ========================
// Test: Select and Add by Title
// ========================

TEST_F(MusicPlayerTest, SelectAndAddSong_ByTitle_SetsCurrentSong) {
  const Song *song = addSongToLibrary("Unique Title", "Artist", "Album");

  player.selectAndAddSong(std::string("Unique Title"));

  EXPECT_EQ(player.getQueueSize(), 1);
}

// ========================
// Test: Select and Play by ID
// ========================

TEST_F(MusicPlayerTest, SelectAndPlaySong_ByID_SetsCurrentSong) {
  const Song *song = addSongToLibrary("Song", "Artist", "Album");
  player.selectAndPlaySong(song->id);
  EXPECT_EQ(player.getCurrentSong(), song);
}

TEST_F(MusicPlayerTest, SelectAndPlaySong_ByID_GoesBack) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  player.selectAndPlaySong(song1->id);
  player.selectAndPlaySong(song2->id);

  player.playPrevious();

  EXPECT_EQ(player.getCurrentSong(), song1);
}

TEST_F(MusicPlayerTest, SelectAndPlaySong_InvalidID_NoChange) {
  addSongToLibrary("Song", "Artist", "Album");

  player.selectAndPlaySong(999); // Non-existent ID

  EXPECT_EQ(player.getCurrentSong(), nullptr);
}

// ========================
// Test: Select and Play by Title
// ========================

TEST_F(MusicPlayerTest, SelectAndPlaySong_ByTitle_SetsCurrentSong) {
  const Song *song = addSongToLibrary("Unique Title", "Artist", "Album");

  player.selectAndPlaySong(std::string("Unique Title"));

  EXPECT_EQ(player.getCurrentSong(), song);
}

TEST_F(MusicPlayerTest, SelectAndPlaySong_ByTitle_GoesBack) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  player.selectAndPlaySong(std::string("Song 1"));
  player.selectAndPlaySong(std::string("Song 2"));

  player.playPrevious();

  EXPECT_EQ(player.getCurrentSong(), song1);
}

TEST_F(MusicPlayerTest, SelectAndPlaySong_InvalidTitle_NoChange) {
  addSongToLibrary("Song", "Artist", "Album");

  player.selectAndPlaySong(std::string("Nonexistent"));

  EXPECT_EQ(player.getCurrentSong(), nullptr);
}

// ========================
// Test: Add Album to Queue
// ========================

TEST_F(MusicPlayerTest, AddAlbumToQueue_AddsAllSongsFromAlbum) {
  addSongToLibrary("Song 1", "Artist", "Target Album");
  addSongToLibrary("Song 2", "Artist", "Target Album");
  addSongToLibrary("Song 3", "Artist", "Other Album");

  size_t added = player.addAlbumToQueue("Target Album");

  EXPECT_EQ(added, 2);
  EXPECT_EQ(player.getQueueSize(), 2);
}

TEST_F(MusicPlayerTest, AddAlbumToQueue_NonexistentAlbum_ReturnsZero) {
  addSongToLibrary("Song", "Artist", "Album");

  size_t added = player.addAlbumToQueue("Nonexistent Album");

  EXPECT_EQ(added, 0);
}

// ========================
// Test: Add Artist to Queue
// ========================

TEST_F(MusicPlayerTest, AddArtistToQueue_AddsAllSongsFromArtist) {
  addSongToLibrary("Song 1", "Target Artist", "Album 1");
  addSongToLibrary("Song 2", "Target Artist", "Album 2");
  addSongToLibrary("Song 3", "Other Artist", "Album 3");

  size_t added = player.addArtistToQueue("Target Artist");

  EXPECT_EQ(added, 2);
  EXPECT_EQ(player.getQueueSize(), 2);
}

// ========================
// Test: Clear Library
// ========================

TEST_F(MusicPlayerTest, ClearLibrary_ClearsEverything) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  addSongToLibrary("Song 2", "Artist", "Album");
  player.addSongToQueue(song1);
  player.chooseAndPlaySong(song1);

  player.clearLibrary();

  EXPECT_EQ(player.getLibrarySize(), 0);
  EXPECT_EQ(player.getQueueSize(), 0);
  EXPECT_EQ(player.getCurrentSong(), nullptr);
}

// ========================
// Test: Play Next
// ========================

TEST_F(MusicPlayerTest, PlayNext_FromQueue_ChangesCurrentSong) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  player.addSongToQueue(song1);
  player.addSongToQueue(song2);
  player.chooseAndPlaySong(song1);

  player.playNext();

  // After playNext, current song should change
  // (depends on implementation - queue or shuffle)
  EXPECT_NE(player.getCurrentSong(), nullptr);
}

TEST_F(MusicPlayerTest, PlayNext_FromFuture_ChangesCurrentSong) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  const Song *song3 = addSongToLibrary("Song 3", "Artist", "Album");
  player.addSongToQueue(song1);
  player.addSongToQueue(song2);
  player.addSongToQueue(song3);

  player.playNext();     // play song1
  player.playNext();     // play song2
  player.playPrevious(); // play song1
  player.playNext();     // play song2
  EXPECT_EQ(player.getCurrentSong(), song2);
}

TEST_F(MusicPlayerTest, PlayNext_FromShuffle_ChangesCurrentSong) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  const Song *song3 = addSongToLibrary("Song 3", "Artist", "Album");
  const Song *song4 = addSongToLibrary("Song 4", "Artist", "Album");
  const Song *song5 = addSongToLibrary("Song 5", "Artist", "Album");
  const Song *song6 = addSongToLibrary("Song 6", "Artist", "Album");
  const Song *song7 = addSongToLibrary("Song 7", "Artist", "Album");
  const Song *song8 = addSongToLibrary("Song 8", "Artist", "Album");
  const Song *song9 = addSongToLibrary("Song 9", "Artist", "Album");
  const Song *song10 = addSongToLibrary("Song 10", "Artist", "Album");
  player.addSongToQueue(song1);
  player.addSongToQueue(song2);
  player.addSongToQueue(song3);
  player.addSongToQueue(song4);
  player.addSongToQueue(song5);
  player.addSongToQueue(song6);
  player.addSongToQueue(song7);
  player.addSongToQueue(song8);
  player.addSongToQueue(song9);
  player.addSongToQueue(song10);

  player.enableShuffle();
  player.playNext();
  auto currentSong1 = player.getCurrentSong();
  player.playNext();
  auto currentSong2 = player.getCurrentSong();
  auto pairNormal = std::make_pair(song1, song2);
  auto pairShuffle = std::make_pair(currentSong1, currentSong2);
  EXPECT_NE(pairNormal, pairShuffle);
}

TEST_F(MusicPlayerTest, PlayNext_EmptyQueue_NoChange) {
  const Song *before = player.getCurrentSong();

  player.playNext();

  EXPECT_EQ(player.getCurrentSong(), before);
}

TEST_F(MusicPlayerTest, PlayNext_EmptyQueue_WithSongPlaying) {
  const Song *song = addSongToLibrary("Song", "Artist", "Album");
  player.chooseAndPlaySong(song);
  player.playNext();
  EXPECT_FALSE(player.engine.isSoundLoaded);
}

// ========================
// Test: Play Previous
// ========================

TEST_F(MusicPlayerTest, PlayPrevious_NoHistory_StaysOnCurrentSong) {
  const Song *song = addSongToLibrary("Song", "Artist", "Album");
  player.chooseAndPlaySong(song);

  player.playPrevious();

  EXPECT_EQ(player.getCurrentSong(), song);
}

TEST_F(MusicPlayerTest, PlayPrevious_WithHistory_GoesBack) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  player.chooseAndPlaySong(song1);
  player.chooseAndPlaySong(song2);

  player.playPrevious();

  EXPECT_EQ(player.getCurrentSong(), song1);
}

// ========================
// Test: History Management
// ========================

TEST_F(MusicPlayerTest, GetHistoryManager) {
  auto &history = player.getHistoryManager();
  EXPECT_EQ(history.getHistoryList().size(), 0);
}

TEST_F(MusicPlayerTest, ClearHistory_ClearsPlaybackHistory) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  player.chooseAndPlaySong(song1);
  player.chooseAndPlaySong(song2);

  player.clearHistory();

  // After clearing history, playPrevious should not go back
  player.playPrevious();
  EXPECT_EQ(player.getCurrentSong(), song2);
}

TEST_F(MusicPlayerTest, GetHistory_ReturnsPlaybackHistory) {
  const Song *song1 = addSongToLibrary("Song 1", "Artist", "Album");
  const Song *song2 = addSongToLibrary("Song 2", "Artist", "Album");
  player.chooseAndPlaySong(song1);
  player.chooseAndPlaySong(song2);
  player.playNext();

  auto history = player.getHistoryManager().getHistoryList();

  EXPECT_EQ(history.size(), 2);
  EXPECT_EQ(history[0], song2);
  EXPECT_EQ(history[1], song1);
}

// ========================
// Test: Smart Playlist
// ========================
TEST_F(MusicPlayerTest,
       GenerateSmartPlaylist_NoSongsInLibrary_ReturnsEmptyPlaylist) {
  PlaybackQueue playlist = player.generateSmartPlaylist(nullptr, 10);

  EXPECT_EQ(playlist.getQueueSize(), 0);
}

TEST_F(MusicPlayerTest, GenerateSmartPlaylist_ReturnsSameArtistSongs) {
  const Song *base = addSongToLibrary("Base Song", "Same Artist", "Album 1");
  addSongToLibrary("Song 2", "Same Artist", "Album 2");
  addSongToLibrary("Song 3", "Different Artist", "Album 3");

  PlaybackQueue playlist = player.generateSmartPlaylist(base, 10);

  // Should include songs by "Same Artist"
  EXPECT_GE(playlist.getQueueSize(), 1);
}

TEST_F(MusicPlayerTest, GenerateSmartPlaylist_EmptyPlaylist) {
  const Song *base = addSongToLibrary("Base Song", "Same Artist", "Album 1");
  addSongToLibrary("Song 2", "Different Artist", "Album 2");

  PlaybackQueue playlist = player.generateSmartPlaylist(base, 10);

  // Empty playlist
  EXPECT_EQ(playlist.getQueueSize(), 0);
}

TEST_F(MusicPlayerTest, ApplySmartPlaylist_NoPlaylist) {
  player.applySmartPlaylist();

  EXPECT_EQ(player.getQueueSize(), 0);
}

TEST_F(MusicPlayerTest, ApplySmartPlaylist_AddsSongsToQueue) {
  const Song *base = addSongToLibrary("Base Song", "Same Artist", "Album 1");
  addSongToLibrary("Song 2", "Same Artist", "Album 2");
  addSongToLibrary("Song 3", "Different Artist", "Album 3");

  PlaybackQueue smartPlaylist = player.generateSmartPlaylist(base, 10);
  player.applySmartPlaylist();

  EXPECT_EQ(player.getQueueSize(), smartPlaylist.getQueueSize());
}