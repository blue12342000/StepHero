#include "stdafx.h"
#include "TextRender.h"


TextRender::TextRender()
{
}

TextRender::TextRender(int rows, int cols) :rows(rows), cols(cols)
{
	system(string("MODE CON COLS=" + to_string(cols + 1) + " LINES=" + to_string(rows + 1)).c_str());
	frontBuffer = new string[rows];
	backBuffer = new string[rows];
	FrontBufferClear();
	BackBufferClear();

	textLayout = new TextLayout[TextLayout::LayoutKind::NONE];
	for (int i = 0; i < TextLayout::LayoutKind::NONE; ++i)
	{
		textLayout[i].Initialize((TextLayout::LayoutKind)i);
	}
}

TextRender::~TextRender()
{
}

void TextRender::FrontBufferClear()
{
	for (int i = 0; i < rows; ++i)
	{
		frontBuffer[i] = string(cols, ' ');
	}
}
void TextRender::FrontBufferClear(int startOffset, int lines)
{
	for (int i = startOffset; i < lines && i < rows; ++i)
	{
		frontBuffer[i] = string(cols, ' ');
	}
}
void TextRender::BackBufferClear()
{
	for (int i = 0; i < rows; ++i)
	{
		backBuffer[i] = string(cols, ' ');
	}
}
void TextRender::BackBufferClear(int startOffset, int lines)
{
	for (int i = startOffset; i < lines && i < rows; ++i)
	{
		backBuffer[i] = string(cols, ' ');
	}
}

string TextRender::MakeString(string str, int width, TextBuffer::TextAlign textAlign, int paddingLeft, int paddingRight)
{
	int textLength = str.length();
	int bufferSize = paddingLeft + width + paddingRight;
	string buffer(bufferSize, ' ');
	switch (textAlign)
	{
	case TextBuffer::LEFT:
		buffer.replace(paddingLeft, textLength, str);
		break;
	case TextBuffer::CENTER:
		buffer.replace(paddingLeft + (width - textLength) / 2, textLength, str);
		break;
	case TextBuffer::RIGHT:
		buffer.replace(paddingLeft + width - textLength, textLength, str);
		break;
	}

	return buffer;
}

void TextRender::AppendBuffer(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos, string str)
{
	textLayout[layoutKind].textBufferMap[layoutPos].PushBack(str);
}

void TextRender::Refresh(TextLayout::LayoutKind layoutKind)
{
	if (!isAnimationRun) textLayout[layoutKind].Refresh();
}

void TextRender::Render()
{
	for (int i = 0; i < rows; ++i)
	{
		cout << frontBuffer[i] << endl;
	}
}

void TextRender::Render(TextLayout::LayoutKind layoutKind)
{
	for (int i = 0; i < rows; ++i)
	{
		string* line = textLayout[layoutKind].GetLine(i);
		if (line != nullptr)
		{
			frontBuffer[i].replace(0, cols, *line);
			cout << frontBuffer[i] << endl;
		}
		else
		{
			break;
		}
	}
}

void TextRender::RenderToBackBuffer(TextLayout::LayoutKind layoutKind)
{
	for (int i = 0; i < rows; ++i)
	{
		string* line = textLayout[layoutKind].PeekLine(i);
		if (line != nullptr)
		{
			backBuffer[i].replace(0, cols, *line);
		}
		else
		{
			break;
		}
	}
}

TextRender::AnimationInfo TextRender::CreateAnimInfo(TextRender::TextChangeAnim animType, TextLayout::LayoutKind fromLayout, TextLayout::LayoutKind toLayout, int animSec)
{
	AnimationInfo animInfo = { animType, fromLayout, toLayout, animSec };

	int size = 0;;
	int* data = nullptr;
	switch (animInfo.animType)
	{
	case TextRender::FADE_OUT_IN:
		size = rows * cols;
		animInfo.dataSize = size;
		animInfo.currFrame = 0;
		animInfo.totalFrame = size;
		animInfo.unitFrame = ((double)size) / animSec;

		data = new int[size];
		for (int i = 0; i < rows * cols; ++i)
		{
			data[i] = i;
		}
		for (int i = 0; i < rows * cols; ++i)
		{
			swap(data[rand() % size], data[rand() % size]);
		}
		animInfo.animData = data;
		break;
	case TextRender::ZIGZAG_OUT_IN:
		size = cols * 1.5f;
		animInfo.currFrame = 0;
		animInfo.totalFrame = size;
		animInfo.unitFrame = ((double)size) / animSec;
		break;
	}

	RenderToBackBuffer(toLayout);
	return animInfo;
}
// 애니메이션순서 createAnim > BufferChangeAnim
bool TextRender::BufferChangeAnim()
{
	int* arrData = nullptr;

	switch (animInfo.animType)
	{
	case TextRender::FADE_OUT_IN:
		arrData = (int*)animInfo.animData;
		break;
	case TextRender::ZIGZAG_OUT_IN:
		break;
	}

	chrono::time_point<chrono::system_clock> endTime;
	std::chrono::duration<double> diff;
	if (animInfo.currFrame == 0)
	{
		animInfo.startTime = chrono::system_clock::now();
		++animInfo.currFrame;
		return true;
	}
	else
	{
		endTime = chrono::system_clock::now();
		diff = endTime - animInfo.startTime;
	}
	double deltaSec = diff.count() * 1000; // 애니메이션이 진행되고서 몇초(초 * 1000)가 지났는가??

	if (deltaSec >= animInfo.animSec)
	{
		// 에니메이션 종료!!!
		animInfo.Release();
		isAnimationRun = false;
		return false;
	}

	int offset = 0;
	switch (animInfo.animType)
	{
	case TextRender::FADE_OUT_IN:
		for (; animInfo.currFrame < animInfo.unitFrame * deltaSec && animInfo.currFrame < animInfo.totalFrame; ++animInfo.currFrame)
		{
			swap(frontBuffer[arrData[animInfo.currFrame] / cols][arrData[animInfo.currFrame] % cols], backBuffer[arrData[animInfo.currFrame] / cols][arrData[animInfo.currFrame] % cols]);
		}
		break;
	case TextRender::ZIGZAG_OUT_IN:
		animInfo.currFrame = animInfo.unitFrame * deltaSec;
		if (animInfo.currFrame < 1) animInfo.currFrame = 1;
		offset = ((animInfo.currFrame * 4 / 3) % cols);
		for (int i = 0; i < rows; ++i)
		{
			if (animInfo.currFrame / (animInfo.totalFrame / 2) == 0)
			{
				// 기존데이터 삭제
				if (i % 2 == 0)
				{
					// 왼쪽 방향으로 움직임
					frontBuffer[i].replace(0, cols - offset, frontBuffer[i], offset, cols - offset);
				}
				else
				{
					//오른쪽
					frontBuffer[i].replace(offset, cols - offset, frontBuffer[i], 0, cols - offset);
				}
			}
			else
			{
				// 새데이터 들어옴
				if (i % 2 == 0)
				{
					// 왼쪽
					frontBuffer[i].replace(cols - offset, offset, backBuffer[i], 0, offset);
				}
				else
				{
					//오른쪽
					frontBuffer[i].replace(0, offset, backBuffer[i], cols - offset, offset);
				}
			}
		}
		break;
	}

	return true;
}

void TextRender::ChangeLayout(TextRender::TextChangeAnim animType, TextLayout::LayoutKind fromLayout, TextLayout::LayoutKind toLayout, int animSec)
{
	animInfo = CreateAnimInfo(animType, fromLayout, toLayout, animSec);
	isAnimationRun = true;
}

void TextRender::Update()
{
	if (isAnimationRun) BufferChangeAnim();
}

void TextRender::Release()
{
	for (int i = 0; i < TextLayout::LayoutKind::NONE; ++i) textLayout[i].Release();
	delete[] textLayout;
	delete[] frontBuffer;
	delete[] backBuffer;
}
