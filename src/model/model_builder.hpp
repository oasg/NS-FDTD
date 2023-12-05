#ifndef MODEL_MODEL_BUILDER_HPP
#define MODEL_MODEL_BUILDER_HPP
#include <iostream>
#include <nlohmann/json.hpp>
#include "FazzyModel.h"
#include "type/Field.h"

class ModelBuilder{
public:
    ModelBuilder(std::string path);
    ~ModelBuilder();
    bool buildModel();
private:
    std::string path;
    nlohmann::json _json_data;
    FazzyModel	*mModel;
	TYPE::Field* mField;	//フィールド

};
#endif