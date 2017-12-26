#pragma once
#include <string>
#include <vector>
#include <fstream>

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <glm\glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include "Input.h"
#include "Lighting.h"

void loadImageSupport();
void requestCoreOpenGL(int majVer, int minVer);

int initialiseSDLWindowAndOpenGL(int windowWidth, int windowHeight);
void quitSDL();

SDL_Window* window;
SDL_GLContext glContext;
int windowWidth;
int windowHeight;

using namespace glm;