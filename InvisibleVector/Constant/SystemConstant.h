#pragma once
/*
	ここはシステム系に関する定数や構造体をまとめたクラス
*/


// [EN] 1second = 1,000,000 microseconds [JP] 1秒 = 1,000,000マイクロ秒
constexpr int ONE_SEC_MICRO{ 1000000 };
constexpr int ONE_SEC_MILLI{ 1000 }; // [EN] 1second = 1,000 milliseconds [JP] 1秒 = 1,000ミリ秒
constexpr int USE_FPS{ 60 }; // [EN] Target FPS value. [JP] 目標FPS値
constexpr int SAMPLE_FRAME_COUNT{ 60 }; // [EN] Number of frames to sample for FPS calculation. [JP] FPS計算のためのサンプルフレーム数
constexpr int DEBUG_FPS_POSITION_X{ 20 };
constexpr int DEBUG_FPS_POSITION_Y{ 40 };
constexpr int DEBUG_DELTA_TIME_POSITION_X{ 20 };
constexpr int DEBUG_DELTA_TIME_POSITION_Y{ 60 };
constexpr float LIMIT_DELTA_TIME{ 0.05f }; // [EN] DeltaTime limit when processing slows down. [JP] 処理落ちの際のデルタタイム制限
