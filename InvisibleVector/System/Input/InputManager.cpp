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
	currentGameStates.fill(false);
	prevGameStates.fill(false);
	currentUIStates.fill(false);
	prevUIStates.fill(false);

	// マウスの座標を最初に取得
	GetMousePoint(&prevMousePosX, &prevMousePosY);

	currentMode = InputMode::Game; // 最初はGameからスタート
}

void InputManager::Update()
{
	// [EN] Save previous state [JP] 前フレームの状態を保存
	prevGameStates = currentGameStates;

	GetHitKeyStateAll(currentKeyBuffer.data()); // [EN] Get input state [JP] 現在の入力の状態を取得
	currentPadInput = GetJoypadInputState(DX_INPUT_PAD1); // [EN] Get input state of pad [JP] パッドの現在の入力状態を取得
	GetJoypadXInputState(DX_INPUT_PAD1, &xinputState); // Trigger等の状態を取得

	// ボタン更新
	UpdateGameKey();

	// カメラ更新
	UpdateCameraInput();

	count++; // カウンターを増やす

}

void InputManager::UpdateGameKey()
{
	/*
		ラムダを使って共通の処理をまとめる
		第一引数: 設定したキーコンの配列
		第二引数: キーやボタンが押されているかが格納された配列
		第三引数: 要素数
	*/
	auto UpdateState = [&](const auto& bindingsArray, auto& stateArray, int count)
		{
			// 全要素数分探索する
			for (int i = 0; i < count; i++)
			{
				bool isDown{ false }; // 押されたか判別する
				const auto& binding{ bindingsArray[i] }; // 現在のキーをキャッシュする

				// パッド判定
				for (int button : binding.padButtonMasks)
				{
					// trigger判定
				/*
					currentPadInputにはTriggerの情報は含まれていないため
					設定されているマスクそのものと比較する必要がある
				*/
				// 左
					if (button == PadCode::TRIGGER_L)
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
					else if (button == PadCode::TRIGGER_R)
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
					else if ((currentPadInput & button) != 0)
					{
						isDown = true; // 押されている場合はtrueに
					}
				}


				// まだ押されていなかったらキーボードを判定する
				if (!isDown)
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

				stateArray[i] = isDown; // 最終的な状態がここで決定する
			}
		};

	UpdateState(gameBindings, currentGameStates, static_cast<int>(ActionID::GameAction::Count)); // ゲームアクション更新
	UpdateState(uiBindings, currentUIStates, static_cast<int>(ActionID::UI::Count)); // UIの更新


	// Axisに関してはゲーム内のみなのでUIは行わなくてよい
	// GameKey型をintにキャストするヘルパー関数
	auto Index = [](ActionID::GameAction key) { return static_cast<int>(key); };

	// 初期化
	axis = axis.Zero();

	// スティック処理
	float stickX = static_cast<float>(xinputState.ThumbLX);
	float stickY = static_cast<float>(xinputState.ThumbLY);

	// デッドゾーン処理(少し倒した状態なら無視するようにする)
	if (std::abs(stickX) < stickRDeadZone) stickX = 0.0f;
	if (std::abs(stickY) < stickRDeadZone) stickY = 0.0f;

	// sitc変数をVector2にまとめつつ値を-1.0～1.0まで丸める
	Vector2 stick{ stickX / MAX_XINPUT_VALUE, stickY / MAX_XINPUT_VALUE };
	axis += stick * stickSensibility;
	//axis += { stickX,stickY };

	// 移動ベクトル作成
	if (currentGameStates[Index(ActionID::GameAction::Up)]) axis.y += 1.0f;
	if (currentGameStates[Index(ActionID::GameAction::Down)]) axis.y -= 1.0f;
	if (currentGameStates[Index(ActionID::GameAction::Right)]) axis.x += 1.0f;
	if (currentGameStates[Index(ActionID::GameAction::Left)]) axis.x -= 1.0f;

	// 正規化処理
	axis.Normalize();
}

void InputManager::UpdateCameraInput()
{
	if (currentMode != InputMode::Game) return; // ゲーム中でないなら計算しない

	cameraAxis.Zero(); //毎フレームリセット

	// マウス処理
	int mouseX;
	int mouseY;

	// 現在の位置を取得
	GetMousePoint(&mouseX, &mouseY);

	// 移動量を現在 - 過去で求める	
	Vector2 delta{ static_cast<float>(mouseX - prevMousePosX), -static_cast<float>(mouseY - prevMousePosY) };

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
	//if (cameraAxis.x > 1.0f) cameraAxis.x = 1.0f;
	//if (cameraAxis.x < -1.0f) cameraAxis.x = -1.0f;
	//if (cameraAxis.y > 1.0f) cameraAxis.y = 1.0f;
	//if (cameraAxis.y < -1.0f) cameraAxis.y = -1.0f;

	// マウスカーソルを画面中央固定する
	if (GetMainWindowHandle() == GetForegroundWindow()) // ウィンドウがアクティブな時だけ
	{
		Vector2Int center{ windowWidth / 2, windowHeight / 2 };
		// 画面中央に戻す
		SetMousePoint(center.x, center.y);

		// 移動量が爆増することを避けるためにprevMousePosも更新する
		prevMousePosX = center.x;
		prevMousePosY = center.y;
	}

}

// 押している間
bool InputManager::GetButtonStay(ActionID::GameAction key) const
{
	if (currentMode != InputMode::Game) return false; // 現在の状態がGame中ではなかったら反応しない
	return currentGameStates[static_cast<int>(key)];
}

bool InputManager::GetButtonStay(ActionID::UI key) const
{
	if (currentMode == InputMode::Game) return false; // ゲーム中なら反応しない
	return currentUIStates[static_cast<int>(key)];
}

// 押した瞬間
bool InputManager::GetButtonDown(ActionID::GameAction key) const
{
	if (currentMode != InputMode::Game) return false; // ゲームモード中以外はfalse
	int i = static_cast<int>(key); // Enumをキャストしてキャッシュする
	return currentGameStates[i] && !prevGameStates[i]; // 今はtrueかつ前フレームはfalse
}

bool InputManager::GetButtonDown(ActionID::UI key) const
{
	if (currentMode == InputMode::Game) return false; // ゲームモード中はfalse
	int i = static_cast<int>(key); // Enumをキャストしてキャッシュする
	return currentUIStates[i] && !prevUIStates[i]; // 今はtrueかつ前フレームはfalse
}
// 離した瞬間
bool InputManager::GetButtonUp(ActionID::GameAction key) const
{
	if (currentMode != InputMode::Game) return false; // ゲーム中以外ならfalse
	int i = static_cast<int>(key);
	return !currentGameStates[i] && prevGameStates[i];
}

bool InputManager::GetButtonUp(ActionID::UI key) const
{
	if (currentMode == InputMode::Game) return false; // ゲーム中ならfalse
	int i = static_cast<int>(key);
	return !currentUIStates[i] && prevUIStates[i];
}

void InputManager::SetBindingKey(ActionID::GameAction action, int key)
{
	int index{ static_cast<int>(action) };

	/*
	[EN] Reset existing keys and register new ones
	[JP] 既存のキーをリセットして新しいものを登録
*/
	gameBindings[index].keyboardKeys.clear(); // リセット
	gameBindings[index].keyboardKeys.push_back(key);
}

void InputManager::SetBindingKey(ActionID::UI action, int key)
{
	int index{ static_cast<int>(action) };

	/*
	[EN] Reset existing keys and register new ones
	[JP] 既存のキーをリセットして新しいものを登録
*/
	uiBindings[index].keyboardKeys.clear(); // リセット
	uiBindings[index].keyboardKeys.push_back(key);
}

void InputManager::SetBindingPad(ActionID::GameAction action, int padMask)
{
	int index{ static_cast<int>(action) };

	/*
	[EN] Reset existing keys and register new ones
	[JP] 既存のキーをリセットして新しいものを登録
	*/
	gameBindings[index].padButtonMasks.clear(); // リセット
	gameBindings[index].padButtonMasks.push_back(padMask);
}

void InputManager::SetBindingPad(ActionID::UI action, int padMask)
{
	int index{ static_cast<int>(action) };

	/*
	[EN] Reset existing keys and register new ones
	[JP] 既存のキーをリセットして新しいものを登録
	*/
	uiBindings[index].padButtonMasks.clear();
	uiBindings[index].padButtonMasks.push_back(padMask);
}


void InputManager::AddKeyBinding(ActionID::GameAction action, int key)
{
	// サブキーをセットする
	int index{ static_cast<int>(action) };
	gameBindings[index].keyboardKeys.push_back(key);
}

void InputManager::AddKeyBinding(ActionID::UI action, int key)
{
	// サブキーをセットする
	int index{ static_cast<int>(action) };
	uiBindings[index].keyboardKeys.push_back(key);
}

void InputManager::AddButtonBinding(ActionID::GameAction action, int buttonMask)
{
	int index{ static_cast<int>(action) };
	gameBindings[index].padButtonMasks.push_back(buttonMask);
}

void InputManager::AddButtonBinding(ActionID::UI action, int buttonMask)
{
	int index{ static_cast<int>(action) };
	gameBindings[index].padButtonMasks.push_back(buttonMask);
}

// デフォルトのキーコンフィグ設定
void InputManager::SetDefaultBindings()
{
	// ----------------[EN] Setting key [JP]キーの割り当てを行う---------------
	SetBindingKey(ActionID::GameAction::Up, KEY_INPUT_W);
	SetBindingKey(ActionID::GameAction::Down, KEY_INPUT_S);
	SetBindingKey(ActionID::GameAction::Right, KEY_INPUT_D);
	SetBindingKey(ActionID::GameAction::Left, KEY_INPUT_A);
	SetBindingKey(ActionID::GameAction::Jump, KEY_INPUT_SPACE);
	SetBindingKey(ActionID::GameAction::Dash, KEY_INPUT_LSHIFT);
	SetBindingKey(ActionID::GameAction::Crouch, KEY_INPUT_LCONTROL);
	SetBindingKey(ActionID::GameAction::Zoom, KEY_INPUT_F);
	SetBindingKey(ActionID::GameAction::Shoot, KEY_INPUT_SPACE);

	SetBindingKey(ActionID::UI::Up, KEY_INPUT_W);
	SetBindingKey(ActionID::UI::Down, KEY_INPUT_S);
	SetBindingKey(ActionID::UI::Right, KEY_INPUT_D);
	SetBindingKey(ActionID::UI::Left, KEY_INPUT_A);
	SetBindingKey(ActionID::UI::Decide, KEY_INPUT_E);
	SetBindingKey(ActionID::UI::Cancel, KEY_INPUT_Q);
	SetBindingKey(ActionID::UI::Pause, KEY_INPUT_ESCAPE);

	SetBindingPad(ActionID::GameAction::Down, PadCode::DOWN);
	SetBindingPad(ActionID::GameAction::Right, PadCode::RIGHT);
	SetBindingPad(ActionID::GameAction::Left, PadCode::LEFT);
	SetBindingPad(ActionID::GameAction::Jump, PadCode::SOUTH);
	SetBindingPad(ActionID::GameAction::Dash, PadCode::WEST);
	SetBindingPad(ActionID::GameAction::Crouch, PadCode::STICK_CLICK_R);
	SetBindingPad(ActionID::GameAction::Zoom, PadCode::SHOULDER_L);
	SetBindingPad(ActionID::GameAction::Shoot, PadCode::TRIGGER_R);

	SetBindingPad(ActionID::UI::Up, PadCode::UP);
	SetBindingPad(ActionID::UI::Down, PadCode::DOWN);
	SetBindingPad(ActionID::UI::Right, PadCode::RIGHT);
	SetBindingPad(ActionID::UI::Left, PadCode::LEFT);
	SetBindingPad(ActionID::UI::Decide, PadCode::EAST);
	SetBindingPad(ActionID::UI::Cancel, PadCode::SOUTH);
	SetBindingPad(ActionID::UI::Pause, PadCode::START);
	SetBindingPad(ActionID::GameAction::Up, PadCode::UP);

	// ----------------[EN] Sub key [JP] サブキー -----------------------------
	AddKeyBinding(ActionID::GameAction::Up, KEY_INPUT_UP);
	AddKeyBinding(ActionID::GameAction::Down, KEY_INPUT_DOWN);
	AddKeyBinding(ActionID::GameAction::Right, KEY_INPUT_RIGHT);
	AddKeyBinding(ActionID::GameAction::Left, KEY_INPUT_LEFT);

	// UI
	AddKeyBinding(ActionID::UI::Up, KEY_INPUT_UP);
	AddKeyBinding(ActionID::UI::Down, KEY_INPUT_DOWN);
	AddKeyBinding(ActionID::UI::Right, KEY_INPUT_RIGHT);
	AddKeyBinding(ActionID::UI::Left, KEY_INPUT_LEFT);
	AddButtonBinding(ActionID::UI::Pause, PAD_INPUT_M); // Xboxのスタートボタンを登録

	// トリガーを押す深さ
	padTriggerDeadZone = DEFAULT_TRIGGER_DEAD_ZONE; // 最初はデフォルトを入れておく
	// 右スティックのデッドゾーン
	stickRDeadZone = DEFAULT_STICK_DEAD_ZONE;
	// マウス感度
	mouseSensibility = DEFAULT_MOUSE_SENSIBILITY;
	// スティック感度
	stickSensibility = DEFAULT_STICK_SENSIBILITY;

}

// Jsonへのセーブ機能
void InputManager::SaveConfig()
{
	json root;

	// 全てのアクションをループさせ、Jsonオブジェクトを作る
	// ゲーム中
	for (int i = 0; i < static_cast<int>(ActionID::GameAction::Count); i++)
	{
		ActionID::GameAction key = static_cast<ActionID::GameAction>(i); // 現在の数をGameKey型へcastするためキャッシュ
		std::string keyName = GameKeyToString(key); // GameKeyの名前を文字列型へ変換する

		// InputBindingの内容をJsonに詰める
		root["Game"][keyName] = {
			{"keys", gameBindings[i].keyboardKeys}, // vectorは自動的に配列で保存される
			{"pad", gameBindings[i].padButtonMasks}
		};
	}

	// ゲーム外
	for (int i = 0; i < static_cast<int>(ActionID::UI::Count); i++)
	{
		ActionID::UI key = static_cast<ActionID::UI>(i); // 現在の数をUI型へと変換
		std::string keyName = UIKeyToString(key); // 文字列へ変換

		// Jsonへ詰める
		root["UI"][keyName] = {
			{"keys", uiBindings[i].keyboardKeys},
			{"pad", uiBindings[i].padButtonMasks}
		};
	}

	// LTRTを押す深さをキーコンフィグから設定できるようにする
	root["TriggerDeadZone"] = padTriggerDeadZone;
	// マウス感度をキーコンフィグから設定できるようにする
	root["MouseSensibility"] = mouseSensibility;
	// 右スティック感度をキーコンフィグ設定からできるようにする
	root["RightStickSensibility"] = stickSensibility;
	// スティックのデッドゾーンをキーコンフィグから設定できるようにする
	root["RightStickDeadZone"] = stickRDeadZone;

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

		// Gameを読み込む
		if (root.contains("Game"))
		{
			// Jsonの中身を解析してbindingにセット
			for (auto& element : root["Game"].items())
			{
				std::string keyName = element.key(); // Jump等のアクション名
				ActionID::GameAction key = StringToGameKey(keyName); // Enumに変換する

				// 無効なキーならスキップ
				if (static_cast<int>(key) < 0) continue;

				int index = static_cast<int>(key); // Gamekey型をintにキャストしたものをキャッシュする

				// 値を取り出す
				gameBindings[index].keyboardKeys = element.value()["keys"].get<std::vector<int>>(); // キーボード
				gameBindings[index].padButtonMasks = element.value()["pad"].get<std::vector<int>>(); // ボタン
			}
		}

		// UIを読み込む
		if (root.contains("UI"))
		{
			for (auto& element : root["UI"].items())
			{
				std::string keyName = element.key(); // アクション名をキャッシュ
				ActionID::UI key = StringToUIKey(keyName); // Enumに変換

				if (static_cast<int>(key) < 0) continue; // 無効ならスキップ

				int index = static_cast<int>(key); // UI型をint型へキャッシュ

				// 値を取り出す
				uiBindings[index].keyboardKeys = element.value()["keys"].get<std::vector<int>>(); // int型配列として取得
				uiBindings[index].padButtonMasks = element.value()["pad"].get<std::vector<int>>(); // int型配列として保存
			}
		}

		// TriggerDeadZoneがあるかどうか確認して読み込む
		if (root.contains("TriggerDeadZone"))
		{
			padTriggerDeadZone = root["TriggerDeadZone"].get<int>();
		}
		// マウス感度があるかどうかをチェックしてから取り込む
		if (root.contains("MouseSensibility"))
		{
			mouseSensibility = root["MouseSensibility"].get<float>();
		}
		// 右スティック感度があるかどうかをチェックしてから取り込む
		if (root.contains("RightStickSensibility"))
		{
			stickSensibility = root["RightStickSensibility"].get<float>();
		}
		// 右スティックデッドゾーンがあるかどうかをチェックして取り込む
		if (root.contains("RightStickDeadZone"))
		{
			stickRDeadZone = root["RightStickDeadZone"].get<int>();
		}
	}
	catch (const std::exception& e)
	{
		// Jsonが壊れていた場合にはデフォルトに戻す
		SetDefaultBindings();
	}

}

// GameKey型から文字列へ
std::string InputManager::GameKeyToString(ActionID::GameAction key)
{
	switch (key)
	{
	case ActionID::GameAction::Up: return "Up";
	case ActionID::GameAction::Down: return "Down";
	case ActionID::GameAction::Right: return "Right";
	case ActionID::GameAction::Left: return "Left";
	case ActionID::GameAction::Jump: return "Jump";
	case ActionID::GameAction::Dash: return "Dash";
	case ActionID::GameAction::Crouch: return "Crouch";
	case ActionID::GameAction::Zoom: return "Zoom";
	case ActionID::GameAction::Shoot: return "Shoot";
	default: return "Unknown";
	}
}

std::string InputManager::UIKeyToString(ActionID::UI key)
{
	switch (key)
	{
	case ActionID::UI::Up: return "Up";
	case ActionID::UI::Down: return "Down";
	case ActionID::UI::Left: return "Left";
	case ActionID::UI::Right: return "Right";
	case ActionID::UI::Decide: return "Decide";
	case ActionID::UI::Cancel: return "Cancel";
	case ActionID::UI::Pause: return "Pause";
	default: return "Unknown";
	}
}

// 文字列からGameKey型へ
ActionID::GameAction InputManager::StringToGameKey(const std::string& str)
{
	if (str == "Up") return ActionID::GameAction::Up;
	if (str == "Down") return ActionID::GameAction::Down;
	if (str == "Left") return ActionID::GameAction::Left;
	if (str == "Right") return ActionID::GameAction::Right;
	if (str == "Jump") return ActionID::GameAction::Jump;
	if (str == "Dash") return ActionID::GameAction::Dash;
	if (str == "Crouch") return ActionID::GameAction::Crouch;
	if (str == "Zoom") return ActionID::GameAction::Zoom;
	if (str == "Shoot") return ActionID::GameAction::Shoot;
	return static_cast<ActionID::GameAction>(-1); // エラー用に用意
}

ActionID::UI InputManager::StringToUIKey(const std::string& str)
{
	if (str == "Up") return ActionID::UI::Up;
	if (str == "Down") return ActionID::UI::Down;
	if (str == "Left") return ActionID::UI::Left;
	if (str == "Right") return ActionID::UI::Right;
	if (str == "Decide") return ActionID::UI::Decide;
	if (str == "Cancel") return ActionID::UI::Cancel;
	if (str == "Pause") return ActionID::UI::Pause;
	return static_cast<ActionID::UI>(-1); // エラー用
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

void InputManager::SetMouseSensibility()
{
	if (currentMode == InputMode::Game) return; // UI操作中でなければ行わない
	if (count % BLOCK_CONTINUOUS_INPUT != 0) return; // 定数フレーム分待つ
	if (GetButtonStay(ActionID::UI::Right)) mouseSensibility += ADJ_MOUSE_SENSIBILITY;
	if (GetButtonStay(ActionID::UI::Left)) mouseSensibility -= ADJ_MOUSE_SENSIBILITY;

	if (mouseSensibility > MAX_MOUSE_SENSIBILITY) mouseSensibility = MAX_MOUSE_SENSIBILITY;
	if (mouseSensibility < MIN_MOUSE_SEBSIBILITY) mouseSensibility = MIN_MOUSE_SEBSIBILITY;
}

void InputManager::SetStickDeadZone()
{
	if (currentMode == InputMode::Game) return; // UI操作中でなければ行わない
	if (count % BLOCK_CONTINUOUS_INPUT != 0) return; // 定数フレーム分待つ
	if (GetButtonStay(ActionID::UI::Right)) stickRDeadZone += ADJ_STICK_DEAD_ZONE;
	if (GetButtonStay(ActionID::UI::Left)) stickRDeadZone -= ADJ_STICK_DEAD_ZONE;

	if (stickRDeadZone > MAX_STICK_DEAD_ZONE) stickRDeadZone = MAX_STICK_DEAD_ZONE;
	if (stickRDeadZone < MIN_STICK_DEAD_ZONE) stickRDeadZone = MIN_STICK_DEAD_ZONE;
}

void InputManager::SetStickSensibility()
{
	if (currentMode == InputMode::Game) return; // UI操作中でなければ行わない
	if (count % BLOCK_CONTINUOUS_INPUT != 0) return; // 定数フレーム分待つ
	if (GetButtonStay(ActionID::UI::Right)) stickSensibility += ADJ_STICK_SENSIBILITY;
	if (GetButtonStay(ActionID::UI::Left)) stickSensibility -= ADJ_STICK_SENSIBILITY;

	if (stickSensibility > MAX_STICK_SENSIBILITY) stickSensibility = MAX_STICK_SENSIBILITY;
	if (stickSensibility < MIN_STICK_SENSIBILITY) stickSensibility = MIN_STICK_SENSIBILITY;
}

void InputManager::SetTriggerDeadZone()
{
	if (currentMode == InputMode::Game) return; // UI操作中でなければ行わない
	if (count % BLOCK_CONTINUOUS_INPUT != 0) return; // 定数フレーム分待つ
	if (GetButtonStay(ActionID::UI::Right)) padTriggerDeadZone += ADJ_TRIGGER_DEAD_ZONE;
	if (GetButtonStay(ActionID::UI::Left)) padTriggerDeadZone -= ADJ_TRIGGER_DEAD_ZONE;

	if (padTriggerDeadZone > MAX_TRIGGER_DEAD_ZONE) padTriggerDeadZone = MAX_TRIGGER_DEAD_ZONE;
	if (padTriggerDeadZone < MIN_TRIGGER_DEAD_ZONE) padTriggerDeadZone = MIN_TRIGGER_DEAD_ZONE;
}