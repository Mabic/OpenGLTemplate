#include "ModelLoader.hpp"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

ModelLoader::ModelLoader(const std::string& pathToFile)
{
	m_directory = pathToFile.substr(0, pathToFile.find_last_of("\\"));

	Assimp::Importer importer;

	auto postprocessFlags = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs;
	const aiScene* scene = importer.ReadFile(pathToFile, postprocessFlags);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene);
}

void ModelLoader::ProcessNode(const aiNode* node, const aiScene* scene)
{
	for (unsigned int meshID = 0; meshID < node->mNumMeshes; ++meshID) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[meshID]];

		std::vector<Vertex> vertices;
		for (unsigned int vertexID = 0; vertexID < mesh->mNumVertices; ++vertexID) {
			glm::vec3 position;
			position.x = mesh->mVertices[vertexID].x;
			position.y = mesh->mVertices[vertexID].y;
			position.z = mesh->mVertices[vertexID].z;

			glm::vec3 normals;
			normals.x = mesh->mNormals[vertexID].x;
			normals.y = mesh->mNormals[vertexID].y;
			normals.z = mesh->mNormals[vertexID].z;

			glm::vec2 textureCoordinates;
			if (mesh->mTextureCoords[0]) {
				textureCoordinates.x = mesh->mTextureCoords[0][vertexID].x;
				textureCoordinates.y = mesh->mTextureCoords[0][vertexID].y;
			} else {
				textureCoordinates = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(Vertex(std::move(position), std::move(textureCoordinates), std::move(normals)));
		}

		std::vector<unsigned int> indices;
		for (unsigned int faceID = 0; faceID < mesh->mNumFaces; ++faceID) {
			aiFace face = mesh->mFaces[faceID];
			for (unsigned int indiceID = 0; indiceID < face.mNumIndices; ++indiceID) {
				indices.push_back(face.mIndices[indiceID]);
			}
		}

		std::vector<Texture> textures;
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
			{
				aiString str;
				material->GetTexture(aiTextureType_DIFFUSE, i, &str);
				textures.push_back(Texture(m_directory + "\\" + str.C_Str()));
			}
		}

		m_meshes.push_back(Mesh(std::move(vertices), std::move(textures), std::move(indices)));
	}

	for (unsigned int childID = 0; childID < node->mNumChildren; ++childID) {
		ProcessNode(node->mChildren[childID], scene);
	}
}

ModelLoader::~ModelLoader()
{

}