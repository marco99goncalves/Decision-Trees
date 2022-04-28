#include "Util.h"

void Util::FillTable(Table &table, string filename) {
    fstream file(filename, ios::in);
    string line, word;
    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> row;

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            table.table.push_back(row);
        }
    }
}

void Util::PrintTable(Table &table) {
    for (auto row : table.table) {
        for (auto col : row)
            cout << col << ' ';
        cout << '\n';
    }
}