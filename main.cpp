#include"libs/Reg_expr_to_state_machine.h"
#include"libs/Make_one_letter_moves.h"
#include"libs/Determinization.h"
#include"libs/Minimization.h"
#include"libs/State_machine_to_reg_expr.h"

int main() {
    // std::string s;
    // std::cin >> s;
    // state_machine g = reg_expr_to_state_machine(s);
    // std::cout << g << "\n\n";
    state_machine a, b;
    std::string s;
    std::cin >> s;
    a = determinization(make_one_letter_moves(reg_expr_to_state_machine(s)));
    std::cout << a;
    std::cout << minimization(a);
    std::cin >> b;
    std::cout << (are_equiv(a, b) ? "YES\n" : "NO\n") << std::endl;
}
