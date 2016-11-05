#pragma once

#include "BaseError.h"

namespace pl {


class PreProcessError : public BaseError {
public:
    PreProcessError(const std::string &msg,
                    const std::string &where = std::string(),
                    const int32_t line = -1);
};


}
