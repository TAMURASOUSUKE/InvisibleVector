#include "../Tools/nlohmann\json.hpp" // キーコンフィグ設定を保存するためのJsonを簡単に使えるようにするためのライブラリ
#include <unordered_map>
#include <fstream>
#include <Dxlib.h>
#include "InputManager.h"

using json = nlohmann::json;

InputManager::InputManager()
{
	// 最初にデータをロードする
	LoadConfig();

	// fill = 指定した値で要素を埋める
	currentKeyBuffer.fill(0);
	currentStates.fill(false);
	prevStates.fill(false);

	// マウスの座標を最初に取得
	GetMousePoint(&prevMousePosX, &prevMousePosY);
}

void InputManager::Update()
{
	// [EN] Save previous state [JP] 前フレームの状態を保存
	prevStates = currentStates;

	GetHitKeyStateAll(currentKeyBuffer.data()); // [EN] Get input state [JP] 現在の入力の状態を取得
	currentPadInput = GetJoypadInputState(DX_INPUT_PAD1); // [EN] Get input state of pad [JP] パッドの現在の入力状態を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState); // Trigger等の状態を取得

	// ボタン更新
	UpdateGameKey();

	// カメラ更新
	UpdateCameraInput();

}

void InputManager::UpdateGameKey()
{
	// 全アクションの状態を更新する
	for (int i = 0; i < static_cast<int>(GameAction::Count); i++)
	{
		bool isDown{ false }; // 押されているか

		const auto& binding{ bindings[i] }; // 配列から設定データへのアクセス変数

		// trigger判定
		/*
			currentPadInputにはTriggerの情報は含まれていないため
			設定されているマスクそのものと比較する必要がある
		*/
		// 左
		if (binding.padButtonMask == PadCode::TRIGGER_L)
		{
			/*
				Xboxコントローラーはtriggerがアナログ式のため
				取得した値をデッドゾーンで比較するが、
				任天堂のプロコンなどはZRやZLがデジタルボタンなので
				0,1で評価する
			*/
			if (xinputState.LeftTrigger > padTriggerDeadZone)
			{
				isDown = true;
			}
			else if ((currentPadInput & PadCode::TRIGGER_L) != 0)
			{
				isDown = true;
			}
		}
		// 右
		else if (binding.padButtonMask == PadCode::TRIGGER_R)
		{
			// Xboxなど
			if (xinputState.RightTrigger > padTriggerDeadZone)
			{
				isDown = true;
			}
			// switchプロコンなど
			else if ((currentPadInput & PadCode::TRIGGER_R) != 0)
			{
				isDown = true;
			}
		}
		// それ以外ゲームパッドのボタン判定
		// AND演算を行い結果が0でなければ押されているとする
		else if ((currentPadInput & binding.padButtonMask) != 0)
		{
			isDown = true; // 押されている場合はtrueに
		}

		// まだ押されていなかったらキーボードを判定する
		if(!isDown)
		{
			// キーボード判定
			for (int key : binding.keyboardKeys)
			{
				// 配列外参照禁止
				if (key >= 0 && key < 256)
				{
					if (currentKeyBuffer[key] != 0)
					{
						isDown = true;
						break; // どれか一つでも押されていればループを抜ける
					}
				}
			}
		}

		currentStates[i] = isDown; // 結果を現在の状態に保存
	}

	// GameKey型をintにキャストするヘルパー関数
	auto Index = [](GameAction key) { return static_cast<int>(key); };

	// 初期化
	axis.Zero();

	// 移動ベクトル作成
	if (currentStates[Index(GameAction::Up)]) axis.y += 1.0f;
	if (currentStates[Index(GameAction::Down)]) axis.y -= 1.0f;
	if (currentStates[Index(GameAction::Right)]) axis.x += 1.0f;
	if (currentStates[Index(GameAction::Left)]) axis.x -= 1.0f;

	// 正規化処理
	axis.Normalize();
}

void InputManager::UpdateCameraInput()
{
	cameraAxis.Zero(); //毎フレームリセット

	// マウス処理
	int mouseX;
	int mouseY;

	// 現在の位置を取得
	GetMousePoint(&mouseX, &mouseY);

	// 移動量を現在 - 過去で求める	
	Vector2 delta{ static_cast<float>(mouseX - prevMousePosX), static_cast<float>(mouseY - prevMousePosY) };

	// 感度を考慮し加算
	cameraAxis = delta * mouseSensibility;

	// 現在位置を保存
	prevMousePosX = mouseX;
	prevMousePosY = mouseY;

	// スティック処理
	float stickX = static_cast<float>(xinputState.ThumbRX);
	float stickY = static_cast<float>(xinputState.ThumbRY);

	// デッドゾーン処理(少し倒した状態なら無視するようにする)
	if (std::abs(stickX) < stickRDeadZone) stickX = 0.0f;
	if (std::abs(stickY) < stickRDeadZone) stickY = 0.0f;

	// sitc変数をVector2にまとめつつ値を-1.0～1.0まで丸める
	Vector2 stick{ stickX / MAX_XINPUT_VALUE, stickY / MAX_XINPUT_VALUE };

	// 実際の加算
	cameraAxis += stick * stickSensibility;
	
	// 値が大きくなりすぎないように-1.0 ～ 1.0に丸める
	if (cameraAxis.x > 1.0f) cameraAxis.x = 1.0f;
	if (cameraAxis.x < -1.0f) cameraAxis.x = -1.0f;
	if (cameraAxis.y > 1.0f) cameraAxis.y = 1.0f;
	if (cameraAxis.y < -1.0f) cameraAxis.y = -1.0f;

}

// 押している間
bool InputManager::GetButtonStay(GameAction key) const
{
	return currentStates[static_cast<int>(key)];
}

// 押した瞬間
bool InputManager::GetButtonDown(GameAction key) const
{
	int i = static_cast<int>(key); // Enumをキャストしてキャッシュする
	return currentStates[i] && !prevStates[i]; // 今はtrueかつ前フレームはfalse
}

// 離した瞬間
bool InputManager::GetButtonUp(GameAction key) const
{
	int i = static_cast<int>(key);
	return !currentStates[i] && prevStates[i];
}

void InputManager::SetBindingKey(GameAction action, int key)
{
	int index{ static_cast<int>(action) };

	/*
	[EN] Reset existing keys and register new ones
	[JP] 既存のキーをリセットして新しいものを登録
*/
	bindings[index].keyboardKeys.clear(); // リセット
	bindings[index].keyboardKeys.push_back(key);
}

void InputManager::SetBindingPad(GameAction action, int padMask)
{
	int index{ static_cast<int>(action) };

	/*
	[EN] Reset existing keys and register new ones
	[JP] 既存のキーをリセットして新しいものを登録
	*/
	bindings[index].padButtonMask = padMask;
}

void InputManager::AddKeyBinding(GameAction action, int key)
{
	// サブキーをセットする
	int index = static_cast<int>(action);
	bindings[index].keyboardKeys.push_back(key);
}

// デフォルトのキーコンフィグ設定
void InputManager::SetDefaultBindings()
{
	// ----------------[EN] Setting key [JP]キーの割り当てを行う---------------
	SetBindingKey(GameAction::Up, KEY_INPUT_W);
	SetBindingKey(GameAction::Down, KEY_INPUT_S);
	SetBindingKey(GameAction::Right, KEY_INPUT_D);
	SetBindingKey(GameAction::Left, KEY_INPUT_A);
	SetBindingKey(GameAction::Jump, KEY_INPUT_SPACE);
	SetBindingKey(GameAction::Dash, KEY_INPUT_LSHIFT);
	SetBindingKey(GameAction::Crouch, KEY_INPUT_LCONTROL);
	SetBindingKey(GameAction::Zoom, KEY_INPUT_F);
	SetBindingPad(GameAction::Up, PadCode::UP);
	SetBindingPad(GameAction::Down, PadCode::DOWN);
	SetBindingPad(GameAction::Right, PadCode::RIGHT);
	SetBindingPad(GameAction::Left, PadCode::LEFT);
	SetBindingPad(GameAction::Jump, PadCode::SOUTH);
	SetBindingPad(GameAction::Dash, PadCode::WEST);
	SetBindingPad(GameAction::Crouch, PadCode::STICK_CLICK_R);
	SetBindingPad(GameAction::Zoom, PadCode::SHOULDER_L);

	// ----------------[EN] Sub key [JP] サブキー -----------------------------
	AddKeyBinding(GameAction::Up, KEY_INPUT_UP);
	AddKeyBinding(GameAction::Down, KEY_INPUT_DOWN);
	AddKeyBinding(GameAction::Right, KEY_INPUT_RIGHT);
	AddKeyBinding(GameAction::Left, KEY_INPUT_LEFT);

	// [EN] Trigger dead zone [JP] トリガーを押す深さ
	padTriggerDeadZone = DEFAULT_TRIGGER_DEAD_ZONE; // 最初はデフォルトを入れておく
}

// Jsonへのセーブ機能
void InputManager::SaveConfig()
{
	json root;

	// 全てのアクションをループさせ、Jsonオブジェクトを作る
	for (int i = 0; i < static_cast<int>(GameAction::Count);i++)
	{
		GameAction key = static_cast<GameAction>(i); // 現在の数をGameKey型へcastするためキャッシュ
		std::string keyName = GameKeyToString(key); // GameKeyの名前を文字列型へ変換する

		// InputBindingの内容をJsonに詰める
		root[keyName] = {
			{"keys", bindings[i].keyboardKeys}, // vectorは自動的に配列で保存される
			{"pad", bindings[i].padButtonMask}
		};
	}

	// LTRTを押す深さをキーコンフィグから設定できるようにする
	root["TriggerDeadZone"] = padTriggerDeadZone;

	// ファイル書き出し
	std::ofstream file(configFilePath);
	if (file.is_open())
	{
		file << root.dump(4);
		file.close();
	}
}

// ファイルの読み込み
void InputManager::LoadConfig()
{
	std::ifstream file(configFilePath);

	// ファイルが存在しない場合はデフォルト設定を使う
	if (!file.is_open())
	{

		SetDefaultBindings(); // 初期設定
		SaveConfig(); // セーブ
		return;
	}


	/*
		Jsonに破損等開けない理由があった場合に対応するためtry-catchを使う
	*/
	try
	{
		json root;
		file >> root;

		// Jsonの中身を解析してbindingにセット
		for (auto& element : root.items())
		{
			std::string keyName = element.key(); // Jump等のアクション名
			GameAction key = StringToGameKey(keyName); // Enumに変換する

			// 無効なキーならスキップ
			if (static_cast<int>(key) < 0) continue;

			int index = static_cast<int>(key); // Gamekey型をintにキャストしたものをキャッシュする

			// 値を取り出す
			bindings[index].keyboardKeys = element.value()["keys"].get<std::vector<int>>(); // キーボード
			bindings[index].padButtonMask = element.value()["pad"].get<int>();
		}

		// TriggerDeadZoneがあるかどうか確認して読み込む
		if (root.contains("TriggerDeadZone"))
		{
			padTriggerDeadZone = root["TriggerDeadZone"].get<int>();
		}
	}
	catch (const std::exception& e)
	{
		// Jsonが壊れていた場合にはデフォルトに戻す
		SetDefaultBindings();
	}

}

// GameKey型から文字列へ
std::string InputManager::GameKeyToString(GameAction key)
{
	switch (key)
	{
	case GameAction::Up: return "Up";
	case GameAction::Down: return "Down";
	case GameAction::Right: return "Right";
	case GameAction::Left: return "Left";
	case GameAction::Jump: return "Jump";
	case GameAction::Dash: return "Dash";
	case GameAction::Crouch: return "Crouch";
	case GameAction::Zoom: return "Zoom";
	default: return "Unknown";
	}
}

// 文字列からGameKey型へ
GameAction InputManager::StringToGameKey(const std::string& str)
{
	if (str == "Up") return GameAction::Up;
	if (str == "Down") return GameAction::Down;
	if (str == "Left") return GameAction::Left;
	if (str == "Right") return GameAction::Right;
	if (str == "Jump") return GameAction::Jump;
	if (str == "Dash") return GameAction::Dash;
	if (str == "Crouch") return GameAction::Crouch;
	if (str == "Zoom") return GameAction::Zoom;
	return static_cast<GameAction>(-1); // エラー用に用意
}

// 押されたキーを返す
int InputManager::GetAnyPressedKey() const
{
	for (int i = 0; i < currentKeyBuffer.size(); i++)
	{
		// 全キーを精査して押された瞬間のものがあればその番号を返す
		if (currentKeyBuffer[i] == 1)
		{
			return i; // 押されていればその番号を返す
		}
	}
	return -1; // 何も押されていなければ-1を返す
}

// 押されたボタンを返す
int InputManager::GetAnyPressedButton() const
{
	// 調べるボタンをまとめたリスト(トリガー以外)
	int checkButttons[]{
		PadCode::NORTH,
		PadCode::SOUTH,
		PadCode::EAST,
		PadCode::WEST,
		PadCode::STICK_CLICK_L,
		PadCode::STICK_CLICK_R,
		PadCode::SHOULDER_L,
		PadCode::SHOULDER_R,
	};


	// トリガー以外の全てのキーと現状の入力されているボタンを&演算し1が返ってきたボタンを戻り値とする
	for (int button : checkButttons)
	{
		if ((button & currentPadInput) != 0)
		{
			return button;
		}
	}

	// トリガー判定
	if (xinputState.LeftTrigger > padTriggerDeadZone)
	{
		return PadCode::TRIGGER_L;
	}

	if (xinputState.RightTrigger > padTriggerDeadZone)
	{
		return PadCode::TRIGGER_R;
	}

	return 0; // 見当たらなかったら0

}