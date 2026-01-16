// main.cpp - Application Entry Point
// PhuongTH33 Music Player

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "../include/Theme.h"
#include "../include/UIComponents.h"

int main(int, char **) {
  // Initialize GLFW
  if (!glfwInit())
    return 1;

  GLFWwindow *window =
      glfwCreateWindow(1280, 800, "PhuongTH33 Player", NULL, NULL);
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  // Initialize ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");

  // Apply theme
  SetupTheme();

  // Load font with Vietnamese glyph range
  ImGuiIO &io = ImGui::GetIO();

  // Vietnamese glyph ranges (Latin + Vietnamese)
  static const ImWchar vietnameseRanges[] = {
      0x0020, 0x00FF, // Basic Latin + Latin Supplement
      0x0102, 0x0103, // Ă ă
      0x0110, 0x0111, // Đ đ
      0x0128, 0x0129, // Ĩ ĩ
      0x0168, 0x0169, // Ũ ũ
      0x01A0, 0x01A1, // Ơ ơ
      0x01AF, 0x01B0, // Ư ư
      0x1EA0, 0x1EF9, // Vietnamese vowels with diacritics
      0,
  };

  ImFontConfig cfg;
  cfg.OversampleH = 2;
  cfg.OversampleV = 1;

  // Regular font (16px)
  ImFont *font = io.Fonts->AddFontFromFileTTF("assets/SpaceGrotesk-Regular.ttf",
                                              16.0f, &cfg, vietnameseRanges);
  if (!font)
    font = io.Fonts->AddFontFromFileTTF("../assets/SpaceGrotesk-Regular.ttf",
                                        16.0f, &cfg, vietnameseRanges);
  if (!font) {
    cfg.SizePixels = 16.0f;
    io.Fonts->AddFontDefault(&cfg);
  }

  // Large font (24px) for titles - global, declared in UIComponents.h
  ImFont *fontLarge = io.Fonts->AddFontFromFileTTF(
      "assets/SpaceGrotesk-Bold.ttf", 24.0f, &cfg, vietnameseRanges);
  if (!fontLarge)
    fontLarge = io.Fonts->AddFontFromFileTTF("../assets/SpaceGrotesk-Bold.ttf",
                                             24.0f, &cfg, vietnameseRanges);
  ImFont *fontPlayer = io.Fonts->AddFontFromFileTTF(
      "assets/Death Star.otf", 16.0f, &cfg, vietnameseRanges);
  if (!fontPlayer)
    fontPlayer = io.Fonts->AddFontFromFileTTF("../assets/Death Star.otf", 16.0f,
                                              &cfg, vietnameseRanges);
  // Path buffer for library loading
  static char pathBuffer[256] = "C:/";

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Render all UI
    RenderMainUI(pathBuffer);

    // OpenGL rendering
    ImGui::Render();
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}