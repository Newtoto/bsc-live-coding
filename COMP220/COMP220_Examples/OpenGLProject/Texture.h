#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>
#include "SDL_image.h"

#include <string>

GLuint LoadTextureFromFile(const std::string& filename);

GLuint CreateTexture(int width, int height);