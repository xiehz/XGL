#include "stdafx.h"
#include "SpecularLight.h"
#include "Vertex.h"
#include "Magick++.h"

namespace XGLModel {

	REGISTER(SpecularLight)

}
XGLModel::SpecularLight::SpecularLight()
{
}


XGLModel::SpecularLight::~SpecularLight()
{
}

void XGLModel::SpecularLight::init()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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

void XGLModel::SpecularLight::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	ModelGL::draw();

	glUniform1i(u_sampler2d, texUnitIndex);
	glUniformMatrix4fv(u_mv, 1, false, cameraMatrix.ptr());
	glUniformMatrix4fv(u_perspective, 1, false, projectMatrix.ptr());

	glUniform1f(u_dlight_ambient_density, 0.01f);
	glUniform1f(u_dlight_diffuse_density, 0.99f);
	glUniform3f(u_dlight_ambient, 1.0f, 1.0f, 1.0f);
	glUniform3f(u_dlight_diffuse, 1.0f, 0.0f, 0.0f);
	glUniform3f(u_dlight_diffuse_direction, 0.0f, -1.0f, 0.0f);
	glUniform1f(u_dlight_specular_density, 0.2f);
	glUniform3f(u_dlight_specular, 1.0f, 1.0f, 1.0f);
	glUniform1f(u_dlight_specular_exp, 16.0f);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (void*)(sizeof(GL_FLOAT) * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (void*)(sizeof(GL_FLOAT) * 5));
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, texObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void XGLModel::SpecularLight::initShader()
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
	readShader("../../XGLModel/SpecularLight.vs", vsSource);
	addShader(program, vs, vsSource);
	postViewMsg(1, vsSource);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fs)
	{
		XGLERROR("create fragment shader failed!");
	}
	readShader("../../XGLModel/SpecularLight.fs", fsSource);
	addShader(program, fs, fsSource);
	postViewMsg(4, fsSource);

	linkProgram();
	u_mv = glGetUniformLocation(program, "mv");
	if (u_mv < 0)
	{
		XGLERROR("get unifrom mv failed!");
	}

	u_perspective = glGetUniformLocation(program, "perspective");
	if (u_perspective < 0)
	{
		XGLERROR("get unifrom perspective failed!");
	}


	u_sampler2d = glGetUniformLocation(program, "sampler2d");
	if (u_sampler2d < 0)
	{
		XGLERROR("get unifrom sampler2d failed!");
	}

	u_dlight_ambient = glGetUniformLocation(program, "dLight.Ambient");
	if (u_dlight_ambient < 0)
	{
		XGLERROR("get unifrom ambient failed!");
	}

	u_dlight_ambient_density = glGetUniformLocation(program, "dLight.AmbientDensity");
	if (u_dlight_ambient_density < 0)
	{
		XGLERROR("get unifrom ambient failed!");
	}

	u_dlight_diffuse_density = glGetUniformLocation(program, "dLight.DiffuseDensity");
	if (u_dlight_diffuse_density < 0)
	{
		XGLERROR("get unifrom diffuse density failed!");
	}

	u_dlight_diffuse = glGetUniformLocation(program, "dLight.Diffuse");
	if (u_dlight_diffuse < 0)
	{
		XGLERROR("get unifrom diffuse  failed!");
	}

	u_dlight_diffuse_direction = glGetUniformLocation(program, "dLight.DiffuseDirection");
	if (u_dlight_diffuse_direction < 0)
	{
		XGLERROR("get unifrom diffuse direction failed!");
	}

	u_dlight_specular = glGetUniformLocation(program, "dLight.Specular");
	if (u_dlight_specular < 0)
	{
		XGLERROR("get unifrom Specular failed!");
	}

	u_dlight_specular_density = glGetUniformLocation(program, "gMaterialDensity");
	if (u_dlight_specular_density < 0)
	{
		XGLERROR("get unifrom  SpecularDensity failed!");
	}

	u_dlight_specular_exp = glGetUniformLocation(program, "gMaterialExp");
	if (u_dlight_specular_exp < 0)
	{
		XGLERROR("get unifrom SpecularExp  failed!");
	}

	validateProgram();

	glUseProgram(program);

}
