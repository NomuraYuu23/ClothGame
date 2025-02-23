#include "ModelLoader.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include <format>
#include "../../base/BufferResource.h"

using namespace DirectX;
using namespace Microsoft::WRL;

std::vector<std::pair<std::string, Matrix4x4>> ModelLoader::boneOffsetMatrixes_;

//objファイルを読む
Model::ModelData ModelLoader::LoadModelFile(const std::string& directoryPath, const std::string& filename) {

	// 変数宣言
	Model::ModelData modelData; // 構築するModelData

	// シーン構築
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	// 読み込み時のオプション
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない

	// オフセット用配列をクリア
	boneOffsetMatrixes_.clear();
	
	// ファイルを識別
	bool thisObj = filename.find(".obj") != -1;

	// メッシュ解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応

		// ボーン解析
		std::vector<std::vector<std::pair<uint32_t, float>>> boneDatas;
		if (mesh->HasBones()) {
			aiBone** bone = mesh->mBones;
			boneDatas.resize(mesh->mNumBones);
			for (uint32_t i = 0; i < mesh->mNumBones; ++i) {

				aiVertexWeight* weights = bone[i]->mWeights;
				for (uint32_t j = 0; j < bone[i]->mNumWeights; ++j) {
					
					if (weights[j].mWeight == 0.0f) {
						continue;
					}

					std::pair<uint32_t, float> boneData;
					boneData.first = weights[j].mVertexId;
					boneData.second = weights[j].mWeight;
					boneDatas[i].push_back(boneData);

				}
				aiString name = bone[i]->mName;
				aiMatrix4x4 aiMatrix = bone[i]->mOffsetMatrix;

				aiVector3D scale, translate;
				aiQuaternion rotate;
				aiMatrix.Decompose(scale, rotate, translate);

				std::pair<std::string, Matrix4x4> boneOffsetMatrix;
				boneOffsetMatrix.first = name.C_Str();
				boneOffsetMatrix.second =
					Matrix4x4::MakeAffineMatrix(
						{ scale.x, scale.y, scale.z },
						{ rotate.x, -rotate.y, -rotate.z, rotate.w },
						{ -translate.x,translate.y, translate.z }
				);
				boneOffsetMatrixes_.push_back(boneOffsetMatrix);

			}
		}

		// 頂点解析
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

			VertexData vertex;
			vertex.position = { -position.x, position.y, position.z, 1.0f };
			vertex.normal = { -normal.x, normal.y, normal.z };
			vertex.texcoord = { texcoord.x, texcoord.y };

			// テクスチャを移動
			if (thisObj) {
				vertex.texcoord.x += (mesh->mMaterialIndex - 1) * 2.0f;
			}
			else {
				vertex.texcoord.x += mesh->mMaterialIndex * 2.0f;
			}

			VertexInfluence vertexInfluence;

			// ボーン情報取得
			if (mesh->HasBones()) {
				for (uint32_t i = 0; i < kNumMaxInfluence; ++i) {
					vertexInfluence.weights[i] = 0.0f;
					vertexInfluence.indecis[i] = 1000;
				}

				// 重みデータ
				for (uint32_t i = 0; i < boneDatas.size(); ++i) {
					for (uint32_t j = 0; j < boneDatas[i].size(); ++j) {
						if (boneDatas[i][j].first == vertexIndex) {
							// 空いている場所
							for (uint32_t k = 0; k < kNumMaxInfluence; ++k) {
								if (vertexInfluence.weights[k] == 0.0f) {
									vertexInfluence.weights[k] = boneDatas[i][j].second;
									vertexInfluence.indecis[k] = i + scene->mNumMeshes + 1;
									break;
								}
							}
						}
					}
				}
			}
			else {
				for (uint32_t i = 1; i < kNumMaxInfluence; ++i) {
					vertexInfluence.weights[i] = 0.0f;
					vertexInfluence.indecis[i] = 1000;
				}
				vertexInfluence.weights[0] = 1.0f;
				if (scene->mNumMeshes == 1) {
					vertexInfluence.indecis[0] = meshIndex; // 親ノード分＋1
				}
				else {
					vertexInfluence.indecis[0] = meshIndex + 1; // 親ノード分＋1
				}
			}

			modelData.vertices.push_back(vertex);
			modelData.vertexInfluences.push_back(vertexInfluence);

		}

		// フェイス解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);// 三角形のみサポート

			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				modelData.indices.push_back(vertexIndex);
			}

		}

	}

	// ノード解析
	modelData.rootNode = ReadNode(scene->mRootNode);

	// マテリアル解析
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePaths.push_back(directoryPath + "/" + textureFilePath.C_Str());
		}
	}

	// ノードアニメーション解析
	if (scene->HasAnimations()) {
		// アニメーション配列確認
		aiAnimation** animation = scene->mAnimations;

		modelData.animations.resize(scene->mNumAnimations);
		for (uint32_t i = 0; i < scene->mNumAnimations; ++i) {
			modelData.animations[i].endTime_ = 0;
			for (uint32_t j = 0; j < animation[i]->mNumChannels; ++j) {
				NodeAnimationData nodeAnimationData;
				assert(animation[i]->mChannels[j]);
				nodeAnimationData.Initialize(*animation[i]->mChannels[j]);
				
				if (modelData.animations[i].endTime_ < nodeAnimationData.endTime_) {
					modelData.animations[i].endTime_ = nodeAnimationData.endTime_;
				}

				modelData.animations[i].nodeAnimationDatas_.push_back(nodeAnimationData);
			}
			modelData.animations[i].nodeAnimationNum_ = static_cast<uint32_t>(modelData.animations[i].nodeAnimationDatas_.size());
		}
	}
	else {
		modelData.nodeAnimationNum = 0;
	}

	return modelData;

}

ModelNode ModelLoader::ReadNode(aiNode* node)
{

	ModelNode result;

	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiVector3D scale, translate;
	aiQuaternion rotate;
	aiLocalMatrix.Decompose(scale, rotate, translate);

	result.initTransform.scale = { scale.x, scale.y, scale.z };
	result.initTransform.rotate = { rotate.x, -rotate.y, -rotate.z, rotate.w };
	result.initTransform.translate = { -translate.x,translate.y, translate.z };

	result.localMatrix =
		Matrix4x4::MakeAffineMatrix(
			result.initTransform.scale,
			result.initTransform.rotate,
			result.initTransform.translate
	);

	result.offsetMatrix = Matrix4x4::MakeIdentity4x4();
	if (!boneOffsetMatrixes_.empty()) {
		for (uint32_t i = 0; i < boneOffsetMatrixes_.size(); ++i) {
			if (boneOffsetMatrixes_[i].first == node->mName.C_Str()) {
				result.offsetMatrix = boneOffsetMatrixes_[i].second;
			}
		}
	}

	result.name = node->mName.C_Str(); // Node名を格納
	result.children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的に読んで階層構造を作る
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	if (node->mMeshes) {
		result.meshNum = *node->mMeshes;
	}
	else {
		result.meshNum = 0;
	}

	return result;

}
