#include "problem.h"
#include <fstream>

Problem::Problem(const string& file_name)
{
    ifstream fin(file_name);

    if (!fin) 
    {
        cerr << "Cannot open file!\n";
        exit(1);
    }

    // read n_vul, n_patch
    fin >> n_vul >> n_patch;

    // read cost
    cost.resize(n_patch);
    for (int i = 0; i < n_patch; i++) 
    {
        fin >> cost[i];
    }

    // read patch
    patch.resize(n_patch);

    for (int i = 0; i < n_patch; i++) 
    {
        int k;
        fin >> k;

        patch[i].resize(k);
        for (int j = 0; j < k; j++) 
        {
            fin >> patch[i][j];
        }
    }

    fin.close();

}
