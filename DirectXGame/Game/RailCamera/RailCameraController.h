#pragma once
#include "KamataEngine.h"

// プレイヤークラスを前方宣言
class Player;

// ボス
class Boss;

class RailCameraController {
public:
	// 初期化
	void Initialize(KamataEngine::Camera* camera, const Player* player, const Boss*);

	// 更新
	void Update();

	KamataEngine::Camera& GetCamera() { return *camera_; }

	// 親を設定（プレイヤー or ボス）
	void SetParent(const KamataEngine::WorldTransform* parent);

	// デバッグ用
	KamataEngine::Vector3 GetWorldPosition();

private:
	// カメラ本体
	KamataEngine::Camera* camera_ = nullptr;

	// カメラ用ワールド変換
	KamataEngine::WorldTransform worldTransform_;

	// レール用パラメータ
	float angle_ = 0.0f;
	float radius_ = 40.0f; // 中心からの距離
	float height_ = 8.0f;  // 高さ

	const Player* player_ = nullptr;
	float lookAhead_ = 10.0f;
	float sideRadius_ = 6.0f; // 左右の最大振れ幅

	// +1.0f = 右側固定 / -1.0f = 左側固定
	float fixedSide_ = 1.0f;

	const Boss* boss_ = nullptr;
};