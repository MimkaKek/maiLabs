#include <iostream>
#include <map>
#include <stack>
#include <vector>
typedef std::vector<std::vector<std::vector<long long>>> TTable;

int main() {
    std::ios::sync_with_stdio(false);
    int num, capacity;
    std::cin >> num >> capacity;
    if (num == 0) {
        std::cout << 0 << '\n';
        return 0;
    }
    std::vector<int> weights(num);
    std::vector<int> costs(num);
    for (int it = 0; it < num; ++it) {
        std::cin >> weights[it] >> costs[it];
    }
    TTable dynamic(num + 1);
    for (int it = 0; it <= num; ++it) {
        dynamic[it].assign(capacity + 1, std::vector<long long>(it + 1, -1));
        for (auto& j_it : dynamic[it]) j_it[0] = 0;  // picked 0 = 0
    }

    for (int viewed = 0; viewed < num; ++viewed) {
        for (int weight = 0; weight <= capacity; ++weight) {
            for (int picked = 0; picked <= viewed; ++picked) {
                int sum_weight = weight + weights[viewed];
                if (sum_weight <= capacity) {
                    if (dynamic[viewed][weight][picked] != -1) {
                        dynamic[viewed + 1][sum_weight][picked + 1] = dynamic[viewed][weight][picked] + costs[viewed];
                    }
                }
                dynamic[viewed + 1][weight][picked] = std::max(dynamic[viewed + 1][weight][picked],dynamic[viewed][weight][picked]);
            }
        }
    }

    long long ans = 0, mark = -1;
    for (int it = 0; it <= num; ++it) {
        long long tmp = it * dynamic[num][capacity][it];
        if (tmp > ans) {
            mark = it;
            ans = tmp;
        }
    }
    std::cout << ans << '\n';

    std::stack<int> path;
    if (mark != -1) {
        int item = num, weight = capacity;
        while (item) {
            if (dynamic[item][weight][mark] == 0) {
                break;
            }
            if (mark == item || dynamic[item][weight][mark] != dynamic[item - 1][weight][mark]) {
                --mark;
                weight -= weights[item - 1];
                path.push(item);
            }
            --item;
        }
    }

    if (!path.empty()) {
        while (!path.empty()) {
            std::cout << path.top();
            path.pop();
            if (!path.empty()) std::cout << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
