#pragma once
#include "TextRender.h"

class TextView
{
protected:
	int width;
	int height;
	TextRender view;

protected:
	TextView() {};

public:
	virtual ~TextView() {};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(vector<string>* targetBuffer = nullptr) = 0;
	virtual void Release() = 0;
};

