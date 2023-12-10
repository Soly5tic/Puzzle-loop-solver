#ifndef READHTML_H
#define READHTML_H

#include <bits/stdc++.h>
using namespace std;

namespace readHtmlGlobalVariables {
    const string MATCH = "loop-task-cell", FOLDR = "htmlFiles/";
}

void readHTMLFile(const int, const int, string, int**);
void readPlainText(string, int&, int&, int**);

#endif
