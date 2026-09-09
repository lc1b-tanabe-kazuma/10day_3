#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Skydome {
public:
	// 初期化
	void Initialize(Model* model, Camera* camera);

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_;

	// スカイドーム
	Skydome* skydome_ = nullptr;

	float roteSpeed = 0.0002f;
};