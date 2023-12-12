/* Print the information related to the puzzle-solving process */

#include "interface.h"

#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

namespace interfaceGlobalVariables {
    char cur[255][255];
    char pCache[255][255];
    int R = 0, C = 0;
}

void interfaceInit() {
	using namespace interfaceGlobalVariables;
	memset(cur, 0, sizeof(cur));
	memset(pCache, 0, sizeof(pCache));
	R = C = 0;
}

inline void Movepos(int x, int y) {
	COORD position;
	position.X = x; position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

// write the current puzzle status into a cache array
inline void writeCache(int n, int m, int *a[], int r[55][55], int d[55][55]) {
    using namespace interfaceGlobalVariables;
	int rid = 0, cid = 0;
	C = 0;
	for (int i = 0;i <= n;i++) {
		pCache[rid][cid++] = '.';
		for (int j = 0;j < m;j++) {
			if (r[i][j] == -1) pCache[rid][cid++] = '?';
			else if (r[i][j] == 1) pCache[rid][cid++] = '-';
			else pCache[rid][cid++] = ' ';
			pCache[rid][cid++] = '.';
			C = max(C, cid);
		}
		rid++; cid = 0;
		if (i < n) {
			if (d[i][0] == -1) pCache[rid][cid++] = '?';
			else if (d[i][0] == 1) pCache[rid][cid++] = '|';
			else pCache[rid][cid++] = ' ';
			for (int j = 1;j <= m;j++) {
				if (a[i][j - 1] >= 0) pCache[rid][cid++] = a[i][j - 1] + '0';
				else pCache[rid][cid++] = ' ';
				if (d[i][j] == -1) pCache[rid][cid++] = '?';
				else if (d[i][j] == 1) pCache[rid][cid++] = '|';
				else pCache[rid][cid++] = ' ';
				C = max(C, cid);
			}
			rid++; cid = 0;
		}
	}
	R = rid;
}

// save answer into a file
inline void writeAns(int n, int m, int *a[], int *r[], int *d[], int _TIME_) {
    using namespace interfaceGlobalVariables;
	ofstream fout("answer.out");
	for (int i = 0;i <= n;i++) {
		fout << ".";
		for (int j = 0;j < m;j++) {
			if (r[i][j] == -1) fout << "?";
			else if (r[i][j] == 1) fout << "-";
			else fout << " ";
			fout << ".";
		}
		fout << endl;
		if (i < n) {
			if (d[i][0] == -1) fout << "?";
			else if (d[i][0] == 1) fout << "|";
			else fout << " ";
			for (int j = 1;j <= m;j++) {
				if (a[i][j - 1] >= 0) fout << a[i][j - 1];
				else fout << " ";
				if (d[i][j] == -1) fout << "?";
				else if (d[i][j] == 1) fout << "|";
				else fout << " ";
			}
			fout << endl;
		}
	}
	fout.close();
	fout.open("tmpFiles/ans4submit.out");
	fout << n << " " << m << endl;
	for (int i = 0;i <= n;i++) {
		for (int j = 0;j < m;j++) fout << r[i][j] << " ";
		fout << endl;
	}
	for (int i = 0;i < n;i++) {
		for (int j = 0;j <= m;j++) fout << d[i][j] << " ";
		fout << endl;
	}
	fout.close();
	int scd = (clock() - _TIME_) * 1000ll / CLOCKS_PER_SEC;
	cout << fixed << setprecision(3);
	cout << "Time elapsed = " << scd / 3600000 << "h, " << (scd % 3600000) / 60000 << "m, " << (scd % 60000) / 1000.00 << "s" << endl;
}

// print the current status onto the screen
void Print(int dep, int &mxdep, int &dfscnt, int n, int m, int *a[], int r[55][55], int d[55][55], int _TIME_) {
    using namespace interfaceGlobalVariables;
	mxdep = max(mxdep, dep);
	writeCache(n, m, a, r, d);
	for (int i = 0;i <= R;i++) {
		for (int j = 0;j <= C;j++) {
			if (cur[i][j] != pCache[i][j]) {
				Movepos(j, i); cout << (cur[i][j] = pCache[i][j]);
			}
		}
	}
	Movepos(0, R + 2);
	cout << "steps = " << dfscnt << ", depth = " << dep << "    " << endl;
    int scd = (clock() - _TIME_) * 1000ll / CLOCKS_PER_SEC;
	cout << fixed << setprecision(3);
	cout << "Time elapsed = " << scd / 3600000 << "h, " << (scd % 3600000) / 60000 << "m, " << (scd % 60000) / 1000.00 << "s       " << endl;
}