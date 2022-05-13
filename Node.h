#pragma once
#include <bits/stdc++.h>

using namespace std;

class Node {
   public:
    vector<shared_ptr<Node>> children;
    Node* parent;
    set<int> removed_rows;

    Node();
};