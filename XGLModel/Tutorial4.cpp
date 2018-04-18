#include "stdafx.h"
#include "Tutorial4.h"

using namespace XGLModel;
REGISTER(Tutorial4)


Tutorial4::Tutorial4()
{
}


Tutorial4::~Tutorial4()
{
}

void XGLModel::Tutorial4::init()
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
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAttribs), vertexAttribs, GL_STATIC_DRAW);



	initShader();
}

void XGLModel::Tutorial4::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

}

void XGLModel::Tutorial4::initShader()
{
	program = glCreateProgram();
	if (!program)
	{
		XGLERROR("error create program");
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	if (!vs)
	{
		XGLERROR("error create shader");
		return;
	}

	readShader("..\\..\\XGLModel\\tutorial4.vs", vsSource);
	addShader(program, vs, vsSource);
	postViewMsg(1, vsSource);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (!fs)
	{
		XGLERROR("error create shader");
		return;
	}
	readShader("..\\..\\XGLModel\\tutorial4.fs",fsSource);
	addShader(program, fs,fsSource);
	postViewMsg(4, fsSource);

	linkProgram();
	validateProgram();

	glUseProgram(program);

}

int XGLModel::Tutorial4::onMessage(int id, std::string & msg)
{
	return 0;
}




