#include <iostream>
#include <fstream>
using namespace std;



int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int count_v = 0, count_e = 0, v1 = 0, v2 = 0;
    cin >> count_v >> count_e;
    int adjacency_matrix[count_v][count_v];
    for (int i = 0; i < count_v; i++) {
        for (int j = 0; j < count_v; j++) {
            adjacency_matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < count_e; i++) {
        cin >> v1 >> v2;
        adjacency_matrix[v1 - 1][v2 - 1] = 1;
    }

    return 0;
}
