#include <bits/stdc++.h>
#include "Data.h"
#include "Util.h"
#include "Node.h"
using namespace std;

void ID3(Data &data, Node &current_node);

void dfs_printing(Node &cur, int tabs){
    if(cur.children.size() > 0){ // It's an attribute
        if(cur.way_taken.size() > 0){ // Ignore the root
            Util::PrintTabs(tabs-1);
            cout << cur.way_taken << ":" << '\n';
        }
        Util::PrintTabs(tabs);
        cout  << '<' << cur.attribute << '>' << '\n';
    }
    else { // It's the class
        Util::PrintTabs(tabs-1);
        cout << cur.way_taken << ": " << cur.attribute << '\n';
    }
    for(auto itr = cur.children.begin(); itr != cur.children.end(); ++itr){
        dfs_printing(*itr->first, tabs+2);
    }
}

void dfs(Node &cur, int level){
    cout << "level" << ": "  << level << " (" << cur.way_taken << ") " <<  cur.attribute << '\n';
    // cout << level << ": " << cur.attribute << '\n';
    // if(cur.children.size() == 0) exit(0);
    for(auto itr = cur.children.begin(); itr != cur.children.end(); ++itr){
        dfs(*itr->first, level+1);
    }
}

void table_printer(Data &data, set<int> &allowed_rows, set<int> &allowed_cols){
    for(int i=0; i<data.table.size(); ++i){
        if(allowed_rows.find(i) == allowed_rows.end()) continue;

        for(int j=0; j<data.table[0].size(); ++j){
            if(allowed_cols.find(j) == allowed_cols.end()) continue;
            cout << data.table[i][j] << ' ' << data.table[i][data.table[0].size()-1];
        } cout << '\n';
    } cout << '\n';
}

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
    int xd = system("clear");
    ID3(data, *root);

    dfs_printing(*root, 0); // Just to print the tree
    return 0;
}

void ID3(Data &data, Node &current_node) {
    // cout << current_node.attribute << "\n";
    map<string, int> m = Util::CountExampleClasses(data, current_node.allowed_rows);

    // table_printer(data, current_node.allowed_rows, current_node.allowed_cols);
    int max_value = -1;
    for (auto itr : m) {  // get the max_value ( for the second case of recursion )
        // cout << "ver: " << itr.first << ' ' << itr.second << '\n';
        if (itr.second > max_value) {
            current_node.most_common_attribute = itr.first;
            max_value = itr.second;
        }
    }
    // cout << '\n';
    if (m.size() == 1) { // All are yes / no  (or all have the same value)
        // Create leaf
        // cout << "this should be a leaf..." << '\n' << '\n';
        current_node.attribute = m.begin()->first;
        // current_node.attribute = "I am the evil basterd!\n";
        return;
    } else if (m.size() == 0) { // The node is empty ( return to parent node )
        current_node.most_common_attribute = current_node.parent->most_common_attribute;
        current_node.attribute = current_node.most_common_attribute;
        // current_node.attribute = "OLACARGO";
        // cout << current_node.attribute << ":  Mae estou aqui\n";
        return;
    }

    // Recursao
    for (auto attribute : data.attributes[current_node.attribute_column].second.second) {
        set<int> next_node_allowed_rows = current_node.allowed_rows;
        set<int> next_node_allowed_cols = current_node.allowed_cols;
        double set_entropy = Util::GetSetEntropy(data, next_node_allowed_rows);

        Util::UpdateAllowedRowsAndCols(data, current_node.attribute_column, attribute, next_node_allowed_rows, next_node_allowed_cols);
        // Get best attribute for this branch
        pair<int, double> best_attribute = Util::GetBestAttribute(data, next_node_allowed_cols, next_node_allowed_rows, set_entropy);

        shared_ptr<Node> next_node = make_shared<Node>(data.attributes[best_attribute.first].first, next_node_allowed_rows, next_node_allowed_cols);
        next_node->attribute_column = best_attribute.first;
        next_node->parent = &current_node;
        next_node->way_taken = attribute;

        


        if(next_node_allowed_cols.size() < 1){ // Neste caso não podemos dar split..
            // Logica seria escolher a melhor contagem neste caso
            // table_printer(data, next_node_allowed_rows, next_node_allowed_cols);
            map<string, int> m1 = Util::CountExampleClasses(data, next_node_allowed_rows);
            string checker = "";
            int max1_value = -1;
            for (auto itr : m1) {  // get the max_value ( for the second case of recursion )
                // cout << "ver: " << itr.first << ' ' << itr.second << '\n';
                if (itr.second > max1_value) {
                    checker = itr.first;
                    max1_value = itr.second;
                }
            }
            next_node->attribute = checker;

            // ========================================================================================
            // Este é o caso em que não podemos dividir mais, mas ainda não atingimos nenhum caso base
            if(max1_value == -1) next_node->attribute = current_node.most_common_attribute;
            // ========================================================================================

            current_node.children.push_back({next_node, "this should be it"});    
        }
        else{
          current_node.children.push_back({next_node, attribute});  
          ID3(data, *next_node);
        } 
    }
}