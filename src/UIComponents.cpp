// UIComponents.cpp - UI Rendering Implementation
#include "../include/UIComponents.h"
#include "../include/Theme.h"
#include "imgui.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
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
void RenderSongItem(const Song *song, bool spacious) {
  ImGui::PushID(song);

  if (spacious) {
    ImGui::BeginGroup();
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", song->title.c_str());
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1), "%s - %s",
                       song->artist.c_str(), song->album.c_str());
    ImGui::EndGroup();
  } else {
    ImGui::Text("%s - %s", song->title.c_str(), song->artist.c_str());
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
    ImGui::EndPopup();
  }

  if (spacious)
    ImGui::Separator();
  ImGui::PopID();
}

// --- RENDER ALBUMS TAB ---
void RenderAlbumsTab() {
  std::map<std::string, std::vector<const Song *>> albums;
  for (const auto &song : player.getLibrary().getAllSongs()) {
    albums[song.album].push_back(&song);
  }

  for (auto const &[name, songs] : albums) {
    if (ImGui::TreeNode(name.c_str())) {
      ImGui::SameLine();
      if (ImGui::SmallButton("Play Album")) {
        player.addAlbumToQueue(name);
      }
      for (const auto *s : songs) {
        RenderSongItem(s, false);
      }
      ImGui::TreePop();
    }
  }
}

// --- RENDER SMART PLAYLIST BUTTON ---
void RenderSmartPlaylistButton(const Song *current) {
  static int playlistSize = 20;

  if (ImGui::Button("Generate Smart Playlist")) {
    ImGui::OpenPopup("SmartPlaylistPopup");
  }

  if (ImGui::BeginPopup("SmartPlaylistPopup")) {
    ImGui::Text("Generate based on: %s", current->artist.c_str());
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
    ImGui::Begin("##Toast", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove);
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
    ImGui::Text("All Songs (%zu)", player.getLibrary().getSize());
    ImGui::Separator();
    ImGuiListClipper clipper;
    const auto &all = player.getLibrary().getAllSongs();
    clipper.Begin(all.size());
    while (clipper.Step()) {
      for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
        RenderSongItem(&all[i]);
      }
    }
  } else if (currentTab == TAB_ALBUMS) {
    RenderAlbumsTab();
  } else if (currentTab == TAB_ARTISTS) {
    std::map<std::string, std::vector<const Song *>> artists;
    for (const auto &song : player.getLibrary().getAllSongs()) {
      artists[song.artist].push_back(&song);
    }
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
          showToast("Added " + std::to_string(songs.size()) + " songs");
        }
        ImGui::PopID();
        ImGui::Spacing();
        for (const auto *s : songs) {
          RenderSongItem(s, false);
        }
        ImGui::Unindent(20.0f);
      }
    }
  } else if (currentTab == TAB_SEARCH) {
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
      ImGui::TextDisabled("Songs:");
      for (const auto &song : player.getLibrary().getAllSongs()) {
        if (containsString(song.title, query) ||
            containsString(song.artist, query) ||
            containsString(song.album, query)) {
          RenderSongItem(&song);
        }
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
          player.addAlbumToQueue(albumName);
          showToast("Added album: " + albumName);
        }
        ImGui::PopID();
      }
    } else {
      ImGui::TextDisabled("Type to search...");
    }
  } else if (currentTab == TAB_QUEUE) {
    ImGui::Text("Upcoming Queue");
    if (ImGui::SmallButton("Clear Queue")) {
      player.clearQueue();
      showToast("Queue cleared");
    }
    ImGui::Separator();
    auto forwardSize = player.getHistoryManager().getForwardSize();
    if (forwardSize > 0) {
      ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.4f, 1.0f),
                         "Next up (from history):");
      ImGui::TextDisabled("  %zu songs in forward queue", forwardSize);
      ImGui::Separator();
    }
    if (player.isShuffleEnabled()) {
      ImGui::TextColored(ImVec4(0.6f, 0.4f, 0.8f, 1.0f), "Shuffle Mode ON");
      const auto &shuffleList = player.getShuffleManager().getShuffleList();
      size_t currentIdx = player.getShuffleManager().getCurrentIndex();
      for (size_t i = currentIdx + 1; i < shuffleList.size(); i++) {
        RenderSongItem(shuffleList[i], false);
      }
      if (currentIdx + 1 >= shuffleList.size()) {
        ImGui::TextDisabled("Last song - will reshuffle on next");
      }
    } else {
      for (const Song *s : player.getQueueManager().getQueueList()) {
        RenderSongItem(s, false);
      }
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

  // Now Playing info (centered)
  if (current) {
    std::string nowPlaying = current->title + " - " + current->artist;
    float textWidth = ImGui::CalcTextSize(nowPlaying.c_str()).x;
    ImGui::SetCursorPosX((barWidth - textWidth) / 2);
    ImGui::Text("%s", nowPlaying.c_str());
  } else {
    const char *noSong = "No song playing";
    float textWidth = ImGui::CalcTextSize(noSong).x;
    ImGui::SetCursorPosX((barWidth - textWidth) / 2);
    ImGui::TextDisabled("%s", noSong);
  }

  ImGui::Spacing();

  // Playback controls (centered)
  float buttonWidth = 40.0f;
  float shuffleWidth = 100.0f;
  float smartPlaylistWidth = current ? 180.0f : 0.0f;
  float totalControlsWidth =
      buttonWidth * 3 + shuffleWidth + smartPlaylistWidth + 40.0f; // spacing
  ImGui::SetCursorPosX((barWidth - totalControlsWidth) / 2);

  if (ImGui::Button("|<", ImVec2(buttonWidth, 0)))
    player.playPrevious();
  ImGui::SameLine();
  if (ImGui::Button(player.engine.isPlaying() ? "||" : ">",
                    ImVec2(buttonWidth, 0))) {
    player.engine.togglePause();
  }
  ImGui::SameLine();
  if (ImGui::Button(">|", ImVec2(buttonWidth, 0)))
    player.playNext();
  ImGui::SameLine();

  if (player.isShuffleEnabled()) {
    if (ImGui::Button("Shuffle: ON", ImVec2(shuffleWidth, 0)))
      player.disableShuffle();
  } else {
    if (ImGui::Button("Shuffle: OFF", ImVec2(shuffleWidth, 0)))
      player.enableShuffle();
  }

  if (current) {
    ImGui::SameLine();
    RenderSmartPlaylistButton(current);
  }

  ImGui::Spacing();

  // Progress bar (full width with time labels)
  ImGui::Text("%s", formatTime(time).c_str());
  ImGui::SameLine();

  ImGui::PushItemWidth(barWidth - 150);
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
  ImGui::Text("%s", formatTime(duration).c_str());

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
