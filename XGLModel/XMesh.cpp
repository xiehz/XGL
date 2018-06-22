#include "stdafx.h"
#include <assert.h>
#include "assimp/postprocess.h"
#include "XGLUtil.h"
#include "XMesh.h"


XGLModel::XMesh::XMesh()
{
}


XGLModel::XMesh::~XMesh()
{
}

bool XGLModel::XMesh::LoadMesh(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace
	);
	if (pScene) {
		Ret = InitFromScene(pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
	}

	return Ret;
}

void XGLModel::XMesh::InitMesh(unsigned int Index, const aiMesh* paiMesh)
{
	m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;

	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		const aiVector3D* pTangent = &(paiMesh->mTangents[i]);
		const aiVector3D* pBinTangent = &(paiMesh->mBitangents[i]);

		Vertex v(XGL::Vec3f(pPos->x, pPos->y, pPos->z),
			XGL::Vec2f(pTexCoord->x, pTexCoord->y),
			XGL::Vec3f(pNormal->x, pNormal->y, pNormal->z),
			XGL::Vec3f(pTangent->x, pTangent->y, pTangent->z),
			XGL::Vec3f(pBinTangent->x, pBinTangent->y, pBinTangent->z)
		);

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[Index].Init(Vertices, Indices);
}

void XGLModel::XMesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);                 // position
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // texture coordinate
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); // normal
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32); // tangent
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)44); // bintangent

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_pTextures.size() && m_pTextures[MaterialIndex]) {
			m_pTextures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}
