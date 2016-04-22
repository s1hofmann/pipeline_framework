#pragma once

#include "BaseError.h"

namespace pl {


class IOError : public BaseError {
public:
    IOError(const std::string &msg,
            const std::string &where = std::string(),
            const int line = -1);
};


}