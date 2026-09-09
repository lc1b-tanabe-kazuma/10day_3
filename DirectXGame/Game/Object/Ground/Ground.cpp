#include "Ground.h"
#include "MyMath.h"

using namespace KamataEngine;

// 初期化
void Ground::Initialize(Model* model, Camera* camera) {
	worldTransform_.Initialize();

	model_ = model;
	camera_ = camera;
	worldTransform_.translation_ = {0.0f, -15.0f, 0.0f};
	WorldTransformUpdate(worldTransform_);
}

// 更新
void Ground::Update() { worldTransform_.TransferMatrix(); }

// 描画
void Ground::Draw() {

	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}
