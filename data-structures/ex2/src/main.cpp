#include <cctype>
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

const std::string ERR_DIV_ZERO = "除零错误";
const std::string ERR_EXTRA_OPERAND = "表达式多余操作数";
const std::string ERR_ILLEGAL_CHAR = "非法字符: ";
const std::string ERR_MISSING_OPERAND = "表达式缺少操作数";
const std::string ERR_MULTIPLE_DOT = "数字内重复小数点";
const std::string ERR_PAREN_MISMATCH = "括号不匹配";
const std::string ERR_UNKNOWN_OP = "未知运算符";

enum TokenType { NUMBER, OPERATOR, PARENTHESIS };

struct Token {
    TokenType type;
    union {
        double value;
        char op;
    };
    Token(TokenType t, double v) : type(t), value(v) {}
    Token(TokenType t, char o) : type(t), op(o) {}
};

std::vector<Token> tokenize_expression(const std::string &expr) {
    std::vector<Token> token_list;
    size_t i = 0;
    while (i < expr.length()) {
        if (std::isspace(expr[i])) {
            ++i;
            continue;
        }
        if (std::isdigit(expr[i]) || expr[i] == '.') {
            size_t start = i;
            bool dot = false;
            while (i < expr.length() && (std::isdigit(expr[i]) || expr[i] == '.')) {
                if (expr[i] == '.') {
                    if (dot) throw std::runtime_error(ERR_MULTIPLE_DOT);
                    dot = true;
                }
                ++i;
            }
            double val = std::stod(expr.substr(start, i - start));
            token_list.push_back(Token(NUMBER, val));
        } else if (std::string("+-*/").find(expr[i]) != std::string::npos) {
            token_list.push_back(Token(OPERATOR, expr[i]));
            ++i;
        } else if (expr[i] == '(' || expr[i] == ')') {
            token_list.push_back(Token(PARENTHESIS, expr[i]));
            ++i;
        } else {
            throw std::runtime_error(ERR_ILLEGAL_CHAR + expr[i]);
        }
    }
    return token_list;
}

int get_precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

double apply_operator(double a, double b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0) throw std::runtime_error(ERR_DIV_ZERO);
        return a / b;
    default:
        throw std::runtime_error(ERR_UNKNOWN_OP);
    }
}

double evaluate_expression(const std::vector<Token> &token_list) {
    std::stack<double> value_stack;
    std::stack<char> op_stack;
    for (size_t i = 0; i < token_list.size(); ++i) {
        const Token &token = token_list[i];
        if (token.type == NUMBER) {
            value_stack.push(token.value);
        } else if (token.type == OPERATOR) {
            while (!op_stack.empty() && get_precedence(op_stack.top()) >= get_precedence(token.op)) {
                if (value_stack.size() < 2) throw std::runtime_error(ERR_MISSING_OPERAND);
                double b = value_stack.top();
                value_stack.pop();
                double a = value_stack.top();
                value_stack.pop();
                char op = op_stack.top();
                op_stack.pop();
                value_stack.push(apply_operator(a, b, op));
            }
            op_stack.push(token.op);
        } else if (token.type == PARENTHESIS) {
            if (token.op == '(') {
                op_stack.push('(');
            } else if (token.op == ')') {
                bool found_left = false;
                while (!op_stack.empty()) {
                    if (op_stack.top() == '(') {
                        op_stack.pop();
                        found_left = true;
                        break;
                    } else {
                        if (value_stack.size() < 2) throw std::runtime_error(ERR_MISSING_OPERAND);
                        double b = value_stack.top();
                        value_stack.pop();
                        double a = value_stack.top();
                        value_stack.pop();
                        char op = op_stack.top();
                        op_stack.pop();
                        value_stack.push(apply_operator(a, b, op));
                    }
                }
                if (!found_left) throw std::runtime_error(ERR_PAREN_MISMATCH);
            }
        }
    }
    while (!op_stack.empty()) {
        if (op_stack.top() == '(' || op_stack.top() == ')') throw std::runtime_error(ERR_PAREN_MISMATCH);
        if (value_stack.size() < 2) throw std::runtime_error(ERR_MISSING_OPERAND);
        double b = value_stack.top();
        value_stack.pop();
        double a = value_stack.top();
        value_stack.pop();
        char op = op_stack.top();
        op_stack.pop();
        value_stack.push(apply_operator(a, b, op));
    }
    if (value_stack.size() != 1) throw std::runtime_error(ERR_EXTRA_OPERAND);
    return value_stack.top();
}

int main() {
    std::string input_line;
    do {
        std::cout << "请输入中缀表达式（按 Ctrl+C 退出）：" << std::endl << ">> ";
        std::getline(std::cin, input_line);
        try {
            auto token_list = tokenize_expression(input_line);
            double result = evaluate_expression(token_list);
            std::cout << " = " << result << std::endl;
        } catch (const std::exception &e) {
            std::cout << "[错误] " << e.what() << std::endl;
        }
    } while (input_line.length());
    return 0;
}
