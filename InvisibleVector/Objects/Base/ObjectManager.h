#pragma once
#include <vector>
#include <memory>
#include "ObjectBase.h"
#include "WorldObjectBase.h"
#include "UIObjectBase.h"

/// <summary>
/// [EN] Manage the objects [JP] オブジェクトの管理クラス
/// </summary>
class ObjectManager
{
public:
	ObjectManager() = default;
	~ObjectManager() = default;

	/// <summary>
	/// [EN] Add a new object to the manager(ownership transfer)
	/// [JP] 新しいオブジェクトを管理下に追加する
	/// </summary>
	/// <param name="obj">[EN] Unique pointer to object [JP] オブジェクトのユニークポインタ</param>
	// void Add(std::unique_ptr<ObjectBase> obj) { objects.push_back(std::move(obj)); }

	void AddWorldObjects(std::unique_ptr<WorldObjectBase> worldObj) { worldBasses.push_back(std::move(worldObj)); }
	void AddUIObjects(std::unique_ptr<UIObjectBase> uiObj) { uiBasses.push_back(std::move(uiObj)); }

	/// <summary>
	/// [EN] Call update on all active objects
	/// [JP] 生存している全てのオブジェクトのUpdateを呼ぶ
	/// </summary>
	void Update();

	/// <summary>
	/// [EN] Call fixedUpdate on all active objects
	/// [JP] 生存している全てのオブジェクトのFixedUpdateを呼ぶ
	/// </summary>
	void FixedUpdate();

	/// <summary>
	/// [EN] Call Draw on all active objects
	/// [JP] 生存している全てのオブジェクトのDrawを呼ぶ
	/// </summary>
	void Draw();

	/// <summary>
	/// [EN] Remove dead objects from memory
	/// [JP] 死んだオブジェクトをメモリから解放する
	/// </summary>
	void Refresh();
private:
	// [EN] List of objects [JP] オブジェクトのリスト
	// std::vector<std::unique_ptr<ObjectBase>> objects;

	std::vector<std::unique_ptr<WorldObjectBase>> worldBasses{};
	std::vector<std::unique_ptr<UIObjectBase>> uiBasses{};

};