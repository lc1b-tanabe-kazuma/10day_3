#define NOMINMAX
#include "Enemy.h"
#include "MyMath.h"
#include "Player.h"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace KamataEngine;

void Enemy::Initialize(Model* model, Camera* camera, Model* bulletModel, Player* player) {

	// NULLポインタのチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// 弾のモデル
	bulletModel_ = bulletModel;

	// ワールド変換の初期化
	worldTranseform_.Initialize();

	worldTranseform_.scale_ = {0.5f, 0.5f, 0.5f};

	// 引数の内容をメンバ変数に記録
	camera_ = camera;

	// プレイヤーのポインタを記録
	player_ = player;
}

void Enemy::Update() {
	if (isDead_ || isOut_) {
		return;
	}

	// 移動
	worldTranseform_.translation_ += moveVec_;

	Vector3 dir = Normalize(moveVec_);

	worldTranseform_.rotation_.y = std::atan2(dir.x, dir.z);

	// 攻撃
	attackTimer_ -= 1.0f / 30.0f;
	if (attackTimer_ <= 0.0f) {
		Attack();
		attackTimer_ = 2.0f;
	}

	// 弾の更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグが立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTranseform_);
}

void Enemy::Attack() {
	EnemyBullet* bullet = new EnemyBullet();

	Vector3 playerPosition = player_->GetWorldPosition();

	Vector3 direction = playerPosition - GetWorldPosition();

	direction = Normalize(direction);

	Vector3 velocity = direction * bulletSpeed_;

	bullet->Initialize(bulletModel_, GetWorldPosition(), velocity);

	bullets_.push_back(bullet);
}

Vector3 Enemy::GetWorldPosition() const {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTranseform_.matWorld_.m[3][0];
	worldPos.y = worldTranseform_.matWorld_.m[3][1];
	worldPos.z = worldTranseform_.matWorld_.m[3][2];
	return worldPos;
}

// 敵の生成時の位置を設定
void Enemy::SetPosition(const Vector3& position) { worldTranseform_.translation_ = position; }

void Enemy::Draw() {

	if (!isDead_ && !isOut_) {
		// 敵の描画
		model_->Draw(worldTranseform_, *camera_);
	}

	// 弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

// 画面外に出た時にデリートする
void Enemy::OutFlag() {
	if (worldTranseform_.translation_.z <= -5.5f || worldTranseform_.translation_.x <= -40.0f || worldTranseform_.translation_.x >= 40.0f) {
		isOut_ = true;
	}
}