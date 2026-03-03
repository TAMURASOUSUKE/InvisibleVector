#pragma once
#include <string>
#include <unordered_map>
#include "ResourceDataBase.h"
#include "ResourceConstant.h"


// 画像やモデルなどのリソースを管理する
class ResourceManager
{
public:
	// シングルトン化
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	// デストラクタで全削除を行う
	~ResourceManager()
	{
		DeleteAll();
	}

	void Load(ResourceUseScene _useScene); // 指定したシーンで必要になるリソースをロードする
	void Delete(ResourceUseScene _useScene); // 指定したシーンでのリソース削除

	int GetResourceHandle(SingleTextureKind _kind); // 単体画像の取得
	int GetResourceHandle(Model3DKind _kind); // モデルの取得
	int GetResourceHandle(FontKind _kind); // フォントの取得(サイズも併せて)
	int GetResourceHandle(PSShaderKind _kind); // PSシェーダーの取得 
	int GetResourceHandle(VSShaderKind _kind); // VSシェーダの取得

private:
	ResourceManager() = default;
	ResourceManager(const ResourceManager& other) = delete;

	void Load(const SingleTextureMap* _texture); // 単体画像読み込み
	void Load(const ModelResourceMap* _models); // モデル読み込み
	void Load(const FontResourceMap* _fonts); // フォント読み込み
	void Load(const PSShaderResourceMap* _psShaders); // シェーダー読み込み
	void Load(const VSShaderResourceMap* _vsShaders); // シェーダー読み込み

	void DeleteAll(); // 全リソース削除

private:
	// リソースハンドル保存用
	std::unordered_map<SingleTextureKind, int> singleGraphHandles{}; // 画像
	std::unordered_map<Model3DKind, int> modelHandles{}; // 3Dモデル
	std::unordered_map<PSShaderKind, int> psShaderHandle{};// PSシェーダー
	std::unordered_map<VSShaderKind, int> vsShaderHandle{};// VSシェーダー
	std::unordered_map<FontKind, int> fontHandle{}; // フォント
	// ファイル情報
	ResourceDataBase dataBase{};
};