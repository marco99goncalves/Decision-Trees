#include <bits/stdc++.h>
#include "Data.h"
#include "Util.h"
using namespace std;

int main(int argc, char** argv) {
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

    set<int> allowed_rows = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    set<int> allowed_cols = {0, 1, 2};

    double set_entropy = Util::GetSetEntropy(data, allowed_rows);
    cout << "Set Entropy: " << set_entropy << "\n";

    // { coluna, information_gain}
    pair<int, double> best_attribute = {-1, -1};
    for (int col = 0; col < data.attributes.size() - 1; col++) {
        double information_gain = Util::GetInformationGain(data, col, set_entropy, allowed_rows);
        if (information_gain > best_attribute.second)
            best_attribute = {col, information_gain};
        cout << data.attributes[col].first << ": " << information_gain << "\n";
    }

    // Chamar Util::ID3(data, best_attribute.first)

    allowed_rows = {1, 3, 4, 6, 7, 8, 11, 12};
    allowed_cols = {0, 1};

    set_entropy = Util::GetSetEntropy(data, allowed_rows);
    cout << "\n-----------------\n";
    cout << "Set Entropy: " << set_entropy << "\n";
    for (auto col : allowed_cols) {
        double information_gain = Util::GetInformationGain(data, col, set_entropy, allowed_rows);
        if (information_gain > best_attribute.second)
            best_attribute = {col, information_gain};
        cout << data.attributes[col].first << ": " << information_gain << "\n";
    }

    // cout << data.attributes[best_attribute.first].first << " " << best_attribute.second << "\n";
    return 0;
}