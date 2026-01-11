#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>

#include "../include/MusicPlayer.h"

// --- HELPER: Time Formatter (MM:SS) ---
std::string formatTime(float seconds) {
    int m = (int)seconds / 60;
    int s = (int)seconds % 60;
    std::ostringstream oss;
    oss << m << ":" << std::setw(2) << std::setfill('0') << s;
    return oss.str();
}

// --- HELPER: Case Insensitive Search ---
bool containsString(const std::string& haystack, const std::string& needle) {
    auto it = std::search(
        haystack.begin(), haystack.end(),
        needle.begin(), needle.end(),
        [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
    );
    return (it != haystack.end());
}

// --- GLOBAL STATE ---
enum Tab { TAB_LIBRARY, TAB_ALBUMS, TAB_ARTISTS, TAB_SEARCH, TAB_QUEUE, TAB_HISTORY };
Tab currentTab = TAB_LIBRARY;
MusicPlayer player;
char searchBuf[128] = "";

// --- THEME SETUP (Soft Dark) ---
void SetupTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.0f); // Soft Dark
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.15f, 0.18f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.25f, 0.3f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.35f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.3f, 0.45f, 1.0f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
}

// --- RENDER FUNCTIONS ---

void RenderSongItem(const Song* song, bool spacious = true) {
    ImGui::PushID(song); // Unique ID for buttons
    
    // Spacious Look: Group with padding
    if (spacious) {
        ImGui::BeginGroup();
        ImGui::TextColored(ImVec4(1,1,1,1), "%s", song->title.c_str());
        ImGui::TextColored(ImVec4(0.6f,0.6f,0.6f,1), "%s - %s", song->artist.c_str(), song->album.c_str());
        ImGui::EndGroup();
    } else {
        ImGui::Text("%s - %s", song->title.c_str(), song->artist.c_str());
    }

    // Interaction
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Double-click to play");
    }
    
    // Logic: Double Click to Play
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
        player.chooseAndPlaySong(song);
    }

    // Logic: Right Click Context Menu
    if (ImGui::BeginPopupContextItem()) {
        if (ImGui::MenuItem("Play Now")) { player.chooseAndPlaySong(song); }
        if (ImGui::MenuItem("Add to Queue")) { player.addSongToQueue(song); }
        if (ImGui::MenuItem("Show Artist")) { 
            strcpy(searchBuf, song->artist.c_str());
            currentTab = TAB_SEARCH; 
        }
        ImGui::EndPopup();
    }
    
    if (spacious) ImGui::Separator();
    ImGui::PopID();
}

void RenderAlbumsTab() {
    // Generate unique albums list (Simple approach)
    // Note: Ideally, this should be cached in MusicLibrary, but we do it here for UI flexibility
    std::map<std::string, std::vector<const Song*>> albums;
    for (const auto& song : player.getLibrary().getAllSongs()) {
        albums[song.album].push_back(&song);
    }

    for (auto const& [name, songs] : albums) {
        if (ImGui::TreeNode(name.c_str())) {
            ImGui::SameLine();
            if (ImGui::SmallButton("Play Album")) {
                player.addAlbumToQueue(name);
            }
            for (const auto* s : songs) {
                RenderSongItem(s, false);
            }
            ImGui::TreePop();
        }
    }
}

void RenderSmartPlaylistButton(const Song* current) {
    if (ImGui::Button("Generate Smart Playlist")) {
        ImGui::OpenPopup("SmartPlaylistPopup");
    }

    if (ImGui::BeginPopup("SmartPlaylistPopup")) {
        ImGui::Text("Generate based on: %s", current->artist.c_str());
        if (ImGui::Button("Create & Replace Queue")) {
            // Your logic here
            player.generateSmartPlaylist(current, 20); // Generate
            player.applySmartPlaylist(); // Apply
            currentTab = TAB_QUEUE; // Switch view to show result
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

int main(int, char**) {
    if (!glfwInit()) return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 800, "Automotive Player", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    SetupTheme();

    // Default Load (Optional)
    // player.loadLibrary("C:/Music"); 

    static char pathBuffer[256] = "C:/";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // FULLSCREEN DOCKING
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("Root", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

        // --- LEFT SIDEBAR (Navigation) ---
        ImGui::BeginChild("Sidebar", ImVec2(250, 0), true);
        
        ImGui::TextDisabled("LIBRARY");
        if (ImGui::Selectable("  All Songs", currentTab == TAB_LIBRARY)) currentTab = TAB_LIBRARY;
        if (ImGui::Selectable("  Albums", currentTab == TAB_ALBUMS)) currentTab = TAB_ALBUMS;
        if (ImGui::Selectable("  Artists", currentTab == TAB_ARTISTS)) currentTab = TAB_ARTISTS;
        
        ImGui::Separator();
        ImGui::TextDisabled("PLAYLISTS");
        if (ImGui::Selectable("  Upcoming Queue", currentTab == TAB_QUEUE)) currentTab = TAB_QUEUE;
        if (ImGui::Selectable("  History", currentTab == TAB_HISTORY)) currentTab = TAB_HISTORY;
        
        ImGui::Separator();
        ImGui::TextDisabled("TOOLS");
        if (ImGui::Selectable("  Search", currentTab == TAB_SEARCH)) currentTab = TAB_SEARCH;

        ImGui::Dummy(ImVec2(0, 20));
        ImGui::Text("Import:");
        ImGui::InputText("##Path", pathBuffer, 256);
        if (ImGui::Button("Load Folder", ImVec2(-1, 0))) {
            player.loadLibrary(pathBuffer);
        }

        ImGui::EndChild();
        ImGui::SameLine();

        // --- RIGHT GROUP (Main Content + Bottom Bar) ---
        ImGui::BeginGroup();

        // --- MAIN CONTENT AREA ---
        ImGui::BeginChild("MainView", ImVec2(0, -90), true); // Leave 90px for bottom bar

        if (currentTab == TAB_LIBRARY) {
            ImGui::Text("All Songs (%zu)", player.getLibrary().getSize());
            ImGui::Separator();
            // Use Clipper for performance with large lists
            ImGuiListClipper clipper;
            const auto& all = player.getLibrary().getAllSongs();
            clipper.Begin(all.size());
            while (clipper.Step()) {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                    RenderSongItem(&all[i]);
                }
            }
        }
        else if (currentTab == TAB_ALBUMS) {
            RenderAlbumsTab();
        }
        else if (currentTab == TAB_SEARCH) {
            ImGui::InputTextWithHint("##SearchBox", "Search Title, ID, Artist...", searchBuf, 128);
            ImGui::Separator();
            
            std::string query(searchBuf);
            if (query.length() > 0) {
                for (const auto& song : player.getLibrary().getAllSongs()) {
                    if (containsString(song.title, query) || containsString(song.artist, query)) {
                        RenderSongItem(&song);
                    }
                }
            } else {
                ImGui::TextDisabled("Type to search...");
            }
        }
        else if (currentTab == TAB_QUEUE) {
            ImGui::Text("Upcoming Queue");
            if (ImGui::SmallButton("Clear Queue")) player.clearQueue();
            ImGui::Separator();
            

            // Note: PlaybackQueue needs getQueueList() to return std::list
            for (const Song* s : player.getQueueManager().getQueueList()) { 
                RenderSongItem(s, false);
            }
        }
        else if (currentTab == TAB_HISTORY) {
            ImGui::Text("History (Most Recent Top)");
            ImGui::Separator();
            // Assuming you added getHistoryList() to PlaybackHistory
            // std::vector<const Song*> hist = player.getHistoryManager().getHistoryList();
            // for (const auto* s : hist) RenderSongItem(s, false);
        }

        ImGui::EndChild();

        // --- BOTTOM BAR (Player Controls) ---
        ImGui::BeginChild("BottomBar", ImVec2(0, 0), true);
        
        const Song* current = player.getCurrentSong();
        float time = player.engine.getCurrentTime();
        float duration = player.engine.getTotalDuration();

        // Left: Song Info
        ImGui::BeginGroup();
        if (current) {
            ImGui::Text("%s", current->title.c_str());
            ImGui::TextDisabled("%s", current->artist.c_str());
        } else {
            ImGui::Text("Not Playing");
        }
        ImGui::EndGroup();

        ImGui::SameLine();
        
        // Middle: Controls & Progress
        float avail = ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 150); // Rough center
        
        if (ImGui::Button("<<")) player.playPrevious();
        ImGui::SameLine();
        if (ImGui::Button(player.engine.isPlaying() ? " || " : " > ")) player.engine.togglePause();
        ImGui::SameLine();
        if (ImGui::Button(">>")) player.playNext();
        
        // Smart Playlist Button (Small)
        if (current) {
            ImGui::SameLine();
            RenderSmartPlaylistButton(current);
        }

        // Progress Bar
        ImGui::NewLine();
        ImGui::Text("%s", formatTime(time).c_str());
        ImGui::SameLine();
        
        ImGui::PushItemWidth(ImGui::GetWindowWidth() - 200);
        float seekVal = time;
        if (ImGui::SliderFloat("##Seek", &seekVal, 0.0f, duration, "")) {
            // Dragging...
        }
        if (ImGui::IsItemDeactivated()) {
            player.engine.seekTo(seekVal);
        }
        ImGui::PopItemWidth();
        
        ImGui::SameLine();
        ImGui::Text("%s", formatTime(duration).c_str());

        ImGui::EndChild();
        ImGui::EndGroup(); // End Right Group

        ImGui::End(); // End Root

        // Logic Loop
        if (current && player.engine.isFinished()) {
            player.playNext();
        }

        ImGui::Render();
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}