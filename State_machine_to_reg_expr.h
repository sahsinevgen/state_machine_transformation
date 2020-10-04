#ifndef State_machine_to_reg_expr
#define State_machine_to_reg_expr

#include"State_machine.h"
#include<algorithm>

std::string kleene_closure(std::string s) {
    return "(" + s + ")*";
}

std::string add_parentheses(std::string s) {
    int balance = 0;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '(') {
            balance++;
        } 
        if (s[i] == ')') {
            balance--;
        } 
        if (s[i] == '+' && balance == 0) {
            return "(" + s + ")";
        } 
    }
    return s;
}

std::string state_machine_to_reg_expr(state_machine g) {
    g.add_vertex();
    g.terminals[g.n - 1] = 1;
    for (int u = 0; u < g.n - 1; u++) {
        if (g.terminals[u]) {
            g.terminals[u] = 0;
            g.g[u].push_back(std::make_pair(g.n - 1, ""));
        }
    }
    std::vector<int> used_vertex(g.n, 0);
    int cnt_used = 0;
    while (g.n - cnt_used > 2) {
        for (int u = 0; u < g.n; u++) {
            if (used_vertex[u]) {
                continue;
            }
            int diff_edges_cnt = 0;
            std::sort(g.g[u].begin(), g.g[u].end());
            for (int i = 0; i < g.g[u].size();) {
                std::pair<int, std::string> new_edge(g.g[u][i]);
                i++;
                for (; i < g.g[u].size() && g.g[u][i].first == g.g[u][i - 1].first; i++) {
                    if (used_vertex[g.g[u][i].first]) {
                        continue;
                    }
                    new_edge.second += "+" + g.g[u][i].second;
                }
                g.g[u][diff_edges_cnt++] = new_edge;
            }
            g.g[u].resize(diff_edges_cnt);
        }
        int now = -1;
        for (int u = 0; u < g.n; u++) {
            if (u != g.start && !used_vertex[u]) {
                now = u;
                break;
            }
        }
        bool has_loop = false;
        std::string loop = "";
        std::vector<std::pair<int, std::string> > in, out;

        for (int u = 0; u < g.n; u++) {
            if (used_vertex[u]) {
                continue;
            }
            if (u == now) {
                for (int i = 0; i < g.g[u].size(); i++) {
                    int v = g.g[u][i].first;
                    if (used_vertex[v]) {
                        continue;
                    }
                    std::string s = g.g[u][i].second;
                    if (u == v) {
                        has_loop = true;
                        loop = s;
                    } else {
                        out.push_back(std::make_pair(v, s));
                    }
                }
            } else {
                for (int i = 0; i < g.g[u].size(); i++) {
                    int v = g.g[u][i].first;
                    if (used_vertex[v]) {
                        continue;
                    }
                    std::string s = g.g[u][i].second;
                    if (v == now) {
                        in.push_back(std::make_pair(u, s));
                        g.g[u].erase(g.g[u].begin() + i);
                        i--;
                    }
                }
           }
        }
        if (has_loop) {
            loop = kleene_closure(loop);
        }
        for (int i = 0; i < in.size(); i++) {
            for (int j = 0; j < out.size(); j++) {
                std::string s = add_parentheses(in[i].second);
                s += loop;
                s += add_parentheses(out[j].second);
                g.g[in[i].first].push_back(std::make_pair(out[j].first, s));
            }
        }
        used_vertex[now] = 1;
        cnt_used++;
    }
    for (int u = 0; u < g.n; u++) {
        if (used_vertex[u]) {
            continue;
        }
        int diff_edges_cnt = 0;
        std::sort(g.g[u].begin(), g.g[u].end());
        for (int i = 0; i < g.g[u].size();) {
            std::pair<int, std::string> new_edge(g.g[u][i]);
            i++;
            for (; i < g.g[u].size() && g.g[u][i].first == g.g[u][i - 1].first; i++) {
                if (used_vertex[g.g[u][i].first]) {
                    continue;
                }
                new_edge.second += "+" + g.g[u][i].second;
            }
            g.g[u][diff_edges_cnt++] = new_edge;
        }
        g.g[u].resize(diff_edges_cnt);
    }
    std::vector<std::pair<bool, std::string> > var(4, std::make_pair(0, ""));
    for (int j = 0; j < 2; j++) {
        int u;
        if (j == 0) {
            u = g.start;
        } else {
            u = g.n - 1;
        }
        for (int i = 0; i < g.g[u].size(); i++) {
            int v = g.g[u][i].first;
            std::pair<int, int> vertexes(u, v);
            if (vertexes == std::make_pair(g.start, g.start)) {
                var[0] = std::make_pair(1, g.g[u][i].second);
            } else if (vertexes == std::make_pair(g.start, g.n - 1)) {
                var[1] = std::make_pair(1, g.g[u][i].second);
            } else if (vertexes == std::make_pair(g.n - 1, g.start)) {
                var[2] = std::make_pair(1, g.g[u][i].second);
            } else if (vertexes == std::make_pair(g.n - 1, g.n - 1)) {
                var[3] = std::make_pair(1, g.g[u][i].second);
            }
        }
    }
    std::string repeating_part("");
    std::string ending("");
    std::string ans;
    if (!var[1].first) {
        return "";
    }
    ending = add_parentheses(var[1].second);
    if (var[3].first) {
        ending += kleene_closure(var[3].second);
    }
    if (var[0].first) {
        ending = kleene_closure(var[0].second) + ending;
    }
    if (!var[2].first) {
        return ending;
    }
    repeating_part = ending + add_parentheses(var[2].second);
    return kleene_closure(repeating_part) + ending;
}


#endif //State_machine_to_reg_expr