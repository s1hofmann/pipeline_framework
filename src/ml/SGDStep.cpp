#include "SGDStep.h"


namespace pl {


SGDStep::SGDStep(const cv::Ptr<SGDConfig> config,
                 const std::string &info)
    :
        MLStep(config,
               info)
{
}


SGDStep::~SGDStep()
{
}


cv::Mat SGDStep::train(const cv::Mat &input,
                       const cv::Mat &param) const
{
    if(input.empty() || param.empty()) {
        exit(-1);
    }

    cv::Mat1d dInput;
    cv::Mat1d dParam;
    if(!input.type() == CV_64FC1) {
        input.convertTo(dInput, CV_64FC1);
    } else {
        dInput = input;
    }

    //Here param is supposed to hold label data
    if(!param.type() == CV_64FC1) {
        param.convertTo(dParam, CV_64FC1);
    } else {
        dParam = param;
    }

    double lambda = this->mConfig.dynamicCast<SGDConfig>()->lambda();
    double learningRate = this->mConfig.dynamicCast<SGDConfig>()->learningRate();
    double epsilon = this->mConfig.dynamicCast<SGDConfig>()->epsilon();
    double multiplier = this->mConfig.dynamicCast<SGDConfig>()->multiplier();
    double bias = this->mConfig.dynamicCast<SGDConfig>()->bias();
    vl_size iterations = this->mConfig.dynamicCast<SGDConfig>()->iterations();
    vl_size maxIterations = this->mConfig.dynamicCast<SGDConfig>()->maxIterations();

    cv::Mat1d model = this->mConfig.dynamicCast<SGDConfig>()->model();

    cv::Ptr<VlFeatWrapper::SGDSolver> solver = new VlFeatWrapper::SGDSolver(dInput,
                                                                            dParam,
                                                                            lambda);

    if(!model.empty()) { solver->setModel(model); }

    if(bias > 0) { solver->setBias(bias); }
    if(learningRate > 0) { solver->setBiasLearningRate(learningRate); }
    if(epsilon > 0) { solver->setEpsilon(epsilon); }
    if(multiplier > 0) { solver->setBiasMultiplier(multiplier); }
    if(iterations > 0) { solver->setStartIterationCount(iterations); }
    if(maxIterations > 0) { solver->setMaxIterations(maxIterations); }

    solver->train();
}


cv::Mat SGDStep::run(const cv::Mat &input,
                     const cv::Mat &param) const
{
    cv::Mat1d model, bias;
}


cv::Mat SGDStep::debugTrain(const cv::Mat &input,
                            const cv::Mat &param) const
{
    if(input.empty() || param.empty()) {
        std::cerr << "SGDStep::train: Missing parameters, aborting." << std::endl;
        exit(-1);
    }

    cv::Mat1d dInput;
    cv::Mat1d dParam;
    if(!input.type() == CV_64FC1) {
        std::cout << "Incompatible type of input data, converting." << std::endl;
        input.convertTo(dInput, CV_64FC1);
    } else {
        dInput = input;
    }

    if(!param.type() == CV_64FC1) {
        std::cerr << "Incompatible type of parameter data, converting." << std::endl;
        param.convertTo(dParam, CV_64FC1);
    } else {
        dParam = param;
    }

    double lambda = this->mConfig.dynamicCast<SGDConfig>()->lambda();
    std::cout << "Lambda: " << lambda << std::endl;
    double learningRate = this->mConfig.dynamicCast<SGDConfig>()->learningRate();
    double epsilon = this->mConfig.dynamicCast<SGDConfig>()->epsilon();
    double multiplier = this->mConfig.dynamicCast<SGDConfig>()->multiplier();
    double bias = this->mConfig.dynamicCast<SGDConfig>()->bias();
    vl_size iterations = this->mConfig.dynamicCast<SGDConfig>()->iterations();
    vl_size maxIterations = this->mConfig.dynamicCast<SGDConfig>()->maxIterations();

    cv::Mat1d model = this->mConfig.dynamicCast<SGDConfig>()->model();

    cv::Ptr<VlFeatWrapper::SGDSolver> solver = new VlFeatWrapper::SGDSolver(dInput,
                                                                            dParam,
                                                                            lambda);

    if(!model.empty()) { solver->setModel(model); }

    if(bias > 0) { solver->setBias(bias); }
    if(learningRate > 0) { solver->setBiasLearningRate(learningRate); }
    if(epsilon > 0) { solver->setEpsilon(epsilon); }
    if(multiplier > 0) { solver->setBiasMultiplier(multiplier); }
    if(iterations > 0) { solver->setStartIterationCount(iterations); }
    if(maxIterations > 0) { solver->setMaxIterations(maxIterations); }

    solver->train();
}


cv::Mat SGDStep::debugRun(const cv::Mat &input,
                          const cv::Mat &param) const
{

}


cv::Mat1d SGDStep::load(const std::string &fileName)
{
    cv::FileStorage fs(fileName, cv::FileStorage::READ);

    if ((fs["model"].isNone() || fs["model"].empty()) ||
        (fs["bias"].isNone() || fs["bias"].empty())) {
        std::cout << "Error, unable to load classifier data from file: " << fileName << ". Aborting." << std::endl;
        //TODO: Define proper exit codes
        exit(-1);
    }

    cv::Mat1d model, bias;

    fs["model"] >> model;
    fs["bias"] >> bias;

    cv::Mat1d result;

    cv::hconcat(model, bias, result);

    fs.release();

    return result;
}


void SGDStep::save(const std::string &fileName,
                   const cv::Mat1d &model,
                   const cv::Mat1d &bias)
{
    cv::FileStorage fs(fileName, cv::FileStorage::WRITE);

    fs << "model" << model;
    fs << "bias" << bias;

    fs.release();
}


}
