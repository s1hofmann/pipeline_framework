#pragma once

#include <iostream>
#include <vector>
#include <cstdint>

/**
  *
  * Global pipeline settings
  *
  */

#ifdef USE_TBB
#include <tbb/parallel_for.h>
#endif

/**
  * Macro which gives name of passed variable. Used for config parsing
  */
#define varName(n) #n

/**
  * Macro which inserts the current method name, used for exceptions
  */
#define currentMethod  __FUNCTION__

/**
  * Macro which inserts the current line, used for exceptions
  */
#define currentLine __LINE__

/**
  * Variadic template functions for debug / warning / log output
  */

//Template method for output of a single argument
template<typename T>
void write(T value) {
    std::cout << value << std::endl;
}


//Template method for output of n arguments
template<typename T, typename... O>
void write(T arg, O... other) {
    std::cout << arg << " ";
    write(other...);
}


template <typename... T>
void debug(T... args) {
    std::cout << "DEBUG: ";
    //Start recursion
    write(args...);
}


template <typename... T>
void inform(T... args) {
    std::cout << "INFO: ";
    write(args...);
}


template <typename... T>
void warning(T... args) {
    std::cout << "WARNING: ";
    write(args...);
}


template <typename... T>
void error(T... args) {
    std::cout << "ERROR: ";
    write(args...);
}


template <typename T>
bool checkRangeProperties(const std::vector<T> &params) {
    return ((params.size() == 3) && (params[2] > 1e-8) && (params[0] < params[1]));
}

