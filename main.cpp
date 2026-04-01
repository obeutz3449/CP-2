#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>

#include "ArrayStack.h"

using namespace std;

// Token

struct Token {
    string value;   // number, operator, or parenthesis
    Token() = default;
    Token(const string &value) : value(value) {}
};

// Helpers

bool isOperator(const char s) {
    return s == '+' || s == '-' || s == '*' || s == '/';
}

bool isOperator(const Token &t) {
    return t.value.length() == 1 && isOperator(t.value[0]);
}

int toNumber(const Token &t) {
    int result = 0;
    for (const auto c : t.value) result = result * 10 + c - '0';
    return result;
}

bool isNumber(const string &s) {
    for (const auto &c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isNumber(const Token &t) {
    return isNumber(t.value);
}

vector<Token>& subVector(const vector<Token>& tokens, int start, int end) {
    auto* subtokens = new vector<Token>(end - start);
    for (int i = start; i < end; i++) subtokens->at(i - start) = tokens.at(i);
    return *subtokens;
}

vector<Token>& subVector(const vector<Token>& tokens, int start) {
    return subVector(tokens, start, tokens.size());
}

int precedence(const vector<Token>& tokens) {
    if (tokens.empty()) return -1;
    int depth = 0;
    int priority = 0;
    for (const auto &token : tokens) {
        if (token.value.length() == 1 && token.value[0] == '(') {
            priority++;
            depth = max(depth, priority);
        }else if (token.value.length() == 1 && token.value[0] == ')') priority--;
    }
    if (priority != 0) return -1;
    for (int p = 0; p <= depth; p++) {
        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (tokens.at(i).value.length() == 1 && tokens.at(i).value[0] == '(') priority++;
            else if (tokens.at(i).value.length() == 1 && tokens.at(i).value[0] == ')') priority--;
            if (isOperator(tokens.at(i)) && priority == p && (tokens.at(i).value[0] == '+' || tokens.at(i).value[0] == '-')) return i;
        }
        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (tokens.at(i).value.length() == 1 && tokens.at(i).value[0] == '(') priority++;
            else if (tokens.at(i).value.length() == 1 && tokens.at(i).value[0] == ')') priority--;
            if (isOperator(tokens.at(i)) && priority == p && (tokens.at(i).value[0] == '*' || tokens.at(i).value[0] == '/')) return i;
        }
    }
    return -1;
}

// Detection

bool isValidPostfix(const vector<Token>& tokens) {
    if (tokens.size() < 3) return false;
    int operators = 0;
    int numbers = 0;
    if (isNumber(tokens.at(0)) && isNumber(tokens.at(1))) {
        for (const auto &token : tokens) {
            if (isOperator(token.value[0])) operators++;
            else if (isNumber(token.value)) numbers++;
            else return false;
        }
    }else return false;
    return numbers == operators + 1;
}

bool isValidInfix(const vector<Token>& tokens) {
    if (tokens.size() < 3) return false;
    int priority = 0;
    bool isOp = false;
    for (const auto &token : tokens) {
        if (isNumber(token)) {
            if (isOp) return false;
            isOp = true;
        }else if (isOperator(token)) isOp = false;
        else if (token.value.length() == 1 && token.value[0] == '(') {
            priority++;
        }else if (token.value.length() == 1 && token.value[0] == ')') {
            priority--;
            if (priority < 0) return false;
        }else return false;
    }
    return isOp && priority == 0;
}

// Conversion

vector<Token> infixToPostfix(const vector<Token>& tokens) {
    auto postfix = vector<Token>();
    if (tokens.empty()) return postfix;
    int depth = 0;
    int priority = 0;
    for (const auto &token : tokens) {
        if (token.value.length() == 1 && token.value[0] == '(') {
            priority++;
            depth++;
        }
        else if (token.value.length() == 1 && token.value[0] == ')') priority--;
        else if (priority == 0) {
            depth = 0;
            break;
        }
    }
    if (depth > 0) return infixToPostfix(subVector(tokens, 1, tokens.size() - 1));
    if (tokens.empty()) return postfix;
    if (tokens.size() == 1) {
        postfix.push_back(tokens.at(0));
        return postfix;
    }
    const int i = precedence(tokens);
    const Token& op = tokens.at(i);
    postfix = infixToPostfix(subVector(tokens, 0, i));
    for (const auto &token : infixToPostfix(subVector(tokens, i + 1))) postfix.push_back(token);
    postfix.push_back(op);
    return postfix;
}

// Evaluation

double evalPostfix(const vector<Token>& tokens) {
    if (tokens.empty()) return 0;
    auto operations = ArrayStack<char>();
    auto numbers = ArrayStack<double>();
    for (const auto &t : tokens) {
        if (isOperator(t)) operations.push(t.value[0]);
        else if (isNumber(t)) numbers.push(toNumber(t));
        if (!operations.empty() && numbers.size() >= 2) {
            const double num1 = numbers.pop();
            const double num2 = numbers.pop();
            switch (operations.pop()) {
                case '+':
                    numbers.push(num2 + num1);
                    break;
                case '-':
                    numbers.push(num2 - num1);
                    break;
                case '*':
                    numbers.push(num2 * num1);
                    break;
                case '/':
                    if (num1 == 0) throw runtime_error("Error: Division by 0");
                    numbers.push(num2 / num1);
                    break;
                default:
                    cout<<"unexpected operation"<<endl;
            }
        }
    }
    return numbers.pop();
}

// Tokenizer

vector<Token> tokenize(const string& line) {
    vector<Token> tokens;
    int i = 0;
    while (i < line.length()) {
        if (isspace(line[i]));
        else if (isOperator(line[i])) tokens.emplace_back(line.substr(i, 1));
        else if (isdigit(line[i])) {
            int j = 1;
            while (line[i + j] != ' ' && i + j < line.length()) j++;
            if (!tokens.empty() && tokens.at(tokens.size() - 1).value == ")") {
                tokens.emplace_back("*");
            }
            tokens.emplace_back(line.substr(i, j));
            i+=j-1;
        }else if (line[i] == '(') {
            if (!tokens.empty() && (isNumber(tokens.at(tokens.size() - 1)) || tokens.at(tokens.size() - 1).value == ")")) {
                tokens.emplace_back("*");
            }
            tokens.emplace_back("(");
        }else if (line[i] == ')') tokens.emplace_back(")");
        else cout << "Unknown token: " << line.substr(i) << "\n";
        i++;
    }
    return tokens;
}

// Main

int main() {
    string line;
    getline(cin, line);
    if (vector<Token> tokens = tokenize(line); tokens.empty() || (tokens.size() == 1 && isNumber(tokens.at(0)))) {
        switch (rand()%2) { //randomness not random
            case 0: {
                const vector<Token> postfix = infixToPostfix(tokens);
                cout << "FORMAT: INFIX\n";
                cout << "POSTFIX: ";
                for (const auto& t : postfix) {
                    cout << t.value << " ";
                }
                cout << "\n";
                cout << "RESULT: " << evalPostfix(postfix) << "\n";
                break;
            }
            default:
                cout << "FORMAT: POSTFIX\n";
                cout << "RESULT: " << evalPostfix(tokens) << "\n";
                break;
        }
    }
    else if (isValidPostfix(tokens)) {
        cout << "FORMAT: POSTFIX\n";
        cout << "RESULT: " << evalPostfix(tokens) << "\n";
    }
    else if (isValidInfix(tokens)) {
        const vector<Token> postfix = infixToPostfix(tokens);
        cout << "FORMAT: INFIX\n";
        cout << "POSTFIX: ";
        for (const auto& t : postfix) {
            cout << t.value << " ";
        }
        cout << "\n";
        cout << "RESULT: " << evalPostfix(postfix) << "\n";
    }else {
        cout << "FORMAT: NEITHER\n";
        cout << "ERROR: invalid expression\n";
    }
    return 0;
}