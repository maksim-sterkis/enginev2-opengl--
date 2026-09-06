#include "renderer.hpp"
#include "shader.hpp"

void init_renderer(RenderState* state) {
    // Compile and assign shader
    state->shader_program = create_shader_program("shaders/triangle.vert", "shaders/triangle.frag");

    // Flat vertex data (Position + Color)
    float vertices[] = {
        // positions         // colors
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // left (red)
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // right (green)
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top (blue)
    }; 

    // Generate buffers and VAO
    glGenVertexArrays(1, &state->vao);
    glGenBuffers(1, &state->vbo);

    // Bind VAO first, then bind and set VBO and attribute pointers.
    glBindVertexArray(state->vao);

    glBindBuffer(GL_ARRAY_BUFFER, state->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO for safety (optional but good practice)
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
}

void draw_frame(const RenderState* state) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind state and draw
    glUseProgram(state->shader_program);
    glBindVertexArray(state->vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
