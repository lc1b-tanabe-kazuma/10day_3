#pragma once
#include "KamataEngine.h"
#include "TransitionBase.h"
#include <functional>

class Fade : public TransitionBase {
public:
	Fade(std::function<void()> onFadeOut = nullptr, int fadeOutFrames = 60, int fadeInFrames = 30);
	~Fade() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool IsFinished() const override;

private:
	enum class Phase { FadeOut, Wait, FadeIn, Finished };

	int fadeOutFrames_;
	int fadeInFrames_;
	int currentFrame_;
	float alpha_;
	bool finished_;

	std::function<void()> onFadeOut_;
	bool fadeOutCalled_ = false;

	uint32_t textureHandle_ = 0;
	KamataEngine::Sprite* sprite_ = nullptr;

	Phase phase_ = Phase::FadeOut;

	int waitFrames_ = 5; // フェードアウトとフェードインの間の待機フレーム数
};