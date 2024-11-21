#include "Simulator.h"
#include "model/HairMultilayerModel.h"
int main(){
    auto builder = std::make_shared<ModelBuilder>("../scene/model/hairmodelRandom.json");
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