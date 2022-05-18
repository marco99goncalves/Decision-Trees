#pragma once
using namespace std;

#define CATEGORIC 0
#define NUMERIC 1

#include "Data.h"

namespace Util {
// Fills the table with values from stdin
void FillTable(Data &data, string filename);
void ReadData(Data &data, string filename);
void PrintTable(Data &data);
void GetClasses(Data &data);
void GetAttributes(Data &data);
void BuildAttributesTypes(Data &data);

unordered_map<string, unordered_map<string, int>> GetColumnProbabilities(int column, Data &data, set<int> &allowed_rows);
double GetEntropy(unordered_map<string, unordered_map<string, int>> &matrix, Data &data, int column);
double EntropyFormula(vector<int> &column, int total, int max_rows);
double EntropyFormula(unordered_map<string, int> &column, int total);
double GetSetEntropy(Data &data, set<int> &allowed_rows);
double GetInformationGain(Data &data, int column, double set_entropy, set<int> &allowed_rows);

void DiscretizeColumn(Data &data, int column);
void GetColumnRanges(Data &data, int column, vector<pair<float, float>> &ranges);
pair<float, float> ChooseRange(vector<pair<float, float>> &ranges, float value);
}  // namespace Util
