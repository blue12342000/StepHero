#pragma once
#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <conio.h>
#include <stdio.h>
#include <map>
#include <functional>
#include <fstream>
#include <chrono>
#include <vector>

#define TA_LEFT 0x01
#define TA_CENTER 0x02
#define TA_RIGHT 0x03

using namespace std;
using FunctionPtr = function<void(void*)>;

struct Difficulty
{
	int dungeonSize[3] = { 10, 15, 20 };
	int heroHP[3] = { 100, 90, 80 };
	float monsterStrength[3] = { 1, 1.5f, 2 };
	int monsterEncounter[3] = { 20, 10, 10 };
	string name[3] = { "EASY", "NORMAL", "HARD" };
};

typedef int TextAlign;
inline string MakeString(TextAlign align, int width, string str);

inline string MakeString(TextAlign align, int width, string str)
{
	if (str.length() > width)
	{
		str = str.substr(0, width);
	}
	else if (str.length() < width)
	{
		if (align == TA_LEFT)
		{
			str.append(string(width - str.length(), ' '));
		}
		else if (align == TA_CENTER)
		{
			int diff = width - str.length();
			int left = diff / 2;
			str.insert(0, string(left, ' '));
			str.append(string(diff - left, ' '));
		}
		else if (align == TA_RIGHT)
		{
			str.insert(0, string(width - str.length(), ' '));
		}
	}
	return str;

}