#include "stdafx.h"
#include "TextViewManager.h"

void TextViewManager::BufferChangeShiftLeft(string & origin, const string & shiftStr, int shift, int fromOffstShiftStr)
{
	origin = origin.substr(shift) + shiftStr.substr(fromOffstShiftStr, shift);
}

void TextViewManager::BufferChangeShiftRight(string & origin, const string & shiftStr, int shift, int fromOffstShiftStr)
{
	origin = shiftStr.substr(fromOffstShiftStr, shift) + origin.substr(0, origin.length() - shift);
}

TextViewManager::TextViewManager()
{
}


TextViewManager::~TextViewManager()
{
}

void TextViewManager::Init(int width, int height)
{
	this->width = 100;
	this->height = 50;

	string str = "MODE CON COLS=" + to_string(width + 1) + " LINES=" + to_string(height + 1);
	system(str.c_str());
}

void TextViewManager::Update()
{
	if (animInfo.state == AnimationInfo::AnimationState::READY)
	{
		animInfo.state = AnimationInfo::AnimationState::PLAYING;
	}
	else if (animInfo.state == AnimationInfo::AnimationState::PLAYING)
	{
		chrono::time_point<chrono::system_clock> endTime;
		std::chrono::duration<double> diff;
		if (animInfo.currFrame == -1)
		{
			animInfo.startTime = chrono::system_clock::now();
			endTime = chrono::system_clock::now();
			diff = endTime - animInfo.startTime;
			animInfo.currFrame = 0;
			return;
		}
		else
		{
			endTime = chrono::system_clock::now();
			diff = endTime - animInfo.startTime;
		}
		double deltaSec = diff.count() * 1000; // �ִϸ��̼��� ����ǰ� ����(�� * 1000)�� �����°�??

		if (deltaSec >= animInfo.animSec)
		{
			// ���ϸ��̼� ����!!!
			animInfo.state = AnimationInfo::AnimationState::END;
			currentView = animInfo.targetView;
			return;
		}

		int deltaFrame = 0;
		switch (animInfo.type)
		{
		case AnimationType::AT_FADE_OUT_IN:
			for (; animInfo.currFrame < animInfo.unitFrame * deltaSec && animInfo.currFrame < animInfo.totalFrame; ++animInfo.currFrame)
			{
				animInfo.fromBuffer[animInfo.tempData[animInfo.currFrame] / width][animInfo.tempData[animInfo.currFrame] % width] = animInfo.toBuffer[animInfo.tempData[animInfo.currFrame] / width][animInfo.tempData[animInfo.currFrame] % width];
			}
			break;
		case AnimationType::AT_ZIGZAG_OUT_IN:
			deltaFrame = animInfo.unitFrame * deltaSec - animInfo.currFrame;
			animInfo.currFrame = animInfo.unitFrame * deltaSec;
			if (deltaFrame > 0)
			{
				for (int i = 0; i < height; ++i)
				{
					if (i % 2 == 0)
					{
						// ���� �������� ������
						BufferChangeShiftLeft(animInfo.fromBuffer[i], animInfo.toBuffer[i], deltaFrame, animInfo.currFrame);
					}
					else
					{
						//������
						BufferChangeShiftRight(animInfo.fromBuffer[i], animInfo.toBuffer[i], deltaFrame, animInfo.currFrame);
					}
				}
			}
			break;
		}
	}
	else if (animInfo.state == AnimationInfo::AnimationState::END)
	{
		if (currentView != TextViewManager::ViewType::VT_NONE) textViews[currentView]->Update();
	}
}

void TextViewManager::Render()
{
	if (animInfo.state == AnimationInfo::AnimationState::INIT)
	{
		if (currentView != TextViewManager::ViewType::VT_NONE) textViews[currentView]->Render(&animInfo.fromBuffer);
		textViews[animInfo.targetView]->Init();
		textViews[animInfo.targetView]->Render(&animInfo.toBuffer);
		animInfo.Render();
		animInfo.state = AnimationInfo::AnimationState::READY;
	}
	else if (animInfo.state == AnimationInfo::AnimationState::READY
			|| animInfo.state == AnimationInfo::AnimationState::PLAYING)
	{
		animInfo.Render();
	}
	else
	{
		if (currentView != TextViewManager::ViewType::VT_NONE) textViews[currentView]->Render();
	}
}

void TextViewManager::Release()
{
	for (auto view : textViews)
	{
		view.second->Release();
		delete view.second;
	}
}

void TextViewManager::AddTextView(ViewType viewType, TextView * textView)
{
	//�ʱ�ȭ
	textViews.insert(make_pair(viewType, textView));
}

void TextViewManager::ChangeView(ViewType targetView, AnimationType animationType, int animSec)
{
	animInfo.targetView = targetView;
	animInfo.type = animationType;
	animInfo.animSec = animSec;

	int size = 0;
	switch (animationType)
	{
	case AnimationType::AT_FADE_OUT_IN:
		animInfo.state = AnimationInfo::AnimationState::INIT;
		size = width * height;
		animInfo.currFrame = 0;
		animInfo.totalFrame = size;
		animInfo.unitFrame = ((double)size) / animSec;

		animInfo.tempData.resize(size);
		for (int i = 0; i < size; ++i)
		{
			animInfo.tempData[i] = i;
		}
		for (int i = 0; i < size; ++i)
		{
			swap(animInfo.tempData[rand() % size], animInfo.tempData[rand() % size]);
		}
		animInfo.currFrame = -1;

		// ���� �ʱ�ȭ
		animInfo.fromBuffer = vector<string>(height, string(width, ' '));
		animInfo.toBuffer = vector<string>(height, string(width, ' '));
		break;
	case AnimationType::AT_ZIGZAG_OUT_IN:
		animInfo.state = AnimationInfo::AnimationState::INIT;
		size = width;
		animInfo.currFrame = -1;
		animInfo.totalFrame = size;
		animInfo.unitFrame = ((double)size) / animSec;

		// ���� �ʱ�ȭ
		animInfo.fromBuffer = vector<string>(height, string(width, ' '));
		animInfo.toBuffer = vector<string>(height, string(width, ' '));
		break;
	case AnimationType::AT_NOT_ANIMATION:
		animInfo.state = AnimationInfo::AnimationState::END;
		currentView = targetView;
		break;
	}
}
