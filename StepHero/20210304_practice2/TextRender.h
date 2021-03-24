#pragma once
class TextRender
{
public:
	// ȭ���� �� ���� �� ������� ����� �����
	enum TextLayout
	{
		TL_TOP,
		TL_CONTENT,
		TL_BOTTOM
	};

	// ���� ��� : ������ ����, ������ �������� ��ü, �Ź� ��ü
	enum RenderType
	{
		RT_STATIC,
		RT_EXIST_DATA,
		RT_EVERYTIME
	};

private:
	struct Layout
	{
		// ���̾ƿ� ��������
		int offset = 0;
		// ���̾ƿ��� �ʺ�
		int width = 0;
		// ���̾ƿ��� ����
		int height = 0;
		// ���� ���
		TextRender::RenderType renderType = RenderType::RT_EVERYTIME;

		// ������ ��
		int front = 0;
		// ������ ��
		int rear = 0;
		// ���������� ���� ��ġ
		int lastReadLine = -1;
		// ����
		vector<string> buffer;

		Layout() {}
		Layout(TextRender::RenderType renderType, int offset, int width, int height) :renderType(renderType), offset(offset), width(width), height(height)
		{
			string line;
			line.reserve(width+1);
			switch (renderType)
			{
			case TextRender::RenderType::RT_STATIC:
				buffer.resize(height + 1, line);
				break;
			case TextRender::RenderType::RT_EXIST_DATA:
				buffer.resize(height * 2 + 1, line);
				break;
			case TextRender::RenderType::RT_EVERYTIME:
				buffer.resize(height * 10 + 1, line);
				break;
			}
		};

		bool IsEmpty() { return front == rear; }
		bool IsFull() { return front == (rear + 1) % buffer.size(); }
		bool IsExsitNewData() { return (lastReadLine + 1) % buffer.size() != rear; }

		void Claer() { for (auto& s : buffer) s.replace(0, width, width, ' '); }
		void Refresh()
		{
			switch (renderType)
			{
			case RenderType::RT_STATIC:
				// ���� �ʴ´�
				break;
			case RenderType::RT_EXIST_DATA:
				// �����Ͱ� �����ϴٸ� �Ѵ�
				if (IsExsitNewData())
				{
					front = (lastReadLine + 1) % buffer.size();
				}
				break;
			case RenderType::RT_EVERYTIME:
				// ������ �Ѵ�
				front = (lastReadLine + 1) % buffer.size();
				break;
			}
		}

		void PushBack(const string& str)
		{
			if (!IsFull())
			{
				//buffer[rear].replace(0, width, width, ' ');
				//buffer[rear++].replace(0, str.length(), str);
				buffer[rear++] = str;
				rear %= buffer.size();
			}
		}
		string* ReadBuffer(int line)
		{
			int layoutLine = line - offset;
			if (layoutLine < height)
			{
				int peek = (front + layoutLine) % buffer.size();
				if (peek != rear)
				{
					// �������� ���� �ƴ϶��
					lastReadLine = peek;
					return &buffer[peek];
				}
			}
			return nullptr;
		}
	};

private:
	int lastDataLine = 0;
	
protected:
	int width = 0;
	int height = 0;
	vector<string> buffer;

	map<TextLayout, Layout> layout;

public:
	TextRender();
	~TextRender();

	int GetWidth() { return width; }
	int GetHeight() { return height; }

	void Init(int width, int height);
	//void Update();
	void Render();
	void Release();

	void Refresh();

	void Write(TextLayout layoutKind, const string& str);
	void Write(int offsetX, int offsetY, const string& str);
	void CopyTo(vector<string>& targetBuffer);

	void AddLayout(TextLayout layoutKind, RenderType renderType, int offset, int height);
	void RemoveLayout(TextLayout layout);

	bool IsBufferEmpty(TextLayout layoutKind);
};

