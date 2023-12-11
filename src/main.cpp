#pragma GCC optimize("Ofast","-funroll-loops")
#pragma GCC target("sse4.1","sse4.2","ssse3","sse3","sse2","sse","avx2","avx","popcnt","tune=native")
#include <bits/stdc++.h>
using namespace std;

#include "solver.h"
#include "readHtml.h"

int *a[55];

int main(int argc, char** argv) {
    for (int i = 0;i < 55;i++) a[i] = new int [55];
    for (;;) {
        cout << "Solve from HTML file: input \"1 [puzzle height] [puzzle length] [HTML file name] [Visibility]\"" << endl;
        cout << "Solve from plain text file: input \"2 [plain text file name] [Visibility]\"" << endl;
        cout << "Input anything other than 1,2 to quit" << endl;
        string s;
        cin >> s;
        if (s == "1" || s == "2") {
            int n, m;
            if (s == "1") {
                string name;
                cin >> n >> m >> name;
                readHTMLFile(n, m, name, a);
            } else {
                string name;
                cin >> name;
                readPlainText(name, n, m, a);
            }
            system("cls");
            if (Solve(n, m, a, 1)) {
                cout << "Found solution" << endl;
            } else {
                cout << "No solution found" << endl;
            }
        } else break;
    }
    for (int i = 0;i < 55;i++) delete [] a[i];
    return 0;
}