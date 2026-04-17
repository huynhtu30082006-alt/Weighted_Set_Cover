#include "algorithm.h"
#include <chrono>
#include <algorithm>

Solution Algorithms::Brute_Force(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    cout << "On Brute force func\n";

    int m = p.get_n_patch();
    cout << "m = " << m << endl;

    if (m <= 0)
        return {{}, 0.0, 0, 0.0};

    if (m >= 60)
    {
        throw std::runtime_error("Brute force not feasible: The number of Patch too large");
    }

    double best_cost = 1e18;
    vector<int> best_solution(m, 0);

    long long total = (1LL << m);

    for (long long mask = 1; mask < total; mask++)
    {
        vector<int> current(m, 0);

        // convert mask → vector
        for (int i = 0; i < m; i++)
        {
            if (mask & (1LL << i))
            {
                current[i] = 1;
            }
        }

        // check cover
        cout << "before is_covered\n";
        if (is_covered(p, current))
        {
            cout << "after is_covered\n";
            double cost = current_cost(p, current);
            cout << "after cost\n";
            if (cost < best_cost)
            {
                best_cost = cost;
                best_solution = current;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double runtime = chrono::duration<double>(end - start).count();

    int count = 0;
    for (int x : best_solution)
    {
        count += x;
    }

    cout << "Done Brute force func\n";

    return {best_solution, best_cost, count, runtime};
}

Solution Algorithms::Greedy(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();

    vector<int> current(m, 0);          // solution vector
    vector<bool> covered(n, false);     // trạng thái coverage

    while (true)
    {
        int best_idx = -1;
        double best_ratio = 1e18;

        for (int i = 0; i < m; i++)
        {
            if (current[i] == 1) continue;

            int gain = 0;

            for (int v : p.get_patch()[i])
            {
                if (!covered[v])
                {
                    gain++;
                }
            }

            if (gain == 0) continue;

            double ratio = p.get_cost()[i] / gain;

            if (ratio < best_ratio)
            {
                best_ratio = ratio;
                best_idx = i;
            }
        }

        if (best_idx == -1) break;

        // chọn patch tốt nhất
        current[best_idx] = 1;

        // update covered
        for (int v : p.get_patch()[best_idx])
        {
            covered[v] = true;
        }

        // nếu cover hết thì dừng
        if (all_of(covered.begin(), covered.end(), [](bool x){ return x; }))
        {
            break;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    double runtime = chrono::duration<double>(end - start).count();

    double cost = current_cost(p, current);

    int count = 0;
    for (int x : current)
    {
        count += x;
    }

    return {current, cost, count, runtime};
}

bool Algorithms::is_covered(const Problem& p, vector<int>& current)
{
    vector<bool> covered(p.get_n_vul(), false); //create 1 vector with n vul and start value is false

    for(int i = 0; i < p.get_n_patch(); i++)
    {
        if(current[i] == 1)
        {
            cout << "V: ";
            for(int v : p.get_patch()[i]) //v return the all number in patch
            {
                cout << v << " ";
                covered[v] = true; 
            }
        }
    }

    for(bool x : covered)
    {
        if(x == false)
        {
            return false;
        }
    }
    return true;

}

double Algorithms::current_cost(const Problem& p, const vector<int>& current) 
{
    double cost = 0;
    for (int i = 0; i < p.get_n_patch(); i++)
    {
        if (current[i]) cost += p.get_cost()[i];
    }
    return cost;
}

double Algorithms::estimate_bound(const Problem& p, vector<int>& current)
{
    vector<bool> covered(p.get_n_vul(), false);

    for(int i = 0; i < p.get_n_patch(); i++)
    {
        if(current[i] == true)
        {
            for(int v : p.get_patch()[i])
            {
                covered[v] = true;
            }
        }
    }

    vector<pair<double,int>> ratio;

    for(int i = 0; i < p.get_n_patch(); i++)
    {
        if(current[i] == true)
        {
            continue;
        }

        int gain = 0;
        for(int v : p.get_patch()[i])
        {
            if(!covered[v])
            {
                gain ++;
            }
        }
        if(gain > 0)
        {
            ratio.push_back({p.get_cost()[i] / gain, i}); //greedy
        }
    }

    sort(ratio.begin(), ratio.end()); //first choose patch has ratio lower 

    double cost_est = 0;
    vector<bool> temp = covered;

    for(auto [r, i] : ratio)
    {
        int gain = 0;
        for(int v : p.get_patch()[i])
        {
            if(!temp[v])
            {
                gain++;
            }
        }
        if(gain == 0)
        {
            continue;
        }
        cost_est += p.get_cost()[i];

        for(int v : p.get_patch()[i])
        {
            temp[v] = true;
        }
        if(all_of(temp.begin(), temp.end(), [](bool x){ return x; }))
        {
            break;
        }
    }
    return cost_est;
}

void Algorithms::DFS_ILP(const Problem& p, int i, vector<int>& current, vector<int>& best_solution, double& best_cost)
{
    if(i == p.get_n_patch())
    {
        if(is_covered(p, current) == true)
        {
            double cost = current_cost(p, current);
            if(cost < best_cost)
            {
                best_cost = cost;
                best_solution = current;
            }
        }
        return;

    }

    double cost_now = current_cost(p, current);
    double bound = cost_now + estimate_bound(p, current);

    if(bound >= best_cost)
    {
        return; //prune
    }

    //choose
    current[i] = 1;
    DFS_ILP(p, i+1, current, best_solution, best_cost);

    //not choose
    current[i] = 0;
    DFS_ILP(p, i+1, current, best_solution, best_cost);

}

Solution Algorithms::ILP(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    vector<int> current(p.get_n_patch(), 0); //patch have choose in search process 
    vector <int> best_solution; //patch have choose
    double best_cost = 1e18; //simulate infinitive

    DFS_ILP(p, 0, current, best_solution, best_cost);

    auto end = chrono::high_resolution_clock::now();
    double runtime = chrono::duration<double>(end-start).count();

    int count = 0;
    for(int x : best_solution)
    {
        count += x;
    }

    return {best_solution, best_cost, count, runtime};
}
