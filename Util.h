#pragma once
using namespace std;

#define CATEGORIC 0
#define NUMERIC 1

#include "Data.h"
#include "Node.h"

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

void UpdateAllowedRowsAndCols(Data &data, int column, string target_value, set<int> &allowed_rows, set<int> &allowed_cols);
pair<int, double> GetBestAttribute(Data &data, set<int> &allowed_cols, set<int> &allowed_rows, double set_entropy);

map<string, int> CountExampleClasses(Data &data, set<int> &allowed_rows);

void DiscretizeColumn(Data &data, int column);
void GetColumnRanges(Data &data, int column, vector<pair<float, float>> &ranges);
pair<float, float> ChooseRange(vector<pair<float, float>> &ranges, float value);
void PrintTabs(int n);

void ReadTestData(Data &training_data, Data &test_data, string filename);
void BuildAttributesTypesForTestData(Data &training_data, Data &test_data);
void DiscretizeColumnTestData(Data &training_data, Data &test_data, int column);
string GetRandomRange(Data &training_data, double value, int column);

void SearchTree(Node &node, vector<string> &row);
}  // namespace Util
