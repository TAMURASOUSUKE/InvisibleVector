#pragma once
/*
	ゲームに関する定数や汎用的な構造体等を定義する
*/

constexpr int SCREEN_WIDTH{ 1920 }; // 横幅
constexpr int SCREEN_HEIGHT{ 1080 }; // 縦幅
constexpr int DEBUG_SCREEN_WIDTH{ 1280 }; // デバッグする際の横幅
constexpr int DEBUG_SCREEN_HEIGHT{ 720 }; // デバッグする際の縦幅
constexpr int COLOR_DEPTH{ 32 }; // 色の深度
constexpr float CAMERA_NEAR{ 1.0f }; // 視錐台の上面までの距離
constexpr float CAMERA_FAR{ 10000.0f }; // 視錐台の底面までの距離

// 確率で使うもの関連
/*
	RANGE類に関しては(rand() % MAX_...RANGE) + MIN_...RANGEの形で実装しています
	なので実際の値としては
	最大 = MAX...RANGE + MIN...RANGE
	最小 = MIN...RANGE
	その間 = 出てきた値 + MIN_...RANGE
	となります
*/
constexpr int MAX_PROBABIRITY{ 100 }; // 確率最大値(100%)
constexpr int BLINK_RESET_PROBABILITY{ 2 }; // 点滅させる際にどのくらいの確率で変数をランダムリセットするか
constexpr int CHANGE_FLICKER_RATE{ 4 }; // 点滅間隔を変える確率
constexpr int MAX_FLICKER_RATE_RANGE{ 30 }; // 最大点滅間隔の範囲(フレーム)
constexpr int MIN_FLICKER_RATE_RANGE{ 4 }; // 最小点滅間隔の範囲(フレーム)
constexpr int MAX_FLICKER_DURATION_RANGE{ 120 }; // 最大点滅時間の設定範囲(フレーム)
constexpr int MIN_FLICKER_DURATION_RANGE{ 3 }; // 最小点滅時間の設定範囲(フレーム)
constexpr float RAND_RESOLUTION{ 1000.0f }; // 0.0 - 1.0の間を1000段階に分ける分解能

// フェード系
constexpr float HALF_TIME{ 0.5f }; // フェード処理に使う時間の半分
constexpr float DEFAULT_LIFT_SPEED{ 0.005f }; // デフォルトのフェード周期
constexpr float ADJ_LIFT_SPEED{ 0.0001f }; // 周期をばらつかせるタイミングで使う

// 各サイズや座標
constexpr float TITLE_FLOATING_WIDTH{ 1600.0f }; // タイトルロゴの横幅
constexpr float TITLE_FLOATING_HEIGHT{ 500.0f }; // タイトルロゴの縦幅
constexpr float TITLE_LOGO_POS_X{ 460.0f }; // タイトルロゴX位置
constexpr float TITLE_LOGO_POS_Y{ 150.0f }; // タイトルロゴY位置
constexpr float TITLE_CHANGE_SCENE_LOGO_X{ 660.0f }; // タイトル遷移ロゴのX座標
constexpr float TITLE_CHANGE_SCENE_LOGO_Y{ 850.0f }; // タイトル遷移ロゴのY座標
constexpr float GAMEOVER_CHANGE_SCENE_LOGO_X{ 660.0f }; // ゲームオーバー遷移ロゴのX座標
constexpr float GAMEOVER_CHANGE_SCENE_LOGO_Y{ 850.0f }; // ゲームオーバー遷移ロゴのY座標
constexpr float GAMECLEAR_CHANGE_SCENE_LOGO_X{ 660.0f }; // ゲームクリア遷移ロゴのX座標
constexpr float GAMECLEAR_CHANGE_SCENE_LOGO_Y{ 850.0f }; // ゲームクリア遷移ロゴのY座標

constexpr float WALL_SIZE{ 20.0f }; // 壁のデフォルトサイズ
constexpr float TARGET_SIZE{ 10.0f }; // 的のデフォルトサイズ

constexpr float RETICLE_SIZE{ 300.0f };

constexpr int FLOATING_BOX_DRAW_VALUE{ 10 }; // ロゴの裏に描画するBoxの数
constexpr int FONT_SIZE{ 80 };
constexpr int FONT_THICK{ 20 };

// 描画のレイヤーを分け描画するタイミングを決定する
enum class RendereLayer
{
	None,
	World, // 3D
	UI, // 2D
};

// 遷移の際GPU側に渡すデータを構造体化する(定数バッファ)
struct TransitionConstantBuffer
{
	float progress; // 遷移状態(0.0f - 1.0f)
	float padding[3]; // 16バイトに調整するための空白
};