#include "Util.h"

void Util::PrintTabs(int n) {
    for (int i = 0; i < n; ++i) {
        // cout << '\t'; // Fica um bocado diferente..
        cout << "    ";
    }
}

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
}

double Util::GetInformationGain(Data &data, int column, double set_entropy, set<int> &allowed_rows) {
    auto column_probabilities = GetColumnProbabilities(column, data, allowed_rows);
    double column_entropy = GetEntropy(column_probabilities, data, column);
    return (set_entropy - column_entropy);
}

unordered_map<string, unordered_map<string, int>> Util::GetColumnProbabilities(int column, Data &data, set<int> &allowed_rows) {
    // {row label, classe} = number
    unordered_map<string, unordered_map<string, int>> value_classes;
    for (const auto &elem : data.classes)
        value_classes[elem];

    for (auto i_row : allowed_rows) {
        auto row = data.table[i_row];
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
        // Nao sei se posso por isto aqui ( na logica matematica... ) (pus o if total para nao dividir por zero)
        if (total > 0) entropy += Util::EntropyFormula(column, total, total_rows);
    }

    return entropy;
}

double Util::GetSetEntropy(Data &data, set<int> &allowed_rows) {
    unordered_map<string, int> count;
    for (auto &c : data.classes)
        count[c] = 0;
    int rows = 0;
    for (auto itr : allowed_rows) {
        auto row = data.table[itr];
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
            continue;
        entropy += -((c.second / (double)total) * log2(c.second / (double)total));
    }
    return entropy;
}

double Util::EntropyFormula(vector<int> &column, int total, int max_rows) {
    double entropy = 0;
    if (total == 0) {
        cout << "cannot divide by zero.. check entropy formula..\n";
        exit(1);
    }
    for (int c : column) {
        if (c == 0)
            continue;
        entropy += -((c / (double)total) * log2(c / (double)total));
    }
    return (total / (double)max_rows) * entropy;
}

void Util::BuildAttributesTypes(Data &data) {
    regex number_regex("^(-?[1-9]+\\d*([.]\\d+)?)$|^(-?0[.]\\d*[1-9]+)$|^0$|^0.0$");
    for (int col = 0; col < data.attributes.size(); col++) {
        unordered_set<double> numbers_set;
        int numbers = 0;
        int strings = 0;
        int type;
        for (auto row : data.table) {
            if (regex_match(row[col], number_regex)) {
                // It's a number
                double number = stod(row[col]);
                if (numbers_set.find(number) == numbers_set.end()) {
                    // number has been seen before
                    numbers_set.insert(number);
                    numbers++;
                }
            } else
                strings++;
        }

        if (strings == 0 && numbers > 6)
            type = NUMERIC;
        else
            type = CATEGORIC;

        data.attributes[col].second.first = type;

        if (type != CATEGORIC)
            Util::DiscretizeColumn(data, col);
    }

    // Fill the possible values for a given attribute
    for (int col = 0; col < data.attributes.size(); col++) {
        for (int row = 0; row < data.table.size(); row++) {
            data.attributes[col].second.second.insert(data.table[row][col]);
        }
    }
}

map<string, int> Util::CountExampleClasses(Data &data, set<int> &allowed_rows) {
    map<string, int> m;
    for (auto r : allowed_rows)
        m[data.table[r][data.table[0].size() - 1]]++;

    return m;
}

void Util::UpdateAllowedRowsAndCols(Data &data, int column, string target_value, set<int> &allowed_rows, set<int> &allowed_cols) {
    for (int i = 0; i < data.table.size(); i++) {
        if (data.table[i][column] != target_value)
            allowed_rows.erase(i);
    }

    allowed_cols.erase(column);
}

// Return: {coluna, information_gain}
pair<int, double> Util::GetBestAttribute(Data &data, set<int> &allowed_cols, set<int> &allowed_rows, double set_entropy) {
    pair<int, double> best_attribute = {-1, -1};
    for (auto col : allowed_cols) {
        double information_gain = Util::GetInformationGain(data, col, set_entropy, allowed_rows);
        if (information_gain > best_attribute.second)
            best_attribute = {col, information_gain};
        // cout << data.attributes[col].first << ": " << information_gain << "\n";
    }
    return best_attribute;
}

void Util::DiscretizeColumn(Data &data, int column) {
    vector<pair<float, float>> ranges;
    GetColumnRanges(data, column, ranges);

    for (auto &row : data.table) {
        stringstream ss;
        pair<float, float> range = ChooseRange(ranges, stof(row[column]));
        ss << "[" << range.first << ", " << range.second << "]";
        row[column] = ss.str();
    }
}

pair<float, float> Util::ChooseRange(vector<pair<float, float>> &ranges, float value) {
    vector<pair<float, float>> possible_ranges;
    for (auto range : ranges)
        if (range.first <= value && value <= range.second)
            possible_ranges.push_back(range);

    int index = rand() % possible_ranges.size();
    pair<float, float> r = possible_ranges[index];
    return r;
}

void Util::GetColumnRanges(Data &data, int column, vector<pair<float, float>> &ranges) {
    vector<pair<double, string>> values(data.table.size());

    int max_precision = 0;
    for (int row = 0; row < data.table.size(); row++) {
        string number = data.table[row][column];
        int dot_position = number.find(".");
        if (dot_position != number.npos)
            max_precision = max(max_precision, (int)(number.length() - dot_position - 1));

        float t1 = stof(number);
        string t2 = data.table[row][data.table[0].size() - 1];
        values[row] = {t1, t2};
    }

    sort(values.begin(), values.end());

    int rows_per_class = ceil(values.size() / (1 + 3.322 * log10(values.size())));

    double min_value;
    double max_value;
    for (int i = 0; i < values.size(); i++) {
        if (i % rows_per_class == 0) {
            // Beggining of new interval
            min_value = values[i].first;
            max_value = values[i + rows_per_class - 1 >= values.size() ? values.size() - 1 : i + rows_per_class - 1].first;
        }
        ranges.push_back({min_value, max_value});
        i += rows_per_class - 1;
    }

    ranges[ranges.size() - 1].second = INT_MAX;  // Make the last interval [value, +inf]
    ranges[0].first = INT_MIN;                   // Make the first interval [-inf, value]
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

void Util::ReadTestData(Data &training_data, Data &test_data, string filename) {
    FillTable(test_data, filename);
    GetAttributes(test_data);
    GetClasses(test_data);
    BuildAttributesTypesForTestData(training_data, test_data);
}

void Util::BuildAttributesTypesForTestData(Data &training_data, Data &test_data) {
    regex number_regex("^(-?[1-9]+\\d*([.]\\d+)?)$|^(-?0[.]\\d*[1-9]+)$|^0$|^0.0$");
    for (int col = 0; col < test_data.attributes.size(); col++) {
        unordered_set<double> numbers_set;
        int numbers = 0;
        int strings = 0;
        int type;
        for (auto row : test_data.table) {
            if (regex_match(row[col], number_regex)) {
                // It's a number
                double number = stod(row[col]);
                if (numbers_set.find(number) == numbers_set.end()) {
                    // number has been seen before
                    numbers_set.insert(number);
                    numbers++;
                }
            } else
                strings++;
        }

        if (strings == 0 && numbers > 6)
            type = NUMERIC;
        else
            type = CATEGORIC;

        test_data.attributes[col].second.first = type;

        if (type != CATEGORIC)
            DiscretizeColumnTestData(training_data, test_data, col);
    }

    // Fill the possible values for a given attribute
    for (int col = 0; col < test_data.attributes.size(); col++) {
        for (int row = 0; row < test_data.table.size(); row++) {
            test_data.attributes[col].second.second.insert(test_data.table[row][col]);
        }
    }
}

void Util::DiscretizeColumnTestData(Data &training_data, Data &test_data, int column) {
    for (auto &row : test_data.table) {
        row[column] = GetRandomRange(training_data, stod(row[column]), column);
    }
}

string Util::GetRandomRange(Data &training_data, double value, int column) {
    vector<string> possible_values;
    for (auto range : training_data.attributes[column].second.second) {
        int range_find = range.find(",");
        double n1 = stod(range.substr(1, range_find - 1));
        double n2 = stod(range.substr(range_find + 2, range.size() - range_find - 3));
        if (n1 <= value && value <= n2)
            possible_values.push_back(range);
    }

    return possible_values[rand() % possible_values.size()];
}

void Util::GetPrediction(Node &node, Data &data) {
    for (auto row : data.table) {
        Util::SearchTree(node, row);
    }
}

void Util::SearchTree(Node &node, vector<string> &row) {
    if (node.children.size() == 0) {
        // Leaf node
        cout << node.attribute << "\n";
        return;
    }
    for (auto child : node.children) {
        if (child.second == row[node.attribute_column]) {
            SearchTree(*child.first, row);
            return;
        }
    }

    // If we got here, it means the attribute we're at doesn't exist in the training data
    cout << "[ERROR]: " << row[node.attribute_column] << " attribute has not been seen in the training data.\n";
    return;
}

void Util::UpdateCounter(Node &node, Data &data) {
    for (auto row : data.table) {
        Util::UpdateCounter(node, row);
    }
}

void Util::UpdateCounter(Node &node, vector<string> &row) {
    if (node.children.size() == 0) {
        // Leaf node
        node.count++;
        return;
    }
    for (auto child : node.children) {
        if (child.second == row[node.attribute_column]) {
            UpdateCounter(*child.first, row);
            return;
        }
    }
}

void Util::PrintTree(Node &cur, int tabs) {
    if (cur.children.size() > 0) {       // It's an attribute
        if (cur.way_taken.size() > 0) {  // Ignore the root
            Util::PrintTabs(tabs - 1);
            cout << cur.way_taken << ":" << '\n';
        }
        Util::PrintTabs(tabs);
        cout << '<' << cur.attribute << '>' << '\n';
    } else {  // It's the class
        Util::PrintTabs(tabs - 1);
        cout << cur.way_taken << ": " << cur.attribute << ' ' << '(' << cur.count << ')' << '\n';
    }
    for (auto itr = cur.children.begin(); itr != cur.children.end(); ++itr) {
        PrintTree(*itr->first, tabs + 2);
    }
}

void Util::ID3(Data &training_data, Node &current_node) {
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