#pragma once

#include "BaseError.h"

namespace pl {


class DimensionalityReductionError : public BaseError {
public:
    DimensionalityReductionError(const std::string &msg,
                                 const std::string &where = std::string(),
                                 const int line = -1);
};


}
