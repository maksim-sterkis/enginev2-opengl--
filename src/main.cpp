#include "glad/glad.h"
#include "renderer.hpp"
#include "ui.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

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
  glfwSwapInterval(1); // Enable VSync by default to prevent thermal throttling

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

  double last_time = glfwGetTime();

  bool mouse_locked = false;
  bool p_pressed = false;
  double last_x = 400, last_y = 300;
  bool first_mouse = true;

  if (glfwRawMouseMotionSupported()) {
      glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  }

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    double current_time = glfwGetTime();
    float delta_time = static_cast<float>(current_time - last_time);
    last_time = current_time;

    // Toggle Mouse Lock with P
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        if (!p_pressed) {
            mouse_locked = !mouse_locked;
            glfwSetInputMode(window, GLFW_CURSOR, mouse_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            first_mouse = true;
            p_pressed = true;
        }
    } else {
        p_pressed = false;
    }

    // Unlock mouse with ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (mouse_locked) {
            mouse_locked = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            first_mouse = true;
        }
    }

    // Camera Input
    if (mouse_locked) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (first_mouse) {
            last_x = xpos;
            last_y = ypos;
            first_mouse = false;
        }

        float xoffset = xpos - last_x;
        float yoffset = last_y - ypos; 
        last_x = xpos;
        last_y = ypos;

        xoffset *= render_state.camera.sensitivity;
        yoffset *= render_state.camera.sensitivity;

        render_state.camera.yaw += xoffset;
        render_state.camera.pitch += yoffset;

        if (render_state.camera.pitch > 89.0f) render_state.camera.pitch = 89.0f;
        if (render_state.camera.pitch < -89.0f) render_state.camera.pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(render_state.camera.yaw)) * cos(glm::radians(render_state.camera.pitch));
        front.y = sin(glm::radians(render_state.camera.pitch));
        front.z = sin(glm::radians(render_state.camera.yaw)) * cos(glm::radians(render_state.camera.pitch));
        render_state.camera.front = glm::normalize(front);

        float velocity = render_state.camera.speed * delta_time;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            render_state.camera.position += render_state.camera.front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            render_state.camera.position -= render_state.camera.front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            render_state.camera.position -= glm::normalize(glm::cross(render_state.camera.front, render_state.camera.up)) * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            render_state.camera.position += glm::normalize(glm::cross(render_state.camera.front, render_state.camera.up)) * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            render_state.camera.position += render_state.camera.up * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            render_state.camera.position -= render_state.camera.up * velocity;
    }

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
