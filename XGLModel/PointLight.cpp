#include "stdafx.h"
#include "Vertex.h"
#include "PointLight.h"
#include "Magick++.h"


namespace XGLModel {
	REGISTER(PointLight)
}

XGLModel::PointLight::PointLight()
{
}


XGLModel::PointLight::~PointLight()
{
}

void XGLModel::PointLight::init()
{
	glClearColor(0.0f,0.0f,0.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	XGLModel::Vertex verts[5];

	verts[0].pos = Vec3f(0.0f, 0.0f, 1.0f);
	verts[0].texcoord = Vec2f(0.0f, 0.0f);

	verts[1].pos = Vec3f(1.0f, 0.0f, 0.0f);
	verts[1].texcoord = Vec2f(1.0f, 0.0f);

	verts[2].pos = Vec3f(0.0f, 0.0f, -1.0f);
	verts[2].texcoord = Vec2f(0.0f, 0.0f);

	verts[3].pos = Vec3f(-1.0f, 0.0f, 0.0f);
	verts[3].texcoord = Vec2f(1.0f, 0.0f);

	verts[4].pos = Vec3f(0.0f, 1.0f, 0.0f);
	verts[4].texcoord = Vec2f(0.5f, 1.0f);

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

	initShader();
}

void XGLModel::PointLight::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	//ModelGL::draw();
	glUniformMatrix4fv(g_mv, 1, GL_FALSE, cameraMatrix.ptr());
	glUniformMatrix4fv(g_perspective, 1, GL_FALSE, projectMatrix.ptr());
	glUniform1i(g_sampler, texUnitIndex);


	glUniform1f(specular_Intensity, 1.0f);
	glUniform1f(shineness, 16.0f);
	static float d = 0.0f;
	Vec3f epos(d,d,d);
	d += 0.001f;

	glUseProgram(0);
	glPointSize(16.0f);
	epos = epos * cameraMatrix;
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
	glVertex3f(0.8f,0.8f,0.8f);
	glEnd();
	ModelGL::drawAxis(1.0f);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectMatrix.ptr());

	glUseProgram(program);
	glUniform3f(pointlight_eposition, epos.x(), epos.y(),epos.z());
	glUniform1f(pointlight_attenuation_constant, 0.1f);
	glUniform1f(pointlight_attenuation_linear, 0.1f);
	glUniform1f(pointlight_attenuation_exp,0.05f);
	glUniform3f(pointlight_light_color,1.0f,1.0f,1.0f);
	glUniform1f(pointlight_light_ambient_intensity,0.1f);
	glUniform1f(pointlight_light_diffuse_intensity,0.4f);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (GLvoid*)(2*sizeof(GL_FLOAT)));
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT)));

	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, texObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

void XGLModel::PointLight::initShader()
{
	program = glCreateProgram();
	if (!program)
	{
		XGLERROR("create program failed!");
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	if (!vs)
	{
		XGLERROR("create vertex shader failed!");
	}
	readShader("../../XGLModel/PointLight.vert", vsSource);
	addShader(program, vs, vsSource);
	postViewMsg(1, vsSource);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fs)
	{
		XGLERROR("create fragment shader failed!");
	}
	readShader("../../XGLModel/PointLight.frag", fsSource);
	addShader(program, fs, fsSource);
	postViewMsg(4, fsSource);

	linkProgram();

	g_mv = glGetUniformLocation(program, "mv");
	g_perspective = glGetUniformLocation(program, "perspective");
	g_sampler = glGetUniformLocation(program, "sampler2d");

	specular_Intensity = glGetUniformLocation(program,"specularIntensity");
	shineness = glGetUniformLocation(program, "shineness");
	pointlight_eposition = glGetUniformLocation(program, "pointLight.Eposition");
	pointlight_attenuation_constant = glGetUniformLocation(program, "pointLight.Attenuation.Constant");
	pointlight_attenuation_linear = glGetUniformLocation(program, "pointLight.Attenuation.Linear");
	pointlight_attenuation_exp = glGetUniformLocation(program, "pointLight.Attenuation.Exp");
	pointlight_light_color = glGetUniformLocation(program, "pointLight.Light.Color");
	pointlight_light_ambient_intensity = glGetUniformLocation(program, "pointLight.Light.AmbientIntensity");
	pointlight_light_diffuse_intensity = glGetUniformLocation(program, "pointLight.Light.DiffuseIntensity");

	if (g_mv && g_perspective && g_sampler < 0
	&& specular_Intensity
	&& shineness
	&& pointlight_eposition
	&& pointlight_attenuation_constant
	&& pointlight_attenuation_linear
	&& pointlight_attenuation_exp
	&& pointlight_light_color
	&& pointlight_light_ambient_intensity
	&& pointlight_light_diffuse_intensity)
	{
		XGLERROR("get uniform failed");
	}

	validateProgram();
}
