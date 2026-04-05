#include <unordered_map>
#include <string>
#include "ResourceDataBase.h"

const SingleTextureMap* ResourceDataBase::GetSingleTexture(ResourceUseScene _useScene)
{
	if (singleTextures.count(_useScene) > 0)
	{
		return &singleTextures[_useScene];
	}

	return nullptr;
}

const ModelResourceMap* ResourceDataBase::GetModelResource(ResourceUseScene _useScene)
{
	if (modelResources.count(_useScene) > 0)
	{
		return &modelResources[_useScene];
	}

	return nullptr;
}

const FontResourceMap* ResourceDataBase::GetFontResource(ResourceUseScene _useScene)
{
	if (fontResources.count(_useScene) > 0)
	{
		return &fontResources[_useScene];
	}

	return nullptr;
}

const VSShaderResourceMap* ResourceDataBase::GetVSShaderResource(ResourceUseScene _useScene)
{
	if (vsShaderResources.count(_useScene) > 0)
	{
		return &vsShaderResources[_useScene];
	}
	return nullptr;
}

const PSShaderResourceMap* ResourceDataBase::GetPSShaderResource(ResourceUseScene _useScene)
{
	if (psShaderResources.count(_useScene) > 0)
	{
		return &psShaderResources[_useScene];
	}
	return nullptr;
}

void ResourceDataBase::MakeDataBase()
{
	MakeCommonDataBase();
	MakeTitleDataBase();
	MakeGameDataBase();
	MakeGameClearDataBase();
	MakeGameOverDataBase();
}

void ResourceDataBase::MakeCommonDataBase()
{
	// パーティクル風の役割を持つ箱はGameScene以外で使うのでCommon
	SingleTextureMap singleTexture
	{
		{SingleTextureKind::FloatingBox, {"Res/Title/TitleBackBox.png"}},
		{SingleTextureKind::CrackRuleMake, {"Res/Mask/EF_a_01_99/EF_a_hibi_01_02.png"}},
		{SingleTextureKind::DebugTestRnak, {"Res/Rank/TestRank.png"}}
	};

	RegisterSingleTexture(ResourceUseScene::Common, singleTexture);

	PSShaderResourceMap shaders
	{
		{PSShaderKind::Crack, {"Shader/Crack.pso"}},
	};

	RegisterPSShaderResource(ResourceUseScene::Common, shaders);
}

void ResourceDataBase::MakeTitleDataBase()
{
	// タイトルで使う画像を指定
	SingleTextureMap singleTexture
	{
		{SingleTextureKind::TitleLogo, {"Res/Title/UI_title_01_01/UI_title_01_01.png"}},
		{SingleTextureKind::TitleBackGround, {"Res/Title/Title_BG.png"}},
		{SingleTextureKind::TitleChangeSceneLogo, {"Res/Title/Click_to_Start.png"}},
	};

	RegisterSingleTexture(ResourceUseScene::Title, singleTexture); // 登録

	// モデル指定
	ModelResourceMap modelResource
	{
		{Model3DKind::Obstacles, {"Res/Game/BGG_01_kabe_01_01/BGG_01_kabe_01_02.mv1"}},
		{Model3DKind::Ground, {"Res/Game/BG_01_haikei_01_01/BG_01_haikei_02_02.mv1"}},
	};

	RegisterModelResource(ResourceUseScene::Title, modelResource);
}

void ResourceDataBase::MakeGameDataBase()
{
	// ゲームで使う画像を指定
	SingleTextureMap singleTexture
	{
		{SingleTextureKind::Reticle, {"Res/Game/UI_rt_01_01/UI_rt_01_01.png"}},
	};

	RegisterSingleTexture(ResourceUseScene::Game, singleTexture); // 登録

	// モデル指定
	ModelResourceMap modelResource
	{
		{Model3DKind::Obstacles, {"Res/Game/BGG_01_kabe_01_01/BGG_01_kabe_01_02.mv1"}},
		{Model3DKind::Ground, {"Res/Game/BG_01_haikei_01_01/BG_01_haikei_02_02.mv1"}},
		{Model3DKind::Gun, {"Res/Game/CR_01_jyuu_01_01/CR_01_jyuu_01_01.mv1"}},
		{Model3DKind::Target, {"Res/Game/P_01_target_01_01_01/P_01_target_01_01_01.mv1"}}
	};

	RegisterModelResource(ResourceUseScene::Game, modelResource);
}

void ResourceDataBase::MakeGameClearDataBase()
{
	// ゲームクリアで使う画像を指定
	SingleTextureMap singleTexture
	{
		// {SingleTextureKind::GameClearLogo, {"Res/GameClear/GameClear.png"}},
		{SingleTextureKind::GameClearBackGround, {"Res/GameClear/GameClear.png"}},
		{SingleTextureKind::GameClearChangeSceneLogo, {"Res/GameClear/Click_to_Title.png"}},
		{SingleTextureKind::GameClearLogo, {"Res/GameClear/UI_EV_01_clear_01_01.png"}},
		{SingleTextureKind::RankA, {"Res/Rank/RankA.png"}},
		{SingleTextureKind::RankB, {"Res/Rank/RankB.png"}},
		{SingleTextureKind::RankC, {"Res/Rank/RankC.png"}},
	};

	RegisterSingleTexture(ResourceUseScene::GameClaer, singleTexture); // 登録

	// モデル指定
	ModelResourceMap modelResource
	{
		{Model3DKind::Obstacles, {"Res/Game/BGG_01_kabe_01_01/BGG_01_kabe_01_02.mv1"}},
		{Model3DKind::Ground, {"Res/Game/BG_01_haikei_01_01/BG_01_haikei_02_02.mv1"}},
	};

	RegisterModelResource(ResourceUseScene::GameClaer, modelResource);

	FontResourceMap fontResource
	{
		{FontKind::Meiryo, {"メイリオ"}}
	};

	RegisterFontResource(ResourceUseScene::GameClaer, fontResource);
}

void ResourceDataBase::MakeGameOverDataBase()
{
	// ゲームオーバーで使う画像を指定
	SingleTextureMap singleTexture
	{
		// {SingleTextureKind::GameOverLogo, {"Res/GameOver/GameOver.png"}},
		{SingleTextureKind::GameOverBackGround, {"Res/GameOver/GameOver.png"}},
		{SingleTextureKind::GameOverChangeSceneLogo, {"Res/GameOver/Click_to_Title.png"}},
		{SingleTextureKind::GameOverLogo, {"Res/GameOver/UI_EV_01_over_01_01.png"}},
		{SingleTextureKind::RankA, {"Res/Rank/RankA.png"}},
		{SingleTextureKind::RankB, {"Res/Rank/RankB.png"}},
		{SingleTextureKind::RankC, {"Res/Rank/RankC.png"}},
	};

	RegisterSingleTexture(ResourceUseScene::GameOver, singleTexture); // 登録

	// モデル指定
	ModelResourceMap modelResource
	{
		{Model3DKind::Obstacles, {"Res/Game/BGG_01_kabe_01_01/BGG_01_kabe_01_02.mv1"}},

		{Model3DKind::Ground, {"Res/Game/BG_01_haikei_01_01/BG_01_haikei_02_02.mv1"}},
	};

	RegisterModelResource(ResourceUseScene::GameOver, modelResource);

	FontResourceMap fontResource
	{
		{FontKind::Meiryo, {"メイリオ"}}
	};

	RegisterFontResource(ResourceUseScene::GameOver, fontResource);
}

void ResourceDataBase::RegisterSingleTexture(ResourceUseScene _useScene, SingleTextureMap& _textures)
{
	// マップ内にある画像をシーンと連結させる
	for (auto& texture : _textures)
	{
		singleTextures[_useScene][texture.first] = texture.second;
	}
}

void ResourceDataBase::RegisterModelResource(ResourceUseScene _useScene, ModelResourceMap& _models)
{
	// マップ内にあるモデルとシーンを連結させる
	for (auto& model : _models)
	{
		modelResources[_useScene][model.first] = model.second;
	}
}

void ResourceDataBase::RegisterPSShaderResource(ResourceUseScene _useScene, PSShaderResourceMap& _psShaders)
{
	// マップ内にあるモデルとシーンを連結させる
	for (auto& shader : _psShaders)
	{
		psShaderResources[_useScene][shader.first] = shader.second;
	}
}

void ResourceDataBase::RegisterVSShaderResource(ResourceUseScene _useScene, VSShaderResourceMap& _vsShaders)
{
	// マップ内にあるモデルとシーンを連結させる
	for (auto& shader : _vsShaders)
	{
		vsShaderResources[_useScene][shader.first] = shader.second;
	}
}

void ResourceDataBase::RegisterFontResource(ResourceUseScene _useScene, FontResourceMap& _fonts)
{
	// マップ内にあるリソースと連結させる
	for (auto& font : _fonts)
	{
		fontResources[_useScene][font.first] = font.second;
	}
}