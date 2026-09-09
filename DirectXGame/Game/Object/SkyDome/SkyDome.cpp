#include "Skydome.h"
#include "MyMath.h"

// 初期化
void Skydome::Initialize(Model* model, Camera* camera) {
	worldTransform_.Initialize();

	model_ = model;
	camera_ = camera;
}

// 更新
void Skydome::Update() {

	// 回転する
	worldTransform_.rotation_.y += roteSpeed;

	// 元に戻す
	if (worldTransform_.rotation_.y > 2.0f * 3.141592654f || worldTransform_.rotation_.y <= 0.0f) {
		roteSpeed = -roteSpeed;
	}

	WorldTransformUpdate(worldTransform_);
}

// 描画
void Skydome::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}
