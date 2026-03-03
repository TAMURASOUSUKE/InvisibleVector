#pragma once
#include <unordered_map>
#include <string>
#include "ResourceConstant.h"


// 各ファイルデータを定義する

// 単体画像や3Dモデルを読み込むためのデータ
struct DataFile
{
	DataFile() {}

	DataFile(std::string _filePath) : filePath{ _filePath } {}

	// LoadGraph用のデータ
	std::string filePath{ "" };
};

// 画像かモデル化などを判別させるmap配列を用意
// リソースの種類をKey,リソースのデータをValueとしたものになっている
using SingleTextureMap = std::unordered_map<SingleTextureKind, DataFile>;
using ModelResourceMap = std::unordered_map<Model3DKind, DataFile>;
using PSShaderResourceMap = std::unordered_map<PSShaderKind, DataFile>;
using VSShaderResourceMap = std::unordered_map<VSShaderKind, DataFile>;
using FontResourceMap = std::unordered_map<FontKind, DataFile>;
// 使うシーンの種類をKey、テクスチャマップをValueとしたmap
using UseSingleTextureMap = std::unordered_map<ResourceUseScene, SingleTextureMap>;
using UseModelResourceMap = std::unordered_map<ResourceUseScene, ModelResourceMap>;
using UsePSShaderResourceMap = std::unordered_map<ResourceUseScene, PSShaderResourceMap>;
using UseVSShaderResourceMap = std::unordered_map<ResourceUseScene, VSShaderResourceMap>;
using UseFontResourceMap = std::unordered_map<ResourceUseScene, FontResourceMap>;
