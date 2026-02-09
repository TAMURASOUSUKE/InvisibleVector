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
	/// [EN] Set key binding for an action (Overwirte)
	/// [JP] アクションに対するキー割り当てを設定する関数
	/// </summary>
	/// <param name="action">[EN] Target action [JP] 設定したいアクション</param>
	/// <param name="key">[EN] Keyboard key code [JP] キーボードを設定する</param>
	/// <param name="padMask">[EN] Gamepad button mask [JP] ゲームパッドのボタンマスク</param>
	void SetBinding(GameKey action, int key, int padMask);

	/// <summary>
	/// [EN] Add a secondary key to an action
	/// [JP] アクションに二つ目のサブキーを追加する
	/// </summary>
	/// <param name="action">[EN] Target action [JP] 設定したいアクション</param>
	/// <param name="key">[EN] Sub key [JP] サブキー</param>
	void AddKeyBinding(GameKey action, int key);

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

	bool GetButtonStay(GameKey key) const; // 押している間(押しっぱなし)
	bool GetButtonDown(GameKey key) const; // 押した瞬間
	bool GetButtonUp(GameKey key) const; // 離した瞬間

private:
	/// <summary>
	/// [EN] Convert concrete input to abstract input
	/// [JP] 具体入力を抽象入力へと変換する
	/// </summary>
	void UpdateGameKey();

	// デフォルトのキーコンフィグ設定(ファイルがないときなどに使用する)
	void SetDefaultBindings();

	// EnumとStringの変換Map
	std::string GameKeyToString(GameKey key);
	// 文字列をEnumへ変換
	GameKey StringToGameKey(const std::string& str);

private:
	// コンフィグファイルへのパス
	const std::string configFilePath = "key_config.json";
	int currentPadInput{ 0 }; // [EN] GamePad [JP] ゲームパッド

	// GameKey::Countの分(状態の数)の長さの配列を作る
	std::array<bool, static_cast<int>(GameKey::Count)> currentStates; // [EN] Now State [JP] 現在の状態
	std::array<bool, static_cast<int>(GameKey::Count)> prevStates; // [EN] Previous State [JP] 1フレーム前の状態
	std::array<InputBinding, static_cast<int>(GameKey::Count)> bindings; // [EN] Mapping data [JP] 全アクションのマッピングデータ
	std::array<char, 256> currentKeyBuffer; // [EN] Keyboard [JP] キーボード

	Vector2 axis{0.0f, 0.0f}; // [EN] Stick value [JP] スティックの値
};
