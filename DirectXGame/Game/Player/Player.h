#pragma once
#include "KamataEngine.h"
#include "PlayerBullet.h"

// 前方宣言
class Aim;
class Boss;

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Model* bulletModel, Aim* aim);
	void Update();
	void Draw();

	void Move();

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition() const;

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// ワールドトランスフォームのゲッターをconst参照をreturnする関数として作る
	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// 半径を取得
	float GetRadius() const { return kRadius; }

	void OnCollision();

	void Attack();

	void SetBoss(Boss* boss) { boss_ = boss; }

	// 敵に攻撃がヒットした
	void OnEnemyHit();

	// コンボ取得
	int GetCombo() const { return combo_; }

	float GetChargeRate() const { return chargeTime_ / kMaxChargeTime; }

	~Player();

private:
	KamataEngine::Model* model_;
	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::Camera* camera_ = nullptr;

	// キーボード入力
	KamataEngine::Input* input_ = nullptr;

	float moveSpeed_ = 0.1f;

	// 弾丸
	std::list<PlayerBullet*> bullets_;

	KamataEngine::Model* bulletModel_ = nullptr;

	// 弾の速度
	float kBulletSpeed = 1.0f;

	// 半径
	const float kRadius = 1.0f;

	//
	Aim* aim_ = nullptr;

	// 無敵時間
	float kInvincibleTime = 2.0f;

	// 被弾フラグ
	bool isHit_ = false;

	float angleZ = 7.85f; // Z軸回転角（ラジアン）
	float radius = 64.0f; // 原点からの距離
	const float kRotateSpeed = 0.01f;
	float verticalSpeed_ = 0.2f;
	float minY_ = -5.0f;
	float maxY_ = 5.0f;

	const float maxAngle = 8.7f;
	const float minAngle = 7.0f;

	Boss* boss_ = nullptr;

	KamataEngine::Vector3 forward_ = {0, 0, 1}; // 見た目と独立した進行方向

	// コンボ
	int combo_ = 0;

	// コンボ継続時間
	float comboTimer_ = 0.0f;

	// コンボが続く時間
	static constexpr float kComboTime = 2.0f;

	enum class ChargeState { None, Charging };

	//
	ChargeState chargeState_ = ChargeState::None;

	float chargeTime_ = 0.0f;

	static constexpr float kMaxChargeTime = 3.0f;
};