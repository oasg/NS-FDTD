#include "HairMultilayerModel.h"
#include <filesystem>
HairMultilayerModel::HairMultilayerModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.06), num_layers(6)
{

}

string HairMultilayerModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairModel").c_str());
	string name;

	if (mField->sig == false) {
		std::filesystem::create_directory((root + "HairModel/incidenceLayer").c_str());				//吸収係数なしの場合
		name = "HairModel/incidenceLayer/" + mField->getStringCellInfo();
	}
	else if (mField->sig == true) {
		std::filesystem::create_directory((root + "HairModel/incidenceLayer_withSig").c_str());		//吸収係数ありの場合
		name = "HairModel/incidenceLayer_withSig/" + mField->getStringCellInfo();
	}

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerModel::calcEPS(const double& x, const double& y, enum INTEG f)
{

	double mx = x - mField->getNpml(); //計算領域内へ写像
	double my = y - mField->getNpml();
	if (mx < 0 || my < 0 || mx >= mField->getNx() || my >= mField->getNy()) return ep2;	//PML層
    
    //upper bound
    const double ub = mField->getNpy()- mField->getNpml();
    const double rb = mField->getNpx()- mField->getNpml();

    double cortex_thick = mField->nanoToCell(1.7*1000.0);
	if (my <= cortex_thick)		return ep1;		//毛皮質部分

    double cuticle_base = cortex_thick;
    double firm_thick = mField->nanoToCell((cmc_width+cuticle_width)*1000.0);
    double cmc_thick = mField->nanoToCell(cmc_width*1000.0);
    double cuticle_thick = mField->nanoToCell(cuticle_width*1000.0);
    for(int i = 0;i<num_layers;i++){
        double base = cuticle_base + i*firm_thick;
        if(my>=base&&my <= base + cmc_thick){
            return ep2;
        }
        if(my>=base + cmc_thick&&my <= base + cmc_thick + cuticle_thick){
            return ep1;
        }


    }
	return ep2;
}