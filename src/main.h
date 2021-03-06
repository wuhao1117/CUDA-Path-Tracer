// CIS565 CUDA Raytracer: A parallel raytracer for Patrick Cozzi's CIS565: GPU Computing at the University of Pennsylvania
// Written by Yining Karl Li, Copyright (c) 2012 University of Pennsylvania
// This file includes code from:
//       Rob Farber for CUDA-GL interop, from CUDA Supercomputing For The Masses: http://www.drdobbs.com/architecture-and-design/cuda-supercomputing-for-the-masses-part/222600097
//       Varun Sampath and Patrick Cozzi for GLSL Loading, from CIS565 Spring 2012 HW5 at the University of Pennsylvania: http://cis565-spring-2012.github.com/
//       Yining Karl Li's TAKUA Render, a massively parallel pathtracing renderer: http://www.yiningkarlli.com

#ifndef MAIN_H
#define MAIN_H

#ifdef __APPLE__
	#include <GL/glfw.h>
#else
	#include <GL/glew.h>
	#include <GL/freeglut.h>
#endif

#include <stdlib.h>
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>
#include "glslUtility.h"
#include "sceneStructs.h"
#include "glm/glm.hpp"
#include "image.h"
#include "raytraceKernel.h"
#include "utilities.h"
#include "scene.h"

#if CUDA_VERSION >= 5000
    #include <helper_cuda.h>
    #include <helper_cuda_gl.h>
    #define compat_getMaxGflopsDeviceId() gpuGetMaxGflopsDeviceId() 
#else
    #include <cutil_inline.h>
    #include <cutil_gl_inline.h>
    #define compat_getMaxGflopsDeviceId() cutGetMaxGflopsDeviceId()
#endif

#define CLEAR_IMAGE \
for(int i=0; i<renderCam->resolution.x*renderCam->resolution.y; i++){ \
	renderCam->image[i] = glm::vec3(0,0,0); \
}
using namespace std;

//-------------------------------
//----------PATHTRACER-----------
//-------------------------------

scene* renderScene;
camera* renderCam;
int targetFrame;
int iterations;
bool finishedRender;
bool singleFrameMode;

//-------------------------------
//------------GL STUFF-----------
//-------------------------------

GLuint positionLocation = 0;
GLuint texcoordsLocation = 1;
const char *attributeLocations[] = { "Position", "Tex" };
GLuint pbo = (GLuint)NULL;
GLuint displayImage;

//-------------------------------
//----------CUDA STUFF-----------
//-------------------------------

int width=800; int height=800;

//-------------------------------
//-------------MAIN--------------
//-------------------------------

int main(int argc, char** argv);

//-------------------------------
//-------------TIME--------------
//------------------------------

int timeSinceLastFrame;
int frames;
int fps;
double now, lastTime;
float delta_t = 0.0f;
bool animFlag = true; // pause the app if false

//-------------------------------
//----------Mouse Control--------
//-------------------------------

int mouse_old_x, mouse_old_y;
unsigned char button_mask = 0x00;

float viewPhi = PI;
float viewTheta = HALF_PI;

float moveSensitivity = 0.5f;

//-------------------------------
//---------RUNTIME STUFF---------
//-------------------------------

void runCuda();

#ifdef __APPLE__
	void display();
#else
	void display();
	void keyboard(unsigned char key, int x, int y);
	void mouseClick(int button, int state, int x, int y);
	void mouseMotion(int x, int y);
	void mouseWheel(int, int, int, int);
#endif

//-------------------------------
//----------SETUP STUFF----------
//-------------------------------


#ifdef __APPLE__
	void init();
#else
	void init(int argc, char* argv[]);
#endif

void initPBO(GLuint* pbo);
void initCuda();
void initTextures();
void initVAO();
GLuint initShader(const char *vertexShaderPath, const char *fragmentShaderPath);

//-------------------------------
//---------CLEANUP STUFF---------
//-------------------------------

void cleanupCuda();
void deleteMeshData();
void deletePBO(GLuint* pbo);
void deleteTexture(GLuint* tex);
void shut_down(int return_code);

#endif
