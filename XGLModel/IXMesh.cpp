#include "stdafx.h"
#include <assert.h>
#include <iostream>
#include "xgl/Vec2f"
#include "xgl/Vec3f"
#include "assimp/postprocess.h"
#include "Picking.h"
#include "IXMesh.h"
#include "XGLUtil.h"

XGLModel::IXMesh::IXMesh()
{
	m_picking = 0;
}


XGLModel::IXMesh::~IXMesh()
{
	Clear();
}

void XGLModel::IXMesh::AcceptPicking(Picking * pick)
{
	m_picking = pick;
}

void XGLModel::IXMesh::ApplyPicking(unsigned int drawcall)
{
	if(m_picking)
		glUniform1ui(m_picking->g_drawcallindex, drawcall);
}

bool XGLModel::IXMesh::LoadMesh(const std::string & filename)
{
	Clear();
	bool ret = false;
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filename.c_str(),
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	
	if (pScene)
	{
		ret = InitFromScene(pScene, filename);
	}
	else {
		char error[65535];
		sprintf_s(error, "Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
		XGLERROR(error);
	}

	return ret;
}

void XGLModel::IXMesh::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

		if (MaterialIndex < m_pTextures.size() && m_pTextures[MaterialIndex]) {
			m_pTextures[MaterialIndex]->Bind(GL_TEXTURE0);
		}

		ApplyPicking(i);

		glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

bool XGLModel::IXMesh::InitFromScene(const aiScene * pScene, const std::string & filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_pTextures.resize(pScene->mNumMaterials);

	const aiMesh* pMesh;
	for (unsigned int i = 0; i < pScene->mNumMeshes; ++i)
	{
		pMesh = pScene->mMeshes[i];
		InitMesh(i, pMesh);
	}

	return InitMaterials(pScene, filename);
}

void XGLModel::IXMesh::InitMesh(unsigned int index, const aiMesh * pMesh)
{
	m_Entries[index].MaterialIndex = pMesh->mMaterialIndex;
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;

	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(pMesh->mVertices[i]);
		const aiVector3D* pNormal = &(pMesh->mNormals[i]);
		const aiVector3D* pTexCoord = pMesh->HasTextureCoords(0) ? &(pMesh->mTextureCoords[0][i]) : &Zero3D;

		Vertex v(XGL::Vec3f(pPos->x, pPos->y, pPos->z),
			XGL::Vec2f(pTexCoord->x, pTexCoord->y),
			XGL::Vec3f(pNormal->x, pNormal->y, pNormal->z));

		Vertices.push_back(v);
	}

	for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
		const aiFace& Face = pMesh->mFaces[i];
		//assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}

	m_Entries[index].Init(Vertices, Indices);
}

bool XGLModel::IXMesh::InitMaterials(const aiScene * pScene, const std::string & filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_pTextures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string FullPath = Dir + "/" + Path.data;
				m_pTextures[i] = new XTexture(GL_TEXTURE_2D, FullPath.c_str());

				if (!m_pTextures[i]->Load()) {
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_pTextures[i];
					m_pTextures[i] = NULL;
					Ret = false;
				}
				else {
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
		}
	}

	return Ret;
}

void XGLModel::IXMesh::Clear()
{
	for (unsigned int i = 0; i < m_pTextures.size(); i++) {
		if (m_pTextures[i] != nullptr)
		{
			delete m_pTextures[i];
			m_pTextures[i] = nullptr;
		}
	}
}

XGLModel::MeshEntry::MeshEntry()
{
	this->VB = INVALID_OGL_VALUE;
	this->IB = INVALID_OGL_VALUE;

	this->NumIndices = 0;
	this->MaterialIndex = INVALID_MATERIAL;
}

XGLModel::MeshEntry::~MeshEntry()
{
	if (this->VB != INVALID_OGL_VALUE) {
		glDeleteBuffers(1, &this->VB);
	}

	if (this->IB != INVALID_OGL_VALUE)
	{
		glDeleteBuffers(1, &this->IB);
	}
}

void XGLModel::MeshEntry::Init(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices)
{
	this->NumIndices = indices.size();

	glGenBuffers(1, &this->VB);
	glBindBuffer(GL_ARRAY_BUFFER, this->VB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &this->IB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* this->NumIndices, &indices[0], GL_STATIC_DRAW);

}

void XGLModel::IXMesh::Render(unsigned int DrawIndex, unsigned int PrimID)
{
	assert(DrawIndex < m_Entries.size());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, m_Entries[DrawIndex].VB);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[DrawIndex].IB);

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (const GLvoid*)(PrimID * 3 * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}