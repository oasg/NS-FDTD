#define _USE_MATH_DEFINES
#include <stdlib.h>
#include "Object.h"
#include "FDTD_TM.h"
#include "Vec.h"
#include <math.h>
#include <cstring>
#include<iostream>

//TODO MEAN?
//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

using namespace std;
const int n_ang = 180;
FDTD_TM::FDTD_TM(std::shared_ptr<TYPE::Field> field, std::shared_ptr<FazzyModel> model)
:Solver(field,model)
{
	//�̈�m��
	Ez  = new complex<double>[3*mField->getNcel()];		//Ez(i,j)      �� Ez(i,j)
	Hx  = new complex<double>[3*mField->getNcel()];		//Hx(i, j+0.5) �� Hx(i,j)
	Hy  = new complex<double>[3*mField->getNcel()];		//Hy(i+0.5, j) �� Hy(i,j) ���Ӗ�����
	Ezx = new complex<double>[3*mField->getNcel()];
	Ezy = new complex<double>[3*mField->getNcel()];

	//�v�Z�p�萔�z��
	C_EZ     = new double[mField->getNcel()];	//Cez(i, j)       �� CEZ(i,j)
	C_EZLH	 = new double[mField->getNcel()];	//Cezlh(i, j)     �� CEZLH(i,j)
	C_HXLY   = new double[mField->getNcel()];	//Chxly(i, j+0.5) �� CHXLY(i,j)
	C_HYLX   = new double[mField->getNcel()];	//Chylx(i+0.5, j) �� CHYLX(i,j) ���Ӗ�����

	C_EZX   = new double[mField->getNcel()]; 
	C_EZY   = new double[mField->getNcel()];
	C_EZXLX = new double[mField->getNcel()];
	C_EZYLY = new double[mField->getNcel()];
	C_HX    = new double[mField->getNcel()];
	C_HY    = new double[mField->getNcel()];
	C_HXLY  = new double[mField->getNcel()];	//Chxly(i, j+0.5) �� CHXLY(i,j)
	C_HYLX  = new double[mField->getNcel()];	//Chylx(i+0.5, j) �� CHYLX(i,j) ���Ӗ�����

	EPS_EZ = new double[mField->getNcel()];
	EPS_HX = new double[mField->getNcel()];
	EPS_HY = new double[mField->getNcel()];

	B_EZXp = new double[mField->getNcel()];
	B_EZXm = new double[mField->getNcel()];
	B_EZYp = new double[mField->getNcel()];
	B_EZYm = new double[mField->getNcel()];
	B_HXp = new double[mField->getNcel()];
	B_HXm = new double[mField->getNcel()];
	B_HYp = new double[mField->getNcel()];
	B_HYm = new double[mField->getNcel()];

	//�̈揉����
	for(int i=0; i<3*mField->getNcel(); i++)
			Ez[i] = Hx[i] = Hy[i] = Ezx[i] = Ezy[i] = 0;

	cout << "FDTD_TM Constructor" << endl;
}

FDTD_TM::~FDTD_TM(){
	delete[] Ez;
	delete[] Hx;
	delete[] Hy;
	delete[] Ezx;
	delete[] Ezy;
	delete[] C_HX;
	delete[] C_HY;
	delete[] C_EZX;
	delete[] C_EZY;
	delete[] C_EZXLX;
	delete[] C_EZYLY;
	delete[] C_HXLY;
	delete[] C_HYLX;
	
	delete[] C_EZ;
	delete[] C_EZLH;

	delete[] EPS_EZ;
	delete[] EPS_HX;
	delete[] EPS_HY;

	delete[] B_EZXp;
	delete[] B_EZXm;
	delete[] B_EZYp;
	delete[] B_EZYm;
	delete[] B_HXp;
	delete[] B_HXm;
	delete[] B_HYp;
	delete[] B_HYm;

	cout << "FDTD_TM Destructor" << endl;
}

void FDTD_TM::Initialize(){
	super::Initialize();
	//�̈揉����
	for(int i=0; i<3*mField->getNcel(); i++)
		Ez[i] = Hx[i] = Hy[i] = Ezx[i] = Ezy[i] = 0;
}

void FDTD_TM::OpenData(string prefix){
	string str = prefix+ "data/" + getWaveData();	//�f�[�^�I�[�v��
	cout << str << endl;

	open_data(Ez, str +"Ez");
	open_data(Hx, str +"Hx");
	open_data(Hy, str +"Hy");
}

void FDTD_TM::SaveData(string prefix){
	string str = MakeDir(prefix+ "data");
	cout << str << endl;
	save_data(Ez, str + "/" + getWaveData() + "Ez");
	save_data(Hx, str + "/" + getWaveData() + "Hx");
	save_data(Hy, str + "/" + getWaveData() + "Hy");
}

void FDTD_TM::field(){
	setWorkingDirPass(mModel->mkdir(getDataDirPass()));	//�f�[�^��ۑ�����f�B���N�g���̐ݒ�
	setWorkingDirPass(MakeDir("TM"));
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=0; i<mField->getNpx(); i++){
		for(int j=0; j<mField->getNpy(); j++){
			EPSEZ(i,j) = mModel->calcEPS(i,j);
			EPSHX(i,j) = mModel->calcEPS(i,j+0.5);
			EPSHY(i,j) = mModel->calcEPS(i+0.5,j);
			N_S(i,j)   = sqrt( mModel->calcEPS(i+0.5,j+0.5) / EPSILON_0_S );	//���ܗ� n = ��(��/ ��0) (�����ł� �� = ��0�Ƃ��Ă���̂�)

			if(mField->sig == true)
				SIG(i, j) = mModel->calcSIG(i, j, lambda_s);	// Hair/incidence���f���̂�(�����j���F�f�l���L)
		}
	}
}

//�U���g
void FDTD_TM::NsScatteredWave(int ang){
	double rad = ang*M_PI/180;	//���W�A���ϊ�
	double _cos = cos(rad)*k_s;	//����v�Z����Ǝ��Ԃ����肻��������,������Ă���
	double _sin = sin(rad)*k_s;	
	//double a = (1-exp(-_pow(0.01*time,2)));		//�s�A���ɓ��˂����̂�h���ׂ̌W��
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=mField->getNpml(); i<mField->getNpx()-mField->getNpml(); i++){
		for(int j=mField->getNpml(); j<mField->getNpy()-mField->getNpml(); j++){
			if(EPSEZ(i,j) == EPSILON_0_S) continue;
			double n  = sqrt(EPSEZ(i,j)/EPSILON_0_S);				//todo �� = 0���O��̘b
			double u0 = sin(w_s/n*H_S/2.0)  / sin(  k_s*H_S/2.0);
			double u1 = sin(w_s/n*DT_S/2.0) / sin(n*k_s*H_S/2.0);
			double _n = u0/u1;
			double ikx = i*_cos + j*_sin;		//k_s*(i*cos + j*sin)
			EZX(i,j,+1) += 0.5*ray_coef*(1.0/(_n*n) - 1)*(polar(1.0, ikx - w_s*(time+DT_S)) - polar(1.0, ikx - w_s*time));
			EZY(i,j,+1) += 0.5*ray_coef*(1.0/(_n*n) - 1)*(polar(1.0, ikx - w_s*(time+DT_S)) - polar(1.0, ikx - w_s*time)); 
			EZ(i, j,+1) += ray_coef*(1.0/(_n*n) - 1)*(polar(1.0, ikx - w_s*(time+DT_S)) - polar(1.0, ikx - w_s*time));
		}
	}
}

//���˔g
void FDTD_TM::IncidentWave(int ang){
	double rad = ang*M_PI/180;	//���W�A���ϊ�
	double _cos = cos(rad), _sin = sin(rad);	//����v�Z����Ǝ��Ԃ����肻��������,������Ă���
	//double a = (1-exp(-_pow(0.01*time,2)));		//�s�A���ɓ��˂����̂�h���ׂ̌W��
	double a = ray_coef;
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=0; i<mField->getNx()-1; i++){
		for(int j=0; j<mField->getNy()-1; j++){
			double ikx = k_s*(i*_cos + j*_sin);
			EZX(i, j, +1) += 0.5*polar(1.0, ikx - w_s*(time + DT_S));
			EZY(i, j, +1) += 0.5*polar(1.0, ikx - w_s*(time + DT_S));
			EZ(i, j, +1) += polar(1.0, ikx - w_s*(time+DT_S));
		}
	}
}

void FDTD_TM::IncidentWaveH(int ang){
	double rad = ang*M_PI/180;	//���W�A���ϊ�
	double _cos = cos(rad), _sin = sin(rad);	//����v�Z����Ǝ��Ԃ����肻��������,������Ă���
	//double a = (1-exp(-_pow(0.01*time,2)));		//�s�A���ɓ��˂����̂�h���ׂ̌W��
		double a = ray_coef;
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=0; i<mField->getNx()-1; i++){
		for(int j=0; j<mField->getNy()-1; j++){
			double ikx = k_s*(i*_cos + j*_sin);
			HY(i,j, +1) += - k_s/(w_s*MU_0_S) * polar(1.0, ikx - w_s*(time+DT_S) );
			HX(i,j, +1) += + k_s/(w_s*MU_0_S) * polar(1.0, ikx - w_s*(time+DT_S) );
		}
	}
}

void FDTD_TM::draw(std::shared_ptr<GUI::ImageBuffer> img){
	//super::draw(Ezx, Ezy);	
	super::draw(Ez,img);	
}

bool FDTD_TM::calc(){
	return true;
}

void FDTD_TM::PML(){

}

//-------------------Near_To_Far�ϊ�---------------------//
void FDTD_TM::NTFFindexform(string label, NTFF::output flag){
	cout << "NTF start" << endl;

	//Solver::MiePrint(Ez, "strong"); // Mie�U���ɂ����āA�U���̔��a1.2�{�ł̋��x Ez ���t�@�C���o��

	int cx = mField->getNpx()/2;	//���W�ϊ���̌��_
	int cy = mField->getNpy()/2;
	
	double r0 = 1.0e6;
    complex<double> iu( 0.0, 1.0 );		//�P�ʋ����x�N�g��
	complex<double> Coef = sqrt( iu*k_s/(8*M_PI*r0) ) * exp( iu*k_s*r0 );	//��(jw/8��cr)e^(jkr)
    int offset = 5;		// closed line offset

	int lt,rt, tp, bm;		//�Ȗʂ̏ꏊ	//�v�Z�̈��
	tp = mField->getNpy() - mField->getNpml() - offset;			//�ォ��-5
	bm = mField->getNpml() + offset;			//������5
	rt = mField->getNpx() - mField->getNpml() - offset;		//�E����-5
	lt = mField->getNpml() + offset;			//������5

	double sum = 0;			//���a
	double strength[360];
	int max_angle = 360;
	for(int ang=0; ang<max_angle; ang++){
		double rad = ang*M_PI/180.0;			//0(deg)start�Ƃ���Ƃ�
		//double rad = (ang-90)*M_PI / 180.0;	//270(deg)start�Ƃ���Ƃ�
		Vec2<double> r(cos(rad), sin(rad));		//�����̕����x�N�g��.
		Vec2<double> r2;						//���S����Z���܂ł̋���

		complex<double> Nz( 0, 0 );
        complex<double> Lx( 0, 0 );
        complex<double> Ly( 0, 0 );
        complex<double> C_EZ, C_HX, C_HY;
		
        // (left,bottom) -> (right,bottom) n=(0, -1)
        for ( int i=lt; i<rt; i++ ) {
            r2   = Vec2<double>( i-cx, bm-cy);	//���S����Z���܂ł̋���
			C_EZ =  EZ(i,bm, +1);
			C_HX = 0.5 * ( HX(i,bm, +1) + HX(i,bm-1, +1) ); 

            Nz  += C_HX * exp( iu * k_s * In_prod(r,r2) );
            Lx  += C_EZ * exp( iu * k_s * In_prod(r,r2) );
        }

        // (right,bottom) -> (right,top) n=(1,0)
        for ( int j=bm; j<tp; j++ ) {
            r2   = Vec2<double>( rt-cx, j-cy);
			C_EZ = EZ(rt,j, +1);
			C_HY = 0.5 * ( HY(rt,j, +1) + HY(rt-1,j, +1) );

            Nz  += C_HY * exp( iu * k_s * In_prod(r,r2) );
            Ly  += C_EZ * exp( iu * k_s * In_prod(r,r2) );	
        }

        // (right,top) -> (left,top)  n=(0,1)
        for ( int i=lt; i<rt; i++ ) {
            r2    = Vec2<double>( i-cx, tp-cy );
			C_EZ = EZ(i,tp, +1);
			C_HX = 0.5 * ( HX(i,tp, +1) + HX(i,tp-1, +1) );
            Nz   -= C_HX * exp( iu * k_s * In_prod(r,r2) );		
            Lx   -= C_EZ * exp( iu * k_s * In_prod(r,r2) );
        }

        // (left,top) -> (left,bottom)
        for ( int j=bm; j<tp; j++ ) {
            r2    = Vec2<double>( lt-cx, j-cy);
            C_EZ  = EZ(lt,j, +1);
            C_HY  = 0.5 * ( HY(lt,j, +1) + HY(lt-1,j, +1));
            Nz   -= C_HY * exp( iu * k_s * In_prod(r,r2) );		
            Ly   -= C_EZ * exp( iu * k_s * In_prod(r,r2) );
        }
 
        // Get Ephi
        complex<double> EZZ = Coef * ( Z_0_S*Nz - Lx*sin(rad) + Ly*cos(rad) ); //���c��y�̎����ł� Lx, Ly��-����
        
		strength[int(ang)] = norm(EZZ);
		sum += norm(EZZ);
	}

	//NTFF�̌��ʂ��o��
	if( (flag & NTFF::NTFFDATA) == NTFF::NTFFDATA ){
		cout << "NTFF�̌��ʂ��o��" << endl;
		ofstream fp = WriteOpen(MakeDir("NTFF") + label + getWaveData());
		for(int i=0; i<max_angle; i++)
			fp << strength[i] << endl;
	}

	//NTFF�̌��ʂ̑��a���o��
	if( (flag & NTFF::TOTAL) == NTFF::TOTAL){
		cout << "NTFF�̌��ʂ̑��a���o��" << endl;
		ofstream fpntff= WriteOpen(MakeDir("NTFF") + label + "WaveAngleStrength", DATAFILE::ADD); //false�͒ǋL���[�h
		fpntff << "(" << to_s((int)Inv_Nano_S(lambda_s)) << "," << wave_angle << ")  " << sum << endl;		//�g�����Ƃ̑��a���o��
	}

	//���˗����o��
	if( (flag & NTFF::REFLEC) == NTFF::REFLEC){
		ofstream fp = WriteOpen(MakeDir("Reflection") + label + getWaveData());
		for(int ang = 0; ang < 360; ang++)
			fp << strength[ang] / sum << endl;
	}

	cout << "NTF finish" << endl;
}
