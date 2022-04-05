#ifndef FUNCTION_H
#define FUNCTION_H

#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <tuple>

// Возвращает приоритет операций
inline short priority(const std::string& s){
    if (s == "+")
        return 0;
    if (s == "-")
        return 1;
    if (s == "*")
        return 2;
    if (s == "/")
        return 3;
    if (s == "int")
        return 4;
    if (s == "^")
        return 5;
    if (s == "log" || s == "sin" || s == "cos" || s == "exp")
        return 6;
    if (s == "e" || s == "pi" || s == "x")
        return 7;
    try {
        char* end;
        strtod(s.c_str(), &end);
        if (end - s.c_str() < s.size() * sizeof(char))
            return 8;
        else
            return 7;
    } catch (std::exception& err) {
        return 8;
    }
}

// Интересный алгоритм, который применяется, только чтобы убрать кучу пробелов
inline void exc_str(std::string& str){
    int i, j;
    for (i = 0, j = 0; i < str.size(); ++i){
        if (str[i] != ' ') {
            str[j] = str[i];
            ++j;
        }
    }
    str.resize(j);
}

class fun{
private:
    // Класс-"вершина" с элементарной функцией
    class elem_fun{
    public:
        std::string s;
        elem_fun *left, *med, *right;
        elem_fun(){
            left = nullptr;
            med = nullptr;
            right = nullptr;
        }
        bool function(const std::string&);
        double fun_val(double, double&);
        ~elem_fun();
    };
    double eps;     // точность вычислений
    bool err;       // есть ли ошибка (true если есть)
    elem_fun* ptr;  // последняя вычисляемая элем. функция
    
public:
    // Конструкторы
    fun() : eps(1e-6), err(1), ptr(nullptr) {}
    fun(std::string f) : eps(1e-6), err(0) {
        ptr = new elem_fun();
        if (ptr == nullptr)
            err = 1;
        else {
            exc_str(f);
            err = ptr->function(f);
            if (err) {
                ptr->~elem_fun();
                delete ptr;
                ptr = nullptr;
            }
        }
    }
    fun(fun const &f) : eps(f.eps), err(f.err) {
        std::queue <std::pair <elem_fun*, elem_fun*>> q;
        ptr = new elem_fun();
        q.push({ptr, f.ptr});
        while (q.size()) {
            auto p = q.front();
            p.first->s = p.second->s;
            if (p.second->left != nullptr) {
                p.first->left = new elem_fun();
                q.push({p.first->left, p.second->left});
            }
            if (p.second->med != nullptr) {
                p.first->med = new elem_fun();
                q.push({p.first->med, p.second->med});
            }
            if (p.second->right != nullptr) {
                p.first->right = new elem_fun();
                q.push({p.first->right, p.second->right});
            }
            q.pop();
        }
    }
    fun& operator = (fun const& f) {
        eps = f.eps;
        err = f.err;
        std::queue <std::pair <elem_fun*, elem_fun*>> q;
        ptr = new elem_fun();
        q.push({ptr, f.ptr});
        while (q.size()) {
            auto p = q.front();
            p.first->s = p.second->s;
            if (p.second->left != nullptr) {
                p.first->left = new elem_fun();
                q.push({p.first->left, p.second->left});
            }
            if (p.second->med != nullptr) {
                p.first->med = new elem_fun();
                q.push({p.first->med, p.second->med});
            }
            if (p.second->right != nullptr) {
                p.first->right = new elem_fun();
                q.push({p.first->right, p.second->right});
            }
            q.pop();
        }
        return *this;
    }
    
    // Акцессоры
    void set_fun(std::string f){
        ptr = new elem_fun();
        if (ptr == nullptr)
            err = 1;
        else {
            exc_str(f);
            err = ptr->function(f);
            if (err) {
                ptr->~elem_fun();
                delete ptr;
                ptr = nullptr;
            }
        }
    }
    void set_acc(double acc){
        if (acc > 0)
            eps = acc;
    }
    bool is_err() { return err; }
    double get_acc() { return eps; }
    
    /*
     * получения значения функции
     * в какой-то точке
     */
    double fun_val(double x){
        if (!err)
            return ptr->fun_val(x, eps);
        return NAN;
    }
    
    // Вычисление минимума на отрезке
    double min(double a, double b){
        if (err) return NAN;
        double m = ptr->fun_val(a, eps), step = eps * (b - a);
        for (double i = a + step; i <= b; i += step) {
            m = (m <= ptr->fun_val(i, eps)) ? m: ptr->fun_val(i, eps);
        }
        return m;
    }
    // Вычисление максимума на отрезке
    double max(double a, double b){
        if (err) return NAN;
        double m = ptr->fun_val(a, eps), step = eps * (b - a);
        for (double i = a + step; i <= b; i += step){
            m = (m >= ptr->fun_val(i, eps)) ? m: ptr->fun_val(i, eps);
        }
        return m;
    }
    
    ~fun(){
        if (ptr != nullptr)
            delete ptr;
    }
};

#endif // FUNCTION_H
