#include <bits/stdc++.h>
#include "Data.h"
#include "Util.h"
#include "Node.h"
using namespace std;

void ID3(Data &training_data, Node &current_node);

void dfs_printing(Node &cur, int tabs) {
    if (cur.children.size() > 0) {       // It's an attribute
        if (cur.way_taken.size() > 0) {  // Ignore the root
            Util::PrintTabs(tabs - 1);
            cout << cur.way_taken << ":" << '\n';
        }
        Util::PrintTabs(tabs);
        cout << '<' << cur.attribute << '>' << '\n';
    } else {  // It's the class
        Util::PrintTabs(tabs - 1);
        cout << cur.way_taken << ": " << cur.attribute << '\n';
    }
    for (auto itr = cur.children.begin(); itr != cur.children.end(); ++itr) {
        dfs_printing(*itr->first, tabs + 2);
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    int xd = system("clear");
    if (argc < 3) {
        cout << "Error: Two .csv files must be provided as arguments\n";
        cout << "Please provide the training file followed by the testing file:\n";
        cout << "$ make && ./out training.csv testing.csv\n";
        return EXIT_FAILURE;
    }

    Data training_data;
    Data test_data;

    Util::ReadData(training_data, argv[1]);
    Util::ReadTestData(training_data, test_data, argv[2]);

    // Util::PrintTable(training_data);
    set<int> allowed_rows;
    set<int> allowed_cols;

    // Inicializar as rows e colunas que vão ser usadas
    for (int i = 0; i < training_data.table.size(); i++)
        allowed_rows.insert(i);
    for (int i = 0; i < training_data.attributes.size() - 1; i++)
        allowed_cols.insert(i);

    double set_entropy = Util::GetSetEntropy(training_data, allowed_rows);

    cout << "Set Entropy: " << set_entropy << "\n";
    pair<int, double> best_attribute = Util::GetBestAttribute(training_data, allowed_cols, allowed_rows, set_entropy);
    shared_ptr<Node> root = make_shared<Node>(training_data.attributes[best_attribute.first].first, allowed_rows, allowed_cols);
    root->attribute_column = best_attribute.first;
    root->parent = nullptr;
    ID3(training_data, *root);

    cout << "\n\n============ Tree Structure ==============\n\n";

    dfs_printing(*root, 0);  // Just to print the tree

    cout << "\n\n============= Predicitions ==============\n\n";
    for (auto row : test_data.table) {
        Util::SearchTree(*root, row);
    }
    cout << "\n=========================================\n\n";
    return 0;
}

void ID3(Data &training_data, Node &current_node) {
    map<string, int> m = Util::CountExampleClasses(training_data, current_node.allowed_rows);
    int max_value = -1;
    // Casos base
    for (auto itr : m) {  // get the max_value ( for the second case of recursion )
        if (itr.second > max_value) {
            current_node.most_common_attribute = itr.first;
            max_value = itr.second;
        }
    }
    if (m.size() == 1) {  // All are yes / no  (or all have the same value)
        // Create leaf
        current_node.attribute = m.begin()->first;
        return;
    } else if (m.size() == 0) {  // The node is empty ( return to parent node )
        current_node.most_common_attribute = current_node.parent->most_common_attribute;
        current_node.attribute = current_node.most_common_attribute;
        return;
    }

    // Recursao
    for (auto attribute : training_data.attributes[current_node.attribute_column].second.second) {
        set<int> next_node_allowed_rows = current_node.allowed_rows;
        set<int> next_node_allowed_cols = current_node.allowed_cols;
        double set_entropy = Util::GetSetEntropy(training_data, next_node_allowed_rows);

        Util::UpdateAllowedRowsAndCols(training_data, current_node.attribute_column, attribute, next_node_allowed_rows, next_node_allowed_cols);
        // Get best attribute for this branch
        pair<int, double> best_attribute = Util::GetBestAttribute(training_data, next_node_allowed_cols, next_node_allowed_rows, set_entropy);

        shared_ptr<Node> next_node = make_shared<Node>(training_data.attributes[best_attribute.first].first, next_node_allowed_rows, next_node_allowed_cols);
        next_node->attribute_column = best_attribute.first;
        next_node->parent = &current_node;
        next_node->way_taken = attribute;

        if (next_node_allowed_cols.size() < 1) {  // Neste caso não podemos dar split..
            // Logica seria escolher a melhor contagem neste caso
            map<string, int> m1 = Util::CountExampleClasses(training_data, next_node_allowed_rows);
            string checker = "";
            int max1_value = -1;
            for (auto itr : m1) {  // get the max_value ( for the second case of recursion )
                if (itr.second > max1_value) {
                    checker = itr.first;
                    max1_value = itr.second;
                }
            }
            next_node->attribute = checker;

            // ========================================================================================
            // Este é o caso em que não podemos dividir mais, mas ainda não atingimos nenhum caso base
            if (max1_value == -1) next_node->attribute = current_node.most_common_attribute;
            // ========================================================================================

            current_node.children.push_back({next_node, attribute});
        } else {
            current_node.children.push_back({next_node, attribute});
            ID3(training_data, *next_node);
        }
    }
}