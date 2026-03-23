#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>

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

bool isNumber(const string &s) {
    for (const auto &c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isNumber(const Token &t) {
    return isNumber(t.value);
}

int precedence(const string& op) {
    // TODO
    // so what is this?
    return 0;
}

// Detection

bool isValidPostfix(const vector<Token>& tokens) {
    int operators = 0;
    int numbers = 0;
    if (isNumber(tokens.at(tokens.size() - 1)) && isNumber(tokens.at(tokens.size() - 2))) {
        for (const auto &token : tokens) {
            if (isOperator(token.value[0])) operators++;
            else if (isNumber(token.value)) numbers++;
            else return false;
        }
    }else return false;
    return numbers == operators + 1;
}

bool isValidInfix(const vector<Token>& tokens) {
    int priority = 0;
    bool isOp = false;
    for (const auto &token : tokens) {
        if (isOperator(token) == isOp) return false;
        if (token.value.length() == 1 && token.value[0] == '(') {
            priority++;
        }else if (token.value.length() == 1 && token.value[0] == ')') {
            priority--;
            if (priority < 0) return false;
        }else if (!isOperator(token) && !isNumber(token)) return false;
        isOp = !isOp && priority == 0;
    }
    return !isOp;
}

// Conversion

vector<Token> infixToPostfix(const vector<Token>& tokens) {
    vector<Token> output;
    // TODO
    return output;
}

// Evaluation

double evalPostfix(const vector<Token>& tokens) {
    ArrayStack<double> stack;
    // TODO
    return 0.0;
}

// Tokenizer

vector<Token> tokenize(const string& line) {
    vector<Token> tokens;
    int i = 0;
    while (i < line.length()) {
        if (isspace(line[i]));
        else if (isOperator(line[i])) tokens.push_back(Token(line.substr(i, 1)));
        else if (isdigit(line[i])) {
            int j = 1;
            while (line[i + j] != ' ' && i + j < line.length()) j++;
            if (!tokens.empty() && (tokens.at(tokens.size() - 1)).value == ")") {
                tokens.push_back(Token("*"));
            }
            tokens.push_back(Token(line.substr(i, j-1)));
            i+=j-1;
        }else if (line[i] == '(') {
            if (!tokens.empty() && isNumber(tokens.at(tokens.size() - 1))) {
                tokens.push_back(Token("*"));
            }
            tokens.push_back(Token("("));
        }else if (line[i] == ')') tokens.push_back(Token(")"));
        else cout << "Error at: " << line.substr(i) << "\n";
        i++;
    }
    return tokens;
}

// Main

int main() {
    string line;
    getline(cin, line);

    vector<Token> tokens = tokenize(line);

    if (isValidPostfix(tokens)) {
        cout << "FORMAT: POSTFIX\n";
        cout << "RESULT: " << evalPostfix(tokens) << "\n";
    }
    else if (isValidInfix(tokens)) {
        vector<Token> postfix = infixToPostfix(tokens);
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