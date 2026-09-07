#include "renderer.hpp"
#include "shader.hpp"
#include <iostream>
#include <algorithm>

void init_renderer(RenderState* state) {
    state->shader_program = create_shader_program("shaders/triangle.vert", "shaders/triangle.frag");

    glGenBuffers(1, &state->instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, state->instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, state->max_instances * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    state->u_view_loc = glGetUniformLocation(state->shader_program, "u_view");
    state->u_proj_loc = glGetUniformLocation(state->shader_program, "u_proj");
}

void draw_frame(const RenderState* state, const ECS* ecs, const AssetPool* pool) {
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

    // DoD Instanced Batcher
    struct RenderData {
        uint32_t entity;
        uint32_t mesh;
        uint32_t tex;
    };
    
    std::vector<RenderData> render_list;
    render_list.reserve(ecs->active_entities.size());
    for(auto ent : ecs->active_entities) {
        render_list.push_back({ent, ecs->render_mesh_ids[ent], ecs->render_texture_ids[ent]});
    }

    // Sort by Mesh then Texture to minimize state changes
    std::sort(render_list.begin(), render_list.end(), [](const RenderData& a, const RenderData& b){
        if (a.mesh != b.mesh) return a.mesh < b.mesh;
        return a.tex < b.tex;
    });

    if (render_list.empty()) return;

    uint32_t current_mesh = render_list[0].mesh;
    uint32_t current_tex = render_list[0].tex;
    std::vector<glm::mat4> transforms;
    transforms.reserve(10000); // Preallocate
    
    auto flush_batch = [&]() {
        if (transforms.empty()) return;
        
        glBindBuffer(GL_ARRAY_BUFFER, state->instance_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, transforms.size() * sizeof(glm::mat4), transforms.data());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pool->textures[current_tex]);

        glBindVertexArray(pool->meshes[current_mesh].vao);
        glDrawElementsInstanced(GL_TRIANGLES, pool->meshes[current_mesh].index_count, GL_UNSIGNED_INT, 0, transforms.size());
        
        transforms.clear();
    };

    for(const auto& item : render_list) {
        if (item.mesh != current_mesh || item.tex != current_tex) {
            flush_batch();
            current_mesh = item.mesh;
            current_tex = item.tex;
        }

        uint32_t ent = item.entity;
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ecs->positions[ent]);
        model = glm::rotate(model, glm::radians(ecs->rotations[ent].x), glm::vec3(1,0,0));
        model = glm::rotate(model, glm::radians(ecs->rotations[ent].y), glm::vec3(0,1,0));
        model = glm::rotate(model, glm::radians(ecs->rotations[ent].z), glm::vec3(0,0,1));
        model = glm::scale(model, ecs->scales[ent]);
        transforms.push_back(model);
    }
    
    // Flush the final batch
    flush_batch();
}
