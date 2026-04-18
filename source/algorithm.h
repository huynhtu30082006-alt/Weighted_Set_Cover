#pragma once
#include "solution.h"
#include "problem.h"
#include <vector>
#include <functional>
#include <cstdint>
using namespace std;

using AlgoFunc = function<Solution(const Problem& p)>;

class Algorithms
{
    public:

        //Brute Force Algorithm
        static Solution Brute_Force(const Problem& p);

        //Greedy Algorithm
        static Solution Greedy(const Problem& p);

        //ILP Algorithm
        // static double estimate_bound(const Problem& p, vector<int>& current);
        // static void DFS_ILP(const Problem& p, int i, vector<int>& current, vector<int>& best_solution, double& best_cost);
        // static bool is_covered(const Problem& p, const vector<int>& current);
        // static double current_cost(const Problem& p, const vector<int>& current);
        // static Solution ILP(const Problem& p);
        static void DFS_ILP(size_t i, uint64_t cover, double cost, const vector<uint64_t>& mask, const vector<int>& costs, uint64_t FULL, vector<int>& current, vector<int>& best_solution, double& best_cost);
        static Solution ILP(const Problem& p);
};