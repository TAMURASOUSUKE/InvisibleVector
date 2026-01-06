#include <algorithm>
#include "ObjectManager.h"

void ObjectManager::Update()
{
	for (auto& obj : objects)
	{
		if (!obj->GetIsActive()) continue;

		obj->Update();
	}
}

void ObjectManager::FixedUpdate()
{
	for (auto& obj : objects)
	{
		if (!obj->GetIsActive()) continue;

		obj->FixedUpdate();
	}
}

void ObjectManager::Draw()
{
	for (auto& obj : objects)
	{
		if (!obj->GetIsActive()) continue;

		obj->Draw();
	}
}

void ObjectManager::Refresh()
{
	/*
		remove_ifによって配列の最初から最後までを検索し条件に合うものを後ろに詰めていく
		remove_ifは有効なデータの次のイテレータを返す(要するに今回ならいらないデータの先頭)
	*/
	auto it = std::remove_if(objects.begin(), objects.end(),
		[](const std::unique_ptr<ObjectBase>& obj)
		{
			return !obj->GetIsActive();
		});

	// いらないデータから配列の最後までを消す
	objects.erase(it, objects.end());
}