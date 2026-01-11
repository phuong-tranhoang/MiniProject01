// UIComponents.h - UI Rendering Components
#ifndef UI_COMPONENTS_H
#define UI_COMPONENTS_H

#include "MusicPlayer.h"
#include "imgui.h"
#include <string>


// Tab enum for navigation
enum Tab {
  TAB_LIBRARY,
  TAB_ALBUMS,
  TAB_ARTISTS,
  TAB_SEARCH,
  TAB_QUEUE,
  TAB_HISTORY
};

// External state (defined in UIComponents.cpp)
extern Tab currentTab;
extern MusicPlayer player;
extern char searchBuf[128];
extern std::string toastMessage;
extern float toastTimer;

// Toast system
void showToast(const std::string &message);

// Helper functions
std::string formatTime(float seconds);
bool containsString(const std::string &haystack, const std::string &needle);

// Render functions
void RenderSongItem(const Song *song, bool showAlbum = true);
void RenderAlbumsTab();
void RenderSmartPlaylistButton(const Song *current);
void RenderToast();

// Main render function for the entire UI
void RenderMainUI(char *pathBuffer);

#endif
