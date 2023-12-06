#include "FDTD.h"
static double min_lambda = 200*_pow(10,-9);		//ďż˝ďż˝ďż˝×ďż˝gďż˝ďż˝ďż˝ĚÍďż˝
static double max_lambda = 800*_pow(10,-9);

FDTD::FDTD(std::shared_ptr<TYPE::Field> field, std::shared_ptr<FazzyModel> model)
:Solver(field,model)
{
	phi  = new complex<double>[3*mField->getNcel()];		//ďż˝Ěďż˝mďż˝ďż˝
	np   = new double[mField->getNcel()];		//ďż˝vďż˝Zďż˝pďż˝č

	for(int i=0; i<3*mField->getNcel(); i++)	phi[i] = 0;				//ďż˝ďż˝ďż˝ďż˝ďż˝ďż˝

	for(int i=0; i<mField->getNcel(); i++)	np[i] = 0;

	cout << "FDTD Constructor" << endl;
}

FDTD::~FDTD(){
	delete[] phi;
	delete[] np;
	cout << "FDTD Destructor" << endl;
}

void FDTD::Initialize(){
	for(int i=0; i<3*mField->getNcel(); i++)	phi[i] = 0;				//ďż˝ďż˝ďż˝ďż˝ďż˝ďż˝
	time = 0;
}

void FDTD::Initialize(double _lambda)
{
	for(int i=0; i<3*mField->getNcel(); i++)
		phi[i] = 0;				//ďż˝ďż˝ďż˝ďż˝ďż˝ďż˝
	time = 0;
}

bool FDTD::calc(){
	return true;
}

//ďż˝`ďż˝ďż˝
void FDTD::draw(std::shared_ptr<GUI::ImageBuffer> img){
	Solver::draw(phi,img);
}