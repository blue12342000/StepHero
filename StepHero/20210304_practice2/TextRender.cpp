#include "stdafx.h"
#include "TextRender.h"


TextRender::TextRender()
{
}

TextRender::TextRender(int rows, int cols) :rows(rows), cols(cols)
{
	system(string("MODE CON COLS=" + to_string(cols + 1) + " LINES=" + to_string(rows + 1)).c_str());
	buffer = new string[rows];
	BufferClear();

	textLayout = new TextLayout[TextLayout::LayoutKind::NONE];
	for (int i = 0; i < TextLayout::LayoutKind::NONE; ++i)
	{
		textLayout[i].Initialize((TextLayout::LayoutKind)i);
	}
}

TextRender::~TextRender()
{
}

void TextRender::BufferClear()
{
	for (int i = 0; i < rows; ++i)
	{
		buffer[i] = string(cols, '-');
	}
}
void TextRender::BufferClear(int startOffset, int lines)
{
	for (int i = startOffset; i < lines && i < rows; ++i)
	{
		buffer[i] = string(cols, ' ');
	}
}

void TextRender::AppendBuffer(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos, string str)
{
	textLayout[layoutKind].textBufferMap[layoutPos].PushBack(str);
}

void TextRender::Refresh(TextLayout::LayoutKind layoutKind)
{
	textLayout[layoutKind].Refresh();
}

void TextRender::Render(TextLayout::LayoutKind layoutKind)
{
	for (int i = 0; i < rows; ++i)
	{
		string* line = textLayout[layoutKind].GetLine(i);
		if (line != nullptr)
		{
			buffer[i].replace(0, cols, *line);
			cout << buffer[i] << endl;
		}
		else
		{
			break;
		}
	}
}

void TextRender::Release()
{
	for (int i = 0; i < TextLayout::LayoutKind::NONE; ++i) textLayout[i].Release();
	delete[] textLayout;
	delete[] buffer;
}
