#include "stdafx.h"
#include "SpotLight.h"
#include "Vertex.h"
#include "Magick++.h"


namespace XGLModel {
	REGISTER(SpotLight)
}

XGLModel::SpotLight::SpotLight()
{
	spotlight.Cutoff = cosf(60.0f * 3.1415926f / 180.f);
	spotlight.Direction = XGL::Vec3f(-1.0f, -1.0f, -1.0f);
	spotlight.Direction.normalize();
}


XGLModel::SpotLight::~SpotLight()
{
}

void XGLModel::SpotLight::initGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	XGLModel::Vertex verts[5];

	verts[0].m_pos = Vec3f(0.0f, 0.0f, 1.0f);
	verts[0].m_tex = Vec2f(0.0f, 0.0f);

	verts[1].m_pos = Vec3f(1.0f, 0.0f, 0.0f);
	verts[1].m_tex = Vec2f(1.0f, 0.0f);

	verts[2].m_pos = Vec3f(0.0f, 0.0f, -1.0f);
	verts[2].m_tex = Vec2f(0.0f, 0.0f);

	verts[3].m_pos = Vec3f(-1.0f, 0.0f, 0.0f);
	verts[3].m_tex = Vec2f(1.0f, 0.0f);

	verts[4].m_pos = Vec3f(0.0f, 1.0f, 0.0f);
	verts[4].m_tex = Vec2f(0.5f, 1.0f);

	GLuint elements[18] = {
		0,1,4,
		1,2,4,
		3,4,2,
		3,0,4,
		3,2,0,
		2,1,0

	};
	XGLModel::Vertex::CalcNormals(elements, 18, verts, 5);
	float vs[5 * 8];
	XGLModel::Vertex::GetPtr(verts, 5, vs);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vs), vs, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	Magick::InitializeMagick("E:/2018/opengl/XGLApp/x64/Debug/");
	Magick::Image image;
	Magick::Blob blob;

	try {
		image.read("E:/2018/opengl/XGLApp/x64/Debug/timg.jpg");
		image.write(&blob, "RGBA");
	}
	catch (Magick::Error& error)
	{
		XGLERROR(error.what());
	}

	texUnit = GL_TEXTURE0;
	texUnitIndex = 0;

	glGenTextures(1, &texObj);
	glBindTexture(GL_TEXTURE_2D, texObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void XGLModel::SpotLight::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	Matrix cameraMatrix = camera->getInverseMatrix();

	glUniformMatrix4fv(g_mv, 1, GL_FALSE, cameraMatrix.ptr());
	glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_sampler, texUnitIndex);


	
	static float d = 0.5f;
	Vec3f epos(d, d, d);
	d += 0.0005f;

	glUseProgram(0);
	glPointSize(16.0f);
	epos = epos * cameraMatrix;
	spotlight.Eposition = epos;

	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_POINTS);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(epos.x(), epos.y(), epos.z());
	glEnd();
	glPushMatrix();
	glLoadMatrixf(cameraMatrix.ptr());
	glBegin(GL_POINTS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex3f(0.8f, 0.8f, 0.8f);
	glEnd();
	ModelGL::drawAxis(1.0f);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectMatrix.ptr());

	glUseProgram(program);

	lightShader.updateUniform(spotlight, 1.0f, 16.0f);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (GLvoid*)(2 * sizeof(GL_FLOAT)));
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT)));

	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, texObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

void XGLModel::SpotLight::initUniform()
{

	g_mv = glGetUniformLocation(program, "g_mv");
	g_perspective = glGetUniformLocation(program, "g_pers");
	g_sampler = glGetUniformLocation(program, "g_sampler2d");

	lightShader.initUniform(program);


	if (g_mv < 0
		|| g_perspective< 0
		|| g_sampler< 0)
	{
		XGLERROR("get uniform failed");
	}

}
