#include"Reg_expr_to_state_machine.h"
#include"Make_one_letter_moves.h"
#include"Determinization.h"
#include"Miniminization.h"
#include"State_machine_to_reg_expr.h"

int main() {
    std::string s;
    std::cin >> s;
    state_machine g = reg_expr_to_state_machine(s);
    std::cout << g << "\n\n";
    std::cout << minimization(determinization(make_one_letter_moves(g)));
}