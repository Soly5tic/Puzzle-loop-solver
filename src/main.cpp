#pragma GCC optimize("Ofast","-funroll-loops")
#pragma GCC target("sse4.1","sse4.2","ssse3","sse3","sse2","sse","avx2","avx","popcnt","tune=native")
#include <bits/stdc++.h>
using namespace std;

#include "solver.h"
#include "interface.h"
#include "readHtml.h"
#include "submit.h"
#include "autoRun.h"

int *a[55], *r[55], *d[55];

int main(int argc, char** argv) {
    try {
        freopen("log.txt", "w", stderr);
        for (int i = 0;i < 55;i++) {
            a[i] = new int [55];
            r[i] = new int [55];
            d[i] = new int [55];
        }
        system("mkdir D:\\loopTmpFiles");
        system("cls");
        for (;;) {
            cout << "Solve from HTML file: input \"1 [puzzle height] [puzzle width] [HTML file name]\"" << endl;
            cout << "Solve from plain text file: input \"2 [plain text file name]\"" << endl;
            cout << "Solve from HTML file and submit: input \"3 [puzzle height] [puzzle width] [HTML file name]\"" << endl;
            cout << "Auto solving on https://www.puzzle-loop.com : input \"4 [puzzle height] [puzzle width] [solve count]\"" << endl;
            cout << "Input anything other than 1,2,3,4 to quit" << endl;
            string s;
            cin >> s;
            if (s == "1" || s == "2" || s == "3") {
                int n, m, rres;
                if (s == "1" || s == "3") {
                    string name;
                    cin >> n >> m >> name;
                    rres = readHTMLFile(n, m, name, a);
                } else {
                    string name;
                    cin >> name;
                    rres = readPlainText(name, n, m, a);
                }
                if (rres == 0) {
                    system("cls");
                    interfaceInit();
                    if (Solve(n, m, a, r, d, 1)) {
                        cout << "Found solution" << endl;
                        if (s == "3") {
                            submitInit();
                            Submit(n, m, r, d, 15000);
                        }
                    } else {
                        cout << "No solution found" << endl;
                    }
                } else {
                    cout << "Reader failed, aborting process" << endl;
                }
            } else if (s == "4") {
                int n, m, cnt;
                cin >> n >> m >> cnt;
                cout << "Auto solving process start in 10 seconds" << endl;
                cout << "Keep the focus on the puzzle-loop web page" << endl;
                cout << "Please make sure you are using the EN-US keyboard setting and has pressed the \"Pin ads\" button on the web page" << endl;
                cout << "WARNING: THIS IS AN EXPERIMENTAL FEATURE AND SUPERVISION IS RECOMMENDED." << endl;
                cout << "Input 'y' to continue, anything else to abort" << endl;
                string s;
                cin >> s;
                if (s == "y") {
                    if (n > 50 || m > 50) {
                        cout << "ERROR: Too large puzzle size, support only puzzles with height and width less than or equal to 50" << endl;
                        cout << "Aborting process" << endl;
                    } else {
                        for (int i = 1;i <= cnt;i++) {
                            cout << "Puzzle " << i << endl;
                            downloadProblem();
                            assert(readHTMLFile(n, m, PTH, a) == 0);
                            system("cls");
                            interfaceInit();
                            Solve(n, m, a, r, d, 1);
                            submitInit();
                            Submit(n, m, r, d, 3000);
                            if (i != cnt) findNext(n, m);
                        }
                        cout << "Auto solving complete" << endl;
                    }
                }
            } else break;
        }
        for (int i = 0;i < 55;i++) {
            delete [] a[i];
            delete [] r[i];
            delete [] d[i];
        }
    } catch (...) {
        cout << "Error occurred!" << endl;
        cout << "Please try to restart the program. If the problem remain, please raise an issue on GitHub about how the problem occurred\
                    and include the log.txt file." << endl;
    }
    return 0;
}