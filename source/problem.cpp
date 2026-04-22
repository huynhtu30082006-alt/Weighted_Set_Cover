#include "problem.h"
#include <fstream>
#include <sstream>

Problem::Problem(const string& file_name)
{
    ifstream fin(file_name);

    if (!fin) 
    {
        cerr << "Cannot open file!\n";
        exit(1);
    }

    fin >> n_vul >> n_patch;

    cost.resize(n_patch);
    for (int i = 0; i < n_patch; i++) 
    {
        fin >> cost[i];
    }

    fin.ignore();

    patch.clear();
    patch_name.clear();

    string line;
    while(getline(fin, line))
    {
        if(line.empty()) continue;

        int pos = line.find('|');

        string name = line.substr(0, pos);
        string rest = line.substr(pos + 1);

        stringstream ss(rest);

        int k;
        ss >> k;

        vector<int> covers;
        for(int i = 0; i < k; i++)
        {
            int x;
            ss >> x;
            covers.push_back(x);
        }

        patch.push_back(covers);
        patch_name.push_back(name);
    }
    fin.close();
}