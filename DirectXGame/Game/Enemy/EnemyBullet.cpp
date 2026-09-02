#include "EnemyBullet.h"
#include "MYMath.h"

using namespace KamataEngine;

void EnemyBullet::Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity) {
	model_ = model;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velocity;

	// 向き
	worldTransform_.rotation_.y = -3.14f / 2.0f;

	WorldTransformUpdate(worldTransform_);
}

void EnemyBullet::Update() {

	// 寿命
	deathTimer_ -= 1.0f / 30.0f;

	if (deathTimer_ <= 0) {
		isDead_ = true;
	}

	// 移動
	worldTransform_.translation_ += velocity_;

	WorldTransformUpdate(worldTransform_);
}

void EnemyBullet::Draw(const KamataEngine::Camera& camera) { model_->Draw(worldTransform_, camera); }

KamataEngine::Vector3 EnemyBullet::GetPosition() const { return worldTransform_.translation_; }