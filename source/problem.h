#pragma once
#include <vector>
#include <iostream>
using namespace std;

class Problem
{
    private:
        int n_vul;
        int n_patch;
        vector<vector<int>> patch;
        vector<int> cost;  
    
    public:
        Problem(const string& file_name);
        
        //getter
        int get_n_patch() const {return n_patch;}
        int get_n_vul() const {return n_vul;}
        const vector<vector<int>>& get_patch() const {return patch;}
        const vector<int>& get_cost() const {return cost;}

};