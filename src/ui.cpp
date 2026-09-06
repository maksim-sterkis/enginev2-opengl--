#include "ui.hpp"
#include "renderer.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <string>

void ui_init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ui_shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ui_new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ui_render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ui_draw_debug_window(float delta_time, GLFWwindow* window, RenderState* state) {
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::Begin("Engine Metrics & Input");
    
    // Performance: 100ms Rolling Average FPS
    static float display_fps = 0.0f;
    static float accum_time = 0.0f;
    static int frame_count = 0;
    accum_time += delta_time;
    frame_count++;
    if (accum_time >= 0.1f) {
        display_fps = (float)frame_count / accum_time;
        accum_time = 0.0f;
        frame_count = 0;
    }
    
    ImGui::Text("FPS (100ms Avg): %.1f", display_fps);
    ImGui::Text("Frame Time: %.3f ms", display_fps > 0.0f ? (1000.0f / display_fps) : 0.0f);
    
    ImGui::Separator();
    
    // Engine Settings Toggles
    static int fps_mode = 0; // 0 = Unlimited, 1 = VSync
    if (ImGui::Combo("FPS Mode", &fps_mode, "Unlimited\0VSync\0")) {
        glfwSwapInterval(fps_mode);
    }
    
    int vmode = (int)state->viewport_mode;
    if (ImGui::Combo("Viewport Mode", &vmode, "Dynamic FOV\0Forced 16:9\0")) {
        state->viewport_mode = (ViewportMode)vmode;
    }

    ImGui::Separator();
    
    // Mouse State
    ImGui::Text("Mouse X: %.1f, Y: %.1f", io.MousePos.x, io.MousePos.y);
    ImGui::Text("Left Click: %s", ImGui::IsMouseDown(ImGuiMouseButton_Left) ? "DOWN" : "UP");
    ImGui::Text("Right Click: %s", ImGui::IsMouseDown(ImGuiMouseButton_Right) ? "DOWN" : "UP");
    static const char* scroll_dir = "NONE";
    static float scroll_timer = 0.0f;
    if (io.MouseWheel > 0.0f) { scroll_dir = "UP"; scroll_timer = 0.5f; }
    else if (io.MouseWheel < 0.0f) { scroll_dir = "DOWN"; scroll_timer = 0.5f; }
    
    if (scroll_timer > 0.0f) { scroll_timer -= delta_time; }
    else { scroll_dir = "NONE"; }
    
    ImGui::Text("Scroll: %s", scroll_dir);

    ImGui::Separator();

    // Keyboard State
    std::string keys_down = "Keys Down:";
    for (int i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++) {
        if (ImGui::IsKeyDown((ImGuiKey)i)) {
            const char* key_name = ImGui::GetKeyName((ImGuiKey)i);
            if (key_name) {
                keys_down += " [";
                keys_down += key_name;
                keys_down += "]";
            }
        }
    }
    
    ImGui::TextWrapped("%s", keys_down.c_str());
    
    ImGui::End();

    // Exit Modal Logic
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        ImGui::OpenPopup("Exit?");
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Exit?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to exit the engine?");
        ImGui::Separator();
        
        float button_width = 120.0f;
        float spacing = ImGui::GetStyle().ItemSpacing.x;
        float total_width = (button_width * 2.0f) + spacing;
        float offset = (ImGui::GetWindowSize().x - total_width) * 0.5f;
        ImGui::SetCursorPosX(offset);
        
        if (ImGui::Button("Exit", ImVec2(button_width, 0))) {
            glfwSetWindowShouldClose(window, true);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(button_width, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
