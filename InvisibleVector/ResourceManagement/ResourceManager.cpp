#include "DxLib.h"
#include "GameConstant.h"
#include <string>
#include <algorithm>
#include "ResourceManager.h"

void ResourceManager::Load(ResourceUseScene _useScene)
{
	// 指定されたシーンで読み込むファイルをデータとして持つ
	const SingleTextureMap* singleTextures{ dataBase.GetSingleTexture(_useScene) };
	const ModelResourceMap* modelResources{ dataBase.GetModelResource(_useScene) };
	const FontResourceMap* fontResourceces{ dataBase.GetFontResource(_useScene) };
	const VSShaderResourceMap* vsShaderResourceMap{ dataBase.GetVSShaderResource(_useScene) };
	const PSShaderResourceMap* psShaderResourceMap{ dataBase.GetPSShaderResource(_useScene) };

	// 読み込み
	Load(vsShaderResourceMap);
	Load(psShaderResourceMap);
	Load(singleTextures);
	Load(modelResources);
	Load(fontResourceces);
}

void ResourceManager::Delete(ResourceUseScene _useScene)
{
	// 指定されたシーンのリソースを削除する
	const SingleTextureMap* singleTextures{ dataBase.GetSingleTexture(_useScene) }; // 削除するデータの取得

	if (singleTextures != nullptr)
	{
		for (auto& fileData : *singleTextures)
		{
			if (singleGraphHandles.count(fileData.first) == 0)
			{
				continue;
			}

			DeleteGraph(singleGraphHandles[fileData.first]);
			singleGraphHandles.erase(fileData.first);
		}
	}

	const ModelResourceMap* modelResources{ dataBase.GetModelResource(_useScene) };

	if (modelResources != nullptr)
	{
		for (auto& fileData : *modelResources)
		{
			if (modelHandles.count(fileData.first) == 0)
			{
				continue;
			}

			MV1DeleteModel(modelHandles[fileData.first]); // モデルの削除
			modelHandles.erase(fileData.first);
		}
	}
}

void ResourceManager::Load(const SingleTextureMap* _textures)
{
	if (_textures == nullptr)
	{
		return;
	}

	// 重複チェック
	for (auto& fileData : *_textures)
	{
		if (singleGraphHandles.count(fileData.first) > 0)
		{
			continue;
		}

		int handle{ LoadGraph(fileData.second.filePath.c_str()) }; // 画像の読み込み

		if (handle != -1)
		{
			// 読み込みが通ったかを確認してから登録する
			singleGraphHandles[fileData.first] = handle;
		}
	}
}

void ResourceManager::Load(const ModelResourceMap* _models)
{
	if (_models == nullptr)
	{
		return;
	}

	// 重複チェック
	for (auto& fileData : *_models)
	{
		if (modelHandles.count(fileData.first) > 0)
		{
			continue;
		}

		int handle{ MV1LoadModel(fileData.second.filePath.c_str()) };

		if (handle != -1)
		{
			// 読み込み通ったか確認してから登録する
			modelHandles[fileData.first] = handle;
		}
	}
}

void ResourceManager::Load(const FontResourceMap* _fonts)
{
	if (_fonts == nullptr) return;

	// 重複チェック
	for (auto& fileData : *_fonts)
	{
		if (fontHandle.count(fileData.first) > 0)
		{
			continue;
		}

		// 一旦固定
		int handle{ CreateFontToHandle(fileData.second.filePath.c_str(), FONT_SIZE, FONT_THICK, DX_FONTTYPE_ANTIALIASING_8X8) };

		if (handle != -1)
		{
			// 読み込み通ったか確認してから登録する
			fontHandle[fileData.first] = handle;
		}

	}
}

void ResourceManager::Load(const VSShaderResourceMap* _vsShader)
{
	if (_vsShader == nullptr)
	{
		return;
	}

	// 重複チェック
	for (auto& fileData : *_vsShader)
	{
		if (vsShaderHandle.count(fileData.first) > 0)
		{
			continue;
		}

		int handle{ LoadVertexShader(fileData.second.filePath.c_str()) }; // VSシェーダーの読み込み

		if (handle != -1)
		{
			// 読み込みが通ったかを確認してから登録する
			vsShaderHandle[fileData.first] = handle;
		}
	}
}

void ResourceManager::Load(const PSShaderResourceMap* _psShader)
{
	if (_psShader == nullptr)
	{
		return;
	}

	// 重複チェック
	for (auto& fileData : *_psShader)
	{
		if (psShaderHandle.count(fileData.first) > 0)
		{
			continue;
		}

		int handle{ LoadPixelShader(fileData.second.filePath.c_str()) }; // VSシェーダーの読み込み

		if (handle != -1)
		{
			// 読み込みが通ったかを確認してから登録する
			psShaderHandle[fileData.first] = handle;
		}
	}
}

int ResourceManager::GetResourceHandle(SingleTextureKind _kind)
{
	if (singleGraphHandles.count(_kind) > 0)
	{
		return singleGraphHandles[_kind];
	}

	return -1;
}

int ResourceManager::GetResourceHandle(Model3DKind _kind)
{
	if (modelHandles.count(_kind) > 0)
	{
		return modelHandles[_kind];
	}

	return -1;
}

int ResourceManager::GetResourceHandle(FontKind _kind)
{
	if (fontHandle.count(_kind) > 0)
	{
		return fontHandle[_kind];
	}
	return -1;
}

int ResourceManager::GetResourceHandle(PSShaderKind _kind)
{
	if (psShaderHandle.count(_kind) > 0)
	{
		return psShaderHandle[_kind];
	}
	return -1;
}

int ResourceManager::GetResourceHandle(VSShaderKind _kind)
{
	if (vsShaderHandle.count(_kind) > 0)
	{
		return vsShaderHandle[_kind];
	}
	return -1;
}

void ResourceManager::DeleteAll()
{
	// 該当するシーンを全てリストアップして全削除を行う
	ResourceUseScene deleteScenes[]
	{
		ResourceUseScene::Common,
		ResourceUseScene::Debug,
		ResourceUseScene::Title,
		ResourceUseScene::Game,
		ResourceUseScene::GameClaer,
		ResourceUseScene::GameOver,
	};

	for (auto& scene : deleteScenes)
	{
		// 削除
		Delete(scene);
	}
}
