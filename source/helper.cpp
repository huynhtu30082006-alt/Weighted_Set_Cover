#include "helper.h"
#include <fstream>
#include <filesystem>

Stats Helper::run_benchmark(const Problem& p, AlgoFunc algo, int runs)
{
    double total_cost = 0;
    double total_time = 0;
    int total_patch = 0;

    for(int i = 0; i < runs; i++)
    {
        Solution s = algo(p);

        total_cost += s.total_cost;
        total_time += s.runtime;
        total_patch += s.num_patches;
    }

    return {total_cost / runs,
            total_time / runs,
            (double) total_patch / runs};
}

void Helper::write_result(vector<pair<string, Stats>>& v, int n_vul, const string& file_name)
{
    ofstream write(file_name);
    if(!write)
    {
        cerr << "Cannot open output file!\n";
        return;

    }
    write << "Algorithm,N_Vul,AvgCost,AvgTime,AvgPatch\n";

    for(auto& [name, s] : v)
    {
        write << name << "," << n_vul << "," << s.avg_cost << "," << s.avg_time << "," << s.avg_patch << endl;
    }
    write.close();
    cout << "The results file were saved successfully.\n";
}

vector<string> Helper::get_file_in_folder(const string& folder_name)
{
    vector<string> file;

    for(const auto& entry : filesystem::directory_iterator(folder_name))
    {
        if(entry.is_regular_file())
        {
            file.push_back(entry.path().string());
        }

    }
    return file;
}

bool Helper::check_exist_file_on_folder(const vector<string>& file)
{
    return file.empty();

}