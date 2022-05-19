#include <bits/stdc++.h>
#include "Data.h"
#include "Util.h"
#include "Node.h"
using namespace std;

void ID3(Data &data, Node &current_node);

int main(int argc, char **argv) {
    srand(time(NULL));

    Data data;
    Util::ReadData(data, argv[1]);

    // Util::PrintTable(data);

    // cout << INT_MIN << " - " << INT_MAX << "\n";
    // for (auto att : data.attributes) {
    //     cout << att.first << " " << att.second.first << '\n';
    //     for (auto& e : att.second.second)
    //         cout << e << " ";
    //     cout << '\n';
    // }

    set<int> allowed_rows;
    set<int> allowed_cols;

    // Inicializar as rows e colunas que vão ser usadas
    for (int i = 0; i < data.table.size(); i++)
        allowed_rows.insert(i);
    for (int i = 0; i < data.attributes.size() - 1; i++)
        allowed_cols.insert(i);

    double set_entropy = Util::GetSetEntropy(data, allowed_rows);

    cout << "Set Entropy: " << set_entropy << "\n";
    pair<int, double> best_attribute = Util::GetBestAttribute(data, allowed_cols, allowed_rows, set_entropy);
    shared_ptr<Node> root = make_shared<Node>(data.attributes[best_attribute.first].first, allowed_rows, allowed_cols);
    root->attribute_column = best_attribute.first;
    root->parent = nullptr;
    ID3(data, *root);

    return 0;
}

void ID3(Data &data, Node &current_node) {
    cout << current_node.attribute << "\n";
    map<string, int> m = Util::CountExampleClasses(data, current_node.allowed_rows);
    int max_value;
    for (auto itr : m) {
        if (itr.second > max_value) {
            current_node.most_common_attribute = itr.first;
            max_value = itr.second;
        }
    }

    if (m.size() == 1) {
        // Create leaf
        current_node.attribute = m.begin()->first;
        return;
    } else if (m.size() == 0) {
        current_node.most_common_attribute = current_node.parent->most_common_attribute;
        current_node.attribute = current_node.most_common_attribute;
        cout << current_node.attribute << ":  Mae estou aqui\n";
        return;
    }

    // Recursao
    for (auto attribute : data.attributes[current_node.attribute_column].second.second) {
        set<int> next_node_allowed_rows = current_node.allowed_rows;
        set<int> next_node_allowed_cols = current_node.allowed_cols;
        Util::UpdateAllowedRowsAndCols(data, current_node.attribute_column, attribute, next_node_allowed_rows, next_node_allowed_cols);
        double set_entropy = Util::GetSetEntropy(data, next_node_allowed_rows);

        // Get best attribute for this branch
        pair<int, double> best_attribute = Util::GetBestAttribute(data, next_node_allowed_cols, next_node_allowed_rows, set_entropy);

        shared_ptr<Node> next_node = make_shared<Node>(data.attributes[best_attribute.first].first, next_node_allowed_rows, next_node_allowed_cols);
        next_node->attribute_column = best_attribute.first;
        next_node->parent = &current_node;

        current_node.children.push_back({next_node, attribute});
        ID3(data, *next_node);
    }
}