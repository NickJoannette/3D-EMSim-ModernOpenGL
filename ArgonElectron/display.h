#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2\SDL.h>
#include<glew.h>
#include <glut.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <SDL2/SDL_audio.h>
#include "camera.h"
#include "freetypehelper.h"
class Display
{
public:

	struct recentInversion {

	};
	Display(int Width, int Height, Camera & cam);
	~Display();
	void swapBuffers();
	void clear(float r, float g, float b, float a);
	bool isQuit();

	inline int ms_x(){ return mouseX;}
	inline int  ms_y(){	return mouseY;}
	inline void unPause(){paused = false;}
	inline int *  getWidth(){return w;}
	inline int  * getHeight(){return h;}
	bool isFrozen(){return freeze;}
	bool closed(){return isClosed;}
	bool isPaused(){return paused;}
	inline SDL_Window* getWindow(){return mainWindow;}
	bool loadBMP();

	int  mouseX, mouseY;
	bool spacePressed = false;
	GLuint *vao{ new GLuint }, *vbo{ new GLuint }, *ebo{ new GLuint }, *tex{ new GLuint };
	GLuint *vertexshader, *fragmentshader, *shaderprog;

protected:
	
	private:

		SDL_Surface* ScreenSurface = NULL;
		SDL_Surface* ImageOnScreen= NULL;
		Camera * camera;

		bool leftMouseButtonDown = false;
		bool rightMouseButtonDown = false;
		bool leftControlDown = false;

		freetypehelper::font_data genericDisplayFont;

		int *w = new int;
		int *h = new int;
		float sensitivity = 1.0f;
		bool paused = false;
		bool freeze = false;
		bool isClosed = false;
		SDL_Window* mainWindow;
		SDL_GLContext mainGLContext;
		
		bool success = true;
		GLuint gProgramID = 0;
};

#endif

