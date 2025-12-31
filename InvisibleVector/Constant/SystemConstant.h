#pragma once
#include <limits>
/*
	ここはシステム系に関する定数や構造体をまとめたクラス
*/

// =========================================================== FPS ===========================================================================

// [EN] 1second = 1,000,000 microseconds [JP] 1秒 = 1,000,000マイクロ秒
constexpr int ONE_SEC_MICRO{ 1000000 };
constexpr int ONE_SEC_MILLI{ 1000 }; // [EN] 1second = 1,000 milliseconds [JP] 1秒 = 1,000ミリ秒
constexpr int USE_FPS{ 60 }; // [EN] Target FPS value. [JP] 目標FPS値
constexpr int SAMPLE_FRAME_COUNT{ 60 }; // [EN] Number of frames to sample for FPS calculation. [JP] FPS計算のためのサンプルフレーム数
constexpr int DEBUG_FPS_POSITION_X{ 20 };
constexpr int DEBUG_FPS_POSITION_Y{ 20 };
constexpr int DEBUG_DELTA_TIME_POSITION_X{ 20 };
constexpr int DEBUG_DELTA_TIME_POSITION_Y{ 40 };
constexpr float LIMIT_DELTA_TIME{ 0.05f }; // [EN] DeltaTime limit when processing slows down. [JP] 処理落ちの際のデルタタイム制限

// =========================================================== Math ==========================================================================
// [EN] DefaultEpsilon [JP] デフォルトのイプシロン
template<typename T>
constexpr T GAME_EPSILON = std::numeric_limits<T>::epsilon();

// [EN] type float [JP] floatのイプシロン
template<>
constexpr float GAME_EPSILON<float>{1e-5f};

// [EN] type double [JP] doubleのイプシロン
template<>
constexpr double GAME_EPSILON<double>{1e-9};

// [EN] type int [JP] intのイプシロン
template<>
constexpr int GAME_EPSILON<int>{0};

constexpr int DEBUG_DELTA_POSITION_LOG_X{ 20 };
constexpr int DEBUG_DELTA_POSITION_LOG_Y{ 60 };