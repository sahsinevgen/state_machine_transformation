#ifndef Make_one_letter_moves
#define Make_one_letter_moves

#include"State_machine.h"
#include<algorithm>

void dfs(const state_machine &g, 
         std::vector<int> &used,
         int used_colour,
         int root, 
         int u,
         state_machine &new_g) {

    if (g.terminals[u]) {
        new_g.terminals[root] = 1;
    }
    for (int i = 0; i < g.g[u].size(); i++) {
        int v = g.g[u][i].first;
        std::string s = g.g[u][i].second;
        if (s == "") {
            if (used[v] != used_colour) {
                used[v] = used_colour;
                dfs(g, used, used_colour, root, v, new_g);
            }
        } else {
            if (u != root) {
                new_g.g[root].emplace_back(g.g[u][i]);
            }
        }
    }
}

state_machine make_one_letter_moves(const state_machine &g) { // + removing multiple edges
    state_machine ans = g;
    std::vector<int> used(g.n, -1);
    for (int u = 0; u < g.n; u++) {
        used[u] = u;
        dfs(g, used, u, u, u, ans);
    }
    for (int u = 0; u < ans.n; u++) {
        for (int i = 0; i < ans.g[u].size(); ) {
            if (ans.g[u][i].second == "") {
                ans.g[u].erase(ans.g[u].begin() + i);
            } else {
                i++;
            }
        }
    }
    /*for (int u = 0; u < g.n; u++) {
        used[u] = -1;
    }
    dfs2(g, used, g.start, ans);
    for (int u = 0; u < g.n; u++) {
        dfs1(g, used, u, u, u, ans);
    }*/
    for (int u = 0; u < ans.n; u++) {
        sort(ans.g[u].begin(), ans.g[u].end());
        ans.g[u].erase(std::unique(ans.g[u].begin(), ans.g[u].end()), ans.g[u].end());
    }
    int cnt = 0;
    for (int u = 0; u < ans.n; u++) {
        for (int i = 0; i < ans.g[u].size(); i++) {
            int v = ans.g[u][i].first;
            std::string s = ans.g[u][i].second;
            if (s.length() > 1) {
                ans.g[u][i] = std::make_pair(ans.n, s[0]);
                ans.add_vertex();
                for (int j = 1; j < s.size() - 1; j++) {
                    ans.g[ans.n - 1].push_back({});
                    ans.g[ans.n - 1][0] = std::make_pair(ans.n, s[j]);
                    ans.add_vertex();
                }
                ans.g[ans.n - 1].push_back({});
                ans.g[ans.n - 1][0] = std::make_pair(v, s.back());
            }
        }
    }
    return ans;
}

#endif //Make_one_letter_moves