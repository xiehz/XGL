#include "stdafx.h"
#include <assert.h>
#include "assimp/postprocess.h"
#include "BoreoheMesh.h"

using namespace XGLModel;


BoreoheMesh::BoreoheMesh()
{
	m_VAO = 0; 
	memset(m_Buffers, 0, sizeof(m_Buffers));
}


BoreoheMesh::~BoreoheMesh()
{
	Clear();
}


void BoreoheMesh::Clear()
{
	if (m_Buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	}

	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}


bool BoreoheMesh::LoadMesh(unsigned int count,float radius)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	std::vector<XGL::Vec3f> Positions;
	std::vector<XGL::Vec3f> normals;

	unsigned int NumVertices = count *2 +2;
	m_Count = NumVertices;

	Positions.reserve(NumVertices);
	normals.reserve(NumVertices);

	float a = 2 * 3.1415926 / count;

	for (int i = 0; i <= count; ++i)
	{
		float x = cosf(a *i);
		float z = sinf(a *i);
		float y = 0;
		Positions.push_back(XGL::Vec3f(x, y, z));
		normals.push_back(XGL::Vec3f(x,0,z));

		y = -1;
		Positions.push_back(XGL::Vec3f(x, y, z));
		normals.push_back(XGL::Vec3f(x, 0, z));
	}




	// Generate and populate the buffers with vertex attributes and the indices

	int pos_location = 0;
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[pos_location]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(pos_location);
	glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int nor_location = 1;
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[nor_location]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(nor_location);
	glVertexAttribPointer(nor_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	int world_location = 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[world_location]);
	const unsigned int size = sizeof(GLfloat) * 16;
	for (unsigned int i = 0; i < 4; i++) {
		glEnableVertexAttribArray(world_location + i);
		glVertexAttribPointer(world_location + i, 4, GL_FLOAT, GL_FALSE, size,
			(const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(world_location + i, 1);
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
	return GLCheckError();
}


void BoreoheMesh::Render(unsigned int NumInstances, const float* WorldMats)
{
	const unsigned int size = sizeof(GLfloat) * 16;

	int world_location = 2;
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[world_location]);
	glBufferData(GL_ARRAY_BUFFER, size* NumInstances, WorldMats, GL_DYNAMIC_DRAW);

	glBindVertexArray(m_VAO);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0,m_Count,NumInstances);
	glBindVertexArray(0);
	GLCheckError();
}


