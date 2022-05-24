#pragma once
#include <bits/stdc++.h>

using namespace std;

class Node {
   public:
    vector<pair<shared_ptr<Node>, string>> children;
    string attribute;
    int attribute_column;
    int count;
    string most_common_attribute;
    Node *parent;
    string way_taken;

    set<int> allowed_rows;
    set<int> allowed_cols;

    Node(string attribute, set<int> &allowed_rows, set<int> &allowed_cols);
    void AddChild(set<int> &allowed_rows, set<int> &allowed_cols);
};