#pragma once


#include <vector>
#include <algorithm>


namespace pl {


template <typename T>
class UniqueNumber
{
private:
    T number;
    T stepSize;
    bool reverse;

public:
    UniqueNumber(T start = 0, T step = 1.0, bool rev = false)
    {
        reverse = rev;
        stepSize = (step <= 0) ? 1.0 : step;
        rev ? number = start+stepSize : number = start-stepSize;
    }
    T operator()() { return reverse ? (number-=stepSize) : (number+=stepSize); }
};


/**
 * Range template class.
 *
 * Provides static methods to generate a std::vector filled
 * with elements, either in ascending/descending order from
 * start to end, or with random values.
 */
template <typename T>
class Range
{
public:
    static std::vector<T> unique(T from, T to, T step = 1.0)
    {
        if(!std::is_integral<T>::value) { return std::vector<T>(); }


        T delta = to-from;
        delta *= (delta < 0) ? -1 : 1;
        T stepSize = (step <= 0) ? 1.0 : step;

        std::vector<T> ret;
        if(step == 0) {
            return ret;
        } else if(step != 1) {
            if(delta % stepSize == 0) {
                ret.resize(delta/stepSize);
            } else {
                ret.resize((delta/stepSize)+1);
            }
        } else {
            ret.resize(delta);
        }
        if(from == to) {
            return std::vector<T>();
        } else if(from > to) {
            std::generate(ret.begin(), ret.end(), UniqueNumber<T>(from, stepSize, true));
        } else {
            std::generate(ret.begin(), ret.end(), UniqueNumber<T>(from, stepSize));
        }
        return ret;
    }


    static std::vector<T> random(T from, T to, T step = 1.0)
    {
        std::vector<T> ret = unique(from, to, step);
        std::random_shuffle(ret.begin(), ret.end());
        return ret;
    }
};


}
