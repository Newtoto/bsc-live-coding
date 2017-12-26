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

	return 0;
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

	// Add tanks
	GameObject tank(vec3(10.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f));
	tank.update();

	// Add camera
	Camera playerCamera;

	// Lighting
	Lighting light;

	// Material
	vec4 ambientMaterialColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	vec4 diffuseMaterialColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	vec4 specualarMaterialColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Color buffer texture
	GLuint colorBufferID = createTexture(windowWidth, windowHeight);

	// Create depth buffer
	GLuint depthRenderBufferID;
	glGenRenderbuffers(1, &depthRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, windowWidth, windowHeight);

	// Create frame buffer
	GLuint frameBufferID;
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorBufferID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unable to create frame buffer for post processing", "Frame buffer error", NULL);
	}

	// Create sceen aligned quad
	GLfloat screenVerts[] =
	{
		-1, -1,
		1, -1,
		-1, 1,
		1, 1
	};

	GLuint screenQuadVBOID;
	glGenBuffers(1, &screenQuadVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), screenVerts, GL_STATIC_DRAW);

	GLuint screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBOID);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	GLuint postProcessingProgramID = LoadShaders("passThroughVert.glsl", "postTextureFrag.glsl");
	GLint texture0Location = glGetUniformLocation(postProcessingProgramID, "texture0");
	if (texture0Location < 0)
	{
		printf("Unable to find %s uniform", "texture0");
	}

	//Create and compile GLSL program from shaders

	GLint fragColorLocation = glGetUniformLocation(tank.programID, "fragColor");
	if (fragColorLocation < 0) 
	{
		printf("Unable to find %s uniform", "fragColor");
	}

	static const GLfloat fragColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };

	GLint currentTimeLocation = glGetUniformLocation(tank.programID, "time");
	if (currentTimeLocation < 0)
	{
		printf("Unable to find %s uniform", "time");
	}

	GLint modelMatrixLocation = glGetUniformLocation(tank.programID, "modelMatrix");
	if (modelMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "modelMatrix");
	}
	GLint viewMatrixLocation = glGetUniformLocation(tank.programID, "viewMatrix");
	if (viewMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "viewMatrix");
	}
	GLint projectionMatrixLocation = glGetUniformLocation(tank.programID, "projectionMatrix");
	if (projectionMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "projectionMatrix");
	}
	GLint textureLocation = glGetUniformLocation(tank.programID, "baseTexture");
	if (textureLocation < 0)
	{
		printf("Unable to find %s uniform", "baseTexture");
	}
	GLint cameraPositionLocation = glGetUniformLocation(tank.programID, "cameraPosition");
	if (cameraPositionLocation < 0)
	{
		printf("Unable to find %s uniform", "cameraPosition");
	}

	// Lighting
	light.InitialiseUniformLocations(tank.programID);

	// Material
	GLint ambientMaterialColorLocation = glGetUniformLocation(tank.programID, "ambientMaterialColor");
	if (ambientMaterialColorLocation < 0)
	{
		printf("Unable to find %s uniform", "ambientMaterialColor");
	}
	GLint diffuseMaterialColorLocation = glGetUniformLocation(tank.programID, "diffuseMaterialColor");
	if (diffuseMaterialColorLocation < 0)
	{
		printf("Unable to find %s uniform", "diffuseMaterialColor");
	}
	GLint specularMaterialColorLocation = glGetUniformLocation(tank.programID, "specularMaterialColor");
	if (specularMaterialColorLocation < 0)
	{
		printf("Unable to find %s uniform", "specularMaterialColor");
	}

	///-----bullet initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----bullet initialization_end-----

	// Creating ground with bullet
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(2.), btScalar(50.)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -10, 0));

	btScalar mass(0.);
	btVector3 localInertia(0, 0, 0);

	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	btRigidBody* groundRigidBody = new btRigidBody(rbInfo);

	//add the body to the dynamics world
	dynamicsWorld->addRigidBody(groundRigidBody);

	// Create tank colision
	tank.createRigidBody();
	dynamicsWorld->addRigidBody(tank.objectRigidBody);

	glEnable(GL_DEPTH_TEST);
	int lastTicks = SDL_GetTicks();
	int currentTicks = SDL_GetTicks();



	SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);

	InputManager inputs;

	//Event loop, we will loop until running is set to false, usually if escape has been pressed or window is closed
	bool running = true;

	//SDL Event structure, this will be checked in the while loop
	SDL_Event event;

	while (running)
	{
		// Get keypresses
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					// Increase mouse sensitivity
					playerCamera.mouseSensitivity += 0.001;
					break;

				case SDLK_DOWN:
					// Decrease mouse sensitivity
					if (playerCamera.mouseSensitivity > 0.001) {
						playerCamera.mouseSensitivity -= 0.001;
					}
					break;

				case SDLK_ESCAPE:
					running = false;
					break;

				case SDLK_w:
					inputs.forward.SetPositive();
					break;

				case SDLK_a:
					inputs.sideways.SetPositive();
					break;

				case SDLK_s:
					inputs.forward.SetNegative();
					break;

				case SDLK_d:
					inputs.sideways.SetNegative();
					break;

				default:
					break;
				}
			}
			if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
					inputs.forward.ZeroPositive();
					break;

				case SDLK_a:
					inputs.sideways.ZeroPositive();
					break;

				case SDLK_s:
					inputs.forward.ZeroNegative();
					break;

				case SDLK_d:
					inputs.sideways.ZeroNegative();
					break;

				default:
					break;
				}
			}
		}

		// Run through inputs
		// Forward and backward movement
		playerCamera.forward(inputs.forward.GetValue());

		// Sideways movement
		playerCamera.sideways(inputs.sideways.GetValue());

		currentTicks = SDL_GetTicks();
		float deltaTime = (float)(currentTicks - lastTicks) / 1000.0f;

		dynamicsWorld->stepSimulation(1.f / 60.f, 10);
		

		// Snap mouse to center
		SDL_WarpMouseInWindow(window, windowWidth / 2, windowHeight / 2);

		// Move camera based on mouse movement
		playerCamera.moveView(windowWidth, windowHeight);

		//Recalculate object position
		tank.update();

		glEnable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

		// Clear the screen
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tank.textureID);

		glUseProgram(tank.programID);

		glUniform4fv(fragColorLocation, 1, fragColor);
		glUniform1f(currentTimeLocation, (float)(currentTicks) / 1000.0f);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(tank.modelMatrix));
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(playerCamera.viewMatrix));
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(playerCamera.projectionMatrix));

		glUniform3fv(cameraPositionLocation, 1, value_ptr(playerCamera.cameraPosition));
		glUniform1i(textureLocation, 0);

		// Lighting
		light.AttatchUniformLocations(tank.programID);

		// Material
		glUniform4fv(ambientMaterialColorLocation, 1, value_ptr(ambientMaterialColor));
		glUniform4fv(diffuseMaterialColorLocation, 1, value_ptr(diffuseMaterialColor));
		glUniform4fv(specularMaterialColorLocation, 1, value_ptr(light.specularColor));
		

		//Draw the triangle
		tank.draw();

		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind post processing shaders
		glUseProgram(postProcessingProgramID);
		
		// Activate texture unit 0 for the color buffer
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBufferID);
		glUniform1i(texture0Location, 0);

		glBindVertexArray(screenVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		SDL_GL_SwapWindow(window);

		lastTicks = currentTicks;
	}
	
	// Destroy models
	dynamicsWorld->removeRigidBody(tank.objectRigidBody);
	tank.destroy();

	dynamicsWorld->removeRigidBody(groundRigidBody);
	// Delete ground
	delete groundShape;
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	// Delete bullet content
	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	// Delete post processing from memory
	glDeleteProgram(postProcessingProgramID);
	glDeleteVertexArrays(1, &screenVAO);
	glDeleteBuffers(1, &screenQuadVBOID);
	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteRenderbuffers(1, &depthRenderBufferID);
	glDeleteTextures(1, &colorBufferID);

	SDL_GL_DeleteContext(glContext);
	//Destroy the window and quit SDL2, NB we should do this after all cleanup in this order!!!
	//https://wiki.libsdl.org/SDL_DestroyWindow
	SDL_DestroyWindow(window);

	IMG_Quit();

	//https://wiki.libsdl.org/SDL_Quit
	SDL_Quit();

	return 0;
}
