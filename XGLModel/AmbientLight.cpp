#include "stdafx.h"
#include "AmbientLight.h"
#include "Magick++.h"


namespace XGLModel {
	REGISTER(AmbientLight)
}

XGLModel::AmbientLight::AmbientLight()
{
}


XGLModel::AmbientLight::~AmbientLight()
{
}

void XGLModel::AmbientLight::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	initShader();

	GLfloat vertextexs[45] = {
		0.0f,0.0f,1.0f,
		0.0f,0.0f,
		1.0f,1.0f,0.0f,1.0f,

		1.0f,0.0f,0.0f,
		1.0f,0.0f,
		1.0f,0.0f,0.0f,1.0f,

		0.0f, 0.0f, -1.0f,
		0.0f,0.0f,
		1.0f,0.0f,0.0f,1.0f,

		-1.0f, 0.0f,0.0f,
		1.0f,0.0f,
		1.0f,0.0f,0.0f,1.0f,

		0.0f, 1.0f, 0.0f,
		0.5f,1.0f,
		1.0f,0.0f,0.0f,1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertextexs), vertextexs , GL_STATIC_DRAW);

	GLuint elements[18] = {
		0,1,4,
		1,2,4,
		3,4,2,
		3,0,4,
		3,2,0,
		2,1,0

	};

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements , GL_STATIC_DRAW);

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

	textureUnit = GL_TEXTURE0;
	textureIndex = 0;
	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void XGLModel::AmbientLight::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	ModelGL::draw();
	glUniform1i(u_sampler, textureIndex);
	glUniformMatrix4fv(u_mvp, 1, false, (cameraMatrix * projectMatrix).ptr());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 9, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 9, (void*)(sizeof(GL_FLOAT) * 3));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, (void*)(sizeof(GL_FLOAT) * 5));
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void XGLModel::AmbientLight::initShader()
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
	readShader("../../XGLModel/ambientLight.vs", vsSource);
	addShader(program, vs, vsSource);
	postViewMsg(1, vsSource);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fs)
	{
		XGLERROR("create fragment shader failed!");
	}
	readShader("../../XGLModel/ambientLight.fs", fsSource);
	addShader(program, fs, fsSource);
	postViewMsg(4, fsSource);

	linkProgram();
	u_mvp = glGetUniformLocation(program, "mvp");
	if (u_mvp < 0)
	{
		XGLERROR("get unifrom mvp failed!");
	}

	u_sampler = glGetUniformLocation(program, "sampler");
	if (u_sampler < 0)
	{
		XGLERROR("get unifrom sampler failed!");
	}

	u_ambient = glGetUniformLocation(program, "ambient");
	if (u_ambient < 0)
	{
		XGLERROR("get unifrom ambient failed!");
	}
	validateProgram();

	glUseProgram(program);
}
