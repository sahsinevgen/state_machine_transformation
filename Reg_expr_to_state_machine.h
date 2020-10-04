#ifndef Reg_expr_to_state_machine
#define Reg_expr_to_state_machine

#include"State_machine.h"

void add_number_to_all_edges(state_machine &g, int x) {
    for (int u = 0; u < g.n; u++) {
        for (int i = 0; i < g.g[u].size(); i++) {
            g.g[u][i].first += x;
        }
    }
}

state_machine state_machine_with_one_move(std::string move) {
    state_machine ans;
    ans.add_vertex(2);
    ans.start = 0;
    ans.terminals[1] = 1;
    ans.g[0].push_back(std::make_pair(1, move));
    return ans;
}

state_machine add_kleene_closure(state_machine a) {
    state_machine ans = a;
    ans.add_vertex(3);
    ans.g[ans.n - 3].push_back(std::make_pair(ans.n - 2, ""));
    ans.g[ans.n - 2].push_back(std::make_pair(ans.n - 1, ""));
    ans.g[ans.n - 2].push_back(std::make_pair(ans.start, ""));
    for (int i = 0; i < ans.n; i++) {
        if (ans.terminals[i]) {
            ans.g[i].push_back(std::make_pair(ans.n - 2, ""));
        }
    }

    ans.start = ans.n - 3;
    for (int i = 0; i < ans.n; i++) {
        ans.terminals[i] = 0;
    }
    ans.terminals[ans.n - 1] = 1;
    
    return ans;
}

state_machine concatenation(state_machine a, state_machine b) {
    int n_last = a.n;
    a.add_vertex(b.n);
    add_number_to_all_edges(b, n_last);
    for (int i = 0; i < n_last; i++) {
        if (a.terminals[i]) {
            a.g[i].push_back(std::make_pair(b.start + n_last, ""));
            a.terminals[i] = 0;
        }
    }
    for (int i = 0; i < b.n; i++) {
        a.g[n_last + i] = b.g[i];
        a.terminals[n_last + i] = b.terminals[i];
    }
    return a;
}

state_machine sum(state_machine a, state_machine b) {

    int n_last = a.n;
    a.add_vertex(b.n + 2);
    add_number_to_all_edges(b, n_last);
    for (int i = 0; i < b.n; i++) {
        a.g[n_last + i] = b.g[i];
        a.terminals[n_last + i] = b.terminals[i];
    }
    a.g[a.n - 2].push_back(std::make_pair(a.start, ""));
    a.g[a.n - 2].push_back(std::make_pair(b.start + n_last, ""));
    for (int i = 0; i < a.n; i++) {
        if (a.terminals[i]) {
            a.g[i].push_back(std::make_pair(a.n - 1, ""));
            a.terminals[i] = 0;
        }
    }
    a.start = a.n - 2;
    a.terminals[a.n - 1] = 1;
    return a;
}

state_machine expr(const std::string &s, int &i);

state_machine term(const std::string &s, int &i) {
    state_machine ans;
    bool is_first = true;
    while (i != s.size() && s[i] != '+' && s[i] != ')') {
        //std::cout << i << std::endl;
        state_machine part;
        if (s[i] == '(') {
            i++;
            part = expr(s, i);
            i++;
        } else {
            std::string move = "";
            if (s[i] != '#') {
                move += s[i];
            }
            part = state_machine_with_one_move(move);
            i++;
        }
        if (i != s.size() && s[i] == '*') {
            part = add_kleene_closure(part);
            i++;
        }
        if (is_first) {
            ans = part;
            is_first = false;
        } else {
            ans = concatenation(ans, part);
        }
    }
    return ans;
}

state_machine expr(const std::string &s, int &i) {
    state_machine ans;
    ans = term(s, i);
    while (i != s.size() && s[i] == '+') {
        i++;
        ans = sum(ans, term(s, i));
    }
    return ans;
}

state_machine reg_expr_to_state_machine(const std::string &s) {
    int i = 0;
    return expr(s, i);
}

#endif //Reg_expr_to_state_machine