#include "stdafx.h"
#include "TextRender.h"

TextRender::TextRender() {}
TextRender::~TextRender() {}

void TextRender::Init(int width, int height)
{
	this->width = width;
	this->height = height;
	string line;
	line.reserve(width + 1);
	buffer.resize(height, line);
}

//void TextRender2::Update()
//{
//}

void TextRender::Render(bool isRefresh)
{
	if (isRefresh) Refresh();
	for (int i = 0; i <= lastDataLine && i < height; ++i)
	{
		cout << buffer[i] << endl;
	}
}

void TextRender::Release()
{
}

void TextRender::Refresh()
{
	int readLine = 0, count = 1;
	for (auto it = layout.begin(); it != layout.end(); ++it, ++count)
	{
		it->second.Refresh();
		bool isLastLayout = count == layout.size();
		bool isDataNull = false;
		for (; readLine < height && readLine < (it->second.offset + it->second.height); ++readLine)
		{
			if (isDataNull)
			{
				buffer[readLine] = "";
				lastDataLine = readLine;
				continue;
			}

			string* line = it->second.ReadBuffer(readLine);
			if (isLastLayout && !line) break;
			else if (line)
			{
				buffer[readLine] = *line;
			}
			else
			{
				isDataNull = true;
				buffer[readLine] = "";
			}
			lastDataLine = readLine;
		}
	}
}

void TextRender::Write(TextLayout layoutKind, const string& str)
{
	if (str.length() != 0 && str.length() != width) layout[layoutKind].PushBack(MakeString(TA_LEFT, width, str));
	else layout[layoutKind].PushBack(str);
}

void TextRender::CopyTo(vector<string>& targetBuffer)
{
	Refresh();
	for (int i = 0; i <= lastDataLine; ++i)
	{
		targetBuffer[i] = MakeString(TA_LEFT, width, buffer[i]);
	}
}

void TextRender::AddLayout(TextLayout layoutKind, RenderType renderType, int offset, int height)
{
	layout.insert(make_pair(layoutKind, Layout(renderType, offset, width, height)));
}

void TextRender::RemoveLayout(TextLayout layoutKind)
{
	layout.erase(layoutKind);
}

bool TextRender::IsBufferEmpty(TextLayout layoutKind)
{
	return layout[layoutKind].IsEmpty();
}
