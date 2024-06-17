#include "Simulator.h"
#include "model/HairMultilayerModel.h"
int main(){
    auto builder = std::make_shared<ModelBuilder>("/home/oasis/Documents/code/NS-FTDT/scene/model/hairmodel.json");
    builder->buildModel();
    auto sim = std::make_shared<Simulator>(TYPE::sim_type::NsFTDT_TE,builder);
    while (true)
    {
        
        int res = sim->calc();
        if(res==0){
            break;
        }
    }
}