#include "stdafx.h"
#include "display.h"
#include "entity.h"
#include <SDL2/SDL_types.h>
#include <algorithm>
#include <glm\glm.hpp>
#include <glm\gtx\intersect.hpp>
#include <Vector>
#include "electron.h"

extern std::vector <cubicEntity> cubic_entities;	glm::vec3 dropspot;
extern GLfloat * sS;
extern Electron * eE;
SDL_AudioDeviceID deviceId;

SDL_AudioSpec wavSpec;
Uint8 *wavBuffer;
Uint8 *wavBuffer2;
Uint8 *wavBuffer3;
Uint32 wavLength;
Uint32 wavLength2;
Uint32 wavLength3;
Display::Display(int WIDTH, int HEIGHT, Camera & cam)
{
	*w = WIDTH;
	*h = HEIGHT;
	camera = &cam;
	Uint32 subsystem_init;
	subsystem_init = SDL_WasInit(SDL_INIT_EVERYTHING);

	if (!subsystem_init)
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_Init(SDL_INIT_AUDIO);

		SDL_LoadWAV("./sounds/test.wav", &wavSpec, &wavBuffer, &wavLength);
		SDL_LoadWAV("./sounds/test2.wav", &wavSpec, &wavBuffer2, &wavLength2);
		SDL_LoadWAV("./sounds/test3.wav", &wavSpec, &wavBuffer3, &wavLength3);

		deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
		SDL_PauseAudioDevice(deviceId, 0);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		mainWindow = SDL_CreateWindow("3D EMSIM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
		ScreenSurface = SDL_GetWindowSurface(mainWindow);
	}
	mainGLContext = SDL_GL_CreateContext(mainWindow);

	glEnable(GL_DEPTH_TEST);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();

	if (glewError != GLEW_OK) std::cerr << "Glew failed to initialize" << std::endl;
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	GLenum error = GL_NO_ERROR;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_CULL_FACE);

	genericDisplayFont.init("./fonts/Oswald-BoldItalic.ttf", 36);
	particleCharacteristicsDisplayFont.init("./fonts/Oswald-BoldItalic.ttf", 12);

}

Display::~Display()
{
}


void Display::swapBuffers()
{

	SDL_Event e;
	float moveMag = 0.15;
	SDL_GetMouseState(&mouseX, &mouseY);

	if (leftControlDown && leftMouseButtonDown) {
		GLint g[2];
		glGetIntegerv(GL_POLYGON_MODE, g);
		int i = -1;
		if (g[0] == GL_LINE) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			i = 0;
		}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_TEXTURE_2D);

			glColor3f(0.8, 0.90, 1.0);
			glLoadIdentity();
			glTranslatef(0, 0, 0);
			glPushMatrix();
			std::string s = "Add ";

			const char * c = &s[0];
			freetypehelper::print(genericDisplayFont, mouseX, 900 - mouseY, c);
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			if (i == 0) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			}
	}

	if (leftControlDown && rightMouseButtonDown) {
		GLint g[2];
		glGetIntegerv(GL_POLYGON_MODE, g);
		int i = -1;
		if (g[0] == GL_LINE) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			i = 0;
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glColor3f(0.8, 0.90, 1.0);
		glLoadIdentity();
		glTranslatef(0, 0, 0);
		glPushMatrix();
		std::string s = "Dec ";
		const char * c = &s[0];
		freetypehelper::print(genericDisplayFont, mouseX, 900 - mouseY, c);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		if (i == 0) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}


	if (rightMouseButtonDown || leftMouseButtonDown) {
		if (mouseX >= 1220)
		{
			camera->xzangle += 0.0125;
			camera->GetM_Forward()->x = cosf(camera->xzangle);
			camera->GetM_Forward()->z = sinf(camera->xzangle);
			camera->UpdateView();
		}
		else if (mouseX < 120)
		{
			camera->xzangle -= 0.0125;
			camera->GetM_Forward()->x = cosf(camera->xzangle);
			camera->GetM_Forward()->z = sinf(camera->xzangle);
			camera->UpdateView();
		}
		if (mouseY > 820)
		{
			camera->GetM_Forward()->y = cosf(camera->xyangle);
			camera->xyangle += 0.0125;
			camera->UpdateView();
		}
		else if (mouseY <= 80)
		{
			camera->GetM_Forward()->y = cosf(camera->xyangle);
			camera->xyangle -= 0.0125;
			camera->UpdateView();
		}

		 if (leftShiftDown && rightMouseButtonDown) {

			 glm::vec3 camPos = *camera->GetCameraPos();
			 glm::mat4 transf = glm::inverse(camera->GetPerspective());
			 float x = (2.0f * (float)mouseX) / 1440.0 - 1.0f;
			 float y = 1.0f - (2.0f * (float)mouseY) / 900.0;
			 float z = 1.0f;
			 glm::vec3 ray_nds = glm::vec3(x, y, z);
			 glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
			 glm::vec4 ray_eye = glm::inverse(camera->GetPerspective()) * ray_clip;
			 ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
			 glm::vec3 ray_wor = (glm::inverse(camera->GetM_View()) * ray_eye);
			 ray_wor = glm::normalize(ray_wor);
			 for (int i = 0; i < 64; i++) {
				 glm::vec3 o = *eE[i].getTransform()->GetPos() - camPos;
				 float odotd = std::max(0.0f, dot(o, ray_wor));
				 o -= ray_wor * odotd;
				 bool intersection = (glm::length(o) * glm::length(o) <= 0.2*0.2);
				 if (intersection) {
					 std::cout << std::to_string(*eE[i].getCharge()) << std::endl;

					 GLint g[2];
					 glGetIntegerv(GL_POLYGON_MODE, g);
					 int wx = -1;
					 if (g[0] == GL_LINE) {
						 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						 wx = 0; 
					 }
					 glEnable(GL_BLEND);
					 glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					 glEnable(GL_TEXTURE_2D);

					 glColor3f(1, 1, 1);
					 glLoadIdentity();
					 glTranslatef(0, 0, 0);
					 glPushMatrix();
					 int h = 64;
					 for (int k = 0; k < 64; k++) if (k != i) {
						 if (eE[i].charge >= eE[k].charge) h--;
					 }
					 std::string s = "Electron\n\nGalactic Hierarchy: " + std::to_string(h) + "\n\nC: " + std::to_string(*eE[i].getCharge()) +
						 "\n\nV: (" + std::to_string(eE[i].velocity.x) + "," + std::to_string(eE[i].velocity.y) + "," + std::to_string(eE[i].velocity.z) + ")";

					 const char * c = &s[0];
					 freetypehelper::print(particleCharacteristicsDisplayFont, mouseX + 25, 1000 - mouseY, c);
					 glPopMatrix();
					 glDisable(GL_TEXTURE_2D);
					 glDisable(GL_BLEND);
					 if (wx == 0) {
						 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					 }
					 break;
				 }
			 }
		}
	}
	//HANDLE MOUSE

	while (SDL_PollEvent(&e))
	{

		switch (e.type)
		{
			
		case SDL_MOUSEBUTTONUP:
			leftMouseButtonDown = false;
			rightMouseButtonDown = false;
			break;
					
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				leftMouseButtonDown = true;
				if (!leftControlDown) {
					SDL_QueueAudio(deviceId, wavBuffer, wavLength/6);
				}
				else {
					SDL_QueueAudio(deviceId, wavBuffer2, wavLength2/6);
				}
				glm::vec3 camPos = *camera->GetCameraPos();
				glm::mat4 transf = glm::inverse(camera->GetPerspective());
				float x = (2.0f * (float)mouseX) / 1440.0 - 1.0f;
				float y = 1.0f - (2.0f * (float)mouseY) / 900.0;
				float z = 1.0f;
				glm::vec3 ray_nds = glm::vec3(x, y, z);
				glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
				glm::vec4 ray_eye = glm::inverse(camera->GetPerspective()) * ray_clip;
				ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
				glm::vec3 ray_wor = (glm::inverse(camera->GetM_View()) * ray_eye);
				ray_wor = glm::normalize(ray_wor);
				for (int i = 0; i < 64; i++) {
					glm::vec3 o = *eE[i].getTransform()->GetPos() - camPos;
					float odotd = std::max(0.0f, dot(o, ray_wor));
					o -= ray_wor * odotd;
					bool intersection = (glm::length(o) * glm::length(o) <= 0.2*0.2);
					if (intersection) {
						if (leftControlDown) {
							eE[i].charge *= 2;
							eE[i].mass *= 2;
						}
						
						else {
							if (sS[i] < 1.0f) {
								sS[i] = 1.0f;
								eE[i].charge *= -1;
							}
							else {
								sS[i] = 0.0f;
								eE[i].charge *= -1;
							}
						}
						break;
					}
				}
			}
			else if (e.button.button == SDL_BUTTON_RIGHT)
			{                      
				rightMouseButtonDown = true;
				if (leftControlDown) {
					SDL_QueueAudio(deviceId, wavBuffer3, wavLength3 / 6);
				}
				glm::vec3 camPos = *camera->GetCameraPos();
				glm::mat4 transf = glm::inverse(camera->GetPerspective());
				float x = (2.0f * (float)mouseX) / 1440.0 - 1.0f;
				float y = 1.0f - (2.0f * (float)mouseY) / 900.0;
				float z = 1.0f;
				glm::vec3 ray_nds = glm::vec3(x, y, z);
				glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0);
				glm::vec4 ray_eye = glm::inverse(camera->GetPerspective()) * ray_clip;
				ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
				glm::vec3 ray_wor = (glm::inverse(camera->GetM_View()) * ray_eye);
				ray_wor = glm::normalize(ray_wor);
				for (int i = 0; i < 64; i++) {
					glm::vec3 o = *eE[i].getTransform()->GetPos() - camPos;
					float odotd = std::max(0.0f, dot(o, ray_wor));
					o -= ray_wor * odotd;
					bool intersection = (glm::length(o) * glm::length(o) <= 0.2*0.2);
					if (intersection) {
						if (leftControlDown) {
							eE[i].charge /= 2;
							eE[i].mass /= 2;
						}
						break;
					}
				}
			}
			break;


		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_LCTRL:
				leftControlDown = false;
				break;

			case SDLK_LSHIFT:
				leftShiftDown = false;
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{

			case SDLK_LCTRL:
				leftControlDown = true;
				break;

			case SDLK_LSHIFT:
				leftShiftDown = true;
				break;

			case SDLK_SPACE:
				for (int i = 0; i < cubic_entities.size(); i++)
				{
					std::cout << "Velocity: " << glm::to_string(*cubic_entities[i].Velocity()) << "\n";
					std::cout << "Position: " << glm::to_string(*cubic_entities[i].Position()) << "\n";
				}

				break;

			case SDLK_n:
				cubic_entities.push_back(cubicEntity(2, -7000000, 70000000, *camera->GetCameraPos() + *camera->GetM_Forward(), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), camera));
	
				break;
			case SDLK_m:
		
				break;

			case SDLK_z:
				//zzz / freeze frame
				Uint32 wavLength;
				Uint8 *wavBuffer;
				SDL_AudioSpec wavSpec;

				SDL_LoadWAV("./sounds/open_menu.wav", &wavSpec, &wavBuffer, &wavLength);
				deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

				SDL_QueueAudio(deviceId, wavBuffer, wavLength);
				SDL_PauseAudioDevice(deviceId, 0);

				freeze = !freeze ? true : false;
				paused = !paused ? true : false;
				break;

			case SDLK_p:
				paused = !paused ? true : false;
				break;

			case SDLK_ESCAPE:
				isClosed = true;
				break;
		
			case SDLK_F1:
				SDL_SetWindowFullscreen(mainWindow, 1);
				break;

			/*case SDLK_t:
				if (!glIsEnabled(GL_CULL_FACE))	glEnable(GL_CULL_FACE);
				else glDisable(GL_CULL_FACE);
				break;
				*/
			case SDLK_1:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;

			case SDLK_2:

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;

			case SDLK_LEFT:
				*camera->GetCameraPos() -= glm::normalize(glm::cross(*camera->GetM_Forward(), *camera->GetM_U()))*=moveMag;
				camera->UpdateView();
				break;

			case SDLK_RIGHT:
				*camera->GetCameraPos() += glm::normalize(glm::cross(*camera->GetM_Forward(), *camera->GetM_U()))*= moveMag;
				camera->UpdateView();
				break;

			case SDLK_DOWN:
				//3d movement *camera->GetCameraPos() -= *camera->GetM_Forward()*0.3f;

				//2d movement in XZ plane
				camera->GetCameraPos()->x -= camera->GetM_Forward()->x*moveMag;
				camera->GetCameraPos()->y -= camera->GetM_Forward()->y*moveMag;
				camera->GetCameraPos()->z -= camera->GetM_Forward()->z*moveMag;

				camera->UpdateView();
				break;

			case SDLK_UP:
				camera->GetCameraPos()->x += camera->GetM_Forward()->x*moveMag;
				camera->GetCameraPos()->z += camera->GetM_Forward()->z*moveMag;

				camera->UpdateView();
				break;

			case SDLK_PAGEUP:
				camera->GetCameraPos()->y += moveMag;
				camera->UpdateView();
				break;

			case SDLK_PAGEDOWN:
				camera->GetCameraPos()->y -= moveMag;
				camera->UpdateView();
				break;

			case SDLK_s:

				camera->GetM_Forward()->y -= 0.02;
				camera->UpdateView();
				break;

			case SDLK_w:
				camera->GetM_Forward()->y += 0.02;
				
				camera->UpdateView();
				break;

				break;
				
			}
		}
	}


	SDL_GL_SwapWindow(mainWindow); // Originally was at start of this function. May affect some functionality

}


bool Display::isQuit()
{
	return isClosed;
}

void Display::clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


bool Display::loadBMP()
{
	bool success = true;

	//Load splash image
	ImageOnScreen = SDL_LoadBMP("pe.bmp");
	if (ImageOnScreen == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError());
		success = false;
	}

	return success;

}