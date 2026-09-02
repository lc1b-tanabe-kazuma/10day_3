#include "DrawNumber.h"
using namespace KamataEngine;

DrawNumber::~DrawNumber() {
	for (int i = 0; i < 5; i++) {
		delete spriteNumbers_[i];
	}
}

// 初期化
void DrawNumber::Initialize(uint32_t textureHandle, Vector2 pos) {
	textureHandle_ = textureHandle;
	// スプライト
	for (int i = 0; i < 5; i++) {
		spriteNumbers_[i] = Sprite::Create(textureHandle_, {pos.x + size_.x * i, pos.y});
	}
}

// 更新
void DrawNumber::Update(uint32_t number) {

	for (int i = 0; i < 5; i++) {
		spriteNumbers_[i]->SetSize(size_);
		spriteNumbers_[i]->SetTextureRect({0, 0}, size_);
	}

	(void)number;

	// 最初に割る数値
	int32_t digit = 10000;

	// 先頭の0を非表示にするためのフラグ
	bool leadingZero = true;

	// 5桁分ループ
	for (int i = 0; i < 5; i++) {

		// 現在の桁の数値を取得
		int nowNumber = number / digit;

		// 先頭の0を非表示にする処理
		if (leadingZero && nowNumber == 0 && digit > 1) {
			spriteNumbers_[i]->SetTextureRect({0, 0}, {0, 0});
		} else {
			leadingZero = false;
			spriteNumbers_[i]->SetTextureRect({size_.x * nowNumber, 0}, {size_.x, size_.y});
		}

		// 残りの桁を数値に更新
		number %= digit;

		// 次の桁に移動するために10で割る
		digit /= 10;
	}
}

// 描画
void DrawNumber::Draw() {
	for (int i = 0; i < 5; i++) {
		spriteNumbers_[i]->Draw();
	}
}