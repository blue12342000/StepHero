#pragma once
#include "TextView.h"

class TextViewManager
{
public:
	enum ViewType
	{
		VT_TITLE,
		VT_INGAME,
		VT_BATTLE,
		VT_SHOP,
		VT_ENDING,
		VT_ESCAPE,
		VT_GAMEOVER,
		VT_NONE
	};

	enum AnimationType
	{
		AT_FADE_OUT_IN, AT_ZIGZAG_OUT_IN, AT_NOT_ANIMATION
	};

	enum ViewType currentView = ViewType::VT_NONE;
private:
	struct AnimationInfo
	{
		enum AnimationState
		{
			INIT,
			READY,
			PLAYING,
			END
		};

		AnimationState state = AnimationState::END;
		AnimationType type = AnimationType::AT_NOT_ANIMATION;
		ViewType targetView;
		
		// �ִϸ��̼��� �� �ð�
		int animSec = 0;
		// ���� ������
		int currFrame = -1;
		// ��ü �����Ӽ�
		int totalFrame = 0;
		// 1 / 1000 �ʴ� �������Ӿ� ����Ǿ�� �ϴ���
		double unitFrame = 0;

		// �ִϸ��̼� ���� �ð�
		chrono::time_point<std::chrono::system_clock> startTime;
		
		vector<string> fromBuffer;
		vector<string> toBuffer;
		vector<int> tempData;

		void Render()
		{
			for (auto s : fromBuffer)
			{
				cout << s << endl;
			}
		}
	};

	int width;
	int height;
	map<ViewType, TextView*> textViews;

	AnimationInfo animInfo;

private:
	void BufferChangeShiftLeft(string& origin, const string& shiftStr, int shift, int fromOffstShiftStr);
	void BufferChangeShiftRight(string& origin, const string& shiftStr, int shift, int fromOffstShiftStr);

public:
	TextViewManager();
	~TextViewManager();

	void Init(int width, int height);
	void Update();
	void Render();
	void Release();

	void AddTextView(ViewType viewType, TextView* textView);
	void ChangeView(ViewType targetView, AnimationType animationType, int animSec = 0);
};

