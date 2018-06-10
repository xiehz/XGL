///////////////////////////////////////////////////////////////////////////////
// ModelGL.cpp
// ===========
// Model component of OpenGL
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2016-05-29
// UPDATED: 2017-12-13
///////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <cmath>
#include <sstream>
#include <GL\glew.h>
#include "xgl\XOrbitCamera.h"
#include "TutorialInterface.h"
#include "TutorialFactory.h"
#include "ModelGL.h"

using namespace XGLModel;

XGLModel::ModelGL::ModelGL()
{
	camera = nullptr;
}

ModelGL::~ModelGL()
{
	if (camera != nullptr)
		delete camera;
	camera = nullptr;
}

void ModelGL::init()
{
	RECT rect;
	::GetClientRect(handle, &rect);

	windowWith = rect.right - rect.left;
	windowHeight = rect.bottom - rect.top;

	initCamera();
	initProject();
	initGL();
	initShader();
}



void ModelGL::draw()
{
	preFrame();
	// clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectMatrix.ptr());
	//glLoadIdentity();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	Matrixf&	z = orbit->getInverseMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->getInverseMatrix().ptr());

	draw3D();   // draw 3D sphere, cursor vector and axis

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	postFrame();
}

void XGLModel::ModelGL::initCamera()
{
	camera = new XGL::XOrbitCamera();

	XOrbitCamera* orbit = dynamic_cast<XOrbitCamera*>(camera);
	orbit->setTransformation(Vec3f(0.0f, 0.0f, 2.0f),
		Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

}

void XGLModel::ModelGL::initProject()
{
	// cofigure projection matrix
	float asptio = windowWith / windowHeight;
	float height = 1.0f;
	float width = height * asptio;

	project(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, 1, 100.0f);
}

void XGLModel::ModelGL::initShader()
{
}



void XGLModel::ModelGL::setViewport(int viewport[4])
{
	memcpy(this->viewport, viewport, sizeof(int) * 4);
}

void XGLModel::ModelGL::project(float l, float r, float b, float t, float n, float f)
{
	this->projectMatrix.makeFrustum(l, r, b, t, n, f);
	//this->projectMatrix.identity();
	////ÁÐÖ÷Ðò
	//projectMatrix.frustum(l, r, b, t, n, f);
	//projectMatrix[0] = 2 * n / (r - l);
	//projectMatrix[5] = 2 * n / (t - b);
	//projectMatrix[8] = (r + l) / (r - l);
	//projectMatrix[9] = (t + b) / (t - b);
	//projectMatrix[10] = -(f + n) / (f - n);
	//projectMatrix[11] = -1;
	//projectMatrix[14] = -(2 * f * n) / (f - n);
	//projectMatrix[15] = 0;
}

void ModelGL::drawAxis(float size)
{
	//glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
	glDisable(GL_LIGHTING);

	// draw axis
	glLineWidth(3);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(size, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, size, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, size);
	glEnd();
	glLineWidth(1);

	// draw arrows(actually big square dots)
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glVertex3f(size, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, size, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, size);
	glEnd();
	glPointSize(1);

	// restore default settings
	glEnable(GL_LIGHTING);
	//glDepthFunc(GL_LEQUAL);
}

///////////////////////////////////////////////////////////////////////////////
// draw 3d
///////////////////////////////////////////////////////////////////////////////
void ModelGL::draw3D()
{

	// draw axis
	drawAxis(1 + 1 * 0.1f);
}


///////////////////////////////////////////////////////////////////////////////
// pre-frame
///////////////////////////////////////////////////////////////////////////////
void ModelGL::preFrame()
{
}


///////////////////////////////////////////////////////////////////////////////
// post-frame
///////////////////////////////////////////////////////////////////////////////
void ModelGL::postFrame()
{
}


void ModelGL::initGL()
{
	glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

												// enable /disable features
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);

	glClearColor(0, 0, 0, 0);                   // background color
	glClearStencil(0);                          // clear stencil buffer
	glClearDepth(1.0f);                         // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);

}


void ModelGL::initLights()
{
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // ambient light
	GLfloat lightKd[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // diffuse light
	GLfloat lightKs[] = { 1, 1, 1, 1 };           // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// position the light
	float lightPos[4] = { 0, sphere_Radius * 3, sphere_Radius * 2, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

