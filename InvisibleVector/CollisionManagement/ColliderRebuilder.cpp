#include "ColliderRebuilder.h"

// 球の作り直し
bool ColliderRebuilder::Rebuild(const ColliderBase* src, SphereCollider& dest)
{
	if (CanRebuild(src, dest.GetType())) return false;

	std::unordered_map<ColliderParamKey, float> parameters{};
	if (!ExtractParameter(dest.GetType(), src, parameters)) return false;

	// パラメータ
	dest.pos = Vector3
	{
		parameters[ColliderParamKey::PosX],
		parameters[ColliderParamKey::PosY],
		parameters[ColliderParamKey::PosZ],
	};
	dest.radius = parameters[ColliderParamKey::Radius];

	return true; // trueを返せば作り直し成功

}

// Boxの作り直し
bool ColliderRebuilder::Rebuild(const ColliderBase* src, BoxCollider& dest)
{
	if (!CanRebuild(src, dest.GetType())) return false;

	std::unordered_map<ColliderParamKey, float> parameters{};
	if (!ExtractParameter(dest.GetType(), src, parameters)) return false;

	// パラメータの反映
	dest.pos = Vector3
	{
		parameters[ColliderParamKey::PosX],
		parameters[ColliderParamKey::PosY],
		parameters[ColliderParamKey::PosZ],
	};

	dest.size = Vector3
	{
		parameters[ColliderParamKey::Width],
		parameters[ColliderParamKey::Height],
		parameters[ColliderParamKey::Depth],
	};

	dest.rotate = Vector3
	{
		parameters[ColliderParamKey::RotaX],
		parameters[ColliderParamKey::RotaY],
		parameters[ColliderParamKey::RotaZ],
	};

	return true;
}

// カプセル
bool ColliderRebuilder::Rebuild(const ColliderBase* src, CapsuleCollider& dest)
{
	if (!CanRebuild(src, dest.GetType())) return false;

	std::unordered_map<ColliderParamKey, float> parameter{};
	if (!ExtractParameter(dest.GetType(), src, parameter)) return false;

	// パラメータ
	dest.startPos = Vector3
	{
		parameter[ColliderParamKey::PosX],
		parameter[ColliderParamKey::PosY],
		parameter[ColliderParamKey::PosZ],
	};

	dest.endPos = Vector3
	{
		parameter[ColliderParamKey::EndPosX],
		parameter[ColliderParamKey::EndPosY],
		parameter[ColliderParamKey::EndPosZ],
	};

	dest.rotate = Vector3
	{
		parameter[ColliderParamKey::RotaX],
		parameter[ColliderParamKey::RotaY],
		parameter[ColliderParamKey::RotaZ],
	};

	dest.radius = parameter[ColliderParamKey::Radius];
	return true;
}


bool ColliderRebuilder::ExtractParameter(ColliderType type, const ColliderBase* src, std::unordered_map<ColliderParamKey, float>& parameters)
{
	src->ExtractParameter(parameters);

	std::vector<ColliderParamKey> keys{};
	SetupRebuildParamKey(type, keys);

	// 情報がすべてそろっているかチェック
	for (auto& key : keys)
	{
		if (parameters.count(key) == 0)
		{
			return false; // 必要な情報が欠如している
		}
	}
	return true;

}

bool ColliderRebuilder::CanRebuild(const ColliderBase* src, ColliderType type)
{
	if (src == nullptr) return false; // ベースに何もなかったらfalse
	if (src->GetType() != type) return false; // BaseのTypeと調べるタイプが一致していなかったらfalse
	return true; // ここまで到達したら作り直せると判断する

}

void ColliderRebuilder::SetupRebuildParamKey(ColliderType type, std::vector<ColliderParamKey>& key)
{
	switch (type)
	{
	case ColliderType::Sphere:
		key.push_back(ColliderParamKey::PosX);
		key.push_back(ColliderParamKey::PosY);
		key.push_back(ColliderParamKey::PosZ);
		key.push_back(ColliderParamKey::Radius);
		break;

	case ColliderType::Box:
		key.push_back(ColliderParamKey::PosX);
		key.push_back(ColliderParamKey::PosY);
		key.push_back(ColliderParamKey::PosZ);
		key.push_back(ColliderParamKey::Width);
		key.push_back(ColliderParamKey::Height);
		key.push_back(ColliderParamKey::Depth);
		key.push_back(ColliderParamKey::RotaX);
		key.push_back(ColliderParamKey::RotaY);
		key.push_back(ColliderParamKey::RotaZ);
		break;

	case ColliderType::Capsule:
		key.push_back(ColliderParamKey::PosX);
		key.push_back(ColliderParamKey::PosY);
		key.push_back(ColliderParamKey::PosZ);
		key.push_back(ColliderParamKey::EndPosX);
		key.push_back(ColliderParamKey::EndPosY);
		key.push_back(ColliderParamKey::EndPosZ);
		key.push_back(ColliderParamKey::RotaX);
		key.push_back(ColliderParamKey::RotaY);
		key.push_back(ColliderParamKey::RotaZ);
		key.push_back(ColliderParamKey::Radius);
		break;

	case ColliderType::Mesh:
		break;
	default:
		break;
	}
}