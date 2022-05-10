#include <bits/stdc++.h>
#include "Data.h"
#include "Util.h"
using namespace std;

int main(int argc, char** argv) {
    srand(time(NULL));

    Data data;
    Util::ReadData(data, argv[1]);

    Util::PrintTable(data);

    // cout << INT_MIN << " - " << INT_MAX << "\n";
    // for (auto att : data.attributes) {
    //     cout << att.first << " " << att.second.first << '\n';
    //     for (auto& e : att.second.second)
    //         cout << e << " ";
    //     cout << '\n';
    // }

    // unordered_map<string, unordered_map<string, int>> matrix = Util::GetColumnProbabilities(0, data);
    // double set_entropy = Util::GetSetEntropy(data);
    // cout << ".." << set_entropy << "\n";
    // for (int col = 0; col < data.attributes.size() - 1; col++)
    //     cout << Util::GetInformationGain(data, col, set_entropy) << "\n";
    return 0;
}