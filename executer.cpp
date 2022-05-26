#include <bits/stdc++.h>
using namespace std;

char* convert(const std::string& source) {
    const auto result = (char*)std::malloc(source.length() + 1);
    source.copy(result, source.length());
    result[source.length()] = '\0';
    return result;
}

signed main(int argc, char** argv) {
    string s = argv[1];
    cout << s << '\n';

    string pre = "g++ -Wall -o separator separator.cpp && ./separator ";
    string pos = " train.csv test.csv expected ";
    pre += s;
    pre += pos;
    string limit = argv[2];
    pre += limit;
    cout << pre << '\n';
    const char* command = convert(pre);
    system(command);
    system("make && ./out train.csv test.csv expected");
}