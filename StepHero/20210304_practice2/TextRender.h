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

	bool IsRemainBuffer()
	{
		return dataCount > 0;
	}

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

	string* PeekLine(int readPeek, bool isDefaultReturn = true)
	{
		int localY = readPeek - offset;
		string* str = nullptr;
		if (localY < 0 || localY >= dataCount)
		{
			if (isDefaultReturn && localY >= 0 && localY < rows)
			{
				buffer[(currPeek + localY) % 100] = string(cols, ' ');
				str = &buffer[(currPeek + localY) % 100];
				return str;
			}
			return str;
		}
		str = &buffer[(currPeek + localY) % 100];
		return str;
	}

	void Clear()
	{
		if (isRefreshed)
		{
			currPeek = 0;
			dataCount = 0;
			lastRenderLine = 0;
		}
	}

	void Refresh()
	{
		if (isRefreshed)
		{
			currPeek = (currPeek + lastRenderLine) % 100;
			dataCount -= lastRenderLine;
			if (dataCount < 0) dataCount = 0;
			lastRenderLine = 0;
		}
	}

	void PushBack(string str)
	{
		// 초기화
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
			textBufferMap.insert(make_pair(LayoutPos::BOTTOM, TextBuffer(TextBuffer::TextAlign::LEFT, 25, 10, 100, true)));

			textBufferTree = new TextBuffer*[3]{ &textBufferMap[LayoutPos::TOP], &textBufferMap[LayoutPos::CONTENT], &textBufferMap[LayoutPos::BOTTOM] };
			size = 3;
			break;
		case LayoutKind::INGAME:
			textBufferMap.insert(make_pair(LayoutPos::TOP, TextBuffer(TextBuffer::TextAlign::CENTER, 0, 8, 100, false)));
			textBufferMap.insert(make_pair(LayoutPos::CONTENT, TextBuffer(TextBuffer::TextAlign::CENTER, 8, 22, 100, true)));
			textBufferMap.insert(make_pair(LayoutPos::BOTTOM, TextBuffer(TextBuffer::TextAlign::LEFT, 30, 21, 100, true)));

			textBufferTree = new TextBuffer*[3]{ &textBufferMap[LayoutPos::TOP], &textBufferMap[LayoutPos::CONTENT], &textBufferMap[LayoutPos::BOTTOM] };
			size = 3;
			break;
		case LayoutKind::BATTLE:
			textBufferMap.insert(make_pair(LayoutPos::TOP, TextBuffer(TextBuffer::TextAlign::CENTER, 0, 8, 100, false)));
			textBufferMap.insert(make_pair(LayoutPos::CONTENT, TextBuffer(TextBuffer::TextAlign::CENTER, 8, 12, 100, true)));
			textBufferMap.insert(make_pair(LayoutPos::BOTTOM, TextBuffer(TextBuffer::TextAlign::CENTER, 20, 31, 100, true)));

			textBufferTree = new TextBuffer*[3]{ &textBufferMap[LayoutPos::TOP], &textBufferMap[LayoutPos::CONTENT], &textBufferMap[LayoutPos::BOTTOM] };
			size = 3;
			break;
		case LayoutKind::SHOP:
			break;
		}
	}

	void Clear()
	{
		for (int i = 0; i < size; ++i)
		{
			textBufferTree[i]->Clear();
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
					if (!line) return nullptr;
				}
				return line;
			}
		}
		return nullptr;
	}

	string* PeekLine(int readPeek)
	{
		for (int i = 0; i < size; ++i)
		{
			if (textBufferTree[i]->offset <= readPeek && readPeek < textBufferTree[i]->offset + textBufferTree[i]->rows)
			{
				string* line = nullptr;
				if (i < size - 1)
				{
					line = textBufferTree[i]->PeekLine(readPeek);
				}
				else
				{
					line = textBufferTree[i]->PeekLine(readPeek, false);
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

	enum TextChangeAnim
	{
		FADE_OUT_IN, ZIGZAG_OUT_IN
	};

	struct AnimationInfo
	{
		TextChangeAnim animType;
		TextLayout::LayoutKind fromLayout;
		TextLayout::LayoutKind toLayout;

		// 애니메이션 되는 시간
		int animSec = 0;
		// 현재 프레임
		int currFrame = -1;
		// 전체 프레임수
		int totalFrame = 0;
		// 1 / 1000 초당 몇프레임씩 진행되어야 하는지
		double unitFrame = 0;

		// 애니메이션 종류에 따른 사용하게되는 추가 데이터
		int dataSize = 0;
		void* animData = nullptr;

		// 애니메이션 시작 시간
		chrono::time_point<std::chrono::system_clock> startTime;

		void Release()
		{
			animSec = 0;
			currFrame = -1;
			totalFrame = 0;
			unitFrame = 0;
			dataSize = 0;
			if (animData)
			{
				switch (animType)
				{
				case TextRender::FADE_OUT_IN:
					delete[] animData;
					break;
				case TextRender::ZIGZAG_OUT_IN:
					break;
				}
				animData = nullptr;
			}
		}
	};

	int rows;
	int cols;

	string* frontBuffer = nullptr;
	string* backBuffer = nullptr;

	TextLayout* textLayout = nullptr;

	bool isAnimationRun = false;
	AnimationInfo animInfo;

	TextRender();
	TextRender(int rows, int cols);
	~TextRender();

	void FrontBufferClear();
	void FrontBufferClear(int startOffset, int lines);

	void BackBufferClear();
	void BackBufferClear(int startOffset, int lines);

	string MakeString(string str, int width, TextBuffer::TextAlign textAlign, int paddingLeft = 0, int paddingRight = 0);
	bool IsRemainBufferStr(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos);

	void AppendBuffer(TextLayout::LayoutKind layoutKind, TextLayout::LayoutPos layoutPos, string str);
	void Clear(TextLayout::LayoutKind layoutKind);
	void Refresh(TextLayout::LayoutKind layoutKind);
	void Render();
	void Render(TextLayout::LayoutKind layoutKind);

	void RenderToBackBuffer(TextLayout::LayoutKind layoutKind);
private:
	// 애니메이션 처리를 위한 내부함수
	void BufferChangeShiftLeft(string& origin, const string& shiftStr, int shift, int fromOffstShiftStr);
	void BufferChangeShiftRight(string& origin, const string& shiftStr, int shift, int fromOffstShiftStr);
	TextRender::AnimationInfo CreateAnimInfo(TextRender::TextChangeAnim animType, TextLayout::LayoutKind fromLayout, TextLayout::LayoutKind toLayout, int fps);

public:
	bool BufferChangeAnim();

	void ChangeLayout(TextRender::TextChangeAnim animType, TextLayout::LayoutKind fromLayout, TextLayout::LayoutKind toLayout, int fps = 30);

	void Update();
	void Release();
};

