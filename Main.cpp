#include <bits/stdc++.h>
#include "Table.h"
#include "Util.h"
using namespace std;

int main(int argc, char** argv) {
    Table table;
    Util::FillTable(table, argv[1]);
    Util::PrintTable(table);
    return 0;
}