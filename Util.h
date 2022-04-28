#pragma once
using namespace std;

#define STRING 0
#define INT 1
#define FLOATING 2

#include "Table.h"

namespace Util {
// Fills the table with values from stdin
void FillTable(Table &table, string filename);

void PrintTable(Table &table);

}  // namespace Util
