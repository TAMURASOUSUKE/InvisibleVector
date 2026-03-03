#pragma once
#include <vector>
#include <memory>
#include "SceneConstant.h"
#include "ObjectBase.h"
#include "WorldObjectBase.h"
#include "UIObjectBase.h"

/*
	02/26追加
	public:
		void TemporaryRegister(std::unique_ptr<WorldObjectBase> _temporaryObj);	// 仮登録用関数
	private:
		std::vector<std::unique_ptr<WorldObjectBase>> temporaryBasses{}; // 仮のオブジェクトのリスト

	cppのUpdate関数の中身にも少し追加があります
*/

// オブジェクトの管理クラス
class ObjectManager
{
public:
	ObjectManager() = default;

	~ObjectManager() = default;

	void RegisterWorld(std::unique_ptr<WorldObjectBase> _worldObj); // WorldObjectを登録する
	void RegisterUI(std::unique_ptr<UIObjectBase> _uiObj); // UIObjectを登録する

	void TemporaryRegister(std::unique_ptr<WorldObjectBase> _temporaryObj);	// 仮登録用関数

	void Update(); // 生存している全てのオブジェクトのUpdateを呼ぶ

	void FixedUpdate(); // 生存している全てのオブジェクトのFixedUpdateを呼ぶ
	void Draw(); // 生存している全てのオブジェクトのDrawを呼ぶ

	void Refresh(); // 死んだオブジェクトをメモリから解放する

	void SortUI(); // UIをソートする基本的には各シーンで生成し終えたら呼び出して描画順を整理する
private:
	std::vector<std::unique_ptr<WorldObjectBase>> worldBasses{}; // ワールドオブジェクトのリスト
	std::vector<std::unique_ptr<UIObjectBase>> uiBasses{}; // UIオブジェクトのリスト

	std::vector<std::unique_ptr<WorldObjectBase>> temporaryBasses{}; // 仮のオブジェクトのリスト

};