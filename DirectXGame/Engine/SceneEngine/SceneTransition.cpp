#include "SceneTransition.h"
#include "Fade.h"

using namespace std;

SceneTransition* SceneTransition::instance = nullptr;

SceneTransition* SceneTransition::GetInstance() {
	if (!instance) {
		instance = new SceneTransition;
	}
	return instance;
}

void SceneTransition::Initialize() { RegisterTransition("Fade", make_unique<Fade>()); }

void SceneTransition::RegisterTransition(const string& name, unique_ptr<TransitionBase> transition) { transitions_[name] = move(transition); }

void SceneTransition::StartTransition(const string& name, function<void()> onTransition) {
	auto it = transitions_.find(name);
	if (it != transitions_.end()) {
		if (name == "Fade") {
			currentTransition_ = make_unique<Fade>(onTransition, 60, 15);
		}
		currentTransition_->Initialize();
	}
}

void SceneTransition::Update() {
	if (currentTransition_) {
		currentTransition_->Update();
		if (currentTransition_->IsFinished()) {
			currentTransition_.reset();
		}
	}
}

void SceneTransition::Draw() {
	if (currentTransition_) {
		currentTransition_->Draw();
	}
}

bool SceneTransition::IsTransitioning() const { return currentTransition_ != nullptr; }