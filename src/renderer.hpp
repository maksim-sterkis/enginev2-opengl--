#pragma once
#include "glad/glad.h"

// ---------------------------------------------------------
// DOD Flat State Struct
// ---------------------------------------------------------
struct RenderState {
    GLuint vao;
    GLuint vbo;
    GLuint shader_program;
};

// ---------------------------------------------------------
// Initialize DOD Render State
// ---------------------------------------------------------
void init_renderer(RenderState* state);

// ---------------------------------------------------------
// Execute Draw Call
// ---------------------------------------------------------
void draw_frame(const RenderState* state);
