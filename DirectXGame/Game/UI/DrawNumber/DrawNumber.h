#pragma once
#include "KamataEngine.h"

class DrawNumber {
public:
	~DrawNumber();

	void Initialize(uint32_t textureHandle, KamataEngine::Vector2 pos);
	void Update(uint32_t number);
	void Draw();

private:
	// 5桁の数字を表示するためのスプライト配列
	KamataEngine::Sprite* spriteNumbers_[5] = {};

	// 数字の表示サイズ
	KamataEngine::Vector2 size_ = {32.0f, 64.0f};

	// 数字のテクスチャハンドル
	uint32_t textureHandle_ = 0;
};