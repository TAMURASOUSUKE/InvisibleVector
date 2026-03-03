#pragma once
#include "ResourceData.h"
#include "ResourceConstant.h"


// リソース管理用のデータベース
class ResourceDataBase
{
public:
	ResourceDataBase()
	{
		MakeDataBase(); // コンストラクタで一気に作成する
	}

public:
	const SingleTextureMap* GetSingleTexture(ResourceUseScene _useScene); // 画像単体のGetter
	const ModelResourceMap* GetModelResource(ResourceUseScene _useScene); // モデルを取得するGetter
	const FontResourceMap* GetFontResource(ResourceUseScene _useScene); // フォントを取得
	const VSShaderResourceMap* GetVSShaderResource(ResourceUseScene _useScene); // 頂点シェーダ
	const PSShaderResourceMap* GetPSShaderResource(ResourceUseScene _useScene); // ピクセルシェーダ

private:
	void MakeDataBase(); // DataBase作成を取りまとめる
	// シーンごとにDataBaseを作る
	void MakeCommonDataBase(); // 汎用DataBase
	void MakeTitleDataBase(); // タイトルDataBase
	void MakeGameDataBase(); // ゲーム用のDataBase
	void MakeGameOverDataBase(); // ゲームオーバー用のDataBase
	void MakeGameClearDataBase(); // ゲームクリア用のDataBase

	// グラフィックスリソースごとのデータ作成関数
	void RegisterSingleTexture(ResourceUseScene _useScene, SingleTextureMap& _textures); // 単体画像 
	void RegisterModelResource(ResourceUseScene _useScene, ModelResourceMap& _models); // モデルリソース
	void RegisterPSShaderResource(ResourceUseScene _useScene, PSShaderResourceMap& _psShaders); // PSシェーダーリソース
	void RegisterVSShaderResource(ResourceUseScene _useScene, VSShaderResourceMap& _vsShaders); // VSシェーダーリソース
	void RegisterFontResource(ResourceUseScene _useScene, FontResourceMap& _fonts); // フォントリソース

private:
	// ファイル情報保存用変数
	UseSingleTextureMap singleTextures{}; // 単体画像
	UseModelResourceMap modelResources{}; // モデル
	UsePSShaderResourceMap psShaderResources{}; // PSシェーダー
	UseVSShaderResourceMap vsShaderResources{}; // VSシェーダー
	UseFontResourceMap fontResources{}; // フォント

};