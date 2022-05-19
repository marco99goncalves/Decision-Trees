#include "Node.h"

vector<pair<shared_ptr<Node>, string>> children;
string attribute;
string most_common_attribute;
Node *parent;

set<int> allowed_rows;
set<int> allowed_cols;

Node::Node(string attribute, set<int> &allowed_rows, set<int> &allowed_cols) {
    this->attribute = attribute;
    this->allowed_rows = allowed_rows;
    this->allowed_cols = allowed_cols;
    children.resize(0);
}

void Node::AddChild(set<int> &allowed_rows, set<int> &allowed_cols) {
    // do stuff here
}
