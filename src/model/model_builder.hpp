#ifndef MODEL_MODEL_BUILDER_HPP
#define MODEL_MODEL_BUILDER_HPP
#include <iostream>
#include <nlohmann/json.hpp>
#include "FazzyModel.h"
#include "type/Field.h"
#include "type/Range.h"
class ModelBuilder{
public:
    ModelBuilder(std::string path);
    ~ModelBuilder();
    bool buildModel();
    std::shared_ptr<TYPE::Field> getField();
    std::shared_ptr<FazzyModel> getModel();
    TYPE::Range<double> getWaveParameterRange(){return WaveParameterRange;};
    TYPE::Range<double> getWaveAngleRange(){return WaveAngleRange;};
private:
    bool enable = false;
    std::string path;
    nlohmann::json _json_data;
	std::shared_ptr<TYPE::Field> mField;
	std::shared_ptr<FazzyModel> mModel;
    TYPE::Range<double> WaveParameterRange;
    TYPE::Range<double> WaveAngleRange;
};
#endif