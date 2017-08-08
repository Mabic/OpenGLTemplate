#include "ModelLoader.hpp"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

ModelLoader::ModelLoader(const std::string& pathToFile)
{
	m_directory = GetCurrentDirectory(pathToFile);

	Assimp::Importer importer;

	auto postprocessFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;
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
		Material material;
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial* materialNode = scene->mMaterials[mesh->mMaterialIndex];

			material = GetMaterial(materialNode);

			for (unsigned int i = 0; i < materialNode->GetTextureCount(aiTextureType_DIFFUSE); i++)
			{
				aiString str;
				materialNode->GetTexture(aiTextureType_DIFFUSE, i, &str);
				textures.push_back(Texture(m_directory + "/" + str.C_Str()));
			}
		}

		m_meshes.push_back(Mesh(std::move(vertices), std::move(textures),
			                    std::move(indices), std::move(material)));
	}

	for (unsigned int childID = 0; childID < node->mNumChildren; ++childID) {
		ProcessNode(node->mChildren[childID], scene);
	}
}

std::string ModelLoader::GetCurrentDirectory(const std::string& path) const
{
	return path.substr(0, path.find_last_of("/"));
}

Material ModelLoader::GetMaterial(const aiMaterial* materialNode) const
{
	Material material;

	aiColor3D color(0.f);
	materialNode->Get(AI_MATKEY_COLOR_AMBIENT, color);
	material.m_ambient.r = color.r;
	material.m_ambient.g = color.g;
	material.m_ambient.b = color.b;

	materialNode->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	material.m_diffuse.r = color.r;
	material.m_diffuse.g = color.g;
	material.m_diffuse.b = color.b;

	materialNode->Get(AI_MATKEY_COLOR_SPECULAR, color);
	material.m_specular.r = color.r;
	material.m_specular.g = color.g;
	material.m_specular.b = color.b;

	float shinines = 0.0f;
	materialNode->Get(AI_MATKEY_SHININESS, shinines);
	material.m_shinines = shinines;

	return material;
}

ModelLoader::~ModelLoader()
{

}