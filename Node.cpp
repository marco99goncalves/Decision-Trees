#include "Node.h"

vector<shared_ptr<Node>> children;
Node *parent;
set<int> removed_rows;

Node::Node() {
    children.resize(0);
}
