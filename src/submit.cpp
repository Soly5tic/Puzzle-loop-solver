#include "submit.h"

#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>
using namespace std;

namespace submitGlobalVariables {
	const int MXN = 105;
	int dgr[MXN][MXN], r[MXN][MXN], d[MXN][MXN], cx = 0, cy = 0;
	vector <pair <int, int> > g[MXN][MXN];
	bool vis[MXN][MXN];
	const int Next[4][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
	map <pair <int, int>, int> mpd;
}

void submitInit() {
	using namespace submitGlobalVariables;
	memset(dgr, 0, sizeof(dgr));
	memset(r, 0, sizeof(r));
	memset(d, 0, sizeof(d));
	cx = 0; cy = 0;
	for (int i = 0;i < MXN;i++) {
		for (int j = 0;j < MXN;j++) g[i][j].clear();
	}
	memset(vis, 0, sizeof(vis));
	mpd.clear();
}

// dir: L = 0, U = 1, R = 2, D = 3
inline void Move(bool flg, int dir) {
	if (flg) keybd_event(VK_CONTROL, 0, 0, 0);
	Sleep(2);
	keybd_event(0x25 + dir, 0, 0, 0);
	Sleep(2);
	keybd_event(0x25 + dir, 0, KEYEVENTF_KEYUP, 0);
	Sleep(2);
	if (flg) keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(2);
}

inline void Dfs(int x, int y, pair <int, int> lst) {
	using namespace submitGlobalVariables;
	if (vis[x][y]) return;
	vis[x][y] = 1;
	pair <int, int> nxt;
	for (auto p : g[x][y]) {
		if (p != lst) {
			nxt = p;
			break;
		}
	}
	int nxtx = nxt.first - x, nxty = nxt.second - y;
	Move(1, mpd[make_pair(nxtx, nxty)]);
	Dfs(nxt.first, nxt.second, make_pair(x, y));
}

void Submit(int n, int m, int **r, int **d, int sleepTime) {
	using namespace submitGlobalVariables;
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j < m;j++) {
			if (r[i][j]) {
				g[i][j].push_back(make_pair(i, j + 1));
				g[i][j + 1].push_back(make_pair(i, j));
			}
		}
	}
	for (int i = 0;i < n;i++) {
		for (int j = 0;j <= m;j++) {
			if (d[i][j]) {
				g[i][j].push_back(make_pair(i + 1, j));
				g[i + 1][j].push_back(make_pair(i, j));
			}
		}
	}
	for (int k = 0;k < 4;k++) mpd[make_pair(Next[k][0], Next[k][1])] = k;
	cout << "Start submitting process in 15 seconds" << endl;
	cout << "Keep focus on game section and put the blue dot on the upleftmost corner" << endl;
	Sleep(sleepTime);
	cerr << "DBG Start submitting process" << endl;
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j <= m;j++) {
			if (g[i][j].size() == 2) {
				cx = i;
				cy = j;
				for (int k = 0;k < i;k++) Move(0, 3);
				for (int k = 0;k < j;k++) Move(0, 2);
				goto done;
			}
		}
	}
	cout << "ERR FAILED TO FIND STARTING POINT" << endl;
	cerr << "ERR FAILED TO FIND STARTING POINT" << endl;
	assert(0);
	done:;
	cerr << "DBG Start at (" << cx << "," << cy << ")" << endl;
	Dfs(cx, cy, make_pair(-1, -1));
	keybd_event(VK_RETURN, 0, 0, 0);
	keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
	cerr << "DBG Complete" << endl;
}