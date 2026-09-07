#include "ecs.hpp"

void ecs_init(ECS* ecs) {
    // Pre-allocate some memory to avoid reallocation stutters
    int max_ents = 10000;
    ecs->active_entities.reserve(max_ents);
    ecs->positions.reserve(max_ents);
    ecs->rotations.reserve(max_ents);
    ecs->scales.reserve(max_ents);
    ecs->render_mesh_ids.reserve(max_ents);
    ecs->render_texture_ids.reserve(max_ents);
}

uint32_t ecs_create_entity(ECS* ecs) {
    uint32_t id = ecs->next_entity_id++;
    ecs->active_entities.push_back(id);
    
    // Default components
    ecs->positions.push_back(glm::vec3(0.0f));
    ecs->rotations.push_back(glm::vec3(0.0f));
    ecs->scales.push_back(glm::vec3(1.0f));
    
    // Default missing mesh/texture (assume 0 is valid or placeholder)
    ecs->render_mesh_ids.push_back(0);
    ecs->render_texture_ids.push_back(0);
    
    return id;
}
