#pragma once
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "assets.hpp"
#include "ecs.hpp"

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

struct RenderState {
    GLuint shader_program;
    GLuint u_view_loc;
    GLuint u_proj_loc;
    
    GLuint instance_vbo = 0;
    int max_instances = 100000; // Room for 100k entities per batch
    
    int window_width = 800;
    int window_height = 600;
    ViewportMode viewport_mode = ViewportMode::DYNAMIC_FOV;
    
    Camera camera;
};

void init_renderer(RenderState* state);
void draw_frame(const RenderState* state, const ECS* ecs, const AssetPool* pool);
