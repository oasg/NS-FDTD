#include "HairMultilayerLongitudinalModel.h"
#include <filesystem>
HairMultilayerLongitudinalModel::HairMultilayerLongitudinalModel(std::shared_ptr<TYPE::Field> f)
: FazzyModel(f), ep1(1.55*1.55*EPSILON_0_S), ep2(EPSILON_0_S), cuticle_width(0.5), cmc_width(0.06)
{
    num_layers = 6;
    cuticle_alpha = 5.0;
    

}

string HairMultilayerLongitudinalModel::mkdir(string root)
{
    std::filesystem::create_directory((root + "HairMultilayerLongitudinalModel").c_str());
	string name;

    std::filesystem::create_directory((root + "HairMultilayerLongitudinalModel/incidenceLayer").c_str());		//吸収係数ありの場合
	name = "HairMultilayerLongitudinalModel/incidenceLayer/" + mField->getStringCellInfo();

	std::filesystem::create_directory((root + name).c_str());	//ディレクトリの作成

	return name + "/";
}

double HairMultilayerLongitudinalModel::calcSIG(const double& x, const double& y, const double lam, enum INTEG f)
{
    return 0.0;
}

// 判断给定点 (x, y) 是否在旋转长方形内
// 参数说明：
//   x, y      : 要判断的点
//   cx, cy    : 长方形中心坐标
//   width     : 长方形的宽度
//   height    : 长方形的高度
//   k         : 斜率，表示长方形相对于水平轴的倾斜度
bool isPointInRotatedRectangle(double x, double y,
                               double cx, double cy,
                               double width, double height,
                               double theta)
{
    
    // 将点平移，使长方形中心为原点
    double dx = x - cx;
    double dy = y - cy;

    // 对点进行逆时针旋转 -theta（即把长方形旋转回水平）
    // 旋转矩阵为：
    // [ cos(theta)   sin(theta)]
    // [-sin(theta)   cos(theta)]
    double localX =  std::cos(theta) * dx + std::sin(theta) * dy;
    double localY = -std::sin(theta) * dx + std::cos(theta) * dy;

    // 判断转换后的点是否在边界内
    if (std::abs(localX) <= width / 2.0 && std::abs(localY) <= height / 2.0) {
        return true;
    }
    return false;
}

double HairMultilayerLongitudinalModel::calcEPS(const double& x, const double& y, enum INTEG f)
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
    double cx =2*rb;
    double theta = cuticle_alpha*(M_PI/180.0);
    double cy = rb*sin(theta)/2.0+cuticle_base;
    for(int i = 0;i<num_layers+6;i++){
        if( my < cuticle_base + cmc_width){
            return ep2;
        }
        cx -= (cuticle_thick+cmc_thick) / sin(theta);
        if(isPointInRotatedRectangle(mx,my,cx,cy,rb*5,cuticle_thick,theta)){
            return ep1;
        }
        
    }


	return ep2;
}