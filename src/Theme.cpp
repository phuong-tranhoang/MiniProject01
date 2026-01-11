// Theme.cpp - UI Theme Implementation
#include "../include/Theme.h"

void SetupTheme() {
  ImGuiStyle &style = ImGui::GetStyle();

  // Rounded corners and padding
  style.WindowRounding = 8.0f;
  style.FrameRounding = 6.0f;
  style.PopupRounding = 6.0f;
  style.ScrollbarRounding = 6.0f;
  style.GrabRounding = 6.0f;
  style.TabRounding = 6.0f;
  style.ChildRounding = 6.0f;

  style.WindowPadding = ImVec2(12, 12);
  style.FramePadding = ImVec2(8, 4);
  style.ItemSpacing = ImVec2(8, 6);

  // Purple Glassmorphism Colors
  ImVec4 purple = ImVec4(0.61f, 0.35f, 0.71f, 1.0f);      // #9B59B6
  ImVec4 purpleLight = ImVec4(0.71f, 0.45f, 0.81f, 1.0f); // Lighter purple

  // Backgrounds with transparency (glassmorphism)
  style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.94f);
  style.Colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.10f, 0.16f, 0.7f);
  style.Colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.14f, 0.95f);

  // Headers
  style.Colors[ImGuiCol_Header] = ImVec4(0.45f, 0.25f, 0.55f, 0.5f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.35f, 0.65f, 0.7f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.61f, 0.35f, 0.71f, 0.9f);

  // Buttons with hover animations
  style.Colors[ImGuiCol_Button] = ImVec4(0.45f, 0.25f, 0.55f, 0.6f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.61f, 0.35f, 0.71f, 0.85f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.71f, 0.45f, 0.81f, 1.0f);

  // Frame backgrounds
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.12f, 0.20f, 0.6f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.15f, 0.28f, 0.8f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.18f, 0.35f, 0.9f);

  // Slider / Progress bar
  style.Colors[ImGuiCol_SliderGrab] = purple;
  style.Colors[ImGuiCol_SliderGrabActive] = purpleLight;

  // Scrollbar
  style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.08f, 0.08f, 0.12f, 0.5f);
  style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.25f, 0.45f, 0.6f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered] =
      ImVec4(0.45f, 0.30f, 0.55f, 0.8f);
  style.Colors[ImGuiCol_ScrollbarGrabActive] = purple;

  // Separators and borders
  style.Colors[ImGuiCol_Separator] = ImVec4(0.45f, 0.35f, 0.55f, 0.3f);
  style.Colors[ImGuiCol_Border] = ImVec4(0.45f, 0.35f, 0.55f, 0.2f);

  // Text
  style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.93f, 0.98f, 1.0f);
  style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.55f, 0.65f, 1.0f);

  // Tab styling
  style.Colors[ImGuiCol_Tab] = ImVec4(0.25f, 0.20f, 0.32f, 0.8f);
  style.Colors[ImGuiCol_TabHovered] = ImVec4(0.55f, 0.35f, 0.65f, 0.8f);
  style.Colors[ImGuiCol_TabActive] = purple;

  // Title bar
  style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.08f, 0.14f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.12f, 0.20f, 1.0f);
}
