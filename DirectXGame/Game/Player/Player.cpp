#define NOMINMAX
#include "Player.h"
#include "MyMath.h"
#include "Aim/Aim.h"

using namespace KamataEngine;

void Player::Initialize(Model* model, Camera* camera, Model* bulletModel, Aim* aim) {
	model_ = model;
	camera_ = camera;
	bulletModel_ = bulletModel;

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.scale_ = {1.3f, 1.3f, 1.3f};
	worldTransform_.rotation_.y = 3.14f / 2.0f;

	input_ = Input::GetInstance();

	aim_ = aim;
}

void Player::OnCollision() {

	// 無敵時間中は当たり判定を無効化
	if (isHit_) {
		return;
	}

	// 被弾フラグを立てる
	isHit_ = true;
}

Player::~Player() {
	// 弾の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
}

void Player::Update() {
	Move();

	// 弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 無敵時間の処理
	if (isHit_) {
		kInvincibleTime -= 1.0f / 30.0f; // 30FPSで減算
	}

	// 無敵時間をリセット
	if (kInvincibleTime <= 0.0f) {
		isHit_ = false;
		kInvincibleTime = 2.0f;
	}
}

void Player::Draw() {

	// 弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}

	// 無敵時間中
	if (isHit_) {

		// 30フレーム周期で点滅
		if (static_cast<int>(kInvincibleTime * 30.0f) % 10 < 5) {
			return;
		}
	}

	// プレイヤーの描画
	model_->Draw(worldTransform_, *camera_);
}

// 移動処理
void Player::Move() {
	if (input_->PushKey(DIK_W)) {
		worldTransform_.translation_.y += moveSpeed_;
	}

	if (input_->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= moveSpeed_;
	}

	if (input_->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= moveSpeed_;
	}

	if (input_->PushKey(DIK_D)) {
		worldTransform_.translation_.x += moveSpeed_;
	}

	// 制限を設ける
	worldTransform_.translation_.x = std::max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = std::min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = std::max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = std::min(worldTransform_.translation_.y, kMoveLimitY);

	// ワールド変換行列の更新
	WorldTransformUpdate(worldTransform_);
}

void Player::Attack() {

	// 弾を生成
	PlayerBullet* newBullet = new PlayerBullet();

	// Aimからマウス方向のRayを取得
	Ray ray = aim_->GetRayFromMouse();

	// Z=22.5の平面との交点
	float t = (kAimZ - ray.origin.z) / ray.direction.z;

	Vector3 targetPos = ray.origin + ray.direction * t;

	// プレイヤーから交点へ向かう
	Vector3 bulletDir = Normalize(targetPos - GetWorldPosition());

	Vector3 bulletVelocity = bulletDir * kBulletSpeed;

	// 弾の初期化
	newBullet->Initialize(bulletModel_, GetWorldPosition(), bulletVelocity);

	// リストへ追加
	bullets_.push_back(newBullet);
}

Vector3 Player::GetWorldPosition() const { return worldTransform_.translation_; }