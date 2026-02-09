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
	InputManager();
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
	/// [EN] Get the trigger pressing depth
	/// [JP] トリガーを押す深さを取得する
	/// </summary>
	/// <returns>[EN] Trigger press depth [JP] トリガーを押す深さ</returns>
	const int GetTriggerDeadZone() const { return padTriggerDeadZone; }
	/// <summary>
	/// [EN] Set only key binding for an action
	/// [EN] キーボードのみの入力設定
	/// </summary>
	/// <param name="action">[EN] Game name [JP] アクション名</param>
	/// <param name="key">[EN] key [JP] 設定するキー</param>
	void SetBindingKey(GameAction action, int key);


	/// <summary>
	/// [EN] Add a secondary key to an action
	/// [JP] アクションに二つ目のサブキーを追加する
	/// </summary>
	/// <param name="action">[EN] Target action [JP] 設定したいアクション</param>
	/// <param name="key">[EN] Sub key [JP] サブキー</param>
	void AddKeyBinding(GameAction action, int key);

	/// <summary>
	/// [EN] Set only pad button binding for an action
	/// [EN] パッドボタンのみの入力設定
	/// </summary>
	/// <param name="action">[EN] Game name [JP] アクション名</param>
	/// <param name="pad">[EN] pad button [JP] 設定するパッドボタン</param>
	void SetBindingPad(GameAction action, int padMask);

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

	bool GetButtonStay(GameAction key) const; // 押している間(押しっぱなし)
	bool GetButtonDown(GameAction key) const; // 押した瞬間
	bool GetButtonUp(GameAction key) const; // 離した瞬間

private:
	/// <summary>
	/// [EN] Convert concrete input to abstract input
	/// [JP] 具体入力を抽象入力へと変換する
	/// </summary>
	void UpdateGameKey();

	// デフォルトのキーコンフィグ設定(ファイルがないときなどに使用する)
	void SetDefaultBindings();

	// EnumとStringの変換Map
	std::string GameKeyToString(GameAction key);
	// 文字列をEnumへ変換
	GameAction StringToGameKey(const std::string& str);

private:
	// コンフィグファイルへのパス
	const std::string configFilePath = "key_config.json";
	int currentPadInput{ 0 }; // [EN] GamePad [JP] ゲームパッド
	int padTriggerDeadZone{ 0 }; // [EN] Trigger press depth [JP] トリガーを押す深さ

	// GameKey::Countの分(状態の数)の長さの配列を作る
	std::array<bool, static_cast<int>(GameAction::Count)> currentStates; // [EN] Now State [JP] 現在の状態
	std::array<bool, static_cast<int>(GameAction::Count)> prevStates; // [EN] Previous State [JP] 1フレーム前の状態
	std::array<InputBinding, static_cast<int>(GameAction::Count)> bindings; // [EN] Mapping data [JP] 全アクションのマッピングデータ
	std::array<char, 256> currentKeyBuffer; // [EN] Keyboard [JP] キーボード

	Vector2 axis{0.0f, 0.0f}; // [EN] Stick value [JP] スティックの値

	XINPUT_STATE xinputState; // Triggerなどのボタンを操作するための変数
};
