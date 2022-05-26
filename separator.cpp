#include <bits/stdc++.h>
using namespace std;
// g++ -Wall -o separator separator.cpp && ./separator car.csv train.csv test.csv expected 11
// make && ./out train.csv test.csv expected

// Novos files: car.csv, nursery.csv, breast-cancer.csv(286 entries)

string readFileIntoString(const string& path) {
    auto ss = ostringstream{};
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '" << path << "'" << '\n';
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

int main(int argc, char **argv){
    ios::sync_with_stdio(0); cin.tie(0);
    if(argc != 6){
        cout << "Error(input mode): [csv_file]  [out_train]  [out_test]  [expected_results]  [threshold]" << '\n';
        return EXIT_FAILURE;
    }
    int limit = stoi(argv[5]) + 1; // ignore attribute row count
    string file_contents;
    std::map<int, std::vector<string>> csv_contents;
    char delimiter = ',';

    file_contents = readFileIntoString(argv[1]);

    istringstream sstream(file_contents);
    std::vector<string> items;
    string record;

    int counter = 0;

    while (std::getline(sstream, record)) {
        istringstream line(record);
        while (std::getline(line, record, delimiter)) {
            items.push_back(record);
        }
        csv_contents[counter] = items;
        items.clear();
        counter += 1;
    }

    ofstream a_file (argv[2]);

    auto iter = csv_contents.begin();

    for(int i=0; i<limit; ++i, ++iter){
        a_file << *iter->second.begin();
        for(auto itr = next(iter->second.begin()); itr != iter->second.end(); ++itr){
            a_file << ',' << *itr;
        } a_file << '\n';
    }
    a_file.close();

    auto itrr = iter;
    ofstream b_file(argv[3]);
    auto x = csv_contents.begin();
    b_file << *x->second.begin();
    for(auto xx = next(x->second.begin()); xx != prev(x->second.end()); ++xx){
        b_file << ',' << *xx;
    } b_file << '\n';
    for(; itrr != csv_contents.end(); ++itrr){
        b_file << *itrr->second.begin();
        for(auto itr = next(itrr->second.begin()); itr != prev(itrr->second.end()); ++itr){
            b_file << ',' << *itr;
        } b_file << '\n';
    }
    b_file.close();



    ofstream c_file(argv[4]);

    for(; iter != csv_contents.end(); ++iter){
        c_file << *prev(iter->second.end()) << '\n';
    }
}