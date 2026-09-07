#pragma once
#include "glad/glad.h"
#include <vector>
#include <string>

struct Mesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    int index_count = 0;
};

struct AssetPool {
    std::vector<Mesh> meshes;
    std::vector<GLuint> textures;
};

void assets_init(AssetPool* pool);
// We pass instance_vbo so the mesh VAO can configure the mat4 instanced attributes
uint32_t assets_load_obj(AssetPool* pool, const std::string& path, GLuint instance_vbo);
uint32_t assets_load_texture(AssetPool* pool, const std::string& path);
