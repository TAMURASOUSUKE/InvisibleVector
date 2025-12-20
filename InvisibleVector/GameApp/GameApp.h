#pragma once




/// <summary>
/// エントリーポイントとゲームループを分離し可読性を向上させるためのクラス。
/// インスタンスがグローバル変数や静的変数として呼ばれた場合にも対応できるように、
/// 明示的に初期化関数を作成しゲームループ開始前に呼ぶようにする
/// このクラスのインスタンスはMain.cppが所有するものとし、コピーを禁止する
/// </summary>
class GameApp
{
public:
	GameApp();
	~GameApp(); // Dxlibの終了処理などを担当する
	GameApp(const GameApp& other) = delete; // コピーコンストラクタの禁止
	GameApp& operator=(const GameApp&) = delete; // コピー代入も禁止

	bool Initialize(); // Dxlibの初期化等を行う
	void Run(); // responsible for GameLoop(ゲームループの責任を持つ)

private:
	void Update(); // 入力やオブジェクトの更新関数
	void Draw(); // 描画だけでなく画面クリアやフリップも担当する

private:
	unsigned int white{0};
	unsigned int red{0};
};