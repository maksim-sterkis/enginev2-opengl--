#pragma once
#include <string>
#include "glad/glad.h"

// ---------------------------------------------------------
// Helper: Read file to string
// ---------------------------------------------------------
std::string read_file_to_string(const std::string& path);

// ---------------------------------------------------------
// Helper: Compile Shader
// ---------------------------------------------------------
GLuint compile_shader(GLenum type, const std::string& source);

// ---------------------------------------------------------
// Helper: Create Shader Program
// ---------------------------------------------------------
GLuint create_shader_program(const std::string& vert_path, const std::string& frag_path);
