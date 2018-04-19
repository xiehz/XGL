#include "stdafx.h"
#include "Tutorial5.h"

using namespace XGLModel;
REGISTER(Tutorial5)
Tutorial5::Tutorial5()
{
}


Tutorial5::~Tutorial5()
{
}

void XGLModel::Tutorial5::init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	float vertexAttribs[21] = { -1.0f,-1.0f,0.0f,
		1.0f,0.0f,0.0f,1.0f,

		1.0f,-1.0f,0.0f,
		0.0f,1.0f,0.0f,1.0f,

		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAttribs), vertexAttribs, GL_STATIC_DRAW);

	initShader();
}

void XGLModel::Tutorial5::draw()
{
	static float fscale = 1.0f;

	glClear(GL_COLOR_BUFFER_BIT);
	glUniform1f(uniform_scale, fscale);

	//Ïà»ú¾ØÕóµÄÄæ
	glUniformMatrix4fv(uniform_model_matrix, 1, false, cameraMatrix.transpose().get());

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, (GLvoid*)(sizeof(GL_FLOAT)*3));

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void XGLModel::Tutorial5::initShader()
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
	readShader("../../XGLModel/tutorial5.vs", vsSource);
	addShader(program,vs,vsSource);
	postViewMsg(1, vsSource);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fs)
	{
		XGLERROR("create fragment shader failed!");
	}
	readShader("../../XGLModel/tutorial5.fs", fsSource);
	addShader(program, fs, fsSource);
	postViewMsg(4, fsSource);


	linkProgram();

	uniform_scale = glGetUniformLocation(program, "fscale");
	if (uniform_scale < 0)
	{
		XGLERROR("get uniform failed");
	}

	uniform_model_matrix = glGetUniformLocation(program, "modelview");
	if (uniform_model_matrix < 0)
	{
		XGLERROR("get modelview uniform failed");
	}
	validateProgram();

	glUseProgram(program);
}
