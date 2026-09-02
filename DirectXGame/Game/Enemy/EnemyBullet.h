#pragma once
#include "KamataEngine.h"

class EnemyBullet {
public:
	void Initialize(KamataEngine::Model* model, const KamataEngine::Vector3& position, const KamataEngine::Vector3& velocity);

	void Update();

	void Draw(const KamataEngine::Camera& camera);

	KamataEngine::Vector3 GetPosition() const;

	float GetRadius() const { return radius_; }

	bool IsDead() const { return isDead_; }

	void OnCollision() { isDead_ = true; }

private:
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	KamataEngine::Vector3 velocity_{};

	float radius_ = 1.0f;

	float deathTimer_ = 3.0f;

	bool isDead_ = false;
};