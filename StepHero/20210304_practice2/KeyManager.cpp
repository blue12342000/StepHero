#include "stdafx.h"
#include "KeyManager.h"


KeyManager::KeyManager()
{
	inputKeyRequest[InputType::SELECT] = InputKeyRequest(3, new char[3]{ 'w', 's', 13 });
	inputKeyRequest[InputType::INGAME] = InputKeyRequest(5, new char[5]{ 'w', 'a', 's', 'd', 13 });
}

KeyManager::~KeyManager()
{
}

void KeyManager::Request(InputType inputType)
{
	requestBuffer[(currRequest + requestCount++) % 5] = inputType;
}

void KeyManager::Clear()
{
	currRequest = 0;
	requestCount = 0;
}

void KeyManager::Update()
{
	inputResult = InputResult::EMPTY;
	memset(keys, false, sizeof(bool) * 256);
	if (requestCount > 0)
	{
		bool isOK = false;
		char inputChar;
		switch (requestBuffer[currRequest])
		{
		case InputType::ANYKEYS:
			inputChar = tolower(_getch());
			fflush(stdin);
			//while (_kbhit() && (getchar()) != '\n');
			isOK = true;
			break;
		case InputType::SELECT:
		case InputType::INGAME:
			inputChar = tolower(_getch());
			fflush(stdin);

			//while (_kbhit() && (getchar()) != '\n');
			for (int i = 0; i < inputKeyRequest[requestBuffer[currRequest]].size; ++i)
			{
				if (inputChar == inputKeyRequest[requestBuffer[currRequest]].keyGroup[i])
				{
					isOK = true;
				}
			}
			break;
		case InputType::TEXT:

			if (_kbhit())
			{
				int a = 0;
			}

			cout <<  setw(45) << "";
			getline(std::cin, inputText);
			isOK = true;
			break;
		}

		inputResult = InputResult::FAIL;
		if (isOK)
		{
			if (requestBuffer[currRequest] != InputType::TEXT) keys[inputChar] = true;
			inputResult = InputResult::SUCCESS;
			currRequest = (currRequest + 1) % 5;
			--requestCount;
		}
	}
	else
	{
		//while (_kbhit() && (getchar()) != '\n');
		fflush(stdin);
	}
}

bool KeyManager::IsPressKey(char key)
{
	return keys[key];
}
