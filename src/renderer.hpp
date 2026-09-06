#pragma once
#include "glad/glad.h"

// ---------------------------------------------------------
// DOD Flat State Struct
// ---------------------------------------------------------
enum class ViewportMode {
    DYNAMIC_FOV,
    FORCED_16_9
};

struct RenderState {
    GLuint vao;
    GLuint vbo;
    GLuint shader_program;
    GLuint u_aspectRatio_loc;
    
    int window_width = 800;
    int window_height = 600;
    ViewportMode viewport_mode = ViewportMode::DYNAMIC_FOV;
};

// ---------------------------------------------------------
// Initialize DOD Render State
// ---------------------------------------------------------
void init_renderer(RenderState* state);

// ---------------------------------------------------------
// Execute Draw Call
// ---------------------------------------------------------
void draw_frame(const RenderState* state);
