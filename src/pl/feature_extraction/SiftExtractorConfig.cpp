//
// Created by Simon Hofmann on 19.11.15.
//

#include <sstream>
#include "SiftExtractorConfig.h"

namespace pl {


SiftExtractorConfig::SiftExtractorConfig(const std::string &identifier,
                                         int nBestFeatures,
                                         int nOctaveLayers,
                                         double contrastThreshold,
                                         double edgeThreshold,
                                         double sigma,
                                         bool augment)
    :
        FeatureConfig(identifier,
                      augment),
        mBestFeatures(nBestFeatures),
        mOctaves(nOctaveLayers),
        mContrastThresh(contrastThreshold),
        mEdgeThresh(edgeThreshold),
        mSigma(sigma),
        mAugment(augment)
{
}


SiftExtractorConfig::~SiftExtractorConfig()
{

}


std::string SiftExtractorConfig::toString() const
{
    std::stringstream configString;

    configString << "Best features: " << getBestFeatures() << std::endl
                 << "Octaves: " << getOctaves() << std::endl
                 << "Contrast threshold: " << getContrastThresh() << std::endl
                 << "Edge threshold: " << getEdgeThresh() << std::endl
                 << "Sigma: " << getSigma() << std::endl
                 << "Augment: ";
    if(mAugment == true) {
        configString << "true" << std::endl;
    } else {
        configString << "false" << std::endl;
    }

    return configString.str();
}

bool SiftExtractorConfig::fromJSON(std::string &file)
{
    Json::Value root = readJSON(file);

    if(root.empty()) {
        return false;
    } else {
        const Json::Value params = root[identifier()];

        mBestFeatures = params.get(varName(mBestFeatures), 0).asInt();
        mOctaves = params.get(varName(mOctaves), 3).asInt();
        mContrastThresh = params.get(varName(mContrastThresh), 0.04).asDouble();
        mEdgeThresh = params.get(varName(mEdgeThresh), 10.0).asDouble();
        mSigma = params.get(varName(mSigma), 1.6).asDouble();
        mAugment = params.get(varName(mAugment), true).asBool();

        return true;
    }
}


}
