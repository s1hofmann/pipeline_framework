//
// Created by Simon Hofmann on 18.11.15.
//


#pragma once

#include "EncodingStep.h"
#include "VladConfig.h"
#include "descriptorpyramid.h"

#include "../core/clustering/kmeanscluster.h"
#include "../core/encoding/vladencoder.h"


namespace pl {


class VladEncodingStep : public EncodingStep {
public:
    VladEncodingStep(const cv::Ptr<ConfigContainer> config);

    virtual ~VladEncodingStep();

    /**
     * @brief train
     * @param input
     * @param mask
     * @return
     */
    virtual cv::Mat train(const cv::Mat &input,
                          const cv::Mat &param = cv::Mat()) const;

    /**
     * @brief run
     * @param input
     * @param mask
     * @return
     */
    virtual cv::Mat run(const cv::Mat &input,
                        const cv::Mat &param = cv::Mat()) const;

    /**
     * @brief debugTrain
     * @param input
     * @param mask
     * @return
     */
    virtual cv::Mat debugTrain(const cv::Mat &input,
                               const cv::Mat &param = cv::Mat()) const;

    /**
     * @brief debugRun
     * @param input
     * @param mask
     * @return
     */
    virtual cv::Mat debugRun(const cv::Mat &input,
                             const cv::Mat &param = cv::Mat()) const;

private:
    cv::Mat encode(const std::string &encoder, const cv::Mat &data) const;

    cv::Mat encodePyramid(const std::string &encoder, const cv::Mat &data) const;
};


}

