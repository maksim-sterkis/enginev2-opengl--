#include "renderer.hpp"
#include "shader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>

void init_renderer(RenderState* state) {
    state->shader_program = create_shader_program("shaders/triangle.vert", "shaders/triangle.frag");

    // Load OBJ
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "assets/cube.obj")) {
        std::cerr << warn << err << std::endl;
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            // Position
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);
            
            // Texture Coordinates (UV)
            if (index.texcoord_index >= 0) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
            
            indices.push_back(indices.size());
        }
    }

    state->main_mesh.index_count = indices.size();

    glGenVertexArrays(1, &state->main_mesh.vao);
    glGenBuffers(1, &state->main_mesh.vbo);
    glGenBuffers(1, &state->main_mesh.ebo);

    glBindVertexArray(state->main_mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, state->main_mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state->main_mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // UV (2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load Texture Image
    glGenTextures(1, &state->main_mesh.texture_id);
    glBindTexture(GL_TEXTURE_2D, state->main_mesh.texture_id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load("assets/texture.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    state->u_view_loc = glGetUniformLocation(state->shader_program, "u_view");
    state->u_proj_loc = glGetUniformLocation(state->shader_program, "u_proj");
}

void draw_frame(const RenderState* state) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = 1.0f;
    if (state->viewport_mode == ViewportMode::DYNAMIC_FOV) {
        glViewport(0, 0, state->window_width, state->window_height);
        aspect = (float)state->window_width / (float)state->window_height;
    } else {
        int view_w = state->window_width;
        int view_h = state->window_width * 9 / 16;
        if (view_h > state->window_height) {
            view_h = state->window_height;
            view_w = state->window_height * 16 / 9;
        }
        int vp_x = (state->window_width - view_w) / 2;
        int vp_y = (state->window_height - view_h) / 2;
        glViewport(vp_x, vp_y, view_w, view_h);
        aspect = 16.0f / 9.0f;
    }

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(state->camera.position, state->camera.position + state->camera.front, state->camera.up);

    glUseProgram(state->shader_program);
    glUniformMatrix4fv(state->u_view_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(state->u_proj_loc, 1, GL_FALSE, &projection[0][0]);

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state->main_mesh.texture_id);

    glBindVertexArray(state->main_mesh.vao);
    glDrawElements(GL_TRIANGLES, state->main_mesh.index_count, GL_UNSIGNED_INT, 0);
}
