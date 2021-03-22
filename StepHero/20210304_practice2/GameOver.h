#pragma once
#include "TextView.h"
class GameOver : public TextView
{
private:
	GameOver();

public:
	GameOver(int width, int height);
	~GameOver();

	void Init() override;
	void Update() override;
	void Render(vector<string>* targetBuffer = nullptr) override;
	void Release() override;
};