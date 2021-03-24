#pragma once
#include "TextRender.h"

class TextPopup
{
private:
	int front = 0;
	int rear = 0;
	vector<string> buffer;

public:
	TextPopup() { buffer.resize(100); }
	~TextPopup() {}

	void Clear() { front = rear; }
	void Write(string str)
	{
		buffer[rear++] = str;
		rear %= buffer.size();
	}

	void Show(TextRender& view, int offsetX, int offsetY, int width, int height)
	{
		if (front == rear) return;

		int curr = front;
		bool isOverFlow = false;
		view.Write(offsetX, offsetY++, string(width, '*'));
		while (--height > 1)
		{
			if (curr == rear) isOverFlow = true;
			if (isOverFlow) view.Write(offsetX, offsetY++, "|" + string(width-2, ' ') + "|");
			else view.Write(offsetX, offsetY++, "|" + MakeString(TA_CENTER, width - 2, buffer[curr]) + "|");
			curr = (curr + 1) % buffer.size();
		}
		view.Write(offsetX, offsetY, string(width, '*'));
		Clear();
	}
};

class TextView
{
protected:
	int width;
	int height;
	TextRender view;
	TextPopup popup;

protected:
	TextView() {};

public:
	virtual ~TextView() {};

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render(vector<string>* targetBuffer = nullptr) = 0;
	virtual void Release() = 0;
};