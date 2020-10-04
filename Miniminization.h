#ifndef Minimization
#define Minimization

#include"State_machine.h"

bool cmp(std::pair<int, std::string> a,
         std::pair<int, std::string> b) {
    return a.second < b.second;
}

state_machine minimization(state_machine g) { //works only for full state machine
    std::vector<std::string> alphabet;
    for (int u = 0; u < g.n; u++) {
        for (int i = 0; i < g.g[u].size(); i++) {
            std::sort(g.g[u].begin(), g.g[u].end(), cmp);
        }
    }
    for (int i = 0; i < g.g[0].size(); i++) {
        alphabet.push_back(g.g[0][i].second);
    }

    std::vector<int> colours(g.n);
    std::vector<std::vector<int> > moves_colours(g.n, std::vector<int>(alphabet.size()));
    for (int u = 0; u < g.n; u++) {
        colours[u] = g.terminals[u];
    }
    while (true) {
        std::vector<int> new_colours(g.n);
        for (int u = 0; u < g.n; u++) {
            for (int i = 0; i < g.g[u].size(); i++) {
                int v = g.g[u][i].first;
                moves_colours[u][i] = colours[v];
            }
        }
        int cnt = 0;
        for (int u = 0; u < g.n; u++) {
            bool fl = false;
            for (int i = 0; i < u; i++) {
                if (colours[u] == colours[i] && moves_colours[u] == moves_colours[i]) {
                    new_colours[u] = new_colours[i];
                    fl = true;
                    break;
                }
            }
            if (!fl) {
                new_colours[u] = cnt++;
            }
        }
        if (new_colours == colours) {
            break;
        } else {
            colours = new_colours;
        }
    }
    std::vector<int> first_colour_entry(g.n, -1);
    for (int u = 0; u < g.n; u++) {
        if (first_colour_entry[colours[u]] == -1) {
            first_colour_entry[colours[u]] = u;
        }
    }
    state_machine ans = state_machine();
    ans.start = first_colour_entry[colours[g.start]];
    for (int u = 0; u < g.n; u++) {
        if (first_colour_entry[colours[u]] != u) {
            continue;
        }
        ans.add_vertex();
        ans.terminals[colours[u]] = g.terminals[u];
        for (int i = 0; i < alphabet.size(); i++) {
            ans.g[colours[u]].push_back(std::make_pair(colours[g.g[u][i].first], g.g[u][i].second));
        }
    }
    return ans;
}

#endif //Minimization