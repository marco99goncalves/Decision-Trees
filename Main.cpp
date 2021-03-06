#include <bits/stdc++.h>
#include "Data.h"
#include "Util.h"
#include <fstream>
#include "Node.h"
using namespace std;

int main(int argc, char **argv) {
    srand(time(NULL));
    // int xd = system("clear");
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

    pair<int, double> best_attribute = Util::GetBestAttribute(training_data, allowed_cols, allowed_rows, set_entropy);
    shared_ptr<Node> root = make_shared<Node>(training_data.attributes[best_attribute.first].first, allowed_rows, allowed_cols);
    root->attribute_column = best_attribute.first;
    root->parent = nullptr;

    /* ================= Training the Tree ==================== */

    Util::ID3(training_data, *root);
    Util::UpdateCounter(*root, training_data);
    
    
    if(argc == 3){
        cout << "\n\n============ Tree Structure ==============\n\n";

        Util::PrintTree(*root, 0);  // Just to print the tree

        cout << "\n\n============= Predictions ==============\n\n";

        Util::GetPrediction(*root, test_data);

        cout << "\n=========================================\n\n";
    }

    else{ // Caso de estarmos a ver os resultados...
        int sum, value;
        ifstream b_file (argv[3]);
        
        vector<string> checker;
        while(!b_file.eof()){
            string next;
            b_file >> next;
            checker.push_back(next);
        } 

        vector<string> results;
        Util::GetPredictionArray(*root, test_data, results); // guarda os resultados

        // Agora comparamos o checker aos resultados
        int total = 0;
        int hit = 0;

        for(int i=0; i<(int)results.size(); ++i){
            cout << "Result: " << results[i] << "  Expected: " << checker[i] << '\n';
            hit += checker[i] == results[i];
            ++total;
        }

        cout << "\nTrained the model using " << training_data.table.size() << " entries.\n";
        cout << "Guessed right in " << hit << " of " << total << " testing entries.\n";
        cout << "Sucess ratio: " << (double)hit / (double)total * 100 << '%' << '\n';
    }
    return EXIT_SUCCESS;
}

