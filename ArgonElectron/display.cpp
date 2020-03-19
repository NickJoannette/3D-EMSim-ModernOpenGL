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
	
}

Display::~Display()
{
}


void Display::swapBuffers()
{
	SDL_GL_SwapWindow(mainWindow);
	SDL_Event e;
	float moveMag = 5;



	SDL_GetMouseState(&mouseX, &mouseY);

	if (mouseButtonDown) {
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
	}
	//HANDLE MOUSE

	while (SDL_PollEvent(&e))
	{

		switch (e.type)
		{
			
		case SDL_MOUSEBUTTONUP:
			mouseButtonDown = false;
			break;
					
		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_LEFT)
			{

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
					bool intersection = (glm::length(o) * glm::length(o) <= 1*1);
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
				mouseButtonDown = true;

			}
			break;


		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_LCTRL:
				leftControlDown = false;
				//std::cout << "Left control KEY UP : : : FALSE" << std::endl;
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{

			case SDLK_LCTRL:
				//std::cout << "Left control KEY DOWN : : : TRUE" << std::endl;

				leftControlDown = true;
				break;


			case SDLK_DELETE:
				cubic_entities.erase(cubic_entities.end() - 1);
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

			case SDLK_t:
				if (!glIsEnabled(GL_CULL_FACE))	glEnable(GL_CULL_FACE);
				else glDisable(GL_CULL_FACE);
				break;

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