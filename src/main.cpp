#include "glad/glad.h"
#include "renderer.hpp"
#include "ui.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

// ---------------------------------------------------------
// Main Entry Point
// ---------------------------------------------------------
int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(800, 600, "EngineV2", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0); // Disable VSync to uncap framerate

  // Initialize GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Initialize UI layer
  ui_init(window);

  // Initialize our flat data structure
  RenderState render_state;
  init_renderer(&render_state);

  float last_time = glfwGetTime();

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    float current_time = glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    // Start UI Frame
    ui_new_frame();

    glfwGetFramebufferSize(window, &render_state.window_width, &render_state.window_height);

    // Draw main frame
    draw_frame(&render_state);

    // Draw UI
    ui_draw_debug_window(delta_time, window, &render_state);
    ui_render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Shutdown UI
  ui_shutdown();

  glfwTerminate();
  return 0;
}
