#pragma once
#include "KamataEngine.h"

class SceneBase {
public:
	SceneBase(KamataEngine::Input* input) : input_(input) {}
	virtual ~SceneBase() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//
	void SetActive(bool active) { isActive_ = active; }
	bool IsActive() const { return isActive_; }

protected:
	KamataEngine::Input* input_;
	bool isActive_ = true; // デフォルトは動く
};