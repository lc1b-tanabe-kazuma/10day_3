#include "Effect.h"
#include "MyMath.h"

using namespace KamataEngine;

Effect::~Effect() { delete model_; }

void Effect::Initialize(const Vector3& position) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	model_ = Model::CreateFromOBJ("Effect");
}

void Effect::Update() {
	// タイマー更新
	timer_ += 1.0f / 60.0f;

	// エフェクトの進行度 0.0 ～ 1.0
	float t = timer_ / lifeTime_;

	if (t >= 1.0f) {
		t = 1.0f;
		isFinished_ = true;
	}

	// 最初は小さく、途中で一気に大きくする
	float scale = 1.0f;

	if (t < 0.3f) {
		// 0 → 0.3秒：急激に拡大
		float localT = t / 0.3f;
		scale = 0.5f + localT * 2.0f;
	} else {
		// 0.3秒 → 0.4秒：少し縮む
		float localT = (t - 0.3f) / 0.7f;
		scale = 2.5f - localT * 1.0f;
	}

	worldTransform_.scale_ = {scale, scale, scale};

	model_->SetAlpha(1.0f - t);

	// ワールド行列更新
	WorldTransformUpdate(worldTransform_);
}

void Effect::Draw(Camera* camera) { model_->Draw(worldTransform_, *camera); }