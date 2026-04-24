#pragma once
#include "solution.h"
#include "problem.h"
#include <vector>
#include <functional>
#include <cstdint>
#include <chrono>
using namespace std;

using AlgoFunc = function<Solution(const Problem& p)>;

class Algorithms
{
    public:

        //Brute Force Algorithm
        static Solution Brute_Force(const Problem& p);

        //Greedy Algorithm
        static Solution Greedy(const Problem& p);

        //GA
        static Solution GA(const Problem& p);

        //SA
        static Solution SA(const Problem& p);
        
        //ILP
        static void DFS_ILP(size_t i, uint64_t cover, double cost, const vector<uint64_t>& mask, const vector<int>& costs, uint64_t FULL, vector<int>& current, vector<int>& best_solution, double& best_cost, chrono::high_resolution_clock::time_point start);
        static Solution ILP(const Problem& p);

};