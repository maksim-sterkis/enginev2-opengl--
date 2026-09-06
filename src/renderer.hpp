#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// ---------------------------------------------------------
// DOD Flat State Struct
// ---------------------------------------------------------
enum class ViewportMode {
    DYNAMIC_FOV,
    FORCED_16_9
};

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;
    float speed = 5.0f;
    float sensitivity = 0.1f;
};

struct Mesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLuint texture_id = 0;
    int index_count = 0;
};

struct RenderState {
    Mesh main_mesh;
    GLuint shader_program;
    GLuint u_view_loc;
    GLuint u_proj_loc;
    
    int window_width = 800;
    int window_height = 600;
    ViewportMode viewport_mode = ViewportMode::DYNAMIC_FOV;
    
    Camera camera;
};

// ---------------------------------------------------------
// Initialize DOD Render State
// ---------------------------------------------------------
void init_renderer(RenderState* state);

// ---------------------------------------------------------
// Execute Draw Call
// ---------------------------------------------------------
void draw_frame(const RenderState* state);
