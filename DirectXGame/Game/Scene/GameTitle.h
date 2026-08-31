#pragma once
#include "KamataEngine.h"
#include "SceneBase.h"

using namespace KamataEngine;

// ゲームシーン
class GameTitle : public SceneBase {
public:
	GameTitle(KamataEngine::Input* input) : SceneBase(input) {}

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	~GameTitle();

private:
	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
};