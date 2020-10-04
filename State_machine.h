#ifndef State_machine
#define State_machine

#include"My_bitset.h"
#include<vector>
#include<string>
#include<iostream>


class state_machine {
public:
    std::vector<std::vector<std::pair<int, std::string> > > g;
    std::vector<int> terminals;
    int n;
    int start;

    state_machine() {
        n = 0;
        start = -1;
    }

    state_machine& operator=(const state_machine &a) {
        g = a.g;
        terminals = a.terminals;
        n = a.n;
        start = a.start;
    }

    void read() {

        std::cin >> n;
        g.resize(n, std::vector<std::pair<int, std::string> >(0));
        terminals.resize(n);
        for (int i = 0; i < n; i++) { 
            g[i].resize(0);
            terminals[i] = 0;
        }

        std::cin >> start;
        start--;

        int m;
        std::cin >> m;

        for (int i = 0; i < m; i++) {
            int j;
            std::cin >> j;
            j--;
            terminals[j] = 1;
        }

        for (int j = 0; j < n; j++) {
            int u, m;
            std::cin >> u >> m;
            u--;
            for (int i = 0; i < m; i++) {
                int v;
                std::string s;
                std::cin >> v >> s;
                v--;
                if (s == "#") {
                    s = "";
                }
                g[u].emplace_back(v, s);
            }
        }
    }

    void print() {
        std::cout << n << "\n";
        std::cout << start + 1 << "\n";
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if (terminals[i]) {
                cnt++;
            }
        }
        std::cout << cnt << "\n";
        for (int i = 0; i < n; i++) {
            if (terminals[i]) {
                std::cout << i + 1 << " ";
            }
        }
        std::cout << "\n\n";
        for (int u = 0; u < n; u++) {
            std::cout << u + 1 << " " << g[u].size() << "\n";
            for (int i = 0; i < g[u].size(); i++) {
                std::cout << g[u][i].first + 1 << " " << 
                    (g[u][i].second == "" ? "#" : g[u][i].second) << "\n";
            }
            std::cout << "\n";
        }
    }

    void add_vertex(int cnt = 1) {
        n += cnt;
        g.resize(n, std::vector<std::pair<int, std::string> >(0));
        terminals.resize(n, 0);
    }

    bool has_word(std::string s) { // works only for machines with 1-letter moves without epsilon-moves
        my_bitset now(n);
        now.set(start, 1);
        for (int i = 0; i < s.size(); i++) {
            my_bitset next(s.size());
            int now_u = (now.get(0) ? 0 : now.next_true(0));
            while (now_u != -1) {
                for (int j = 0; j < g[now_u].size(); j++) {
                    if (g[now_u][j].second[0] == s[i]) {
                        next.set(g[now_u][j].first, 1);
                    }
                }
                now_u = now.next_true(now_u);
            }
            now = next;
        }
        int now_u = (now.get(0) ? 0 : now.next_true(0));
        while (now_u != -1) {
            if (terminals[now_u]) {
                return true;
            }
            now_u = now.next_true(now_u);
        }
        return false;
    }
};

std::istream& operator>>(std::istream& in, state_machine& g) {
    int n;
    g = state_machine();
    in >> n >> g.start;
    g.start--;
    g.add_vertex(n);
    int cnt_terminals;
    in >> cnt_terminals;
    for (int i = 0; i < cnt_terminals; i++) {
        int u;
        in >> u;
        u--;
        g.terminals[u] = 1;
    }
    int m;
    in >> m;
    for (int i = 0; i < m; i++) {
        int u, v;
        std::string s;
        in >> u >> v >> s;
        u--;
        v--;
        if (s == "#") {
            s = "";
        }
        g.g[u].push_back(std::make_pair(v, s));
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const state_machine& g) {
    
    out << g.n << "\n" << g.start + 1 << "\n";
    int cnt_terminals = 0;
    for (int u = 0; u < g.n; u++) {
        if (g.terminals[u]) {
            cnt_terminals++;
        }
    }
    out << cnt_terminals << "\n";
    for (int u = 0; u < g.n; u++) {
        if (g.terminals[u]) {
            out << u + 1 << " ";
        }
    }
    out << "\n";
    int m = 0;
    for (int u = 0; u < g.n; u++) {
        m += g.g[u].size();
    }
    out << m << "\n";
    for (int u = 0; u < g.n; u++) {
        for (int i = 0; i < g.g[u].size(); i++) {
            std::string move = g.g[u][i].second;
            if (move == "") {
                move = "#";
            }
            out << u + 1 << " " << g.g[u][i].first + 1 << " " << move << "\n";
        }
    }

    return out;
}

void dfs_for_equiv(const state_machine &a, const state_machine &b, int* used, int ua, int ub, int len) {
    std::cout << ua << "->" << a.terminals[ua] << " " << ub << "->" << b.terminals[ub] << std::endl;
    for (int i = 0; i < a.g[ua].size(); i++) {
        int va = a.g[ua][i].first;
        int vb = b.g[ub][i].first;
        if (a.g[ua][i].second != b.g[ub][i].second || *(used + a.n * va + vb)) {
            continue;
        }
        *(used + b.n * va + vb) = 1;
        if (len + 1 < a.n + b.n) {
            dfs_for_equiv(a, b, used, va, vb, len + 1);
        }
    }
}

bool are_equiv(state_machine a, state_machine b) { //works only for full DKA with same alphabet
    int used[a.n][b.n];
    for (int ua = 0; ua < a.n; ua++) {
        for (int ub = 0; ub < b.n; ub++) {
            used[ua][ub] = 0;
        }
    }
    used[a.start][b.start];
    dfs_for_equiv(a, b, (int*) used, a.start, b.start, 0);
    for (int ua = 0; ua < a.n; ua++) {
        for (int ub = 0; ub < b.n; ub++) {
            if (used[ua][ub] && a.terminals[ua] != b.terminals[ub]) {
                std::cout << "used" << ua << " " << ub << std::endl;
                return false;
            }
        }
    }
    return true;
}

#endif //State_machine