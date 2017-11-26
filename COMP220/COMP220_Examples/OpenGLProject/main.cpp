//main.cpp - define the entry point of the application

#include "main.h"

void loadImageSupport() {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted&flags) != flags)
	{
		printf("IMG_Init: Failed to init required jpg and png support.\n");
		printf("IMG_Init: %s\n", IMG_GetError());
	}
}

void requestCoreOpenGL(int majorVersion, int minorVersion) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

int initialiseSDLWindowAndOpenGL(int windowWidth, int windowHeight) {
	// load support for JPG and PNG image formats (tiffs also supported, but not checked)
	loadImageSupport();
	//Request 3.1 core OpenGL
	requestCoreOpenGL(3, 1);

	//Initialises the SDL Library, passing in SDL_INIT_VIDEO to only initialise the video subsystems
	//https://wiki.libsdl.org/SDL_Init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return 1;
	}

	//Create a window, note we have to free the pointer returned using the DestroyWindow Function
	//https://wiki.libsdl.org/SDL_CreateWindow
	window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//Checks to see if the window has been created, the pointer will have a value of some kind
	if (window == nullptr)
	{
		return 1;
	}

	// Hide mouse cursor
	SDL_ShowCursor(SDL_DISABLE);

	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	if (glContext == nullptr)
	{
		return 1;
	}

	//Init GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		return 1;
	}
}

void quitSDL() {
	// Clean up
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
};

int main(int argc, char* args[])
{
	// Define window size
	int windowWidth = 800;
	int windowHeight = 800;

	// Check SDL initialisation
	if (initialiseSDLWindowAndOpenGL(windowWidth, windowHeight) == 1) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, SDL_GetError(), "SDL and openGL failed to initialise", NULL);
		quitSDL();
	}

	std::vector<Mesh*> meshes;
	loadMeshFromFile("Tank1.FBX", meshes);
	GLuint textureID = loadTextureFromFile("Tank1DF.png");

	vec3 trianglePosition = vec3(0.0f, 0.0f, 0.0f);
	vec3 triangleScale = vec3(1.0f, 1.0f, 1.0f);
	vec3 triangleRotation = vec3(0.0f, 0.0f, 0.0f);

	mat4 rotationMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(1.0f, 0.0f, 1.0f));
	mat4 scaleMatrix = scale(triangleScale);
	mat4 translationMatrix = translate(trianglePosition);

	mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	// default camera position
	vec3 cameraPosition = vec3(0.0f, 2.0f, -5.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	mat4 viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	vec3 position = cameraPosition;
	float horizontalAngle = 0.0f;
	float verticalAngle = 0.0f;
	float fieldOfView = 45.0f;

	float cameraSpeed = 2.0f;
	float mouseSensitivity = 0.01;

	mat4 projectionMatrix = perspective(radians(90.0f), float(4 / 3), 0.1f, 100.0f);

	//Create and compile GLSL program from shaders
	GLuint programID = LoadShaders("textureVert.glsl", "textureFrag.glsl");

	GLint fragColorLocation = glGetUniformLocation(programID, "fragColor");
	if (fragColorLocation < 0) 
	{
		printf("Unable to find %s uniform", "fragColor");
	}

	static const GLfloat fragColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	GLint currentTimeLocation = glGetUniformLocation(programID, "time");
	if (currentTimeLocation < 0)
	{
		printf("Unable to find %s uniform", "time");
	}
	GLint modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	if (modelMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "modelMatrix");
	}
	GLint viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	if (viewMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "viewMatrix");
	}
	GLint projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	if (projectionMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "projectionMatrix");
	}
	GLint textureLocation = glGetUniformLocation(programID, "baseTexture");
	if (textureLocation < 0)
	{
		printf("Unable to find %s uniform", "baseTexture");
	}
	glEnable(GL_DEPTH_TEST);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();


	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;
	//SDL Event structure, this will be checked in the while loop
	SDL_Event ev;

	while (running)
	{
		//Poll for the events which have happened in this frame
		//https://wiki.libsdl.org/SDL_PollEvent
		while (SDL_PollEvent(&ev))
		{
			//Switch case for every message we are intereted in
			switch (ev.type)
			{
				//QUIT Message, usually called when the window has been closed
			case SDL_QUIT:
				running = false;
				break;
				//KEYDOWN Message, called when a key has been pressed down
			case SDL_KEYDOWN:
				//Check the actual key code of the key that has been pressed
				switch (ev.key.keysym.sym)
				{
					//Escape key
				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_UP:
					cameraPosition.z += 0.1;
					break;

				case SDLK_DOWN:
					cameraPosition.z -= 0.1;
					break;

				case SDLK_RIGHT:
					cameraTarget.x -= 0.1;
					break;

				case SDLK_LEFT:
					cameraTarget.x += 0.1;
					break;
				};
			}
		}

		currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		// Get mouse movement
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		// Snap mouse to center
		SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);

		// Move camera based on mouse movement
		horizontalAngle += mouseSensitivity * float(windowWidth / 2 - mouseX);
		verticalAngle += mouseSensitivity * float(windowHeight / 2 - mouseY);
		printf("%", verticalAngle);
		vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));

		//Recalculate camera
		viewMatrix = lookAt(cameraPosition, direction + cameraPosition, cameraUp);

		//Recalculate translations
		rotationMatrix = rotate(triangleRotation.x, vec3(1.0f, 0.0f, 0.0f))*rotate(triangleRotation.y, vec3(0.0f, 1.0f, 0.0f))*rotate(triangleRotation.z, vec3(1.0f, 0.0f, 1.0f));
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

		// Clear the screen
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glUseProgram(programID);

		glUniform4fv(fragColorLocation, 1, fragColor);
		glUniform1f(currentTimeLocation, (float)(currentTicks) / 1000.0f);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(projectionMatrix));
		glUniform1i(textureLocation, 0);
		

		//Draw the triangle
		for (Mesh *currentMesh : meshes)
		{
			currentMesh->render();
		}

		SDL_GL_SwapWindow(window);

		lastTicks = currentTicks;
	}

	//Delete content
	auto iter = meshes.begin();
	while (iter != meshes.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	meshes.clear();

	glDeleteTextures(1, &textureID);
	glDeleteProgram(programID);

	SDL_GL_DeleteContext(glContext);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);

	IMG_Quit();

	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}
