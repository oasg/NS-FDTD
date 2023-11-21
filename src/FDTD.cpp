#include "FDTD.h"
static double min_lambda = 200*_pow(10,-9);		//���ׂ�g���͈̔�
static double max_lambda = 800*_pow(10,-9);

FDTD::FDTD()
:Solver()
{
	phi  = new complex<double>[3*mField->getNcel()];		//�̈�m��
	np   = new double[mField->getNcel()];		//�v�Z�p�萔

	for(int i=0; i<3*mField->getNcel(); i++)	phi[i] = 0;				//������

	for(int i=0; i<mField->getNcel(); i++)	np[i] = 0;

	cout << "FDTD Constructor" << endl;
}

FDTD::~FDTD(){
	delete[] phi;
	delete[] np;
	cout << "FDTD Destructor" << endl;
}

void FDTD::Initialize(){
	for(int i=0; i<3*mField->getNcel(); i++)	phi[i] = 0;				//������
	time = 0;
}

void FDTD::Initialize(double _lambda)
{
	for(int i=0; i<3*mField->getNcel(); i++)
		phi[i] = 0;				//������
	time = 0;
}

bool FDTD::calc(){
	return true;
}

//�`��
void FDTD::draw(std::shared_ptr<GUI::ImageBuffer> img){
	Solver::draw(phi,img);
}