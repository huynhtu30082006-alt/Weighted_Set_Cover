#pragma once

#include <vector>
#include "problem.h"
#include "algorithm.h"
using namespace std;
class CLI
{
    private:
        enum class State
        {
            MAIN_MENU,
            RUN_SINGLE_ALGO,
            RUN_ALL_ALGO,
            PLOT_GRAPH,
            EXIT
        };
        enum class Algorithm_Type
        {
            ALGORITHM_MENU,
            BRUTE_FORCE,
            GREEDY,
            GA,
            SA,
            ILP,
            EXIT
        };
        enum class Plot_Graph_State
        {
            GRAPH_MENU,
            COST_GRAPH,
            RUNTIME_GRAPH,
            SCALABILITY_GRAPH,
            QUALITY_VS_OPTIMAL,
            EXIT
        };

    public:

        //graph
        void show_graph_menu();
        Plot_Graph_State plot_graph(const string& type);
        void show_result_file_exist_on_folder(const vector<string>& file);
        Plot_Graph_State activity_graph();
        State control_plot_graph();

        //run single algo
        string get_file(const string& folder_name);
        void show_patch_selected(const Problem& p, const Solution& s);
        Algorithm_Type run_algo(const string& file_name, AlgoFunc algo, const string& algo_name);
        void show_algo_menu();
        Algorithm_Type activity_algo();
        State control_run_single_algo();

        //run all algo
        State control_run_all_algo();

        //start 
        void show_start_interface();
        State activity();
        void main_control();

        //helper
        void clear();
        void waitting_screen();
        void show_failed_interface(const string& center_message, const string& message);


        void show_result_after_create_obj(const Problem& p);
};