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
		
		// 애니메이션의 총 시간
		int animSec = 0;
		// 현재 프레임
		int currFrame = -1;
		// 전체 프레임수
		int totalFrame = 0;
		// 1 / 1000 초당 몇프레임씩 진행되어야 하는지
		double unitFrame = 0;

		// 애니메이션 시작 시간
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

