#pragma once
#include "KamataEngine.h"

class Effect {
public:
	~Effect();

	void Initialize(const KamataEngine::Vector3& position);
	void Update();
	void Draw(KamataEngine::Camera* camera);

	bool IsFinished() const { return isFinished_; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	float timer_ = 0.0f;
	float lifeTime_ = 0.4f;

	bool isFinished_ = false;
};