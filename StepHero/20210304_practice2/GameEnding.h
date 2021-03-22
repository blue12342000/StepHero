#pragma once
#include "TextView.h"
class GameEnding : public TextView
{
private:
	GameEnding();

public:
	GameEnding(int width, int height);
	~GameEnding();

	void Init() override;
	void Update() override;
	void Render(vector<string>* targetBuffer = nullptr) override;
	void Release() override;
};