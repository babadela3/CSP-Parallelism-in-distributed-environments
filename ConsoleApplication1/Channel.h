#pragma once
#include <iostream>
#include <string>

using namespace std;

class Channel
{
public:
    Channel();
	void run();
	friend void operator<=(const Channel& c1, char* str);
	friend char* operator<=(char* str, const Channel& c1);
};