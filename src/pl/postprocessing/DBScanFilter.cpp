#include "DBScanFilter.h"

namespace pl {

DBScanFilter::DBScanFilter(const cv::Ptr<ConfigContainer> config)
    :
        PostProcessingStep(config)
{
}

DBScanFilter::~DBScanFilter()
{

}

cv::Mat DBScanFilter::train(const cv::Mat &input, const cv::Mat &param) const
{
    cv::Ptr<DBScanConfig> config;
    try {
        config = config_cast<DBScanConfig>(this->mConfig);
    } catch(std::bad_cast) {
        std::stringstream s;
        s << "Wrong config type: " << this->mConfig->identifier();
        throw PostProcessError(s.str(), currentMethod, currentLine);
    }

    if(input.empty()) {
        throw PostProcessError("Missing parameters, input empty.", currentMethod, currentLine);
    }

    DbScan cl;
    std::vector<std::set<int>> clusters;
    clusters = cl.cluster(input.colRange(input.cols - 2,
                                         input.cols),
                          config->minPoints(),
                          config->neighborhoodSize());

    cv::Mat1f allDescriptors;
    //Loop through all clusters, assemble descriptors and classify them
    for(size_t cluster = 0; cluster < clusters.size(); ++cluster) {
        cv::Mat1f clusterDescriptor(clusters[cluster].size(), input.cols);
        int i = 0;
        for(auto idx = clusters[cluster].begin(); idx != clusters[cluster].end(); ++idx) {
            input.row(*idx).copyTo(clusterDescriptor.row(i));
            ++i;
        }

        if(!clusterDescriptor.empty()) {
            if(allDescriptors.empty()) {
                allDescriptors = clusterDescriptor.clone();
            } else {
                cv::hconcat(clusterDescriptor, allDescriptors);
            }
        }
    }

    return allDescriptors.rowRange(0, allDescriptors.rows - 2);
}

cv::Mat DBScanFilter::run(const cv::Mat &input, const cv::Mat &param) const
{
    return this->train(input, param);
}

cv::Mat DBScanFilter::debugTrain(const cv::Mat &input, const cv::Mat &param) const
{
    return this->train(input, param);
}

cv::Mat DBScanFilter::debugRun(const cv::Mat &input, const cv::Mat &param) const
{
    return this->train(input, param);
}


}