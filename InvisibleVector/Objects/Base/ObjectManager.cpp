#include <algorithm>
#include <memory>
#include "GameConstant.h"
#include "ObjectManager.h"


void ObjectManager::RegisterWorld(std::unique_ptr<WorldObjectBase> _worldObj)
{
	worldBasses.push_back(std::move(_worldObj));
}

void ObjectManager::RegisterUI(std::unique_ptr<UIObjectBase> _uiObj)
{
	uiBasses.push_back(std::move(_uiObj));
}

void ObjectManager::TemporaryRegister(std::unique_ptr<WorldObjectBase> _temporaryObj)
{
	temporaryBasses.push_back(std::move(_temporaryObj));
}

// 全体の更新
void ObjectManager::Update()
{
	// ワールドオブジェクト
	for (auto& worldObj : worldBasses)
	{
		// 死んでいるオブジェクトは更新しない
		if (!worldObj->GetIsActive())
		{
			continue;
		}

		worldObj->Update();
	}

	// UIオブジェクト
	for (auto& uiObj : uiBasses)
	{
		// 死んでいるオブジェクトは更新しない
		if (!uiObj->GetIsActive())
		{
			continue;
		}

		uiObj->Update();
	}

	// 02/26追加
	// 仮登録したオブジェクトをworldBassesに登録
	if (!temporaryBasses.empty())
	{
		worldBasses.reserve(worldBasses.size() + temporaryBasses.size());

		worldBasses.insert(
			worldBasses.end(),
			std::make_move_iterator(temporaryBasses.begin()),
			std::make_move_iterator(temporaryBasses.end())
		);
		temporaryBasses.clear();
	}
}

void ObjectManager::FixedUpdate()
{
	for (auto& worldObj : worldBasses)
	{
		// 死んでいれば更新しない
		if (!worldObj->GetIsActive())
		{
			continue;
		}

		worldObj->FixedUpdate();
	}
}

void ObjectManager::Draw()
{
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR); // カメラのリセット

	for (auto& worldObj : worldBasses)
	{
		// 死んでいれば更新しない
		if (!worldObj->GetIsActive())
		{
			continue;
		}

		worldObj->Draw();
	}

	// 2D用の設定に切り替える
	SetUseZBuffer3D(false);   // 奥行きテストをしない
	SetWriteZBuffer3D(false); // 奥行きを書き込まない

	for (auto& uiObj : uiBasses)
	{
		// 死んでいるオブジェクトは更新しない
		if (!uiObj->GetIsActive())
		{
			continue;
		}

		// 非表示オブジェクトは描画しない
		if (!uiObj->GetIsVisible())
		{
			continue;
		}

		uiObj->Draw();
	}

	SetUseZBuffer3D(true);   // 奥行きテストを有効
	SetWriteZBuffer3D(true); // 奥行きを書き込む
}

void ObjectManager::Refresh()
{
	/*
		remove_ifによって配列の最初から最後までを検索し条件に合うものを後ろに詰めていく
		remove_ifは有効なデータの次のイテレータを返す(要するに今回ならいらないデータの先頭)
	*/
	auto worldIt = std::remove_if(worldBasses.begin(), worldBasses.end(),
		[](const std::unique_ptr<WorldObjectBase>& obj)
		{
			return !obj->GetIsActive(); // 死んでいるやつを取り出す
		});

	// いらないデータから配列の最後までを消す
	worldBasses.erase(worldIt, worldBasses.end());


	auto uiIt = std::remove_if(uiBasses.begin(), uiBasses.end(),
		[](const std::unique_ptr<UIObjectBase>& obj)
		{
			return !obj->GetIsActive(); // 死んでいるやつを取り出す
		});

	// いらないデータから配列の最後までを消す
	uiBasses.erase(uiIt, uiBasses.end());
}

void ObjectManager::SortUI()
{
	// uiBassesの配列をPriority基準に昇順でソートする(これで描画順を決定する)
	std::sort(uiBasses.begin(), uiBasses.end(),
		[](std::unique_ptr<UIObjectBase>& _a, std::unique_ptr<UIObjectBase>& _b)
		{
			return _a->GetPriority() < _b->GetPriority();
		});
}