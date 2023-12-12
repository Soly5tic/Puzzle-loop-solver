#include "autoRun.h"

#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

namespace keyOperation {

	inline void pushColon() {
		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(VK_OEM_1, 0, 0, 0); keybd_event(VK_OEM_1, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
		keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void pushPeriod() {
		keybd_event(VK_OEM_PERIOD, 0, 0, 0); keybd_event(VK_OEM_PERIOD, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void pushBackslash() {
		keybd_event(VK_OEM_5, 0, 0, 0); keybd_event(VK_OEM_5, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void pushF12() {
		keybd_event(VK_F12, 0, 0, 0); keybd_event(VK_F12, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void pushCtrlS() {
		keybd_event(VK_CONTROL, 0, 0, 0); keybd_event('S', 0, 0, 0); Sleep(50);
		keybd_event('S', 0, KEYEVENTF_KEYUP, 0); keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void pushLetter(char c) {
		if (c >= 'A' && c <= 'Z') {
			keybd_event(VK_SHIFT, 0, 0, 0); keybd_event(c, 0, 0, 0); Sleep(50);
			keybd_event(c, 0, KEYEVENTF_KEYUP, 0); keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
		} else {
			keybd_event(c - 32, 0, 0, 0); keybd_event(c - 32, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
		}
	}

	inline void pushPath(string s) {
		for (char c : s) {
			if (c == '\\') pushBackslash();
			else if (c == ':') pushColon();
			else if (c == '.') pushPeriod();
			else pushLetter(c);
		}
	}

	inline void pushEnter() {
		keybd_event(VK_RETURN, 0, 0, 0); keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void Up() {
		keybd_event(VK_UP, 0, 0, 0); keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void Right() {
		keybd_event(VK_RIGHT, 0, 0, 0); keybd_event(VK_RIGHT, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void Down() {
		keybd_event(VK_DOWN, 0, 0, 0); keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}

	inline void Left() {
		keybd_event(VK_LEFT, 0, 0, 0); keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	}


}

void downloadProblem() {
	using namespace keyOperation;
	Sleep(10000);
	system(("del \"" + PTH + "\"").c_str());
	system(("rd /s/q \"" + PTF + "\"").c_str());

	pushF12(); Sleep(1000);
	pushCtrlS(); Sleep(1000);
    
    pushPath(PTH);

	pushEnter(); Sleep(500);
	//keybd_event(VK_LEFT, 0, 0, 0); keybd_event(VK_LEFT, 0, KEYEVENTF_KEYUP, 0); Sleep(500);
	//keybd_event(VK_RETURN, 0, 0, 0); keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0); Sleep(10000);
	ifstream fin;
	fin.open(PTH.c_str());
	while (!fin.is_open()) {
		Sleep(100);
		cout << "downloading" << endl;
		fin.open(PTH.c_str());
	}
	cout << "download complete" << endl;
	keybd_event(VK_ESCAPE, 0, 0, 0); keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0); Sleep(50);
	pushF12(); Sleep(500);
	pushEnter(); Sleep(5000);
	Right(); Left();
}

void findNext(int n, int m) {
	using namespace keyOperation;
	Sleep(10000);
	Right(); Left();
	for (int i = 1;i <= n;i++) Down();
	for (int i = 1;i <= m;i++) Right();
	for (int i = 1;i <= 74;i++) {
		keybd_event(VK_TAB, 0, 0, 0); keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0); Sleep(20);
	}
	keybd_event(VK_SPACE, 0, 0, 0); keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
}