#pragma once

#include "BaseLogger.h"

namespace pl {


class ConsoleLogger : public BaseLogger
{
public:
    ConsoleLogger();

    virtual ~ConsoleLogger();

    template <typename ... T>
    void inform(T ... information) {
        std::cout << i(information ...) << std::flush;
    }

    template <typename ... T>
    void debug(T ... msg) {
        std::cout << d(msg ...) << std::flush;
    }

    template <typename ... T>
    void warn(T ... warning) {
        std::cout << w(warning ...) << std::flush;
    }

    template <typename ... T>
    void report(T ... error) {
        std::cout << e(error ...) << std::flush;
    }
};


}
