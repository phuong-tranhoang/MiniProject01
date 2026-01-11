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

  // Load font
  ImGuiIO &io = ImGui::GetIO();
  ImFont *font =
      io.Fonts->AddFontFromFileTTF("assets/SpaceGrotesk-Regular.ttf", 16.0f);
  if (!font)
    font = io.Fonts->AddFontFromFileTTF("../assets/SpaceGrotesk-Regular.ttf",
                                        16.0f);
  if (!font) {
    ImFontConfig cfg;
    cfg.SizePixels = 16.0f;
    io.Fonts->AddFontDefault(&cfg);
  }

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