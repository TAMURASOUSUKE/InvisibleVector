#pragma once
// 物理挙動に関する定数を作る

constexpr float GRAVITY{ 800.0f }; // 重力
constexpr float DEFAULT_MASS{ 1.0f }; // 質量
constexpr float DEFAULT_BAUNCINESS{ 0.3f }; // 反発係数
constexpr float DEFAULT_DRAG{ 0.01f }; // 抵抗力
constexpr float DEFAULT_FRICTION{1.0f}; // 摩擦
constexpr float GROUND_NORMAL_THRESHOLD{ 0.7f }; // 法線のY成分がこれ以上なら地面扱い
constexpr float VELOCITY_SLEEP_THRESHOLD{ 0.5f }; // 速度がこれ以下なら停止とみなす
constexpr float UNGROUND_VELOCITY{ 5.0f };  // これ以上の上向き速度で接地解除