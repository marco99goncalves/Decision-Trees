#include "Util.h"

void Util::FillTable(Data &data, string filename) {
    fstream file(filename, ios::in);
    string line, word;
    if (file.is_open()) {
        while (getline(file, line)) {
            vector<string> row;
            stringstream str(line);
            int i = 0;
            while (getline(str, word, ','))
                if (i++ != 0)
                    row.push_back(word);
            data.table.push_back(row);
        }
    }
}

void Util::GetAttributes(Data &data) {
    for (auto att : data.table[0])
        data.attributes.push_back({att, {-1, {}}});
    data.table.erase(data.table.begin());

    for (int col = 0; col < data.table[0].size(); col++) {
        for (int row = 0; row < data.table.size(); row++) {
            data.attributes[col].second.second.insert(data.table[row][col]);
        }
    }
}

unordered_map<string, unordered_map<string, int>> Util::GetColumnProbabilities(int column, Data &data) {
    // {row label, classe} = number
    unordered_map<string, unordered_map<string, int>> value_classes;
    for (const auto &elem : data.classes)
        value_classes[elem];

    for (auto row : data.table) {
        value_classes[row[row.size() - 1]][row[column]]++;
        for (const auto &elem : data.classes)
            value_classes[elem][row[column]];
    }

    return value_classes;
}

double Util::GetEntropy(unordered_map<string, unordered_map<string, int>> &matrix, Data &data, int column) {
    int total_rows = 0;
    for (auto &elem : matrix)
        for (auto &m_elem : matrix[elem.first])
            total_rows += m_elem.second;

    double entropy = 0;
    for (auto s : data.attributes[column].second.second) {  // For each possible value of the attribute
        vector<int> column;
        int total = 0;
        for (auto &col : matrix) {  // for each class
            int cell = matrix[col.first][s];
            column.push_back(cell);
            total += cell;
        }
        entropy += Util::EntropyFormula(column, total, total_rows);
    }

    return entropy;
}

double Util::GetSetEntropy(Data &data) {
    unordered_map<string, int> count;
    for (auto &c : data.classes)
        count[c] = 0;
    int rows = 0;
    for (auto row : data.table) {
        count[row[row.size() - 1]]++;
        rows++;
    }

    return EntropyFormula(count, rows);
}

// For the entire set
double Util::EntropyFormula(unordered_map<string, int> &column, int total) {
    double entropy = 0;
    for (auto &c : column) {
        if (c.second == 0)
            return 0;
        entropy += -((c.second / (double)total) * log2(c.second / (double)total));
    }
    return entropy;
}

double Util::EntropyFormula(vector<int> &column, int total, int max_rows) {
    double entropy = 0;
    for (int c : column) {
        if (c == 0)
            return 0;
        entropy += -((c / (double)total) * log2(c / (double)total));
    }
    return (total / (double)max_rows) * entropy;
}

double Util::GetInformationGain(Data &data, int column, double set_entropy) {
    auto column_probabilities = GetColumnProbabilities(column, data);
    double column_entropy = GetEntropy(column_probabilities, data, column);
    return (set_entropy - column_entropy);
}

void Util::BuildAttributesTypes(Data &data) {
    regex number_regex("^(-?[1-9]+\\d*([.]\\d+)?)$|^(-?0[.]\\d*[1-9]+)$|^0$|^0.0$");
    for (int col = 0; col < data.attributes.size(); col++) {
        int integers = 0;
        int strings = 0;
        int floats = 0;
        int type;
        for (auto row : data.table) {
            if (regex_match(row[col], number_regex)) {
                // It's a number
                if (row[col].find(".") != string::npos)
                    floats++;
                else
                    integers++;
            } else
                strings++;
        }

        if (strings == 0)
            if (floats == 0)
                type = INTEGER;
            else
                type = FLOAT;
        else
            type = STRING;

        data.attributes[col].second.first = type;

        // if (type != STRING)
        //     Util::Discretize(data, col);
    }
}

void Util::GetClasses(Data &data) {
    for (auto row : data.table)
        data.classes.insert(row[row.size() - 1]);
}

void Util::ReadData(Data &data, string filename) {
    FillTable(data, filename);
    GetAttributes(data);
    GetClasses(data);
    BuildAttributesTypes(data);
}

void Util::PrintTable(Data &data) {
    for (auto row : data.table) {
        for (auto col : row)
            cout << col << ' ';
        cout << '\n';
    }
}