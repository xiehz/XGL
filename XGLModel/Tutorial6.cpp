#include "stdafx.h"
#include "Tutorial6.h"
#include "Magick++.h"

using namespace XGLModel;
REGISTER(Tutorial6)

Tutorial6::Tutorial6()
{
}


Tutorial6::~Tutorial6()
{
}

void XGLModel::Tutorial6::init()
{
	glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	textureUnit = GL_TEXTURE0;
	textureUnitIndex = 0;

	GLfloat vertextexs[25] = {
		0.0f,0.0f,1.0f,
		0.0f,0.0f,

		1.0f,0.0f,0.0f,
		1.0f,0.0f,

		0.0f, 0.0f, -1.0f,
		0.0f,0.0f,

		-1.0f, 0.0f,0.0f,
		1.0f,0.0f,

		0.0f, 1.0f, 0.0f,
		0.5f,1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertextexs), vertextexs, GL_STATIC_DRAW);

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

	glGenTextures(1, &textureObj);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.columns(), image.rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, blob.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	initShader();
}

void XGLModel::Tutorial6::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	Matrix4 mvp =  projectMatrix * cameraMatrix;
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE,mvp.get() );

	glUniform1i(uniform_sampler, textureUnitIndex);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (GLvoid*)(sizeof(GL_FLOAT) * 3));
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void XGLModel::Tutorial6::initShader()
{
	program = glCreateProgram();
	if (!program)
	{
		XGLERROR("create program failed");
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	if (!vs)
	{
		XGLERROR("create vertex shader failed");
	}
	readShader("../../XGLModel/tutorial6.vs", vsSource);
	addShader(program, vs, vsSource);
	postViewMsg(0, vsSource);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fs)
	{
		XGLERROR("create fragment shader failed");
	}
	readShader("../../XGLModel/tutorial6.fs", fsSource);
	addShader(program, fs, fsSource);
	postViewMsg(0, fsSource);


	linkProgram();
	uniform_mvp = glGetUniformLocation(program, "mvp");
	if (uniform_mvp < 0)
	{
		XGLERROR("get mvp uniform failed");
	}

	uniform_sampler = glGetUniformLocation(program, "sampler");
	if (uniform_sampler < 0)
	{
		XGLERROR("get sampler uniform failed");
	}

	validateProgram();
	glUseProgram(program);
}
