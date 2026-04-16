#include "interface.h"
#include <iostream>
#include "problem.h"
#include "stats.h"
#include "algorithm.h"
#include "helper.h"


void CLI::waitting_screen()
{
    cout << "-------------------------------------\n";
    cout << " Press any key to continue...\n";

    cin.ignore();  // clear buffer
    cin.get();     // wait for key press
}

void CLI::show_failed_interface(const string& center_message, const string& message)
{
    clear();

    cout << "=====================================\n";
    cout << "           "  << center_message << "   \n";
    cout << "=====================================\n\n";
    cout << message << endl;

    cout << "-------------------------------------\n";
    cout << " Press any key to continue...\n";
    cin.ignore(); 
    cin.get();     

}

void CLI::show_result_file_exist_on_folder(const vector<string>& file)
{
    clear();

    cout << "Available File:\n";
    for(int i = 0; i < file.size(); i ++)
    {
        cout << i+1 << ". " << file[i] << endl;
    }
}

CLI::Plot_Graph_State CLI::plot_graph(const string& type)
{
    vector<string> file = Helper::get_file_in_folder("result");
    if(type != "scalability")
    {
        if(!Helper::check_exist_file(file))
        {
            while(true)
            {
                show_result_file_exist_on_folder(file);
                cout << "Choose file: ";
                int choice;
                cin >> choice;

                if (choice < 1 || choice > file.size()) 
                {
                    cout << "Invalid choice!\n";
                    continue;
                }
                string cmd = "python graph/graph.py " + type + " result/" + file[choice - 1];
                system(cmd.c_str());
                break;
            }

        }
        else
        {
            cout << "No file found!!!\n";
        }
    }
    else
    {
        if(file.size() >= 3)
        {
            system("python graph/graph.py scalability result/");

        }
        else
        {
            cout << "Need at least 2 datasets to plot scalability!\n";
        }

    }
    return Plot_Graph_State::GRAPH_MENU;

}

void CLI::show_graph_menu()
{
    clear();

    cout << "=====================================\n";
    cout << "            PLOT GRAPH  \n";
    cout << "=====================================\n\n";
    cout << "What do you want to do?\n";
    //có thể thêm run single algorithm
    cout << "1. Draw Cost Graph Comparision\n";
    cout << "2. Draw Run Time Graph Comparision\n";
    cout << "3. Draw Scalability Graph Comparision\n";
    cout << "4. Draw Quality vs Optimal Graph Comparision\n";
    cout << "5. Exit Plot Graph Flow\n";
    cout << "-------------------------------------\n";
    cout << "Enter (1) to (4) to start: ";

}

CLI::Plot_Graph_State CLI::activity_graph()
{
    while(true)
    {
        show_graph_menu();

        //input
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice < 1 || choice > 5)
        {
            show_failed_interface("PLOT GRAPH", "Invalid Input!!! Try Again!");
            continue;

        }
        switch(choice)
        {
            case 1:
            {
                return Plot_Graph_State::COST_GRAPH;
            }
            case 2:
            {
                return Plot_Graph_State::RUNTIME_GRAPH;
            }
            case 3:
            {
                return Plot_Graph_State::SCALABILITY_GRAPH;
            }
            case 4:
            {
                return Plot_Graph_State::QUALITY_VS_OPTIMAL;
            }
            case 5:
            {
                return Plot_Graph_State::EXIT;
            }
        }
    }

}

CLI::State CLI::control_plot_graph()
{
    Plot_Graph_State pgs = Plot_Graph_State::GRAPH_MENU;
    while(pgs != Plot_Graph_State::EXIT)
    {
        switch(pgs)
        {
            case(Plot_Graph_State::GRAPH_MENU):
            {
                pgs = activity_graph();
                break;
            }
            case(Plot_Graph_State::COST_GRAPH):
            {
                pgs = plot_graph("cost");
                break;
            }
            case(Plot_Graph_State::RUNTIME_GRAPH):
            {
                pgs = plot_graph("runtime");
                break;
            }
            case(Plot_Graph_State::SCALABILITY_GRAPH):
            {
                pgs = plot_graph("scalability");
                break;
            }
            case(Plot_Graph_State::QUALITY_VS_OPTIMAL):
            {
                pgs = plot_graph("quality");
                break;
            }
            
        }

    }
    return State::MAIN_MENU;

}

void CLI::show_patch_selected(Problem& p, Solution& s)
{
    vector<vector<int>> patch = p.get_patch();

    for(int i = 0; i < s.patch_selected.size(); i ++)
    {
        cout << "[";
        for(int j = 0; j < patch[i].size(); j++)
        {
           if(j == (patch[i].size() - 1))
           {
                cout << patch[i][j];
           }
           cout << patch[i][j] << ","; 
        }
        cout << "]\n";

    }

}

CLI::Algorithm_Type CLI::run_algo(const string& file_name, AlgoFunc algo, const string& algo_name)
{
    Problem p(file_name);
    Solution s = algo(p);

    clear();

    cout << "=====================================\n";
    cout << "            " << algo_name << "      \n";
    cout << "=====================================\n\n";
    cout << "Patches Selected: ";
    show_patch_selected(p, s);
    cout << "Cost: " << s.total_cost << "\n";
    cout << "Run Time: " << s.runtime << "s\n";

    return Algorithm_Type::ALGORITHM_MENU;
}

void CLI::show_algo_menu()
{
    clear();

    cout << "=====================================\n";
    cout << "            ALGORITHM  \n";
    cout << "=====================================\n\n";
    cout << "What do you want to do? (Result incl\n";
    cout << "1. Brute-Force\n";
    cout << "2. Greedy\n";
    cout << "3. Genetic Algorithm (GA)\n";
    cout << "4. Simulated Annealing (SA)\n";
    cout << "5. Integer Linear Programming (ILP)\n";
    cout << "6. Exit\n";
    cout << "-------------------------------------\n";
    cout << "Note: The results of each algorithm will be printed out, including: the optimal patches selected, the total cost for those patches, and the algorithm's execution time!\n";
    cout << "Enter (1) to (5) to start: ";

}
CLI::Algorithm_Type CLI::activity_algo()
{
    while(true)
    {
        show_algo_menu();

        //input
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice < 1 || choice > 6)
        {
            show_failed_interface("ALGORITHM", "Invalid Input!!! Try Again!");
            continue;

        }
        switch(choice)
        {
            case 1:
            {
                return Algorithm_Type::BRUTE_FORCE;
            }
            case 2:
            {
                return Algorithm_Type::GREEDY;
            }
            case 3:
            {
                return Algorithm_Type::GA;
            }
            case 4:
            {
                return Algorithm_Type::SA;
            }
            case 5:
            {
                return Algorithm_Type::ILP;
            }
            case 6:
            {
                return Algorithm_Type::EXIT;
            }
        }
    }

}

string CLI::choose_file()
{
    vector<string> file = Helper::get_file_in_folder("result"); 

    if(!Helper::check_exist_file(file))
    {
        while(true)
        {
            show_result_file_exist_on_folder(file);
            cout << "Choose file: ";

            int choice;
            cin >> choice;

            if (choice < 1 || choice > file.size()) 
            {
                cout << "Invalid choice!\n";
                continue;
            }
            return file[choice - 1];
        }

    }
    return "";

}

CLI::State CLI::control_run_single_algo()
{
    Algorithm_Type at = CLI::Algorithm_Type::ALGORITHM_MENU;
    string file_name;
    while(at != Algorithm_Type::EXIT)
    {
        switch(at)
        {
            case(Algorithm_Type::ALGORITHM_MENU):
            {
                file_name = choose_file();
                if(file_name.empty())
                {
                    cout << "No file found! Try Again!!!";
                    cout << "-------------------------------------\n";
                    cout << " Press any key to continue...\n";
                    cin.ignore();  // clear buffer
                    cin.get();     // wait for key press
                    return State::MAIN_MENU;
                }
                at = activity_algo();
                break;
            }
            case(Algorithm_Type::BRUTE_FORCE):
            {
                at = run_algo(file_name, Algorithms::Brute_Force, "BRUTE FORCE");
                break;
            }
            case(Algorithm_Type::GREEDY):
            {
                at = run_algo(file_name, Algorithms::Greedy, "GREEDY");
                break;
            }
            // case(Algorithm_Type::GA):
            // {
            //     // at = plot_graph("scalability");
            //     break;
            // }
            // case(Algorithm_Type::SA):
            // {
            //     // at = plot_graph("quality");
            //     break;
            // }
            case (Algorithm_Type::ILP):
            {
                at = run_algo(file_name, Algorithms::ILP, "ILP");
                break;
            }
            
        }

    }
    return State::MAIN_MENU;
    

}

CLI::State CLI::control_run_all_algo()
{
    clear();

    int n;
    cout << "Enter the number of time to run: ";
    cin >> n;
    Problem p("input.txt");

    string file_name = "Result_" + to_string(p.get_n_vul()) + ".csv";
    vector<pair<string, Stats>> results;

    vector<pair<string, AlgoFunc>> algorithms = {
        {"ILP", Algorithms::ILP}
    };

    for(auto& [name, algo] : algorithms)
    {
        Stats s = Helper::run_benchmark(p, algo, n);

        cout << "=== " << name << " ===\n";
        cout << "Avg Cost: " << s.avg_cost << "\n";
        cout << "Avg Time: " << s.avg_time << "\n";
        cout << "Avg Patch: " << s.avg_patch << "\n\n";

        results.push_back({name, s});

    }
    Helper::write_result(results, p.get_n_vul(), file_name);
    waitting_screen();
    return State::MAIN_MENU;
}

void CLI::show_start_interface()
{
    clear();

    cout << "=====================================\n";
    cout << "     WEIGHTED SET COVER CLI \n";
    cout << "=====================================\n\n";
    cout << "What do you want to do?\n";
    cout << "1. Run Single Algorithm\n";
    cout << "2. Run All Algorithm\n";
    cout << "3. I already have the result file! Plot Graph?\n";
    cout << "4. Exit Program!\n";
    cout << "-------------------------------------\n";
    cout << "Enter (1) to (3) to start: ";

}

CLI::State CLI::activity()
{
    while(true)
    {
        show_start_interface();

        //input
        int choose;
        cin >> choose;
        cin.ignore();

        if (choose < 1 || choose > 3)
        {
            show_failed_interface("WEIGHTED SET COVER CLI", "Invalid Input!!! Try Again!");
            continue;

        }
        switch(choose)
        {
            case 1:
            {
                return State::RUN_SINGLE_ALGO;
            }
            case 2:
            {
                return State::RUN_ALL_ALGO;
            }
            case 3:
            {
                return State::PLOT_GRAPH;
            }
            case 4:
            {
                return State::EXIT;
            }

        }
    }

}

void CLI::main_control()
{
    State s = State::MAIN_MENU;
    while(s != State::EXIT)
    {
        switch(s)
        {
            case(State::MAIN_MENU):
            {
                s = activity();
                break;
            }
            case (State::RUN_SINGLE_ALGO):
            {
                s = control_run_single_algo();
                break;
            }
            case(State::RUN_ALL_ALGO):
            {
                s = control_run_all_algo();
                break;
            }
            case(State::PLOT_GRAPH):
            {
                s = control_plot_graph();
                break;

            }
        }

    };
    
}

void CLI::clear()
{
    system("cls");
}