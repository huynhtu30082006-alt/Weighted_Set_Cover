#pragma once
#include "problem.h"
#include "solution.h"
#include "algorithm.h"
#include "stats.h"
#include <iostream>
#include <vector>
using namespace std;

class Helper
{
    public:
        static Stats run_benchmark(const Problem& p, AlgoFunc algo, int runs);
        static vector<string> get_file_in_folder(const string& folder_name);
        static void write_result(vector<pair<string, Stats>>& v,  int n_vul, const string& file_name);
        static bool check_exist_file(const vector<string>& file);
};