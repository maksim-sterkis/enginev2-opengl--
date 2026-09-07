#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <cstdint>

struct ECS {
    std::vector<uint32_t> active_entities;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> rotations; // euler angles in degrees
    std::vector<glm::vec3> scales;
    std::vector<uint32_t> render_mesh_ids;
    std::vector<uint32_t> render_texture_ids;

    uint32_t next_entity_id = 0;
};

void ecs_init(ECS* ecs);
uint32_t ecs_create_entity(ECS* ecs);
// void ecs_destroy_entity(ECS* ecs, uint32_t id); // omitted for simplicity for now
