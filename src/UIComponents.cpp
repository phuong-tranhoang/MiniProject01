// UIComponents.cpp - UI Rendering Implementation
#include "../include/UIComponents.h"
#include "../include/Theme.h"
#include "imgui.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

// --- GLOBAL STATE DEFINITIONS ---
Tab currentTab = TAB_LIBRARY;
MusicPlayer player;
char searchBuf[128] = "";
std::string toastMessage = "";
float toastTimer = 0.0f;
const float TOAST_DURATION = 2.0f;

// --- TOAST SYSTEM ---
void showToast(const std::string &message) {
  toastMessage = message;
  toastTimer = TOAST_DURATION;
  std::cout << "Toast: " << message << std::endl;
}

// --- HELPER: Time Formatter (MM:SS) ---
std::string formatTime(float seconds) {
  int m = (int)seconds / 60;
  int s = (int)seconds % 60;
  std::ostringstream oss;
  oss << m << ":" << std::setw(2) << std::setfill('0') << s;
  return oss.str();
}

// --- HELPER: Case Insensitive Search ---
bool containsString(const std::string &haystack, const std::string &needle) {
  auto it = std::search(haystack.begin(), haystack.end(), needle.begin(),
                        needle.end(), [](char ch1, char ch2) {
                          return std::toupper(ch1) == std::toupper(ch2);
                        });
  return (it != haystack.end());
}

// --- RENDER SONG ITEM ---
void RenderSongItem(const Song *song, bool spacious, bool inQueue) {
  ImGui::PushID(song);

  if (spacious) {
    ImGui::BeginGroup();
    if (song == player.getCurrentSong()) {
      ImGui::TextColored(ImVec4(0.6f, 0.4f, 0.8f, 1), "%s",
                         song->title.c_str());
    } else {
      ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", song->title.c_str());
    }
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "%s - %s",
                       song->artist.c_str(), song->album.c_str());
    ImGui::EndGroup();
  } else {
    if (song == player.getCurrentSong()) {
      ImGui::TextColored(ImVec4(0.6f, 0.4f, 0.8f, 1), "%s - %s",
                         song->title.c_str(), song->artist.c_str());
    } else {
      ImGui::Text("%s - %s", song->title.c_str(), song->artist.c_str());
    }
  }
  if (ImGui::IsItemHovered()) {
    ImGui::SetTooltip("Double-click to play");
  }

  if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
    player.chooseAndPlaySong(song);
  }

  std::string popupId =
      "##ctx" + std::to_string(reinterpret_cast<uintptr_t>(song));
  if (ImGui::BeginPopupContextItem(popupId.c_str())) {
    if (ImGui::MenuItem("Play Now")) {
      player.chooseAndPlaySong(song);
    }
    if (ImGui::MenuItem("Add to Queue")) {
      player.addSongToQueue(song);
    }
    if (ImGui::MenuItem("Show Artist")) {
      strcpy(searchBuf, song->artist.c_str());
      currentTab = TAB_SEARCH;
    }
    if (inQueue) {
      if (ImGui::MenuItem("Remove Song")) {
        player.removeSongFromQueue(song);
      }
    }
    ImGui::EndPopup();
  }

  if (spacious)
    ImGui::Separator();
  ImGui::PopID();
}

// --- RENDER ALBUMS TAB ---
void RenderAlbumsTab() {
  const auto &albums = player.getLibrary().getAlbumIndex();
  ImGui::Text("Albums (%zu)", albums.size());
  ImGui::Separator();
  for (const auto &[albumName, songs] : albums) {
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.55f, 0.30f, 0.65f, 0.6f));
    bool open = ImGui::CollapsingHeader(albumName.c_str());
    ImGui::PopStyleColor();
    if (open) {
      ImGui::Indent(20.0f);
      ImGui::TextDisabled("%zu songs", songs.size());
      ImGui::SameLine();
      ImGui::PushID(albumName.c_str());
      if (ImGui::SmallButton("Add All to Queue")) {
        auto size = player.addAlbumToQueue(albumName);
        showToast("Added " + std::to_string(size) + " songs from " +
                  albumName);
      }
      ImGui::PopID();
      ImGui::Spacing();
      ImGui::Separator();
      for (const auto *s : songs) {
        RenderSongItem(s, false);
        ImGui::Separator();
      }
      ImGui::Unindent(20.0f);
    }
  }
}
// --- RENDER ARTIST TAB ---
void RenderArtistTab() {
  const auto &artists = player.getLibrary().getArtistIndex();
  ImGui::Text("Artists (%zu)", artists.size());
  ImGui::Separator();
  for (const auto &[artistName, songs] : artists) {
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.55f, 0.30f, 0.65f, 0.6f));
    bool open = ImGui::CollapsingHeader(artistName.c_str());
    ImGui::PopStyleColor();
    if (open) {
      ImGui::Indent(20.0f);
      ImGui::TextDisabled("%zu songs", songs.size());
      ImGui::SameLine();
      ImGui::PushID(artistName.c_str());
      if (ImGui::SmallButton("Add All to Queue")) {
        player.addArtistToQueue(artistName);
        showToast("Added " + std::to_string(songs.size()) + " songs from " +
                  artistName);
      }
      ImGui::PopID();
      ImGui::Spacing();
      ImGui::Separator();
      for (const auto *s : songs) {
        RenderSongItem(s, false);
        ImGui::Separator();
      }
      ImGui::Unindent(20.0f);
    }
  }
}
// --- RENDER SEARCH TAB ---
void RenderSearchTab() {
  ImGui::InputTextWithHint(
      "##SearchBox", "Search Title, Artist, Album, or ID...", searchBuf, 128);
  ImGui::Separator();
  std::string query(searchBuf);
  if (query.length() > 0) {
    bool isNumeric = true;
    for (char c : query) {
      if (!std::isdigit(c)) {
        isNumeric = false;
        break;
      }
    }
    if (isNumeric) {
      int searchId = std::stoi(query);
      const Song *found = player.getLibrary().findSongByID(searchId);
      if (found) {
        ImGui::TextColored(ImVec4(0.6f, 0.8f, 0.6f, 1.0f), "Found by ID:");
        RenderSongItem(found);
      }
    }
    std::list<const Song *> matchedSongs;
    int matchedCount = 0;
    ;
    ImGui::TextDisabled("Songs:");
    for (const auto &song : player.getLibrary().getAllSongs()) {
      if (containsString(song.title, query) ||
          containsString(song.artist, query) ||
          containsString(song.album, query)) {
        matchedSongs.push_back(&song);
        matchedCount++;
      }
    }
    ImGui::SameLine();
    ImGui::Text("Found %zu related song(s)", matchedCount);
    ImGui::Separator();
    if (ImGui::SmallButton("Add All to Queue")) {
      for (auto &songs : matchedSongs) {
        player.addSongToQueue(songs);
      }
    }
    for (auto &songs : matchedSongs) {
      RenderSongItem(songs);
    }
    ImGui::Separator();
    ImGui::TextDisabled("Albums:");
    std::set<std::string> matchedAlbums;
    for (const auto &song : player.getLibrary().getAllSongs()) {
      if (containsString(song.album, query)) {
        matchedAlbums.insert(song.album);
      }
    }
    for (const auto &albumName : matchedAlbums) {
      ImGui::PushID(albumName.c_str());
      ImGui::Text("%s", albumName.c_str());
      ImGui::SameLine();
      if (ImGui::SmallButton("Add Album to Queue")) {
        auto size = player.addAlbumToQueue(albumName);
        showToast(
            "Added album: " + albumName + " with " + std::to_string(size) + " songs");
      }
      ImGui::PopID();
    }
  } else {
    ImGui::TextDisabled("Type to search...");
  }
}

// --- RENDER SMART PLAYLIST BUTTON ---
void RenderSmartPlaylistButton(const Song *current) {
  static int playlistSize = 20;

  if (ImGui::Button("Smart Playlist", ImVec2(110.0f, 0))) {
    ImGui::OpenPopup("SmartPlaylistPopup");
  }

  if (ImGui::BeginPopup("SmartPlaylistPopup")) {
    ImGui::Text("Generate based on album: %s and artist: %s",
                current->album.c_str(), current->artist.c_str());
    ImGui::Separator();
    ImGui::SliderInt("Songs", &playlistSize, 5, 100);
    ImGui::Spacing();
    if (ImGui::Button("Create & Replace Queue")) {
      player.generateSmartPlaylist(current, playlistSize);
      player.applySmartPlaylist();
      currentTab = TAB_QUEUE;
      showToast("Smart playlist: " + std::to_string(playlistSize) + " songs");
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }
}

// --- RENDER TOAST ---
void RenderToast() {
  if (toastTimer > 0.0f) {
    toastTimer -= ImGui::GetIO().DeltaTime;
    float alpha = std::min(1.0f, toastTimer / 0.5f);

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, 50),
                            ImGuiCond_Always, ImVec2(0.5f, 0.0f));
    ImGui::SetNextWindowBgAlpha(0.8f * alpha);
    ImGui::SetNextWindowFocus(); // Bring to front
    ImGui::Begin(
        "##Toast", nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoInputs); // No inputs prevents focus issues
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, alpha), "%s",
                       toastMessage.c_str());
    ImGui::End();
  }
}

// --- RENDER MAIN UI ---
void RenderMainUI(char *pathBuffer) {
  // FULLSCREEN DOCKING
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
  ImGui::Begin("Root", nullptr,
               ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

  // --- LEFT SIDEBAR ---
  ImGui::BeginChild("Sidebar", ImVec2(250, 0), true);

  ImGui::TextDisabled("LIBRARY");
  if (ImGui::Selectable("  All Songs", currentTab == TAB_LIBRARY))
    currentTab = TAB_LIBRARY;
  if (ImGui::Selectable("  Albums", currentTab == TAB_ALBUMS))
    currentTab = TAB_ALBUMS;
  if (ImGui::Selectable("  Artists", currentTab == TAB_ARTISTS))
    currentTab = TAB_ARTISTS;

  ImGui::Separator();
  ImGui::TextDisabled("PLAYLISTS");
  if (ImGui::Selectable("  Upcoming Queue", currentTab == TAB_QUEUE))
    currentTab = TAB_QUEUE;
  if (ImGui::Selectable("  History", currentTab == TAB_HISTORY))
    currentTab = TAB_HISTORY;

  ImGui::Separator();
  ImGui::TextDisabled("TOOLS");
  if (ImGui::Selectable("  Search", currentTab == TAB_SEARCH))
    currentTab = TAB_SEARCH;

  ImGui::Dummy(ImVec2(0, 20));
  ImGui::Text("Import:");
  ImGui::InputText("##Path", pathBuffer, 256);
  if (ImGui::Button("Load Folder", ImVec2(-1, 0))) {
    player.loadLibrary(pathBuffer);
    showToast("Library loaded");
  }
  if (ImGui::Button("Unload Library", ImVec2(-1, 0))) {
    player.clearLibrary();
    showToast("Library unloaded");
  }

  ImGui::EndChild();
  ImGui::SameLine();

  // --- RIGHT GROUP ---
  ImGui::BeginGroup();

  // --- MAIN CONTENT ---
  ImGui::BeginChild("MainView", ImVec2(0, -140), true);

  if (currentTab == TAB_LIBRARY) {
    static bool sortAZ = false;
    ImGui::Text("All Songs (%zu)", player.getLibrary().getSize());
    ImGui::SameLine();
    if (ImGui::SmallButton(sortAZ ? "SORT: A-Z" : "SORT: OFF")) {
      sortAZ = !sortAZ;
    }
    ImGui::Separator();
    ImGuiListClipper clipper;
    if (sortAZ) {
      auto sorted = player.getLibrary().getSortedSongs();
      clipper.Begin(sorted.size());
      while (clipper.Step()) {
        ImGui::Indent(20.0f);
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
          ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "%zu", i + 1);
          ImGui::SameLine();
          ImGui::SetCursorPosX(55.0f);
          ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "|");
          ImGui::SameLine();
          RenderSongItem(sorted[i]);
        }
        ImGui::Unindent();
      }
    } else {
      const auto &all = player.getLibrary().getAllSongs();
      clipper.Begin(all.size());
      while (clipper.Step()) {
        ImGui::Indent(20.0f);
        for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
          ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "%zu", i + 1);
          ImGui::SameLine();
          ImGui::SetCursorPosX(55.0f);
          ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "|");
          ImGui::SameLine();
          RenderSongItem(&all[i]);
        }
        ImGui::Unindent();
      }
    }
  } else if (currentTab == TAB_ALBUMS) {
    RenderAlbumsTab();
  } else if (currentTab == TAB_ARTISTS) {
    RenderArtistTab();
  } else if (currentTab == TAB_SEARCH) {
    RenderSearchTab();
  } else if (currentTab == TAB_QUEUE) {
    const Song *current = player.getCurrentSong();
    ImGui::Text("Upcoming Queue");
    if (current) {
      std::string nowPlaying = current->title + " - " + current->artist;
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(0.6f, 0.4f, 0.8f, 1), "   (Now Playing: %s)",
                         nowPlaying.c_str());
    }
    if (ImGui::SmallButton("Clear Queue")) {
      player.clearQueue();
      showToast("Queue cleared");
    }
    ImGui::Separator();
    auto forwardSize = player.getHistoryManager().getForwardSize();
    auto forwardList = player.getHistoryManager().getForwardList();
    if (forwardSize > 0) {
      ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.4f, 1.0f),
                         "Next up (from history):");
      ImGui::Indent(20.0f);
      for (size_t i = 1; i <= forwardSize; i++) {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "%zu", i);
        ImGui::SameLine();
        ImGui::SetCursorPosX(55.0f);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "|");
        ImGui::SameLine();
        RenderSongItem(forwardList[forwardSize - i], false);
      }
      ImGui::Unindent();
      ImGui::Separator();
    }
    if (player.isShuffleEnabled()) {
      ImGui::TextColored(ImVec4(0.6f, 0.4f, 0.8f, 1.0f), "Shuffle Mode ON");
      const auto &shuffleList = player.getShuffleManager().getShuffleList();
      size_t currentIdx = player.getShuffleManager().getCurrentIndex();
      ImGui::Indent(20.0f);
      for (size_t i = currentIdx + 1; i < shuffleList.size(); i++) {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "%zu",
                           forwardSize + (i - currentIdx));
        ImGui::SameLine();
        ImGui::SetCursorPosX(55.0f);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "|");
        ImGui::SameLine();
        RenderSongItem(shuffleList[i], false);
      }
      ImGui::Unindent();
      if (currentIdx + 1 >= shuffleList.size()) {
        ImGui::TextDisabled("Last song - will reshuffle on next");
      }
    } else {
      int idx = forwardSize;
      ImGui::Indent(20.0f);
      for (const Song *s : player.getQueueManager().getQueueList()) {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "%zu", ++idx);
        ImGui::SameLine();
        ImGui::SetCursorPosX(55.0f);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "|");
        ImGui::SameLine();
        RenderSongItem(s, false, true);
      }
      ImGui::Unindent();
    }
    if (player.getQueueManager().getQueueSize() == 0 &&
        !player.isShuffleEnabled()) {
      ImGui::TextDisabled("Queue is empty. Add songs to play!");
    }
  } else if (currentTab == TAB_HISTORY) {
    ImGui::Text("History (Most Recent Top)");
    ImGui::SameLine();
    if (ImGui::SmallButton("Clear History")) {
      player.clearHistory();
      showToast("History cleared");
    }
    ImGui::Separator();
    std::vector<const Song *> hist =
        player.getHistoryManager().getHistoryList();
    if (hist.empty()) {
      ImGui::TextDisabled("No songs played yet.");
    } else {
      for (const auto *s : hist) {
        RenderSongItem(s, false);
      }
    }
  }

  ImGui::EndChild();

  // --- BOTTOM BAR (height 140px, no scroll) ---
  ImGui::BeginChild("BottomBar", ImVec2(0, 0), true,
                    ImGuiWindowFlags_NoScrollbar);
  const Song *current = player.getCurrentSong();
  float time = player.engine.getCurrentTime();
  float duration = player.engine.getTotalDuration();
  float barWidth = ImGui::GetWindowWidth();

  // Now Playing info (centered, large font, fixed position)
  float titleY = 15.0f; // Fixed Y position for title
  ImGui::SetCursorPosY(titleY);

  // Use global fontLarge (defined in main.cpp, declared in UIComponents.h)
  if (current) {
    std::string nowPlaying = current->title + " - " + current->artist;
    auto fontLarge = ImGui::GetIO().Fonts->Fonts.Size > 1
                         ? ImGui::GetIO().Fonts->Fonts[1]
                         : nullptr;
    if (fontLarge)
      ImGui::PushFont(fontLarge);
    float textWidth = ImGui::CalcTextSize(nowPlaying.c_str()).x;
    ImGui::SetCursorPosX((barWidth - textWidth) / 2);
    ImGui::Text("%s", nowPlaying.c_str());
    if (fontLarge)
      ImGui::PopFont();
  } else {
    const char *noSong = "No song playing";
    float textWidth = ImGui::CalcTextSize(noSong).x;
    ImGui::SetCursorPosX((barWidth - textWidth) / 2);
    ImGui::TextDisabled("%s", noSong);
  }

  // Fixed position for controls (below title)
  ImGui::SetCursorPosY(titleY + 40.0f);

  // Playback controls (centered)
  float buttonWidth = 40.0f;
  float shuffleWidth = 100.0f;
  float smartPlaylistWidth = 100.0f;

  ImGui::SetCursorPosX((barWidth - buttonWidth * 3) / 2 - shuffleWidth - 10.0f -
                       60.0f);
  if (player.isShuffleEnabled()) {
    if (ImGui::Button("Shuffle: ON", ImVec2(shuffleWidth, 0)))
      player.disableShuffle();
  } else {
    if (ImGui::Button("Shuffle: OFF", ImVec2(shuffleWidth, 0)))
      player.enableShuffle();
  }
  ImGui::SameLine(0.0f, 60.0f);
  ImFont *fontPlayer = ImGui::GetIO().Fonts->Fonts.Size > 1
                           ? ImGui::GetIO().Fonts->Fonts[2]
                           : nullptr;
  ImGui::PushFont(fontPlayer);
  if (ImGui::Button("|<", ImVec2(buttonWidth, 0)))
    player.playPrevious();
  ImGui::SameLine();
  if (ImGui::Button(player.engine.isPlaying() ? "|  |" : ">",
                    ImVec2(buttonWidth, 0))) {
    player.engine.togglePause();
  }
  ImGui::SameLine();
  if (ImGui::Button(">|", ImVec2(buttonWidth, 0)))
    player.playNext();
  ImGui::SameLine();
  ImGui::PopFont();
  if (current) {
    ImGui::SameLine(0.0f, 60.0f);
    RenderSmartPlaylistButton(current);
  }

  ImGui::Spacing();

  // Progress bar (full width with time labels)
  ImGui::SetCursorPosY(titleY + 75.0f);
  ImGui::Text("    %s", formatTime(time).c_str());
  ImGui::SameLine();
  ImGui::SetCursorPosX(65.0f);
  ImGui::PushItemWidth(barWidth - 135.0f);
  static float seekVal = 0.0f;
  static bool isDragging = false;
  if (!isDragging)
    seekVal = time;
  if (ImGui::SliderFloat("##Seek", &seekVal, 0.0f, duration, "")) {
    isDragging = true;
  }
  if (ImGui::IsItemDeactivatedAfterEdit()) {
    player.engine.seekTo(seekVal);
    isDragging = false;
  }
  if (!ImGui::IsItemActive() && isDragging)
    isDragging = false;
  ImGui::PopItemWidth();

  ImGui::SameLine();
  ImGui::Text("   %s", formatTime(duration).c_str());

  ImGui::EndChild();
  ImGui::EndGroup();

  ImGui::End();

  // Toast
  RenderToast();

  // Auto-play next
  if (current && player.engine.isFinished()) {
    player.playNext();
  }
}
