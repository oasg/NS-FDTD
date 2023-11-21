#include "Solver.h"
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>
#include <math.h>
#include <stdio.h> 
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <omp.h>
#include<glad/glad.h>

//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _USE_MATH_DEFINES

Solver::Solver()
	:H_S(1.0), DT_S(1.0)
{
	//mField = new Field(2038, 2038, 2, 20); //width, height, ��h, Npml
	//mField = new Field(1700, 1700, 5, 20);
	//mField = new Field(2500, 2500, 10, 20);
	//mField = new Field(1845, 1845, 5, 20);
	//mField = new Field(1865, 1865, 5, 20);

	/****************** �L���[�e�B�N�������̂� ******************/
	/* Field�����T�C�Y                                          */
	/* Field(8000, 8000, 5, 10) Field(16000, 8000, 10, 10) �Ȃ� */
	/************************************************************/
	//mField = new Field(1000, 1000, 10, 8); //1��
	//mField = new Field(1002, 1002, 6, 8);//2
	
	//mField = new Field(1002, 1002, 3, 8);//3
	//mField = new Field(3000, 3000, 4, 16);//4
	//mField = new Field(1000, 1000, 4, 8);

	mField = new Field(4000, 4000, 10, 10);

	//LambdaRange    = Range<double>(Nano_S(400), Nano_S(600), Nano_S(30));
	//LambdaRange    = Range<double>(Nano_S(100), Nano_S(100), Nano_S(5)); // ��
	//LambdaRange = Range<double>(Nano_S(50), Nano_S(50), Nano_S(5)); //5
	//80 //6
	//120 //7
	//200 //8
	//400 //9
	LambdaRange = Range<double>(Nano_S(380), Nano_S(700), Nano_S(5)); 
	WaveAngleRange = Range<int>   (-90, 0, 5);




	time = 0;
	// T = 1/f = ��/c
	maxStep  = 20000; // t/T == 100
	//mField->sig = false;		//�z���W���Ђ̗L���@�L�Ftrue / ���Ffalse (FazzyHair_incidence(Layer)Model�̂ݑI���A ���̑��̏ꍇfalse)
	mField->sig = true;
	n_s     = new double[mField->getNcel()];	//���ܗ�
	Sig_hair = new double[mField->getNcel()];	//�z����


	//mModel	= new FazzySlabModel(mField);
	//mModel	= new FazzyMieModel(mField);

	//mModel = new BuprestidaeModel(mField, Inv_Nano_S(lambda_s));
	//mModel = new BuprestidaeModelWithNoise(mField, Inv_Nano_S(lambda_s));
	mModel	= new FazzyHair_incidenceLayerModel_try(mField);
	//mModel	= new FazzyHair_incidenceLayerModel(mField);
	//mModel	= new FazzyHair_normalModel(mField);
	//mModel	= new FazzyHair_NONcuticleModel(mField);

	//mModel = new BuprestidaeModelWithNoise2nd(mField,Inv_Nano_S(lambda_s));
	//mModel = new BuprestidaeModelsmooth2nd(mField, Inv_Nano_S(lambda_s));
	//mModel = new BuprestidaeModelSmooth24(mField, Inv_Nano_S(lambda_s));



	SetWaveParameter(LambdaRange.MIN());
	wave_angle = WaveAngleRange.MIN();


	cout << "lambda =" << lambda_s << endl;

	DataDir		=  "dataset";
	//DataDir = "..\\Data\\Set\\";
	WorkingDir  =  "simulation_dir";

	cout << "Solver Constructor" << endl;

#ifdef _OPENMP
	cout << "OpenMP used" << endl;
	cout << "The number of processors is " << omp_get_num_procs() << endl;
	cout << "OpenMP : Enabled (Max # of threads = " << omp_get_max_threads() << ")" << endl;
#else
	cout << "OpenMP not used" << endl;
#endif
}


Solver::~Solver(){
	delete[] n_s;
	delete[] Sig_hair;
	delete mModel;
	delete mField;
	cout << "Solver Destructor" << endl;
}

//Bilinear Interpolation���  �����l�̔z��ԍ�������(�l������̔�Œl���o��)
double Solver::bilinear_interpolation(complex<double> *p, double x, double y){
	int i = floor(x);
	int j = floor(y);
	double dx = (x - 1.0*i);	//�����_�ȉ��̒l
	double dy = (y - 1.0*j);	

	return     norm(p[index(i,    j)]) * (1.0-dx)*(1.0-dy)
	         + norm(p[index(i+1,  j)]) * dx*(1.0-dy)
	         + norm(p[index(i,  j+1)]) * (1.0-dx)*dy
	         + norm(p[index(i+1,j+1)]) * dx*dy;
}

//�J���[�}�b�v
Color Solver::color(double phi){
	double range = 2.0;
	Color c;
	double ab_phi = abs(phi);
/*
//	double a = ab_phi < 1 ? (ab_phi <  0.34 ? min(1.0, max(0.0, 3*ab_phi)) : (-1.5*ab_phi+2) ) : 0.5;
	double a = ab_phi < range ? (ab_phi <  range/3.0 ? 3.0/range*ab_phi : (-3.0/4.0/range*ab_phi+1.25) ) : 0.5;
	c.red  = phi > 0 ? a:0;
	c.blue = phi < 0 ? a:0;
	c.green = min(1.0, max(0.0, -3*ab_phi+2));
*/
	//Analyzer�ƒl�����킹��
	double nv = max(0.0, min(1.0, ab_phi));
	// Get color
	if (phi >= 0.75) { c.red = 1.0; c.green = 4.0*(1.0 - nv); c.blue = 0.0; }
	else if (phi >= 0.5) { c.red = 4.0*(nv - 0.5); c.green = 1.0; c.blue = 0.0; }
	else if (phi >= 0.25) { c.red = 0.0; c.green = 1.0; c.blue = 4.0*(0.5 - nv); }
	else { c.red = 0.0; c.green = 4.0*nv; c.blue = 1.0; }
	
	return c;
}



void Solver::MiePrint(complex<double>* p, string name){
	printf("Mie print\n");
	ofstream ofs = WriteOpen(name+"MieANSERex");

	if(ofs) {
		for(int i=0; i<=360; i++){
			double _x = 1.2*lambda_s*cos((i-90)*PI/180) + mField->getNpx()/2;
			double _y = 1.2*lambda_s*sin((i-90)*PI/180) + mField->getNpy()/2;
			double _val = bilinear_interpolation(p,_x,_y);
			ofs << _val << endl;
		}
		printf("Mie print finished\n");
	}
	else{
		printf("No file\n");
	}
}


//---------------------------------------//
//				 ���˔g				 	        //
//--------------------------------------//
//�_�����̓���
void Solver::pointLightSource(complex<double> *p){
	p[index(mField->getNpx()/2, mField->getNpy()/2, +1)] += ray_coef*polar(1.0, w_s*time);
}

//�������̓���
void Solver::linearLightSource(complex<double> *p){
	for(int i=1; i<mField->getNy()-1; i++)
		p[index(5,i, +1)]  +=ray_coef*polar(1.0, w_s*time);	//todo ���E��ɂ�����Ă����̂�?
}

//�U���g�̌v�Z
void Solver::scatteredWave(complex<double> *p, double *eps){
	double rad = wave_angle*M_PI/180;	//���W�A���ϊ�
	double _cos = cos(rad), _sin = sin(rad);	//����v�Z����Ǝ��Ԃ����肻��������,������Ă���
	std::complex<double> MinusOne(-1, 0), I;
	I = sqrt(MinusOne);

	for(int i=mField->getNpml(); i<mField->getNpx(); i++){
		for(int j=mField->getNpml(); j<mField->getNpy(); j++){
			if( N_S(i,j) == 1.0 ) continue;		//���ܗ���1�Ȃ�U���͋N���Ȃ�
			double ikx = k_s*(i*_cos + j*_sin);
			p[index(i,j, +1)] += ray_coef*(1/_pow(N_S(i,j), 2)-1)
				                    *(polar(1.0, ikx-w_s*(time+DT_S))+polar(1.0, ikx-w_s*(time-DT_S))-2.0*polar(1.0, ikx-w_s*time)); 
/*		
			p[index(i, j)] += ray_coef*(EPSILON_0_S / eps[index(i, j)] - 1)*(
				cos(ikx - w_s*(time + 0.5)) + I*sin(ikx - w_s*(time + 0.5))
				- cos(ikx - w_s*(time - 0.5)) - I*sin(ikx - w_s*(time - 0.5))
				);
*/		}
	}
}


//4�ߖT�𒲂ׂ�
bool Solver::neighber(int _x, int _y){
	for(int i=-2; i<=1; i++)
		if(n_s[index(_min(mField->getNx()-1, _max(0, _x+i%2)), _min(mField->getNy()-1, _max(0,_y+(i+1)%2)) )] != n_s[index(_x,_y)]) return false;

	return true;
}

//-------------------------------------------//
//------------St�z�����E--------------------//
//------------------------------------------//
void Solver::absorbing_stRL(complex<double> *p, int X, enum DIRECT offset){
	double u;
	for(int j=1; j<mField->getNpy()-1; j++){
		u = LIGHT_SPEED_S*DT_S/n_s[index(X,j)];
		if(j == 1 || j == mField->getNpy()-2)		// �l���̉��͈ꎟ���z�����E
			p[index(X,j, +1)] = p[index(X+offset,j, 0)] + (1- u)/(1+u)*(p[index(X,j, 0)] - p[index(X+offset,j, +1)]);

		else						//����ȊO�͓񎟌��z�����E
			p[index(X,j, +1)] = - p[index(X+offset,j, -1)] 
								     - (1-u)/(1+u)*(p[index(X,j, -1)] + p[index(X+offset,j, +1)]) 
								     +     2/(1+u)*(p[index(X,j,  0)] + p[index(X+offset,j,  0)]) 
								     + u*u/(1+u)/2*( Dy2(p, X,j, 0)   +  Dy2(p, X+offset,j, 0)	);
												        //  dy^2 ��n     +   dy^2 ��b
	}
}

void Solver::absorbing_stTB(complex<double> *p, int Y, enum DIRECT offset){
	double u;
	for(int i=1; i<mField->getNpx()-1; i++){
		u = LIGHT_SPEED_S*DT_S/n_s[index(i,Y)];

		if(i==1 || i==mField->getNpx()-2)	//�l���̉��͈ꎟ���z�����E
			p[index(i,Y, +1)]    = p[index(i,Y+offset, 0)]    + (1- u)/(1+u)*(p[index(i,Y, 0)]    - p[index(i,Y+offset, +1)]);

		else				//�񎟌��z�����E
			p[index(i,Y, +1)]  = -p[index(i,Y+offset, -1)]    
								  - (1-u)/(1+u)*(p[index(i,Y, -1)] + p[index(i,Y+offset, +1)]) 
							      +     2/(1+u)*(p[index(i,Y, 0)]  + p[index(i,Y+offset, 0)])     
								  + u*u/(1+u)/2*( Dx2(p, i,Y, 0)   + Dx2(p, i,Y+offset, 0) 	);
												  //  dx^2 ��n     +   dx^2 ��b
	}		
}

//-----------------------------------------------------------//
//-------------------------Ns�z�����E------------------------//
//----------------------------------------------------------//
/**���E�̕ǂ�NS�z�����E
** �K�p�z��
** �K�p����x���W
** �E������
*/
void Solver::absorbing_nsRL(complex<double> *p, int X, enum DIRECT offset){
	double kx_s = 1/sqrt(sqrt(2.0)) * k_s;
	double ky_s = sqrt(1 - 1/sqrt(2.0) ) * k_s;
	double u1, u2;
	double w_b  = w_s*DT_S/2;
	double k_b  = k_s*H_S/2;
	double kx_b = kx_s*H_S/2;
	double ky_b = ky_s*H_S/2;

	for (int j = 1; j < mField->getNpy() - 1; j++) {
		u1 = tan(w_b / n_s[index(X, j)]) / tan(k_b);
		u2 = 2 * _pow(sin(w_b / N_S(X, j)), 2) / _pow(sin(ky_b), 2) * (1 - tan(kx_b) / tan(k_b));

		if(j == 1 || j == mField->getNpy()-2)		// �l���̉��͈ꎟ���z�����E
			p[index(X,j, +1)] = p[index(X+offset,j, 0)] + (1- u1)/(1+u1)*(p[index(X,j, 0)] - p[index(X+offset,j, +1)]);

/*		if (j == 1) {
			complex <double> D2_1 = p[index(j + 1, X, 0)] + p[index(mField->getNpy() - 2, X, 0)] - 2.0*p[index(j, X, 0)];
			complex <double> D2_2 = p[index(j + 1, X + offset, 0)] + p[index(mField->getNpy() - 2, X + offset, 0)] - 2.0*p[index(j, X + offset, 0)];
			p[index(j, X, +1)] = -p[index(j, X + offset, -1)]
				- (1 - u1) / (1 + u1)*(p[index(j, X, -1)] + p[index(j, X + offset, +1)])
				+ 2 / (1 + u1)*(p[index(j, X, 0)] + p[index(j, X + offset, 0)])
				+ u2*u2 / (1 + u1) / 2 * (D2_1 + D2_2 + Dt2(p, X, j) + Dt2(p, X + offset, j));
		}
		else if (j == mField->getNpy() - 2) {
			complex <double> D2_1 = p[index(1, X, 0)] + p[index(j - 1, X, 0)] - 2.0*p[index(j, X, 0)];
			complex <double> D2_2 = p[index(1, X + offset, 0)] + p[index(j - 1, X + offset, 0)] - 2.0*p[index(j, X + offset, 0)];
			p[index(j, X, +1)] = -p[index(j, X + offset, -1)]
				- (1 - u1) / (1 + u1)*(p[index(j, X, -1)] + p[index(j, X + offset, +1)])
				+ 2 / (1 + u1)*(p[index(j, X, 0)] + p[index(j, X + offset, 0)])
				+ u2*u2 / (1 + u1) / 2 * (D2_1 + D2_2 + Dt2(p, X, j) + Dt2(p, X + offset, j));
		}
*/		else {						//����ȊO�͓񎟌��z�����E
			p[index(X, j, +1)] = -p[index(X + offset, j, -1)]
				- (1 - u1) / (1 + u1)*(p[index(X, j, -1)] + p[index(X + offset, j, +1)])
				+ 2 / (1 + u1)*(p[index(X, j, 0)] + p[index(X + offset, j, 0)])
				+ u2*u2 / (1 + u1) / 2 * (Dy2(p, X, j, 0) + Dy2(p, X + offset, j, 0) + Dt2(p, X, j) + Dt2(p, X + offset, j));
											//  dy^2 ��n     +   dy^2 ��b
		}
	}
}

/**�㉺�̕ǂ�NS�z�����E
** �K�p�z��
** �K�p����y���W
** �ォ����
*/
void Solver::absorbing_nsTB(complex<double> *p, int Y, enum DIRECT offset){
	double kx_s = 1/sqrt(sqrt(2.0)) * k_s;
	double ky_s = sqrt(1 - 1/sqrt(2.0) ) * k_s;
	double u1, u2;
	double w_b  = w_s*DT_S/2;
	double k_b  = k_s*H_S/2;
	double kx_b = kx_s*H_S/2;
	double ky_b = ky_s*H_S/2;

	for(int i=1; i<mField->getNpx()-1; i++){
		u1 = tan(w_b/n_s[index(i,Y)]) / tan(k_b);
		u2 = 2 * _pow(sin(w_b/n_s[index(i,Y)]), 2) / _pow(sin(ky_b),2) * (1 - tan(kx_b)/tan(k_b));

		if(i==1 || i==mField->getNpx()-2)	//�l���̉��͈ꎟ���z�����E
			p[index(i,Y, +1)] = p[index(i,Y+offset, 0)] + (1- u1)/(1+u1)*(p[index(i,Y, 0)] - p[index(i,Y+offset, +1)]);

/*		if (i == 1) {
			complex <double> D2_1 = p[index(i + 1, Y, 0)] + p[index(mField->getNpx() - 2, Y, 0)] - 2.0*p[index(i, Y, 0)];
			complex <double> D2_2 = p[index(i + 1, Y + offset, 0)] + p[index(mField->getNpx() - 2, Y + offset, 0)] - 2.0*p[index(i, Y + offset, 0)];
			p[index(i, Y, +1)] = -p[index(i, Y + offset, -1)]
				- (1 - u1) / (1 + u1)*(p[index(i, Y, -1)] + p[index(i, Y + offset, +1)])
				+ 2 / (1 + u1)*(p[index(i, Y, 0)] + p[index(i, Y + offset, 0)])
				+ u2*u2 / (1 + u1) / 2 * (D2_1 + D2_2 + Dt2(p, i, Y) + Dt2(p, i, Y + offset));
		}
		else if (i == mField->getNpx() - 2) {
			complex <double> D2_1 = p[index(1, Y, 0)] + p[index(i - 1, Y, 0)] - 2.0*p[index(i, Y, 0)];
			complex <double> D2_2 = p[index(1, Y + offset, 0)] + p[index(i - 1, Y + offset, 0)] - 2.0*p[index(i, Y + offset, 0)];
			p[index(i, Y, +1)] = -p[index(i, Y + offset, -1)]
				- (1 - u1) / (1 + u1)*(p[index(i, Y, -1)] + p[index(i, Y + offset, +1)])
				+ 2 / (1 + u1)*(p[index(i, Y, 0)] + p[index(i, Y + offset, 0)])
				+ u2*u2 / (1 + u1) / 2 * (D2_1 + D2_2 + Dt2(p, i, Y) + Dt2(p, i, Y + offset));
		}
*/		else {				//�񎟌��z�����E
			p[index(i, Y, +1)] = -p[index(i, Y + offset, -1)]
				- (1 - u1) / (1 + u1)*(p[index(i, Y, -1)] + p[index(i, Y + offset, +1)])
				+ 2 / (1 + u1)*(p[index(i, Y, 0)] + p[index(i, Y + offset, 0)])
				+ u2*u2 / (1 + u1) / 2 * (Dx2(p, i, Y, 0) + Dx2(p, i, Y + offset, 0) + Dt2(p, i, Y) + Dt2(p, i, Y + offset));
											//  dx^2 ��n     +   dx^2 ��b
		}
	}		
}

//----------------------------------------------//
//----------------�������E-----------------------//
//-----------------------------------------------//

void Solver::cycle_stRL(complex<double> *p, int X, enum DIRECT offset){
	double u;
	for(int i=1; i<mField->getNy()-1; i++){
		u = LIGHT_SPEED_S*DT_S/n_s[index(X,i)];
			//�񎟌��z�����E
		p[index(X,i, +1)] = - p[index(X+offset,i, -1)]     
							- (1-u)/(1+u)*(p[index(X,i, -1)] + p[index(X+offset,i, +1)])    
							+     2/(1+u)*(p[index(X,i,  0)] + p[index(X+offset,i,  0)]) 
							+ u*u/(1+u)/2*(  Dy2(p,X,i, 0)   +   Dy2(p,X+offset,i, 0) );
												//  dy^2 ��n     +   dy^2 ��b
	}		
}

//---------------------------�`��------------------------------//
//������(0,0), �E�オ(Nx,Ny)
void Solver::draw(Complex *p, Complex *q,std::shared_ptr<GUI::ImageBuffer> img){
	//critical section
	//the calculation threads will be locked
	//the drawing thread will copy the field to the image buffer
	std::lock_guard<std::mutex> lock(field_mutex);
	for (int i = 0; i < mField->getNpx(); i++){
		for (int j = 0; j < mField->getNpy(); j++){
			Color c = color( norm(p[index(i,j)] + q[index(i,j)]) );
			//Color c = color(30.0*(p[index(i,j)].real() + q[index(i,j)].real()));
//			if(j==mField->getNpy()/2 || i==mField->getNpx()/2) glColor3d(1,1,1);
			img->Write(i, j, c.red*255, c.green*255, c.blue*255);
		}
	}

	draw_model(img);
}

//---------------------------�`��------------------------------//
//������(0,0), �E�オ(Nx,Ny)
void Solver::draw(Complex *p,std::shared_ptr<GUI::ImageBuffer> img){
	//critical section
	//the calculation threads will be locked
	//the drawing thread will copy the field to the image buffer
	std::lock_guard<std::mutex> lock(field_mutex);
	for (int i = 0; i < mField->getNpx(); i++){
		for (int j = 0; j < mField->getNpy(); j++){
			Color c = color( norm(p[index(i,j)]) );
			//Color c = color(30.0*p[index(i,j)].real());
			img->Write(i, j, c.red*255, c.green*255, c.blue*255);
			if(j==mField->getNpy()/2 || i==mField->getNpx()/2) {
				img->Write(i, j, 255, 255, 255);
			}
		}
	}
	draw_model(img);
}

//�U���̂̕`��
void Solver::draw_model(std::shared_ptr<GUI::ImageBuffer> img){
	//already in critical section
	double N = max(mField->getNx(),mField->getNy());
	double ws = img->getWidth()/N;
	double hs = img->getHeight()/N;
	for (int i = 0; i < mField->getNpx(); i++){
		for (int j = 0; j < mField->getNpy(); j++){
			//�}�����E
			const double n = N_S(i, j);	//������,���ܗ������������Ă͂����Ȃ�
			const double s = SIG(i, j);
			img->ColorBlend(i, j, (0.3/(n+s))*255, (0.3/(n+s))*255, (0.3/(n+s))*255);
			if(n == 1.0) continue;	//���ܗ���1�Ȃ�Ƃ΂�	
		}
	}
}

void Solver::modelCheck() {
	//capture("../../model");
	exit(-1);
}

//-----------------�f�[�^�̕ۑ�----------------------//
void Solver::save_data(complex<double> *data, string name){
	ofstream out = WriteOpen(name);

	for(int k=-1; k<2; k++)
		for(int i=0; i<mField->getNx(); i++)
			for(int j=0; j<mField->getNy(); j++)
				out << data[index(i,j, k)] << endl;

}

void Solver::open_data(complex<double> *data, string name){
	ifstream in = ReadOpen(name);

	for(int k=-1; k<2; k++)
		for(int i=0; i<mField->getNx(); i++)
			for(int j=0; j<mField->getNy(); j++){
				if(!in) {throw  "cannot open file" ;}
				in >> data[index(i,j, k)];
			}
}

/*
//OpenCV�֌W
#include <opencv2/opencv.hpp> // �C���N���[�h�t�@�C���w��
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv_lib.hpp> // �ÓI�����N���C�u�����̎w��
#include <opencv2/core/core.hpp>
#pragma comment(lib,"opencv_highgui2411.lib")
#pragma comment(lib,"opencv_core2411.lib")

// @brief ���݂̉�ʂ̏�Ԃ��L���v�`������png�ɕۑ����� //

void Solver::capture(string name)
{
	int width = WINDOW_W, height = WINDOW_H;

	cv::Mat cvmtx(cv::Size(width, height), CV_8UC4, cv::Scalar(0, 0, 0));//���ŏ�����
																		 // �摜�̃L���v�`��
	glReadBuffer(GL_FRONT);// �t�����g��ǂݍ��ޗl�ɐݒ肷��
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // �����l��4
	glReadPixels(0, 0, width, height, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)cvmtx.data);
	//�㉺�t�ɂ���
	cv::flip(cvmtx, cvmtx, 0);
	// �摜�̏����o�� 
	cv::imwrite(DataDir + name + ".jpg", cvmtx);
	cout << "image captured" << endl;
}
*/

//---------------------------------------------//
//--------------PML----------------------------//
//--------------------------------------------//

/*
Color Solver::color2(double a){
	Color c;
	c.red  = max(0.0, min(1.0, a-1.0));
	c.blue = max(0.0, min(-a+1, 1.0));
	c.green = max(0.0, min(1.0, -2*abs(a-1)+1));
	return c;
}

*/