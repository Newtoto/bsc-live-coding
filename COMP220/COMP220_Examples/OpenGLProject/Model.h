#pragma once

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <string>
#include <vector>

#include <GL\glew.h>
#include <SDL_opengl.h>

#include "Vertex.h"
#include "Mesh.h"

bool LoadModelFromFile(const std::string& filename, GLuint VBO, GLuint EBO, unsigned int& numberOfVerts, unsigned int& numberOfIndices);

bool LoadMeshFromFile(const std::string& filename, std::vector<Mesh*>& meshes);