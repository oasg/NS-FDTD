#include "Simulator.h"
#include "model/HairMultilayerModel.h"
#include "cmdline.h"
#include <iostream>


int main(int argc, char *argv[]){
    cmdline::parser parser;
    parser.add<std::string>("model", 'm', "model file path", true, "");
    parser.parse_check(argc, argv);
    std::cout << parser.get<std::string>("model") << std::endl;
    if(parser.exist("model")){
        auto builder = std::make_shared<ModelBuilder>(parser.get<std::string>("model"));
        builder->buildModel();
        auto sim = std::make_shared<Simulator>(TYPE::sim_type::NsFTDT_TM,builder);
        while (true)
        {
            int res = sim->calc();
            if(res==0){
                break;
            }
        }
    }
    else{
        std::cout << "Please specify the model file path" << std::endl;
    }
}