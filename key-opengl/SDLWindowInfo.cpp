#include "SDLWindowInfo.h"

#include <array>
#include <functional>
#include <boost/function.hpp>
#include <boost/assign.hpp>
#include <key-opengl/GLOp.h>

using namespace std;
using namespace key;

void SDLWindowInfo::makeCurrent()
{
	SDL_GL_MakeCurrent(sdlWindow, context);
}

void SDLWindowInfo::resize()
{
	this->makeCurrent();

	this->refV8->NativeObject()->initRenderList.NativeObject()->process();

	//SDL_GL_SetSwapInterval(1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//glViewport(0,0, renderWidth / 2, renderHeight / 2);
	
	gluPerspective(60.0f,renderWidth / (float)renderHeight, 0.1, 500);
	//glOrtho(0.0f, renderWidth, renderHeight, 0.0f, 0.0f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
}

void SDLWindowInfo::render()
{
	this->makeCurrent();

	this->refV8->NativeObject()->renderList.NativeObject()->process();

	glClearColor(0.3f, 0.6f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef ( 0.0, 0.0, -500.0 );

	glBegin(GL_QUADS);
		glColor3f(1, 0, 0); glVertex3f(0, 0, 0);
		glColor3f(1, 1, 0); glVertex3f(100, 0, 0);
		glColor3f(1, 0, 1); glVertex3f(100, 100, 0);
		glColor3f(1, 1, 1); glVertex3f(0, 100, 0);
	glEnd();

	//auto keyWindow = this->refV8->NativeObject();

	//keyWindow->

	SDL_GL_SwapWindow(this->sdlWindow);
}