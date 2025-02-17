#include "HairMultilayerDamagedModel.h"
#include <filesystem>
HairMultilayerDamagedModel::HairMultilayerDamagedModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.06)
{
    num_layers = 3;

}

string HairMultilayerDamagedModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairDamagedModel").c_str());
	string name;

    std::filesystem::create_directory((root + "HairDamagedModel/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairDamagedModel/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerDamagedModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerDamagedModel::calcEPS(const double& x, const double& y, enum INTEG f)
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
    
    double damaged_base = 0;
    double field_width = mField->getNx();
    
    for(int i = 0;i<num_layers;i++){
        double base = cuticle_base + i*firm_thick;
        if(my>=base&&my <= base + cmc_thick){
            return ep2;
        }
        if(my>=base + cmc_thick&&my <= base + cmc_thick + cuticle_thick){
            if(i == 1 && mx > field_width*0.5){
                return ep2;
            }else return ep1;
        }
        damaged_base = base + cmc_thick + cuticle_thick;
    }
    
    double slope1 = 0.04;
    double distance = fabs(my - damaged_base-30 - slope1 * mx) / sqrt(1 + slope1 * slope1);
    if(distance < cuticle_thick /2.0 ){
        return ep1;
    }
    
    double slope2 = 0.1;
    double distance2 = fabs(my - damaged_base-firm_thick-30 - slope2 * mx) / sqrt(1 + slope2 * slope2);
    if(distance2 < cuticle_thick /2.0 && mx < field_width*0.5){
        return ep1;
    }

    double slope3 = 0.25;
    double distance3 = fabs(my - damaged_base - firm_thick*2-30 - slope3 * mx) / sqrt(1 + slope3 * slope3);
    if(distance3 < cuticle_thick /2.0 && mx < field_width*0.2 ){
        return ep1;
    }
	return ep2;
}


//cmc 0.2
HairMultilayerDamagedLargeDisModel::HairMultilayerDamagedLargeDisModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2)
{
    num_layers = 6;

}

string HairMultilayerDamagedLargeDisModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairDamagedModelLargeDis").c_str());
	string name;

    std::filesystem::create_directory((root + "HairDamagedModelLargeDis/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairDamagedModelLargeDis/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerDamagedLargeDisModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerDamagedLargeDisModel::calcEPS(const double& x, const double& y, enum INTEG f)
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




HairMultilayerDamagedLargeDisPerlinModel::HairMultilayerDamagedLargeDisPerlinModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2),noise_scale(0.05),noise_strength(2.0)
{
    num_layers = 6;
    for(int i = 0;i<num_layers;i++){
        perlins.push_back(siv::PerlinNoise(123+2*i));
        perlins.push_back(siv::PerlinNoise(123+2*i+1));
    }
}

string HairMultilayerDamagedLargeDisPerlinModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairDamagedModelLargeDisPerlin").c_str());
	string name;

    std::filesystem::create_directory((root + "HairDamagedModelLargeDisPerlin/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairDamagedModelLargeDisPerlin/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerDamagedLargeDisPerlinModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerDamagedLargeDisPerlinModel::calcEPS(const double& x, const double& y, enum INTEG f)
{
    double mx = x - mField->getNpml(); //計算領域内へ写像
	double my = y - mField->getNpml();
	if (mx < 0 || my < 0 || mx >= mField->getNx() || my >= mField->getNy()) return ep2;	//PML層
    
    //upper bound
    const double ub = mField->getNpy()- mField->getNpml();
    const double rb = mField->getNpx()- mField->getNpml();

    auto cortexbasenoise = siv::PerlinNoise(123456);
    double cortex_thick = mField->nanoToCell(1.7*1000.0);
	if (my <= cortex_thick+cortexbasenoise.noise2D(mx*noise_scale,my*noise_scale)*8)		
        return ep1;		//毛皮質部分

    double cuticle_base = cortex_thick;
    double firm_thick = mField->nanoToCell((cmc_width+cuticle_width)*1000.0);
    double cmc_thick = mField->nanoToCell(cmc_width*1000.0);
    double cuticle_thick = mField->nanoToCell(cuticle_width*1000.0);
    for(int i = 0;i<num_layers;i++){
        double base = cuticle_base + i*firm_thick;
        double dy1 = perlins[2*i].noise2D(mx*noise_scale,my*noise_scale)*8;
        double dy2 = perlins[2*i+1].noise2D(mx*noise_scale,my*noise_scale)*8;
        if(my+dy1>=base&&my+dy1 <= base + cmc_thick){
            return ep2;
        }
        if(my+dy2>=base + cmc_thick&&my+dy2 <= base + cmc_thick + cuticle_thick){
            return ep1;
        }


    }
	return ep2;
}




HairMultilayerDamagedLackLayerModel::HairMultilayerDamagedLackLayerModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2),noise_scale(0.05),noise_strength(2.0)
{
    num_layers = 6;
}

string HairMultilayerDamagedLackLayerModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairMultilayerDamagedLackLayerModel").c_str());
	string name;

    std::filesystem::create_directory((root + "HairMultilayerDamagedLackLayerModel/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairMultilayerDamagedLackLayerModel/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerDamagedLackLayerModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerDamagedLackLayerModel::calcEPS(const double& x, const double& y, enum INTEG f)
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

        if(my>=base + cmc_thick&&my <= base + cmc_thick + cuticle_thick&& i%2 == 0){
            return ep1;
        }


    }
	return ep2;
}


HairMultilayerDamagedTiltModel::HairMultilayerDamagedTiltModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2),noise_scale(0.05),noise_strength(2.0)
{
    num_layers = 6;
}

string HairMultilayerDamagedTiltModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairMultilayerDamagedTiltModel").c_str());
	string name;

    std::filesystem::create_directory((root + "HairMultilayerDamagedTiltModel/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairMultilayerDamagedTiltModel/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerDamagedTiltModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerDamagedTiltModel::calcEPS(const double& x, const double& y, enum INTEG f)
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
    for(int i = 0;i<num_layers-2;i++){
        double base = cuticle_base + i*firm_thick;
        if(my>=base&&my <= base + cmc_thick){
            return ep2;
        }

        if(my>=base + cmc_thick&&my <= base + cmc_thick + cuticle_thick){
            return ep1;
        }


    }

    double theta1d = 5;
    double theta = theta1d/180.0*M_PI;
    double cx = rb/2;
    double cy = cuticle_base + firm_thick*4;
    cy = cy + rb*tan(theta)/2.0+cuticle_thick;
    if(isPointInRotatedRectangle(mx,my,cx,cy,rb*5,cuticle_thick,theta)){
        return ep1;
    }
    double theta2d = 10;
    theta = theta2d/180.0*M_PI;
    cy = cuticle_base + firm_thick*5;
    cy = cy + rb*tan(theta)/2.0+cuticle_thick;
    if(isPointInRotatedRectangle(mx,my,cx,cy,rb*5,cuticle_thick,theta)){
        return ep1;
    }

        
	return ep2;
}





HairMultilayerDamagedLackLayerPerlinModel::HairMultilayerDamagedLackLayerPerlinModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2),noise_scale(0.05),noise_strength(2.0)
{
    num_layers = 6;
    for(int i = 0;i<num_layers;i++){
        perlins.push_back(siv::PerlinNoise(123+2*i));
        perlins.push_back(siv::PerlinNoise(123+2*i+1));
    }
}

string HairMultilayerDamagedLackLayerPerlinModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairMultilayerDamagedLackLayerPerlinModel").c_str());
	string name;

    std::filesystem::create_directory((root + "HairMultilayerDamagedLackLayerPerlinModel/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairMultilayerDamagedLackLayerPerlinModel/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerDamagedLackLayerPerlinModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerDamagedLackLayerPerlinModel::calcEPS(const double& x, const double& y, enum INTEG f)
{
    double mx = x - mField->getNpml(); //計算領域内へ写像
	double my = y - mField->getNpml();
	if (mx < 0 || my < 0 || mx >= mField->getNx() || my >= mField->getNy()) return ep2;	//PML層
    
    //upper bound
    const double ub = mField->getNpy()- mField->getNpml();
    const double rb = mField->getNpx()- mField->getNpml();

    double cortex_thick = mField->nanoToCell(1.7*1000.0);
	double dy = perlins[2].noise2D(mx*noise_scale,my*noise_scale)*8;
	if (my <= cortex_thick+dy)		return ep1;		//毛皮質部分

    double cuticle_base = cortex_thick;
    double firm_thick = mField->nanoToCell((cmc_width+cuticle_width)*1000.0);
    double cmc_thick = mField->nanoToCell(cmc_width*1000.0);
    double cuticle_thick = mField->nanoToCell(cuticle_width*1000.0);
    for(int i = 0;i<num_layers;i++){
        double base = cuticle_base + i*firm_thick;
        double dy1 = perlins[2*i].noise2D(mx*noise_scale,my*noise_scale)*8;
        double dy2 = perlins[2*i+1].noise2D(mx*noise_scale,my*noise_scale)*8;
        if(my+dy1>=base&&my+dy1 <= base + cmc_thick){
            return ep2;
        }
        if(my+dy2>=base + cmc_thick&&my+dy2 <= base + cmc_thick + cuticle_thick&& i%2 == 0){
            return ep1;
        }

    }
	return ep2;
}


HairMultilayerDamagedTiltPerlinModel::HairMultilayerDamagedTiltPerlinModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.2),noise_scale(0.05),noise_strength(2.0)
{
    num_layers = 6;
    for(int i = 0;i<num_layers;i++){
        perlins.push_back(siv::PerlinNoise(123+2*i));
        perlins.push_back(siv::PerlinNoise(123+2*i+1));
    }
}

string HairMultilayerDamagedTiltPerlinModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairMultilayerDamagedTiltModel").c_str());
	string name;

    std::filesystem::create_directory((root + "HairMultilayerDamagedTiltModel/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairMultilayerDamagedTiltModel/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerDamagedTiltPerlinModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

double HairMultilayerDamagedTiltPerlinModel::calcEPS(const double& x, const double& y, enum INTEG f)
{
    double mx = x - mField->getNpml(); //計算領域内へ写像
	double my = y - mField->getNpml();
	if (mx < 0 || my < 0 || mx >= mField->getNx() || my >= mField->getNy()) return ep2;	//PML層
    
    //upper bound
    const double ub = mField->getNpy()- mField->getNpml();
    const double rb = mField->getNpx()- mField->getNpml();
    
    double cortex_thick = mField->nanoToCell(1.7*1000.0);

    double dy = perlins[2].noise2D(mx*noise_scale,my*noise_scale)*8;
	if (my <= cortex_thick+dy)		return ep1;		//毛皮質部分

    double cuticle_base = cortex_thick;
    double firm_thick = mField->nanoToCell((cmc_width+cuticle_width)*1000.0);
    double cmc_thick = mField->nanoToCell(cmc_width*1000.0);
    double cuticle_thick = mField->nanoToCell(cuticle_width*1000.0);
    for(int i = 0;i<num_layers-2;i++){
        double dy1 = perlins[2*i].noise2D(mx*noise_scale,my*noise_scale)*8;
        double dy2 = perlins[2*i+1].noise2D(mx*noise_scale,my*noise_scale)*8;
        double base = cuticle_base + i*firm_thick;
        if(my+dy1>=base&&my+dy1 <= base + cmc_thick){
            return ep2;
        }
        if(my+dy2>=base + cmc_thick&&my+dy2 <= base + cmc_thick + cuticle_thick&& i%2 == 0){
            return ep1;
        }
    }

    double theta1d = 5;
    double theta = theta1d/180.0*M_PI;
    double cx = rb/2;
    double cy = cuticle_base + firm_thick*4;
    cy = cy + rb*tan(theta)/2.0+cuticle_thick;
    double dy1 = perlins[2*(num_layers-2)].noise2D(mx*noise_scale,my*noise_scale)*8;
    double dy2 = perlins[2*(num_layers-2)+1].noise2D(mx*noise_scale,my*noise_scale)*8;
    if(isPointInRotatedRectangleYRandom(mx,my,cx,cy,dy1,dy2,rb*5,cuticle_thick,theta)){
        return ep1;
    }
    double theta2d = 10;
    theta = theta2d/180.0*M_PI;
    cy = cuticle_base + firm_thick*5;
    cy = cy + rb*tan(theta)/2.0+cuticle_thick;
    dy1 = perlins[2*(num_layers-1)].noise2D(mx*noise_scale,my*noise_scale)*8;
    dy2 = perlins[2*(num_layers-1)+1].noise2D(mx*noise_scale,my*noise_scale)*8;
    if(isPointInRotatedRectangleYRandom(mx,my,cx,cy,dy1,dy2,rb*5,cuticle_thick,theta)){
        return ep1;
    }
	return ep2;
}

