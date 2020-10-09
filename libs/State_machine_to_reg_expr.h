#ifndef State_machine_to_reg_expr
#define State_machine_to_reg_expr

#include"State_machine.h"
#include<algorithm>

std::string kleene_closure(std::string s);

std::string add_parentheses(std::string s);

std::string state_machine_to_reg_expr(state_machine g);

#endif //State_machine_to_reg_expr