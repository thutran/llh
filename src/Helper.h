//
// Created by thu on 4/26/18.
//

#ifndef LIKELIHOOD_HELPER_H
#define LIKELIHOOD_HELPER_H

#include <vector>
#include <stdexcept>
#include <iostream>

namespace Helper{
    const int MOD = 1000000007;
    const unsigned int MAX_UNSIGNED_INT = 4294967295;
    const double LN2 = 0.69314718056;
    const unsigned int DEFAULT_RUNTIME = 672;
    const double E = 2.71828182846;
    const double LOG_CONST = 1e-9;

    template <typename T>
    void DeletePointer(T* p) {
        if (p) {
            delete p;
            p = nullptr;
        }
    }

    template <typename T>
    void DeleteVector(std::vector<T> &v){
        if (!v.empty()){
            std::vector<T>().swap(v);
        }
    }

    template <typename T>
    void RemoveFromVector(std::vector<T> &v, const int& i){
        int v_size = v.size();
        if (i >= v_size || i < 0)
            throw std::runtime_error("Index out of bound");
        // the only element
        else if ( v_size == 1 )
            std::vector<T>().swap(v);
        // last element
        else if ( i == (v_size - 1) )
            v.pop_back();
        else {
            T tmp = v[i];
            v[i] = v.back();
            v[v_size-1] = tmp;
            v.pop_back();
        }
        v.shrink_to_fit();
    }

    template <typename T>
    void RemoveFromVector(std::vector<T> &v, typename std::vector<T>::iterator it){
        int v_size = v.size();
        if ((it - v.begin()) >= v_size || (it - v.begin() ) < 0)
            throw std::runtime_error("Iterator out of bound");
        else if (v_size == 1)
            std::vector<T>().swap(v);
        else if (it == (v.end() - 1))
            v.pop_back();
        else {
            T tmp = *it;
            *it = v.back();
            v[v_size - 1] = tmp;
            v.pop_back();
        }
        v.shrink_to_fit();
    }

    template <typename T>
    void RemoveFromVector(std::vector<T> &v, typename std::vector<T>::reverse_iterator rit){
        int v_size = v.size();
        if ((rit - v.rbegin()) >= v_size || (rit - v.rbegin() ) < 0)
            throw std::runtime_error("Reverse iterator out of bound");
        else if (v_size == 1)
            std::vector<T>().swap(v);
        else if (rit == v.rbegin())
            v.pop_back();
        else {
            T tmp = *rit;
            *rit = v.back();
            v[v_size - 1] = tmp;
            v.pop_back();
        }
        v.shrink_to_fit();
    }

    template <typename T>
    void PrintVector(std::vector<T> const &v){
//        for (typename std::vector<T>::iterator it = v.cbegin(); it != v.end(); ++it){
        for (auto it = v.cbegin(); it != v.cend(); ++it){
            std::cout << *it << " ";
        }
    }

    // fast exponential by squaring for integers
    /*long long Power_Integer(const long long &base, const long long &exp){
        if (base == 0)
            return 0;
        else if (exp == 0 || base == 1)
            return 1;
        else if (exp == 1)
            return base;
        else if (exp < 0)
            return Power_Integer(1/base, -exp);
        else if (exp & 1) //odd
            return base*Power_Integer(base*base, (exp-1) >> 1);
        else
            return Power_Integer(base*base, exp >> 1);
    }*/

    // age-dependent (parasite age) maximum fraction of parasite population that can be affected by drug

}

#endif //LIKELIHOOD_HELPER_H
