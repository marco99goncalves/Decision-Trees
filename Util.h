#pragma once
using namespace std;

#define STRING 0
#define INTEGER 1
#define FLOAT 2

#include "Data.h"

namespace Util {
// Fills the table with values from stdin
void FillTable(Data &data, string filename);
void ReadData(Data &data, string filename);
void PrintTable(Data &data);
void GetClasses(Data &data);
void GetAttributes(Data &data);
void BuildAttributesTypes(Data &data);

unordered_map<string, unordered_map<string, int>> GetColumnProbabilities(int column, Data &data);
double GetEntropy(unordered_map<string, unordered_map<string, int>> &matrix, Data &data, int column);
double EntropyFormula(vector<int> &column, int total, int max_rows);
double EntropyFormula(unordered_map<string, int> &column, int total);
double GetSetEntropy(Data &data);
double GetInformationGain(Data &data, int column, double set_entropy);
}  // namespace Util
