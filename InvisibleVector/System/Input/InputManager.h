#pragma once
#include <array>
#include <string>
#include "Vector_Dxlib.h"
#include "GameKey.h"


/*
	[EN] Implement Abstraction input system [JP]抽象的なインプットシステムを実装する
*/
class InputManager
{
public:
	InputManager(int windowWidth, int windowHeight);
	virtual ~InputManager() = default;

	/// <summary>
	/// [EN] Update input state (Call every frame)
	/// [JP] 入力の更新 毎フレーム呼ぶ
	/// </summary>
	void Update();

	/// <summary>
	/// [EN] Get normalized movement vector
	/// [JP] 正規化された移動ベクトルを取得
	/// </summary>
	/// <returns>[EN] Normalized vector [JP] 正規化されたベクトル</returns>
	const Vector2& GetAxis() const { return axis; }

	/// <summary>
	/// [EN] Get current camera axis
	/// [JP] 現在のカメラのaxisを取得する
	/// </summary>
	/// <returns>[EN] camera axis [JP] カメラaxis</returns>
	const Vector2& GetCameraAxis() const { return cameraAxis; }

	/// <summary>
	/// [EN] Get current input mode
	/// [JP] 現在の入力状態を取得する
	/// </summary>
	/// <returns></returns>
	const InputMode GetInputMode() const{ return currentMode; }

	/// <summary>
	/// [EN] Get the trigger pressing depth
	/// [JP] トリガーを押す深さを取得する
	/// </summary>
	/// <returns>[EN] Trigger press depth [JP] トリガーを押す深さ</returns>
	const int GetTriggerDeadZone() const { return padTriggerDeadZone; }

	/// <summary>
	/// [EN] Get the R stick dead zone
	/// [JP] Rスティックのデッドゾーンを取得
	/// </summary>
	/// <returns></returns>
	const int GetStickDeadZone() const { return stickRDeadZone; }

	/// <summary>
	/// [EN] Get the mouse sensibility
	/// [JP] マウス感度を取得
	/// </summary>
	/// <returns></returns>
	const float GetMouseSensibility() const { return mouseSensibility; }
	
	/// <summary>
	/// [EN] Get the R stick sensibility
	/// [JP] スティック感度を取得
	/// </summary>
	/// <returns></returns>
	const float GetStickSensibility() const { return stickSensibility; }
	/// <summary>
	/// [EN] Set only key binding for an action
	/// [EN] キーボードのみの入力設定
	/// </summary>
	/// <param name="action">[EN] Game name [JP] アクション名</param>
	/// <param name="key">[EN] key [JP] 設定するキー</param>
	void SetBindingKey(ActionID::GameAction action, int key);

	// キー入力設定関数をUI用にoverrideしたもの
	void SetBindingKey(ActionID::UI action, int key);


	/// <summary>
	/// [EN] Add a secondary key to an action
	/// [JP] アクションに二つ目のサブキーを追加する
	/// </summary>
	/// <param name="action">[EN] Target action [JP] 設定したいアクション</param>
	/// <param name="key">[EN] Sub key [JP] サブキー</param>
	void AddKeyBinding(ActionID::GameAction action, int key);

	// サブキーを追加する際のUI用override
	void AddKeyBinding(ActionID::UI action, int key);

	/// <summary>
	/// [EN] Add a secondary button to an action
	/// [JP] アクションに二つ目のサブボタンを追加する
	/// </summary>
	/// <param name="action">[EN] Target action [JP] 設定したいアクション</param>
	/// <param name="buttonMask">[EN] Sub button [JP] サブボタン</param>
	void AddButtonBinding(ActionID::GameAction action, int buttonMask);

	// サブボタンを追加する際のUI用override
	void AddButtonBinding(ActionID::UI action, int buttonMask);

	/// <summary>
	/// [EN] Set only pad button binding for an action
	/// [EN] パッドボタンのみの入力設定
	/// </summary>
	/// <param name="action">[EN] Game name [JP] アクション名</param>
	/// <param name="pad">[EN] pad button [JP] 設定するパッドボタン</param>
	void SetBindingPad(ActionID::GameAction action, int padMask);

	// パッドキーUI設定用のoverride関数
	void SetBindingPad(ActionID::UI action, int padMask); 

	/// <summary>
	/// [EN] Change current input mode
	/// [JP] 現在の入寮状態を変更する
	/// </summary>
	/// <param name="inputMode">[EN] mode [JP] 変更したい状態</param>
	void SetInputMode(InputMode inputMode) { currentMode = inputMode; }

	// Jsonへのキーコンフィグ保存用関数
	void SaveConfig();
	// Jsonからのキーコンフィグ読み込み関数
	void LoadConfig();

	/// <summary>
	/// [EN] Retrun pressed key code
	/// [JP] 押されたキーを返す
	/// </summary>
	/// <returns>[EN] Pressed key [JP] 押されたキー</returns>
	int GetAnyPressedKey() const;

	/// <summary>
	/// [EN] Return pressed button
	/// [JP] 押されたボタンを返す
	/// </summary>
	/// <returns>[EN] Pressed button [JP] 押されたボタン</returns>
	int GetAnyPressedButton() const;

	bool GetButtonStay(ActionID::GameAction key) const; // 押している間(押しっぱなし)
	bool GetButtonStay(ActionID::UI key) const; // 押している間(押しっぱなし)
	bool GetButtonDown(ActionID::GameAction key) const; // 押した瞬間
	bool GetButtonDown(ActionID::UI key) const; // 押した瞬間
	bool GetButtonUp(ActionID::GameAction key) const; // 離した瞬間
	bool GetButtonUp(ActionID::UI key) const; // 離した瞬間


	// これらの関数は今後作るファイルで実装する責任のものなので要修正
	void SetMouseSensibility(); // マウス感度調整関数
	void SetStickSensibility(); // スティック感度
	void SetTriggerDeadZone(); // トリガーを押す深さを調整
	void SetStickDeadZone(); // スティックのデッドゾーンを調整

private:
	/// <summary>
	/// [EN] Convert concrete input to abstract input
	/// [JP] 具体入力を抽象入力へと変換する
	/// </summary>
	void UpdateGameKey();

	/// <summary>
	/// [EN] Update camera axis from Mouse and Right Stick
	/// [JP] マウスと右スティックからカメラ入力を更新する
	/// </summary>
	void UpdateCameraInput();

	// デフォルトのキーコンフィグ設定(ファイルがないときなどに使用する)
	void SetDefaultBindings();

	// EnumとStringの変換Map
	std::string GameKeyToString(ActionID::GameAction key);
	std::string UIKeyToString(ActionID::UI key);
	// 文字列をEnumへ変換
	ActionID::GameAction StringToGameKey(const std::string& str);
	ActionID::UI StringToUIKey(const std::string& str);

private:
	InputMode currentMode{ InputMode::Game }; // 現在の入力状態を管理する
	// コンフィグファイルへのパス
	const std::string configFilePath = "key_config.json";

	int count{ 0 }; // 連続入力防止に使うカウンター
	int currentPadInput{ 0 }; // [EN] GamePad [JP] ゲームパッド
	int padTriggerDeadZone{ 0 }; // [EN] Trigger press depth [JP] トリガーを押す深さ
	int prevMousePosX{ 0 }; // マウスのX座標
	int prevMousePosY{ 0 }; // マウスのY座標
	int stickRDeadZone{ 5000 }; // Rスティックを動かすときのデッドゾーン
	int windowWidth{ 0 }; // 画面横幅
	int windowHeight{ 0 }; // 画面縦幅

	float mouseSensibility{ 0.005f }; // マウス感度
	float stickSensibility{ 1.0f }; // スティック感度

	/*
		UI用のバインディングとゲーム用のバインディング配列を作成し
		それぞれで割り振る
	*/
	// Game用配列
	std::array<bool, static_cast<int>(ActionID::GameAction::Count)> currentGameStates; // [EN] Now State [JP] 現在の状態
	std::array<bool, static_cast<int>(ActionID::GameAction::Count)> prevGameStates; // [EN] Previous State [JP] 1フレーム前の状態
	std::array<InputBinding, static_cast<int>(ActionID::GameAction::Count)> gameBindings; // [EN] Mapping data [JP] 全アクションのマッピングデータ
	// UI用配列
	std::array<bool, static_cast<int>(ActionID::UI::Count)> currentUIStates; // [EN] Now State [JP] 現在の状態
	std::array<bool, static_cast<int>(ActionID::UI::Count)> prevUIStates; // Preciours States [JP] 1フレーム前の状態
	std::array<InputBinding, static_cast<int>(ActionID::UI::Count)> uiBindings; // Mapping data [JP] 全アクションのマッピングデータ
	
	std::array<char, 256> currentKeyBuffer; // [EN] Keyboard [JP] キーボード

	Vector2 axis{0.0f, 0.0f}; // [EN] Stick value [JP] スティックの値
	Vector2 cameraAxis{ 0.0f, 0.0f }; // [EN] Camera input value [JP] カメラへの入力値

	XINPUT_STATE xinputState; // Triggerなどのボタンを操作するための変数
};
