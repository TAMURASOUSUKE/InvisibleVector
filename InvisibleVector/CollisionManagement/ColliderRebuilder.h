#pragma once

#include <vector>
#include <unordered_map>
#include "Collider.h"

// 判定の作り直しを起こなう
class ColliderRebuilder
{
public:
	// 各Colliderの作り直し
	bool Rebuild(const ColliderBase* src, SphereCollider& dest); // 球
	bool Rebuild(const ColliderBase* src, BoxCollider& dest); // ボックス
	bool Rebuild(const ColliderBase* src, CapsuleCollider& dest); // カプセル

private:
	// 元データから作り直せるかどうかのチェック
	bool CanRebuild(const ColliderBase* src, ColliderType type);

	// 元データから必要な情報の抽出
	bool ExtractParameter(ColliderType type, const ColliderBase* src, std::unordered_map<ColliderParamKey, float>& parameters);
	
	// 各Colliderの作り直しに必要なデータの抽出
	void SetupRebuildParamKey(ColliderType tyep, std::vector<ColliderParamKey>& keys);
};