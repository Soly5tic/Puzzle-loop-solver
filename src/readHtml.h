#ifndef READHTML_H
#define READHTML_H

#include <bits/stdc++.h>
using namespace std;

namespace readHtmlGlobalVariables {
    const string MATCH = "loop-task-cell", FOLDR = "htmlFiles/";
}

int readHTMLFile(const int, const int, string, int**);
int readPlainText(string, int&, int&, int**);

#endif
