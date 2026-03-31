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
        for (int i = 0; i < tokens.size(); i++) {
            if (tokens.at(i).value.length() == 1 && tokens.at(i).value[0] == '(') priority++;
            else if (tokens.at(i).value.length() == 1 && tokens.at(i).value[0] == ')') priority--;
            if (isOperator(tokens.at(i)) && priority == p && (tokens.at(i).value[0] == '+' || tokens.at(i).value[0] == '-')) return i;
        }
        for (int i = 0; i < tokens.size(); i++) {
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
    return isOp;
}

// Conversion

vector<Token> infixToPostfix(vector<Token>& tokens) {
    auto postfix = vector<Token>();
    if (tokens.empty()) return postfix;
    while (tokens.at(0).value[0] == '(' && tokens.at(tokens.size() - 1).value[0] == ')') {
        tokens = subVector(tokens, 1, tokens.size() - 1);
    }
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

double evalPostfix(ArrayStack<Token>& tokens) {
    if (tokens.empty()) return 0;
    Token top = tokens.pop();
    if (isNumber(top)) {
        int num = 0;
        for (const auto c : top.value) {
            num*=10;
            num+=(c-'0');
        }
        return num;
    }
    double num1 = evalPostfix(tokens);
    double num2 = evalPostfix(tokens);
    if (isOperator(top)) {
        switch (top.value[0]) {
            case '+':
                return num2 + num1;
            case '-':
                return num2 - num1;
            case '*':
                return num2 * num1;
            case '/':
                return num2 / num1;
            default:
                cout<<"unexpected operation "<<top.value<<endl;
        }
    }
    return -1;
}

double evalPostfix(const vector<Token>& tokens) {
    if (tokens.empty()) return 0;
    ArrayStack<Token> expression = ArrayStack<Token>();
    for (auto const &token : tokens) expression.push(token);
    return evalPostfix(expression);
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
            tokens.push_back(Token(line.substr(i, j)));
            i+=j-1;
        }else if (line[i] == '(') {
            if (!tokens.empty() && isNumber(tokens.at(tokens.size() - 1))) {
                tokens.push_back(Token("*"));
            }
            tokens.push_back(Token("("));
        }else if (line[i] == ')') tokens.push_back(Token(")"));
        else cout << "Unknown token: " << line.substr(i) << "\n";
        i++;
    }
    return tokens;
}

// Main

int main() {
    string line;
    getline(cin, line);

    if (vector<Token> tokens = tokenize(line); isValidPostfix(tokens)) {
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