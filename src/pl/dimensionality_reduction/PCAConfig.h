#pragma once

#include "../pipeline/ConfigContainer.h"

#include <sstream>
#include <iostream>

namespace pl {


class PCAConfig : public ConfigContainer
{
public:
    PCAConfig(const std::string &identifier,
              const int components = 64,
              const double epsilon = 0.001,
              const bool whiten = true,
              const std::string &path = ".");

    virtual std::string toString() const;

    virtual bool fromJSON(std::string &file);

    int components() const;
    bool setComponents(int components);

    double epsilon() const;
    bool setEpsilon(double epsilon);

    bool whitening() const;
    bool setWhitening(bool whitening);

    std::string path() const;
    bool setPath(const std::string &path);

private:
    int mComponents;
    double mEpsilon;
    bool mWhitening;
    std::string mPath;
};


}
