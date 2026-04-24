#include "helper.h"
#include <fstream>

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
            (double) (total_patch / runs)};
}

filesystem::path Helper::change_folder_of_file(const string& new_folder_path, const string& current_path_file_name)
{
    //convert to path type
    filesystem::path p = current_path_file_name;
    filesystem::path file_name = p.filename();
    file_name.replace_extension(".csv");
    filesystem::path new_path = filesystem::path(new_folder_path) / file_name;
    return new_path;
}

void Helper::write_result(vector<pair<string, Stats>>& v, int n_vul, const string& file_name)
{
    filesystem::path f = change_folder_of_file("result/", file_name);
    ofstream write(f);
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

    cout << "Your results have been successfully saved on: '" << f << "'"<< "\n";
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