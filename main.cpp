#include <iostream>
#include <vector>
#include <stack>
#include <thread>
#include <future>

using namespace std;

// Proste DFS przy użyciu stosu
vector<int> przeszukajDFS(const vector<vector<int>>& g, int start) {
    vector<bool> odw(g.size(), false);
    vector<int> kolejnosc;
    stack<int> stos;
    stos.push(start);

    while (!stos.empty()) {
        int v = stos.top();
        stos.pop();
        if (!odw[v]) {
            odw[v] = true;
            kolejnosc.push_back(v);
            for (int u : g[v]) {
                if (!odw[u])
                    stos.push(u);
            }
        }
    }
    return kolejnosc;
}

int main() {
    int ile;
    cout << "Podaj liczbe grafow: ";
    cin >> ile;

    vector<thread> watki;
    vector<future<vector<int>>> futury;

    for (int i = 0; i < ile; i++) {
        int n, m;
        cout << "Graf " << i+1 << " - wierzcholki i krawedzie: ";
        cin >> n >> m;
        vector<vector<int>> graf(n);

        cout << "Podaj krawedzie (u v):\n";
        for (int j = 0; j < m; j++) {
            int u, v;
            cin >> u >> v;
            graf[u].push_back(v);
        }

        packaged_task<vector<int>()> zadanie([graf]() {
            return przeszukajDFS(graf, 0);
        });
        futury.push_back(zadanie.get_future());
        watki.emplace_back(move(zadanie));
    }

    for (size_t i = 0; i < watki.size(); i++) {
        watki[i].join();
        vector<int> wynik = futury[i].get();
        cout << "DFS grafu " << i+1 << ": ";
        for (int v : wynik)
            cout << v << " ";
        cout << "\n";
    }

    return 0;
}
