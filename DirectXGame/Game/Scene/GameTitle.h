#pragma once
#include "Aim/Aim.h"
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

	// チュートリアルボタン
	KamataEngine::Sprite* tutorialButtonSprite_ = nullptr;
	KamataEngine::Vector2 tutorialButtonSize = {256.0f, 128.0f};
	KamataEngine::Vector2 tutorialButtonPos = {windowWidth / 1.25f, windowHeight / 1.25f};
	uint32_t tutorialButtonSpriteTH_ = 0;

	// チュートリアル画像
	KamataEngine::Sprite* tutorialImageSprite_ = nullptr;
	uint32_t tutorialImageSpriteTH_ = 0;

	// チュートリアル用の黒い背景
	KamataEngine::Sprite* tutorialBackgroundSprite_ = nullptr;
	uint32_t tutorialBackgroundSpriteTH_ = 0;

	enum class ButtonState { None, Tutorial };
	ButtonState buttonState_ = ButtonState::None;

	// 背景
	uint32_t backgroundSpriteTH_ = 0;
	KamataEngine::Sprite* backgroundSprite_ = nullptr;

	// タイトルロゴ
	uint32_t titleLogoSpriteTH_ = 0;
	KamataEngine::Sprite* titleLogoSprite_ = nullptr;

	// BGM関連
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;

	// SE（効果音）用ハンドル
	uint32_t clickSoundHandle_ = 0;
};