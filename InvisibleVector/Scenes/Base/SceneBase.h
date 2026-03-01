#pragma once
#pragma once
#include "SceneConstant.h"



#ifdef _DEBUG

#include "DxLib.h"
#include "Vector2.h"

#endif // _DEBUG



// 各シーンの規定となるクラス
class SceneBase
{
public:
	SceneBase() = default;
	virtual ~SceneBase() = default;

	SceneType Main()
	{
		switch (currentStep)
		{
		case SceneStep::None:
			break;
		case SceneStep::Initialize:
			Initialize();
			break;
		case SceneStep::Execute:
			Execute();
			break;
		case SceneStep::Terminate:
			return Terminate(); // ここではシーン終了処理となるためretrunで処理を抜ける
		}

		return GetType();
	}

	virtual void Draw() = 0;

	virtual SceneType GetType() = 0; // シーンのタイプを返す関数

protected:
	virtual void Initialize() = 0; // シーンが切り替わった際最初に呼び出される処理
	virtual void Execute() = 0; // シーンが実行中繰り返される処理
	virtual SceneType Terminate() = 0; // シーンを終了する際に出される処理

protected:
	SceneStep currentStep{ SceneStep::Initialize }; // 今のステップ

#ifdef _DEBUG
	unsigned int white{ GetColor(255, 255, 255) }; // 白色を簡単に作れるようにする


#endif // _DEBUG

};