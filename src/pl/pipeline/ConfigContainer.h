//
// Created by Simon Hofmann on 19.11.15.
//


#pragma once

#include <string>
#include <fstream>
#include <typeinfo>
#include <opencv2/core/core.hpp>

#include "globals.h"
#include "../core/utils/json/json.h"

namespace pl {


/**
 * Base class for each steps config class.
 * Every config class has te be derived from
 */
class ConfigContainer {
public:
    /**
     * @brief usage
     * @return
     */
    std::string usage() const;

    /**
     * @brief help
     * @return
     */
    std::string help() const;

    /**
     * @brief identifier
     * @return
     */
    std::string identifier() const;

    /**
     * @brief toString
     * @return
     */
    virtual std::string toString() const = 0;

    /**
     * @brief fromJSON
     * @param file
     * @return
     */
    virtual bool fromJSON(std::string &file) = 0;

    std::string jsonFile() const;
    void setJsonFile(const std::string &jsonFile);

    template<typename T>
    bool setConfigParameter(const std::string &id, const std::vector<T> value) {
        Json::Value conf = getConfigFile();
        if(!conf.empty()) {
            conf[identifier()][id].clear();
            for(T v : value) {
                conf[identifier()][id].append(v);
            }
            return updateConfigFile(conf);
        }
        return false;
    }

    template<typename T>
    bool setConfigParameter(const std::string &id, const T value) {
        Json::Value conf = getConfigFile();
        if(!conf.empty()) {
            conf[identifier()][id] = value;
            return updateConfigFile(conf);
        }
        return false;
    }

protected:
    /**
     * @brief ConfigContainer
     * @param usage
     * @param help
     */
    ConfigContainer(const std::string &identifier,
                    const std::string &usage = "No usage text provided, I'm sorry.",
                    const std::string &help = "No help text provided, I'm sorry.");

    Json::Value readJSON(const std::string &file);

    bool writeJSON(const Json::Value &json) const;

    Json::Value getConfigFile();
    bool updateConfigFile(const Json::Value &newConfig) const;

    /**
     * @brief ~ConfigContainer
     */
    virtual ~ConfigContainer();

private:
    /**
     * @brief mUsageText
     */
    std::string mUsageText;

    /**
     * @brief mHelpText
     */
    std::string mHelpText;

    /**
     * @brief mIdentifier
     */
    std::string mIdentifier;

    /**
     * @brief mJsonFile
     */
    std::string mJsonFile;
};

template <typename T>
cv::Ptr<T> config_cast(cv::Ptr<ConfigContainer> other) {
    try {
        cv::Ptr<T> result = other.dynamicCast<T>();
        if(result == nullptr) {
            throw std::bad_cast();
        } else {
            return result;
        }
    } catch(std::bad_cast) {
        throw;
    }
    throw std::bad_cast();
}


}
