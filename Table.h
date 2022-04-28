#pragma once
#include <bits/stdc++.h>

using namespace std;
class Table {
   public:
    vector<vector<string>> table;
    unordered_set<string> classes;
    vector<pair<string, string>> attributes;
};