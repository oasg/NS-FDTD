#include "model_builder.hpp"
#include <fstream>
#include <string>
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
    //build field
    auto field = _json_data["field"];
    if(!field.empty()){
        try{
            int fieldx = field["size[nm]"][0];
            int fieldy = field["size[nm]"][1];
            auto cell_size = field["cell_size[nm]"].template get<double>();
            int npml = field["boundary_size"];
            mField = new TYPE::Field(fieldx, fieldy, cell_size, npml);
        }catch (const nlohmann::json::exception& e){
            std::cout << e.what() << '\n';
            return false;
        }
    }else return false;
    auto model = _json_data["use_model"];
    if(!model.empty()){
        if(model["type"] == "FazzyHair_incidenceLayerModel"){
            mModel = new FazzyHair_incidenceLayerModel(mField);
        }else{
            return false;
        }
    }else return false;
    // auto WaveParameter = _json_data["WaveParameter"];
    // if(!WaveParameter.empty()){
        
    // }else return false;
    return true;

}
