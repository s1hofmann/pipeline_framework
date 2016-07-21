#include "trainingprocessor.h"

TrainingProcessor::TrainingProcessor(int argc, char *argv[])
{
    pl::ArgumentProcessor ap("train");
    ap.addArgument("c", "Pipeline config.", false);
    ap.addArgument("i", "Label file.", false);
    ap.addArgument("e", "Encoding", true, {"vlad"});
    ap.addArgument("f", "Feature descriptor.", false, {"sift"});
    ap.addArgument("m", "Feature mask.", true, {"vessel"});
    ap.addArgument("r", "Dimensionality reduction.", true, {"pca"});
    ap.addArgument("l", "Learning method.", false, {"sgd"});
    ap.addSwitch("d", "Debug mode");

    try {
        mArguments = ap.parse(argc, argv);
    } catch(const pl::CommandLineError &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << ap.help() << std::endl;
        throw;
    }
}

int TrainingProcessor::run()
{
    // Create pipeline config first
    cv::Ptr<pl::PipelineConfig> pipeCfg = new pl::PipelineConfig("global");
    std::string file = mArguments["c"];
    pipeCfg->fromJSON(file);

    pl::FileLogger logger(pipeCfg->getLogFile());
    pl::ConsoleLogger console;

    bool debugMode = !mArguments["d"].empty();

    pipeCfg->setDebugMode(debugMode);

    if(debugMode) {
        console.inform("Assembling pipeline(s).");
    }

    // And the actual pipeline objects
    // Pipeline which takes care of preprocessing and feature extraction for text classification
    try {
        pl::PipeLine trainPipe(pipeCfg);

        // Create a feature detector / descriptor to the pipeline
        cv::Ptr<pl::ConfigContainer> feCfg;
        std::string featureDetector = mArguments["f"];
        if(!featureDetector.compare("sift")) {
            feCfg = new pl::SiftExtractorConfig(featureDetector);
            feCfg->fromJSON(file);
        }

        if(!mArguments["m"].empty()) {
            // With an additional mask generator
            std::string maskType = mArguments["m"];
            if(!maskType.compare("vessel")) {
                cv::Ptr<pl::VesselMask> vesselMask = new pl::VesselMask(maskType);
                vesselMask->fromJSON(file);
                trainPipe.addFeatureExtractionStep(new pl::SiftExtractor(feCfg), vesselMask);
            }
        } else {
            // Without mask
            trainPipe.addFeatureExtractionStep(new pl::SiftExtractor(feCfg), cv::Ptr<pl::MaskGenerator>());
        }

    //    Dimensionality reduction
        std::string reductionMethod = mArguments["r"];
        if(!reductionMethod.compare("pca")) {
            cv::Ptr<pl::PCAConfig> pca = new pl::PCAConfig(reductionMethod);
            pca->fromJSON(file);
            trainPipe.addDimensionalityReductionStep(new pl::PCAStep(pca));
        }

    //    Encoding
        std::string encodingMethod = mArguments["e"];
        if(!encodingMethod.compare("vlad")) {
            cv::Ptr<pl::VladConfig> encoding = new pl::VladConfig(encodingMethod);
            encoding->fromJSON(file);
            trainPipe.addEncodingStep(new pl::VladEncodingStep(encoding));
        }

    //    Learning algorithm
        std::string learningAlgo = mArguments["l"];
        if(!learningAlgo.compare("sgd")) {
            cv::Ptr<pl::SGDConfig> sgd = new pl::SGDConfig(learningAlgo);
            sgd->fromJSON(file);
            trainPipe.addClassificationStep(new pl::SGDStep(sgd));
        }

        trainPipe.showPipeline();
        pl::FileUtil fu;

        std::pair<std::vector<std::string>, std::vector<int>> filesWithLabels = fu.getFilesFromLabelFile(mArguments["i"]);
        trainPipe.train(filesWithLabels.first, filesWithLabels.second);
    } catch(const pl::ClusterError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_CLUSTERING_ERROR;
    } catch(const pl::DimensionalityReductionError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_DIM_ERROR;
    } catch(const pl::EncodingError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_ENCODING_ERROR;
    } catch(const pl::FeatureExError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_FEATURE_EX_ERROR;
    } catch(const pl::IOError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_IO_ERROR;
    } catch(const pl::MLError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_ML_ERROR;
    } catch(const pl::OCVError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_OPENCV_ERROR;
    } catch(const cv::Exception &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_OPENCV_ERROR;
    } catch(const pl::BaseError &e) {
        console.report(e.what());
        logger.report(e.what());
        return ReturnValues::RETURN_ERROR;
    }
    console.inform("Done.");
    return ReturnValues::RETURN_SUCCESS;
}
