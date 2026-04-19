#include "SA.h"
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <numeric>

using namespace std;

Solution SimulatedAnnealing::run(const Problem& p)
{
    auto start = chrono::high_resolution_clock::now();

    int m = p.get_n_patch();
    int n = p.get_n_vul();
    const auto& patches = p.get_patch();
    const auto& costs = p.get_cost();

    // Precompute mask để kiểm tra độ phủ siêu tốc bằng Bitwise
    vector<uint64_t> mask(m, 0);
    for (int i = 0; i < m; i++)
        for (int v : patches[i])
            mask[i] |= (1ULL << v);

    uint64_t FULL = (1ULL << n) - 1;

    // Khởi tạo ban đầu: Chọn tất cả các patch để đảm bảo an toàn
    vector<int> current(m, 1);
    double current_cost = 0;
    for (int i = 0; i < m; i++) current_cost += costs[i];

    vector<int> best_solution = current;
    double best_cost = current_cost;

    double T = 1000.0;          // Nhiệt độ ban đầu
    double cooling_rate = 0.995; // Tốc độ giảm nhiệt

    while (T > 0.1)
    {
        // Chọn ngẫu nhiên 1 bit để đảo ngược (thêm hoặc bớt 1 patch)
        int flip_idx = rand() % m;
        vector<int> neighbor = current;
        neighbor[flip_idx] = 1 - neighbor[flip_idx];

        uint64_t cover = 0;
        double neighbor_cost = 0;
        for (int i = 0; i < m; i++) {
            if (neighbor[i]) {
                cover |= mask[i];
                neighbor_cost += costs[i];
            }
        }

        // Phạt thật nặng (Penalty) nếu bộ vá này không lấp đủ toàn bộ lỗ hổng
        if (cover != FULL) neighbor_cost += 1e9;

        double delta = neighbor_cost - current_cost;

        // Chấp nhận nếu chi phí giảm, HOẶC chấp nhận với xác suất ngẫu nhiên nếu chi phí tăng (để tránh tối ưu cục bộ)
        if (delta < 0 || exp(-delta / T) >((double)rand() / RAND_MAX)) {
            current = neighbor;
            current_cost = neighbor_cost;

            // Cập nhật kỷ lục nếu tìm thấy giải pháp rẻ hơn và hợp lệ
            if (current_cost < best_cost && cover == FULL) {
                best_cost = current_cost;
                best_solution = current;
            }
        }
        T *= cooling_rate; // Giảm nhiệt độ
    }

    auto end = chrono::high_resolution_clock::now();
    int count = accumulate(best_solution.begin(), best_solution.end(), 0);

    return { best_solution, best_cost, count, chrono::duration<double>(end - start).count() };
}