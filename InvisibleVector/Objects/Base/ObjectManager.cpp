#include <algorithm>
#include "ObjectManager.h"

void ObjectManager::Update()
{
	for (auto& worldObj : worldBasses)
	{
		if (!worldObj->GetIsActive()) continue;

		worldObj->Update();
	}

	for (auto& uiObj : uiBasses)
	{
		if (!uiObj->GetIsActive()) continue;

		uiObj->Update();
	}
}

void ObjectManager::FixedUpdate()
{
	for (auto& worldObj : worldBasses)
	{
		if (!worldObj->GetIsActive()) continue;

		worldObj->FixedUpdate();
	}
}

void ObjectManager::Draw()
{
	for (auto& worldObj : worldBasses)
	{
		if (!worldObj->GetIsActive()) continue;

		worldObj->Update();
	}

	for (auto& uiObj : uiBasses)
	{
		if (uiObj->GetIsActive()) continue;

		uiObj->Update();
	}
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
			return !obj->GetIsActive();
		});

	// いらないデータから配列の最後までを消す
	worldBasses.erase(worldIt, worldBasses.end());


	auto uiIt = std::remove_if(uiBasses.begin(), uiBasses.end(),
		[](const std::unique_ptr<UIObjectBase>& obj)
		{
			return !obj->GetIsActive();
		});

	// いらないデータから配列の最後までを消す
	uiBasses.erase(uiIt, uiBasses.end());
}