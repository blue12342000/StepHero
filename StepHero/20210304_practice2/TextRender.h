#pragma once

class TextBuffer
{
public:

	enum TextAlign
	{
		LEFT, CENTER, RIGHT
	};

	bool isRefreshed = false;
	TextAlign textAlign = TextAlign::LEFT;

	int offset = 0;
	int rows = 0;
	int cols = 0;

	int currPeek = 0;
	int dataCount = 0;
	int lastRenderLine = 0;
	string buffer[100];

	TextBuffer() {}
	TextBuffer(TextAlign align, int offset, int rows, int cols, bool isRefreshed) :textAlign(align), offset(offset), rows(rows), cols(cols), isRefreshed(isRefreshed) {}

	string* GetLine(int readPeek, bool isDefaultReturn = true)
	{
		int localY = readPeek - offset;
		string* str = nullptr;
		if (localY < 0 || localY >= dataCount)
		{
			if (isDefaultReturn && localY >= 0 &&  localY < rows)
			{
				buffer[(currPeek + localY) % 100] = string(cols, ' ');
				str = &buffer[(currPeek + localY) % 100];
				return str;
			}
			return str;
		}
		str = &buffer[(currPeek + localY) % 100];
		++lastRenderLine;
		return str;
	}

	void Refresh()
	{
		if (isRefreshed)
		{
			currPeek = (currPeek + lastRenderLine) % 100;
			dataCount -= lastRenderLine;
			lastRenderLine = 0;
		}
	}

	void PushBack(string str)
	{
		// ÃÊ±âÈ­
		buffer[(currPeek + dataCount) % 100] = string(cols, ' ');

		int size = (str.length() < cols) ? str.length() : cols;
		int begin = 0;
		switch (textAlign)
		{
		case TextAlign::LEFT:
			break;
		case TextAlign::CENTER:
			begin = (cols - size) / 2;
			break;
		case TextAlign::RIGHT:
			begin = cols - size;
			break;
		}
		buffer[(currPeek + dataCount) % 100].replace(begin, size, str);
		++dataCount;
	}

	void Release()
	{

	}
};

class TextLayout
{
public:

	enum LayoutKind
	{
		TITLE = 0, INGAME, BATTLE, SHOP, NONE
	};

	enum LayoutPos
	{
		TOP = 0, CONTENT, BOTTOM
	};

	int size = 0;

	TextBuffer** textBufferTree = nullptr;
	map<LayoutPos, TextBuffer> textBufferMap;

	TextLayout() {}

	void Initialize(LayoutKind layoutKind)
	{
		switch (layoutKind)
		{
		case LayoutKind::TITLE:
			textBufferMap.insert(make_pair(LayoutPos::TOP, TextBuffer(TextBuffer::TextAlign::CENTER, 0, 16, 100, false)));
			textBufferMap.insert(make_pair(LayoutPos::CONTENT, TextBuffer(TextBuffer::TextAlign::CENTER, 16, 9, 100, true)));

			textBufferTree = new TextBuffer*[2]{ &textBufferMap[LayoutPos::TOP], &textBufferMap[LayoutPos::CONTENT] };
			size = 2;
			break;
		case LayoutKind::INGAME:
			textBufferMap.insert(make_pair(LayoutPos::TOP, TextBuffer(TextBuffer::TextAlign::CENTER, 0, 8, 100, false)));
			textBufferMap.insert(make_pair(LayoutPos::CONTENT, TextBuffer(TextBuffer::TextAlign::CENTER, 8, 22, 100, true)));
			textBufferMap.insert(make_pair(LayoutPos::BOTTOM, TextBuffer(TextBuffer::TextAlign::LEFT, 30, 21, 100, true)));

			textBufferTree = new TextBuffer*[3]{ &textBufferMap[LayoutPos::TOP], &textBufferMap[LayoutPos::CONTENT], &textBufferMap[LayoutPos::BOTTOM] };
			size = 3;
			break;
		case LayoutKind::BATTLE:
			break;
		case LayoutKind::SHOP:
			break;
		}
	}

	void Refresh()
	{
		for (int i = 0; i < size; ++i)
		{
			textBufferTree[i]->Refresh();
		}
	}

	string* GetLine(int readPeek)
	{
		for (int i = 0; i < size; ++i)
		{
			if (textBufferTree[i]->offset <= readPeek && readPeek < textBufferTree[i]->offset + textBufferTree[i]->rows)
			{
				string* line = nullptr;
				if (i < size - 1)
				{
					line = textBufferTree[i]->GetLine(readPeek);
				}
				else
				{
					line = textBufferTree[i]->GetLine(readPeek, false);
				}
				return line;
			}
		}
		return nullptr;
	}

	void Release()
	{
		delete[] textBufferTree;
		for (auto p : textBufferMap)
		{
			p.second.Release();
		}
	}
};

class TextRender
{
public:

	int rows;
	int cols;

	string* buffer = nullptr;

	TextLayout* textLayout = nullptr;


	TextRender();
	TextRender(int rows, int cols);
	~TextRender();

	void BufferClear();
	void BufferClear(int startOffset, int lines);

	void AppendBuffer(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos, string str);

	void Refresh(TextLayout::LayoutKind layoutKind);

	void Render(TextLayout::LayoutKind layoutKind);

	void Release();
};

