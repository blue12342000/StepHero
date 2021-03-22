#pragma once
#include "TextView.h"
class GameEscape : public TextView
{
private:
	GameEscape();

public:
	GameEscape(int width, int height);
	~GameEscape();


	void Init() override;
	void Update() override;
	void Render(vector<string>* targetBuffer = nullptr) override;
	void Release() override;
};

