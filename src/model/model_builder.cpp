#include "model_builder.hpp"
#include <fstream>
#include <string>
#include "HairMultilayerModel.h"
#include "HairMultilayerDamagedModel.h"

ModelBuilder::ModelBuilder(std::string path)
{
    std::ifstream ifs(path);
    _json_data = nlohmann::json::parse(ifs);

}

ModelBuilder::~ModelBuilder()
{
}

bool ModelBuilder::buildModel()
{
    // build field
    auto field = _json_data["field"];
    if (!field.empty())
    {
        try
        {
            int fieldx = field["size[nm]"][0];
            int fieldy = field["size[nm]"][1];
            auto cell_size = field["cell_size[nm]"].template get<double>();
            int npml = field["boundary_size"];
            std::cout<<"Field:   x:"<<fieldx<<"  y: "<<fieldy<<std::endl;
            mField = std::make_shared<TYPE::Field>(fieldx, fieldy, cell_size, npml);
        }
        catch (const nlohmann::json::exception &e)
        {
            std::cout << "build field error: " << e.what() << '\n';
            return false;
        }
    }
    else
        return false;
    auto model = _json_data["use_model"];
    if (!model.empty())
    {
        if (model["type"] == "HairMultilayerModel")
        {
            mModel = std::make_shared<HairMultilayerModel>(mField);
        }else if(model["type"] == "HairMultilayerDamagedModel"){
            mModel = std::make_shared<HairMultilayerDamagedModel>(mField);
        }else if(model["type"] == "HairMultilayerDamagedLargeDisModel"){
            mModel = std::make_shared<HairMultilayerDamagedLargeDisModel>(mField);
        }
        else
        {
            std::cout << "build model error: " << model["type"] << '\n';
            return false;
        }
    }
    else
        return false;
    auto WaveParameter = _json_data["WaveParameter"];
    if(!WaveParameter.empty()){
        auto LambdaRange = WaveParameter["LambdaRange"];
        auto wangelRange = WaveParameter["WaveAngleRange"];
        WaveParameterRange = TYPE::Range<double>(LambdaRange[0],LambdaRange[1],LambdaRange[2]);
        WaveAngleRange = TYPE::Range<double>(wangelRange[0],wangelRange[1],wangelRange[2]);
    }else return false;

    return true;
}

std::shared_ptr<TYPE::Field> ModelBuilder::getField()
{
    return mField;
}

std::shared_ptr<FazzyModel> ModelBuilder::getModel()
{
    return mModel;
}
