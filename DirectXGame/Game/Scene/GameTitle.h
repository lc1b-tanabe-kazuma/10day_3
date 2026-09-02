#pragma once
#include "KamataEngine.h"
#include "SceneBase.h"
#include "Aim/Aim.h"

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

	bool IsMouseOver(KamataEngine::Vector2 mouse, KamataEngine::Vector2 pos, KamataEngine::Vector2 size);

	~GameTitle();

private:
	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera camera_;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	// ウィンドウの幅と高さ
	float windowWidth = 1280.0f;
	float windowHeight = 720.0f;

	Aim* aim_ = nullptr;

	float buttonTimer_ = 0.0f;

	// 進むボタン
	KamataEngine::Sprite* startButtonSprite_ = nullptr;
	KamataEngine::Vector2 startButtonSize = {256.0f, 128.0f};
	KamataEngine::Vector2 startButtonPos = {windowWidth / 2.0f, windowHeight / 1.25f};
	uint32_t startButtonSpriteTH_ = 0;
};