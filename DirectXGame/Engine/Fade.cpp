#include "Fade.h"
using namespace KamataEngine;

Fade::Fade(std::function<void()> onFadeOut, int fadeOutFrames, int fadeInFrames) : fadeOutFrames_(fadeOutFrames), fadeInFrames_(fadeInFrames), onFadeOut_(onFadeOut) {
	currentFrame_ = 0;
	alpha_ = 0.0f;
	finished_ = false;
	fadeOutCalled_ = false;
	phase_ = Phase::FadeOut;
	Initialize();
}

Fade::~Fade() { delete sprite_; }

void Fade::Initialize() {
	textureHandle_ = TextureManager::Load("white1x1.png");
	sprite_ = Sprite::Create(textureHandle_, {0, 0});
	sprite_->SetSize({WinApp::kWindowWidth, WinApp::kWindowHeight});
	sprite_->SetColor({0, 0, 0, 0});
	phase_ = Phase::FadeOut;
	currentFrame_ = 0;
	alpha_ = 0.0f;
	finished_ = false;
	fadeOutCalled_ = false;
}

void Fade::Update() {
	if (finished_)
		return;

	switch (phase_) {
	case Phase::FadeOut:
		currentFrame_++;
		alpha_ = (float)currentFrame_ / fadeOutFrames_;

		if (currentFrame_ >= fadeOutFrames_) {
			alpha_ = 1.0f;

			if (!fadeOutCalled_ && onFadeOut_) {
				onFadeOut_(); // ← ここでInitialize
				fadeOutCalled_ = true;
			}

			phase_ = Phase::Wait;
			currentFrame_ = 0;
		}
		break;

	case Phase::Wait:
		alpha_ = 1.0f; // 完全黒固定

		currentFrame_++;
		if (currentFrame_ >= 1) { // ← 1フレームだけでもOK
			phase_ = Phase::FadeIn;
			currentFrame_ = 0;
		}
		break;
	case Phase::FadeIn:
		currentFrame_++;
		alpha_ = 1.0f - static_cast<float>(currentFrame_) / fadeInFrames_;
		if (alpha_ < 0.0f)
			alpha_ = 0.0f;
		if (currentFrame_ >= fadeInFrames_) {
			phase_ = Phase::Finished;
			finished_ = true;
		}
		break;
	case Phase::Finished:
		finished_ = true;
		break;
	}

	sprite_->SetColor({0, 0, 0, alpha_});
}

void Fade::Draw() {
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
}

bool Fade::IsFinished() const { return finished_; }