#include "function.h"
/*
 * функция, распознающая следующую по приоритету
 * элементарную функцию в строке и выделяющая её
 * в отдельную подфункцию
 * возвращает 1, если есть ошибка во входной строке
 */
bool fun::elem_fun::function(const std::string& f){
    if (f == "")
        return 1;
    // Эта часть кода просматривает строку на наличие ошибок в расставлении скобок
    unsigned i;
    std::stack <char> pars;
    unsigned t = 0;
    for (i = 0; i < f.size() && f[i] == '('; ++i){
        ++t;
        pars.push(')');
    }
    for (; i < f.size(); ++i){
        if (pars.size() != 0 && pars.top() == f[i])
            pars.pop();
        else if (f[i] == '(')
            pars.push(')');
        if (pars.size() < t && i < f.size() - t)
            --t;
    }
    if (pars.size() != 0) {
        return 1;
    }

    // Эта часть кода ищет операцию с наименьшим приоритетом
    std::string h;
    short pr = 8;
    unsigned pos = 0;
    if (3 < f.size()) { // функции с аргументами после
        h = f.substr(t, 3);
        if (priority(h) < pr) {
            s = h;
            pr = priority(h);
            pos = t;
        }
    }
    h.resize(1);
    size_t size = f.size() - t;
    for (i = t; i < size; ++i){
        if (pars.size() != 0 && pars.top() == f[i])
            pars.pop();
        else if (f[i] == '(')
            pars.push(')');
        if (pars.size() == 0) { // только если операция не в скобках
            h[0] = f[i];
            if (priority(h) < pr || priority(h) == pr && (h == "/" || h == "-")) {
                s = h;
                pr = priority(h);
                pos = i;
            }
        }
    }

    /*\
     * Эта часть кода разделяет сложную функцию как функцию
     * от нескольких функций, аля f(g(x), h(x)) = g(x) + h(x)
    \*/
    bool has_err;
    if (pr != 7) { // создание новых элем. функций
        left = new elem_fun();
    } if (pr <= 5) {
        right = new elem_fun();
    } if (pr < 4 || pr == 5) {
        h = f.substr(t, pos - t);
        if (h == "" && pr < 2)
            h = "0";
        has_err = left->function(h);
        has_err |= right->function(f.substr(pos + 1, size - (pos + 1)));
        return has_err;
    } if (pr == 6) {
        has_err = left->function(f.substr(pos + 3, size - (pos + 3)));
        return has_err;
    } if (pr == 4) {
        unsigned kostyl = 0; // нужен для учитывания скобок в 3-ем цикле
        med = new elem_fun();
        h.clear();
        for (i = pos + 4; i < f.size() && f[i] != ';' && f[i] != ','; ++i){
            h.push_back(f[i]);
        }
        has_err = left->function(h);
        h.clear();
        for (++i; i < f.size() && f[i] != ';' && f[i] != ','; ++i){
            h.push_back(f[i]);
        }
        has_err |= med->function(h);
        h.clear();
        for (++i; f.size() && (f[i] != ')' || kostyl); ++i){
            kostyl += (f[i] == '(');
            kostyl -= (f[i] == ')');
            h.push_back(f[i]);
        }
        has_err |= right->function(h);
        return has_err;
    } if (pr == 7) {
        s = f;
        s = f.substr(pos, 1);
        return 0;
    }
    return 1;
}

// Вычисление значения функции
double fun::elem_fun::fun_val(double x, double& eps){
    if (s == "x")
        return x;
    if (s == "-")
        return left->fun_val(x, eps) - right->fun_val(x, eps);
    if (s == "+")
        return left->fun_val(x, eps) + right->fun_val(x, eps);
    if (s == "*")
        return left->fun_val(x, eps) * right->fun_val(x, eps);
    if (s == "/")
        return left->fun_val(x, eps) / right->fun_val(x, eps);
    if (s == "pi")
        return M_PI;
    if (s == "^")
        return pow(left->fun_val(x, eps), right->fun_val(x, eps));
    if (s == "e")
        return M_E;
    if (s == "log")
        return log(left->fun_val(x, eps));
    if (s == "sin")
        return sin(left->fun_val(x, eps));
    if (s == "cos")
        return cos(left->fun_val(x, eps));
    if (s == "exp")
        return exp(left->fun_val(x, eps));
    if (s == "int") { // вычисление интеграла
        double a, b;
        a = left->fun_val(x, eps);
        b = med->fun_val(x, eps);
        double sum = right->fun_val(a, eps) + right->fun_val(b, eps), prev_sum = 0;
        double hlp_var = 0;
        long long n = 1;
        while (fabs(sum - 2 * prev_sum) > eps * (15 * n)){
            n <<= 1;
            if (n == 0) // произошло переполнение
                break;
            prev_sum = sum;
            sum -= 2 * hlp_var;
            hlp_var = 0;
            for (long long i = 1; i < n; i += 2)
                hlp_var += right->fun_val(a + (b - a) * i / n, eps);
            sum += 4 * hlp_var;
        }
        if (n != 0)
            return sum * (b - a) / (3 * n);
    }
    return stod(s);
}

// Деструктор элем. функции
fun::elem_fun::~elem_fun(){
    if (left != nullptr) {
        left->~elem_fun();
        delete left;
        left = nullptr;
    }
    if (med != nullptr) {
        med->~elem_fun();
        delete med;
        med = nullptr;
    }
    if (right != nullptr) {
        right->~elem_fun();
        delete right;
        right = nullptr;
    }
}
