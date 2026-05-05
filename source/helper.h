#pragma once
#include "problem.h"
#include "solution.h"
#include "algorithm.h"
#include "stats.h"
#include <iostream>
#include <filesystem>
#include <vector>
using namespace std;

class Helper
{
    public:
        static vector<pair<string, Stats>> compute_average(const vector<vector<Stats>>& all_results, const vector<pair<string, AlgoFunc>>& algorithms);
        static Stats run_benchmark(const Problem& p, AlgoFunc algo, int runs);
        static vector<string> get_file_in_folder(const string& folder_name);
        static vector<string> get_folder_in_directory(const string& path);
        static void write_result(vector<pair<string, Stats>>& v,  int n_vul, const string& dataset_folder);
        static filesystem::path change_folder_of_file(const string& new_folder_path, const string& current_path_file_name);
        static bool check_exist_file_or_folder(const vector<string>& obj);
};