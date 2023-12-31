/*
    Read a problem from a file
*/

#include "readHtml.h"

#include <bits/stdc++.h>
using namespace std;

// read a HTML file downloaded from puzzle-loop.com
// need to specify puzzle size
int readHTMLFile(const int N, const int M, string inputName, int **saveProblem) {
    if (N > 50 || M > 50) {
        cout << "ERROR: Too large puzzle size, support only puzzles with height and width less than or equal to 50" << endl;
        return -1;
    }
    string s;
    using namespace readHtmlGlobalVariables;
    ifstream fin((inputName).c_str());
    if (!fin.is_open()) {
        cerr << "ERR failed to open file" << endl;
        cout << "ERROR: failed to open file" << endl;
        return -1;
    }
    string t;
    while (getline(fin, t)) s += t;
    vector <int> a;
    for (int i = 0;i < s.length();i++) {
        if (s[i] == '<') {
            int l = i, r = i;
            while (s[r] != '>') r++;
            bool isc = 0;
            for (int j = l;j <= r - MATCH.length();j++) {
                bool flg = 1;
                for (int k = 0;k < MATCH.length();k++) {
                    if (s[j + k] != MATCH[k]) {
                        flg = 0;
                        break;
                    }
                }
                isc |= flg; if (isc) break;
            }
            if (isc) {
                if (r < s.length() && s[r + 1] >= '0' && s[r + 1] <= '3') a.push_back(s[r + 1] - '0');
                else a.push_back(-1);
            }
            i = r;
        }
    }
    cerr << "DBG read complete" << endl;
    cerr << "DBG read " << a.size() << " cells" << endl;
    if (a.size() != N * M) {
        cout << "ERROR incorrect puzzle size: expected " << N << "*" << M << "=" << N * M << "cells, found " << a.size() << " cells" << endl;
        return -1;
    }
    int pnt = 0;
    for (int i = 1;i <= N;i++) {
        for (int j = 1;j <= M;j++) {
            saveProblem[i - 1][j - 1] = a[pnt];
            pnt++;
        }
    }
    return 0;
}

// Read the plain text version
int readPlainText(string inputName, int &n, int &m, int **a) {
    ifstream fin(inputName.c_str());
    if (!fin.is_open()) {
        cerr << "ERR failed to open file" << endl;
        cout << "ERROR: failed to open file" << endl;
        return -1;
    }
	fin >> n >> m;
    if (n > 50 || m > 50) {
        cout << "ERROR: Too large puzzle size, support only puzzles with height and width less than or equal to 50" << endl;
        return -1;
    }
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < m;j++) {
			char c; fin >> c;
			if (c == '-') a[i][j] = -1;
			else a[i][j] = c - '0';
		}
	}
    cerr << "DBG read complete" << endl;
    return 0;
}