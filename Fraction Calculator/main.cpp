#include <iostream>
#include <string>
#include <cctype>
#include "dictionary.h"
#include "fraction.h"
#include "stack.h"

using namespace std;

struct Value {
    string name;
    Fraction fraction;
};


bool precedence(char top, char current){
    if(top == '*' || top == '/'){return true;}
    if(top == '(' || top == '=' || top == '$'){return false;}
    return current == '+' || current == '-' || current == '=';
}

void operate(Stack<Value> &numStack, Stack<char> &opStack, Dictionary &vars){
    Value v2 = numStack.pop();
    char op = opStack.pop();
    Value v1 = numStack.pop();
    Value newval;

    if(!v2.name.empty()){
        v2.fraction = vars.search(v2.name);
    }
    if(op != '=' && !v1.name.empty()){
        v1.fraction = vars.search(v1.name);
    }

    switch(op){
        default:
            //default?
            break;
        case '=':
            vars.add(v1.name, v2.fraction);
            newval = v2;
            break;
        case '+':
            newval.fraction = v1.fraction+v2.fraction;
            break;
        case '-':
            newval.fraction =  v1.fraction-v2.fraction;
            break;
        case '/':
            newval.fraction =  v1.fraction/v2.fraction;
            break;
        case '*':
            newval.fraction =  v1.fraction*v2.fraction;
            break;
    }
    numStack.push(newval);
}

Fraction Eval(string s, Stack<Value> &numStack, Stack<char> &opStack, Dictionary &vars){
    numStack.clear();
    opStack.clear();
    opStack.push('$');
    int i = 0;
    string oplist = "+-*/=";

    while(i<s.length()){
        if(s[i] == ' '){
            i++;
        }
        else if (isdigit(s[i])){
            string num;
            while(isdigit(s[i]) && i < s.length()) {
                num += s[i];
                i++;
            }
            Value v;
            v.fraction = Fraction(stoi(num), 1);
            numStack.push(v);
        }
        else if (isalpha(s[i])){
            string name;
            while (isalnum(s[i]) && i < s.length()){
                if (s[i] == ' '){
                    i++;
                }
                name += s[i];
                i++;
            }
            Value v;
            v.name = name;
            numStack.push(v);
        }
        else if (s[i] == '('){
            opStack.push('(');
            i++;
        }
        else if (s[i] == ')'){
            while(opStack.peek() != '(') {
                operate(numStack, opStack, vars);
            }
            opStack.pop();
            i++;
        }
        else if (oplist.find(s[i]) < -1){
            while (precedence(opStack.peek(), s[i])){
                operate(numStack, opStack, vars);
            }
            opStack.push(s[i]);
            i++;
        }
        else{i++;}
    }
    while(opStack.peek() != '$'){
        operate(numStack, opStack, vars);
    }
    return numStack.peek().fraction;
}

int main() {

    Dictionary vars;
    Fraction
        f(2,3);
    Stack<Value> numStack;
    Stack<char> opStack;

    string expression;

    while (expression != "quit") {
        cout << "Enter an expression. Enter quit to stop.\n";
        getline(cin, expression);
        cout << Eval(expression, numStack, opStack, vars) << endl << endl;
    }

    try {
        vars.add("foo", f);
    }
    catch (const std::overflow_error &e) {
        std::cout << "Caught an overflow" << std::endl;
        std::cout << "Message: " << e.what() << std::endl;
    }
    catch (const std::domain_error &e) {
        std::cout << "Caught a domain error, which shouldn't happen"
            << std::endl;
    }
    catch (...) {
        std::cout << "Caught some generic exception" << std::endl;
    }

    return 0;
}