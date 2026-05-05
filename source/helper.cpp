#include "helper.h"
#include <fstream>

vector<pair<string, Stats>> Helper::compute_average(const vector<vector<Stats>>& all_results, const vector<pair<string, AlgoFunc>>& algorithms)
{
    vector<pair<string, Stats>> final_results;

    for (size_t j = 0; j < algorithms.size(); j++)
    {
        const string& name = algorithms[j].first;
        const vector<Stats>& vec = all_results[j];

        double sum_cost = 0;
        double sum_time = 0;

        for (const Stats& s : vec)
        {
            sum_cost += s.avg_cost;
            sum_time += s.avg_time;
        }
        int count = vec.size();

        Stats avg;
        avg.avg_cost = sum_cost / count;
        avg.avg_time = sum_time / count;
        final_results.push_back({name, avg});
    }

    return final_results;
}

Stats Helper::run_benchmark(const Problem& p, AlgoFunc algo, int runs)
{
    double total_cost = 0;
    double total_time = 0;

    for(int i = 0; i < runs; i++)
    {
        Solution s = algo(p);
        total_cost += s.total_cost;
        total_time += s.runtime;
        
    }

    return {total_cost / runs,
            total_time / runs,
    };
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

void Helper::write_result(vector<pair<string, Stats>>& v, int n_vul, const string& dataset_folder)
{
    string dataset_name = filesystem::path(dataset_folder).filename().string();

    filesystem::path output_path = "result/" + dataset_name + ".csv";
    ofstream write(output_path);
    if(!write)
    {
        cerr << "Cannot open output file!\n";
        return;

    }
    write << "Algorithm,N_Vul,AvgCost,AvgTime\n";

    for(auto& [name, s] : v)
    {
        write << name << "," << n_vul << "," << s.avg_cost << "," << s.avg_time << endl;
    }
    write.close();

    cout << "Your results have been successfully saved on: '" << output_path << "'"<< "\n";
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

vector<string> Helper::get_folder_in_directory(const string& path)
{
    vector<string> folders;

    for (const auto& entry : filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            folders.push_back(entry.path().string());
        }
    }
    return folders;

}

bool Helper::check_exist_file_or_folder(const vector<string>& obj)
{
    return obj.empty();

}


