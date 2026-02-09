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
}

void InputManager::Update()
{
	// [EN] Save previous state [JP] 前フレームの状態を保存
	prevStates = currentStates;

	GetHitKeyStateAll(currentKeyBuffer.data()); // [EN] Get input state [JP] 現在の入力の状態を取得
	currentPadInput = GetJoypadInputState(DX_INPUT_PAD1); // [EN] Get input state of pad [JP] パッドの現在の入力状態を取得

	// [EN] Convert Concrete to Abstract [JP] 具体から抽象へ変換
	UpdateGameKey();
}

void InputManager::UpdateGameKey()
{
	// 全アクションの状態を更新する
	for (int i = 0; i < static_cast<int>(GameKey::Count); i++)
	{
		bool isDown{ false }; // 押されているか

		const auto& binding{ bindings[i] }; // 配列から設定データへのアクセス変数

		// ゲームパッドの判定
		// AND演算を行い結果が0でなければ押されているとする
		if ((currentPadInput & binding.padButtonMask) != 0)
		{
			isDown = true; // 押されている場合はtrueに
		}
		else
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
	auto Index = [](GameKey key) { return static_cast<int>(key); };

	// 初期化
	axis.Zero();

	// 移動ベクトル作成
	if (currentStates[Index(GameKey::Up)]) axis.y += 1.0f;
	if (currentStates[Index(GameKey::Down)]) axis.y -= 1.0f;
	if (currentStates[Index(GameKey::Right)]) axis.x += 1.0f;
	if (currentStates[Index(GameKey::Left)]) axis.x -= 1.0f;

	// 正規化処理
	axis.Normalize();
}

// 押している間
bool InputManager::GetButtonStay(GameKey key) const
{
	return currentStates[static_cast<int>(key)];
}

// 押した瞬間
bool InputManager::GetButtonDown(GameKey key) const
{
	int i = static_cast<int>(key); // Enumをキャストしてキャッシュする
	return currentStates[i] && !prevStates[i]; // 今はtrueかつ前フレームはfalse
}

// 離した瞬間
bool InputManager::GetButtonUp(GameKey key) const
{
	int i = static_cast<int>(key);
	return !currentStates[i] && prevStates[i];
}

void InputManager::SetBinding(GameKey action, int key, int padMask)
{
	int index = static_cast<int>(action);

	/*
		[EN] Reset existing keys and register new ones
		[JP] 既存のキーをリセットして新しいものを登録
	*/
	bindings[index].keyboardKeys.clear(); // リセット
	bindings[index].keyboardKeys.push_back(key);
	bindings[index].padButtonMask = padMask;
}

void InputManager::AddKeyBinding(GameKey action, int key)
{
	// サブキーをセットする
	int index = static_cast<int>(action);
	bindings[index].keyboardKeys.push_back(key);
}

// デフォルトのキーコンフィグ設定
void InputManager::SetDefaultBindings()
{
	// ----------------[EN] Setting key [JP]キーの割り当てを行う---------------
	SetBinding(GameKey::Up, KEY_INPUT_W, PAD_INPUT_UP);
	SetBinding(GameKey::Down, KEY_INPUT_S, PAD_INPUT_DOWN);
	SetBinding(GameKey::Right, KEY_INPUT_D, PAD_INPUT_RIGHT);
	SetBinding(GameKey::Left, KEY_INPUT_A, PAD_INPUT_LEFT);
	SetBinding(GameKey::Jump, KEY_INPUT_SPACE, PAD_INPUT_A);
	SetBinding(GameKey::Dash, KEY_INPUT_LSHIFT, PAD_INPUT_C); // C = Xボタン
	SetBinding(GameKey::Crouch, KEY_INPUT_LCONTROL, PAD_INPUT_9); // 左スティック押し込み
	SetBinding(GameKey::Zoom, KEY_INPUT_F, PAD_INPUT_L);

	// ----------------[EN] Sub key [JP] サブキー -----------------------------
	AddKeyBinding(GameKey::Up, KEY_INPUT_UP);
	AddKeyBinding(GameKey::Down, KEY_INPUT_DOWN);
	AddKeyBinding(GameKey::Right, KEY_INPUT_RIGHT);
	AddKeyBinding(GameKey::Left, KEY_INPUT_LEFT);

}

// Jsonへのセーブ機能
void InputManager::SaveConfig()
{
	json root;

	// 全てのアクションをループさせ、Jsonオブジェクトを作る
	for (int i = 0; i < static_cast<int>(GameKey::Count);i++)
	{
		GameKey key = static_cast<GameKey>(i); // 現在の数をGameKey型へcastするためキャッシュ
		std::string keyName = GameKeyToString(key); // GameKeyの名前を文字列型へ変換する

		// InputBindingの内容をJsonに詰める
		root[keyName] = {
			{"keys", bindings[i].keyboardKeys}, // vectorは自動的に配列で保存される
			{"pad", bindings[i].padButtonMask}
		};
	}

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
		// 出なければ、まだどこかにファイルが残っている
		OutputDebugString("File NOT found! Loading Defaults\n");

		SetDefaultBindings(); // 初期設定
		SaveConfig(); // セーブ
		return;
	}

	// もしここに来てしまったら、まだファイルが残っている証拠
	OutputDebugString("File FOUND! Loading from JSON...\n");

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
			GameKey key = StringToGameKey(keyName); // Enumに変換する

			// 無効なキーならスキップ
			if (static_cast<int>(key) < 0) continue;

			int index = static_cast<int>(key); // Gamekey型をintにキャストしたものをキャッシュする

			// 値を取り出す
			bindings[index].keyboardKeys = element.value()["keys"].get<std::vector<int>>(); // キーボード
			bindings[index].padButtonMask = element.value()["pad"].get<int>();
		}
	}
	catch (const std::exception& e)
	{
		// Jsonが壊れていた場合にはデフォルトに戻す
		SetDefaultBindings();
	}

}

// GameKey型から文字列へ
std::string InputManager::GameKeyToString(GameKey key)
{
	switch (key)
	{
	case GameKey::Up: return "Up";
	case GameKey::Down: return "Down";
	case GameKey::Right: return "Right";
	case GameKey::Left: return "Left";
	case GameKey::Jump: return "Jump";
	case GameKey::Dash: return "Dash";
	case GameKey::Crouch: return "Crouch";
	case GameKey::Zoom: return "Zoom";
	default: return "Unknown";
	}
}

// 文字列からGameKey型へ
GameKey InputManager::StringToGameKey(const std::string& str)
{
	if (str == "Up") return GameKey::Up;
	if (str == "Down") return GameKey::Down;
	if (str == "Left") return GameKey::Left;
	if (str == "Right") return GameKey::Right;
	if (str == "Jump") return GameKey::Jump;
	if (str == "Dash") return GameKey::Dash;
	if (str == "Crouch") return GameKey::Crouch;
	if (str == "Zoom") return GameKey::Zoom;
	return static_cast<GameKey>(-1); // エラー用に用意
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
	
}