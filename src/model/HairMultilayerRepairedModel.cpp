#include "HairMultilayerRepairedModel.h"
#include <filesystem>

HairMultilayerRepairedModel::HairMultilayerRepairedModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2)
{
    num_layers = 6;

}

string HairMultilayerRepairedModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairRepairedModelLargeDis").c_str());
	string name;

    std::filesystem::create_directory((root + "HairRepairedModelLargeDis/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairRepairedModelLargeDis/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerRepairedModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerRepairedModel::calcEPS(const double& x, const double& y, enum INTEG f)
{
    double mx = x - mField->getNpml(); //計算領域内へ写像
	double my = y - mField->getNpml();
	if (mx < 0 || my < 0 || mx >= mField->getNx() || my >= mField->getNy()) return ep2;	//PML層
    
    //upper bound
    const double ub = mField->getNpy()- mField->getNpml();
    const double rb = mField->getNpx()- mField->getNpml();

    //oil eps
    double ep3 = 1.45*1.45*EPSILON_0_S;

    double cortex_thick = mField->nanoToCell(1.7*1000.0);
	if (my <= cortex_thick)		return ep1;		//毛皮質部分

    double cuticle_base = cortex_thick;
    double firm_thick = mField->nanoToCell((cmc_width+cuticle_width)*1000.0);
    double cmc_thick = mField->nanoToCell(cmc_width*1000.0);
    double cuticle_thick = mField->nanoToCell(cuticle_width*1000.0);
    for(int i = 0;i<num_layers;i++){
        double base = cuticle_base + i*firm_thick;
        if(my>=base&&my <= base + cmc_thick){
            return ep3;
        }
        if(my>=base + cmc_thick&&my <= base + cmc_thick + cuticle_thick){
            return ep1;
        }


    }
	return ep2;
}




HairMultilayerRepairedLargeDisPerlinModel::HairMultilayerRepairedLargeDisPerlinModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2),noise_scale(0.05),noise_strength(2.0)
{
    num_layers = 6;
    for(int i = 0;i<num_layers;i++){
        perlins.push_back(siv::PerlinNoise(123+2*i));
        perlins.push_back(siv::PerlinNoise(123+2*i+1));
    }
}

string HairMultilayerRepairedLargeDisPerlinModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairRepairedModelLargeDisPerlin").c_str());
	string name;

    std::filesystem::create_directory((root + "HairRepairedModelLargeDisPerlin/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairRepairedModelLargeDisPerlin/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerRepairedLargeDisPerlinModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerRepairedLargeDisPerlinModel::calcEPS(const double& x, const double& y, enum INTEG f)
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
    //oil eps
    double ep3 = 1.45*1.45*EPSILON_0_S;
    for(int i = 0;i<num_layers;i++){
        double base = cuticle_base + i*firm_thick;
        double dy1 = perlins[2*i].noise2D(mx*noise_scale,my*noise_scale)*10;
        double dy2 = perlins[2*i+1].noise2D(mx*noise_scale,my*noise_scale)*10;
        if(my+dy1>=base&&my+dy1 <= base + cmc_thick){
            return ep3;
        }
        if(my+dy2>=base + cmc_thick&&my+dy2 <= base + cmc_thick + cuticle_thick){
            return ep1;
        }
    }
	return ep2;
}

