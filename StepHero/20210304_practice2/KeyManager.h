#pragma once

class InputKeyRequest
{
public:
	int size = 0;
	char* keyGroup = nullptr;

	InputKeyRequest() {}
	InputKeyRequest(int size, char* keyGroup) :size(size), keyGroup(keyGroup) {}

	void Release()
	{
		if (keyGroup) delete[] keyGroup;
	}
};

class KeyManager
{
public:
	enum InputType
	{
		ANYKEYS = 0, SELECT, INGAME, TEXT, NONE
	};

	enum InputResult
	{
		SUCCESS = 0, FAIL, EMPTY
	};

	bool keys[256] = { false, };
	InputResult inputResult = InputResult::EMPTY;
	InputKeyRequest inputKeyRequest[InputType::NONE];

	string inputText;

	int currRequest = 0;
	int requestCount = 0;
	InputType requestBuffer[5];

	KeyManager();
	~KeyManager();

	void Request(InputType inputType);
	void Clear();
	void Update();
	bool IsPressKey(char key);
};

