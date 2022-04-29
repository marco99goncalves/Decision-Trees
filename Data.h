#pragma once
#include <bits/stdc++.h>

using namespace std;
class Data {
   public:
    vector<vector<string>> table;
    unordered_set<string> classes;
    vector<pair<string, pair<int, unordered_set<string>>>> attributes;
};