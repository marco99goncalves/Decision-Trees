#pragma once
#include <bits/stdc++.h>

using namespace std;

class Node {
   public:
    vector<pair<shared_ptr<Node>, string>> children;
    string attribute;
    int attribute_column;
    string most_common_attribute;
    Node *parent;

    set<int> allowed_rows;
    set<int> allowed_cols;

    Node(string attribute, set<int> &allowed_rows, set<int> &allowed_cols);
    void AddChild(set<int> &allowed_rows, set<int> &allowed_cols);
};