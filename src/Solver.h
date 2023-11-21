#ifndef _SOLVER_H
#define _SOLVER_H

#include "Object.h"
#include "MenuWindow.h"
#include "Model.h"
//#include <direct.h>
#include<stack>
#include<math.h>
#include<iomanip>
#include "PhysicalConstant.h"
#include "Field.h"
#include<filesystem>
#include "ImageBuffer.hpp"
#include<mutex>

#define _USE_MATH_DEFINES

enum DIRECT{
	LEFT = +1,
	RIGHT = -1,
	TOP  = -1,
	BOTTOM = +1
};

//NTTF���鎞�̏����o���I�v�V����
namespace NTFF{
	typedef unsigned int output;
	const output NTFFDATA = 1;			//�ϊ������f�[�^�i�G�l���M�[�j�������o��
	const output TOTAL    = 1 << 1;		//�ϊ������f�[�^�̍��v���Ō�̍s�ɏ����o��
	const output REFLEC   = 1 << 2;		//�ϊ������f�[�^�����v�Ŋ��������˗��������o��
}

namespace DATAFILE{
	typedef bool filemode;
	const filemode    ADD = false;
	const filemode REMOVE = true;
}

template <class T> class Range
{
	T Min;
	T Max;
	T interval;
public:
	Range(T mi, T ma)
	{
		Min = min(mi,ma);
		Max = max(mi,ma);
	}
	
	Range(T mi, T ma, T in)
	{
		Min = min(mi,ma);
		Max = max(mi,ma);
		interval = in;
	}

	Range(T m):Min(m), Max(m)
	{
	}

	Range()
	{
	}

	T MAX(){ return Max;}

	T MIN(){ return Min; }

	T INTERVAL(){ return interval; }
};

class Solver{
private:
	string DataDir;			//�f�[�^��u���f�B���N�g���ւ̃p�X
	string WorkingDir;		//���[�g���烏�[�L���O�f�B���N�g���ւ̃p�X
	stack<double> LamList;
	stack<int>	  WaveAngleList;
protected:
	const double H_S, DT_S;	//1�Z���̑傫���̃V�~�����[�V�����l, ������, 1�X�e�b�v������̎���
	double time;	//����
	double lambda_s, w_s, k_s, T_s;		//�V�~�����[�V�����p�̒l
	int		wave_angle;	//�g�̊p�x
	double	*n_s;		//���ܗ�, �U�d��
	double  *Sig_hair;	//�����j���F�f�̋z���W��
	double ray_coef;
	int maxStep;
	Range<double>	LambdaRange;
	Range<int>		WaveAngleRange;
	FazzyModel	*mModel;
	//protect the field to access by gui(main) thread
	std::mutex field_mutex;
	Field		*mField;	//�t�B�[���h
public:
	Solver();
	virtual ~Solver();
	virtual bool calc() = 0;
	virtual void draw(std::shared_ptr<GUI::ImageBuffer> img) = 0;
	virtual void field() = 0;
	double getTime(){ return time; }
	Field* getFild(){ return mField; }
	void nextTime(){
		time += DT_S;							//���Ԃ̍X�V
		ray_coef = 1-exp(-0.0001*time*time);	//�g���s�A���ɓ��˂����̂�h�����߂̌W��
		if (((int)time) % 100 == 0)		cout << time << endl;
/*
		if ((int)time == 1)
			modelCheck();		//�U���̃��f����capture���ďI��
*/		
/*
		if (((int)time) % 100 == 1) {
			ostringstream oss;
			oss << std::setfill('0') << std::right << std::setw(5) << time - 1;
			string name = oss.str();
			capture(name);		//100step���Ƃɏo�͂��L�^�@avi����쐬�p
		}
*/
	};

protected:
	void draw(Complex *p,std::shared_ptr<GUI::ImageBuffer> img);
	void draw(Complex *p, Complex *q,std::shared_ptr<GUI::ImageBuffer> img);
    void draw_model(std::shared_ptr<GUI::ImageBuffer> img);
    void modelCheck();

    //---------------------------------------------------------//
	//--------------------���˔g-------------------------------//
	//---------------------------------------------------------//
	void linearLightSource(complex<double> *p);	//�������̐�����
	void pointLightSource(complex<double> *p);	//���S�ɓ_����
	void scatteredWave(complex<double> *p, double *eps);

	virtual void Initialize(){
		time = 0;	//���Ԃ�0��
		//DataDir		=  "../DataSet/";
		//DataDir = "..\\Data\\Set\\";
		DataDir = "..\\data\\simulation";
	}

	void SetWaveParameter(double lam){
		lambda_s = lam;				//�g���ݒ�
		k_s      = 2*PI/lambda_s;	//�g��
		w_s      = LIGHT_SPEED_S*k_s;			//�p���g��
		T_s      = 2*M_PI/w_s;		//����
		mModel->InitializeLambda(Inv_Nano_S(lambda_s));
	}

	double bilinear_interpolation(complex<double> *, double, double);//�o�C���C�i�[�C���^�|���[�V�������

	Color color(double);
	bool neighber(int i, int j);

	bool nextLambda(){
		SetWaveParameter(lambda_s + LambdaRange.INTERVAL());
		if(lambda_s > LambdaRange.MAX())
			return false;
		return true;
	}

	bool nextWaveAngle(){
		wave_angle += WaveAngleRange.INTERVAL();
		if(wave_angle > WaveAngleRange.MAX())
			return false;

		return true;
	}

	bool Terminate(){
	//	return false; //���͏�ɏI��

		if( !nextLambda()){
			if(!nextWaveAngle()){
				if(!mModel->update(10)){
					return false;
				}
				else{
					wave_angle = WaveAngleRange.MIN();
					SetWaveParameter(LambdaRange.MIN());
					//mModel->InitializeLambda(Inv_Nano_S(lambda_s));
				}
			}
			else{
				SetWaveParameter(LambdaRange.MIN());
			}
		}
		cout << to_s(Inv_Nano_S(lambda_s)) + "nm" << endl;
		cout << to_s(wave_angle) + "deg" << endl;
		return true;
	}

	//�z�����E
	void absorbing_stRL(complex<double> *p, int _X, enum DIRECT a);	//�K�p�z���, �ǂ̈ʒu, ���E�㉺�ǂ̕ǂ�
	void absorbing_stTB(complex<double> *p, int _Y, enum DIRECT a);
	void absorbing_nsRL(complex<double> *p, int _X, enum DIRECT a);
	void absorbing_nsTB(complex<double> *p, int _Y, enum DIRECT a);

	//�������E
	void cycle_stRL(complex<double> *p, int _X, enum DIRECT a);


	void MiePrint(complex<double>*, string);	//Mie�U�����̏o��

	void save_data(complex<double> *data, string name);
	void open_data(complex<double> *data, string name);
	//void capture(string name);

	// ���S�����̓����� x����	
	complex<double> Dx2(complex<double>*p, int i, int j, int t){
		return p[index(i+1,j, t)] + p[index(i-1,j, t)] - 2.0*p[index(i,j, t)];
	};

	// ���S�����̓����� y���� 
	complex<double> Dy2(complex<double>*p, int i, int j, int t){
		return p[index(i,j+1, t)] + p[index(i,j-1, t)] - 2.0*p[index(i,j, t)];
	};

	//���S�����̓�����, t����
	complex<double> Dt2(complex<double>* p, int i, int j){
		return p[index(i,j, +1)] + p[index(i,j, -1)] - 2.0*p[index(i,j, 0)];
	};

	//DxDy2 = Dx2Dy
	complex<double> DxDy2(complex<double> *p, int i, int j, int t) {
		return     p[index(i + 1, j - 1, t)] + p[index(i + 1, j + 1, t)] + p[index(i - 1, j - 1, t)] + p[index(i - 1, j + 1, t)]
			- 2.0*(p[index(i + 1, j, t)] + p[index(i - 1, j, t)] + p[index(i, j + 1, t)] + p[index(i, j - 1, t)])
			+ 4.0*p[index(i, j, t)];
	};

	int index(const int& i, const int& j, const int& t){
		int k = ( t + (int)time + 3) % 3;
		return k*mField->getNcel() + index(i,j);
		//return index(i,j);
	};

	int index(const int& i, const int& j){
		return mField->index(i,j);
	};

	//pml�p�̔z��ԍ��擾
	int pmlIndex(const int &i, const int &j, const int &t){
		int k = ( t + (int)time + 3) % 3;
		return k*mField->getNcel() + pmlIndex(i,j);
		//return pmlIndex(i,j); //todo
	}

	int pmlIndex(const int &i, const int &j){
		return mField->pmlIndex(i,j);
	}


	//�Q�b�^�[
	double& N_S(const int& i, const int& j){
		return n_s[index(i,j)];
	}

	double& SIG(const int& i, const int& j) {
		return Sig_hair[index(i, j)];
	}

	ofstream WriteOpen(string name, DATAFILE::filemode f=DATAFILE::REMOVE){
		ofstream ofp;
		if(f){
			ofp.open(DataDir + name + ".txt");
		}
		else{
			ofp.open(DataDir + name + ".txt",ios::out | ios::app);
		}
		ofp.setf(ios::fixed, ios::floatfield);	//�Œ菬���_, 20�����x�Ŏw��
		ofp.precision(30);
		return ofp;
	}

	ifstream ReadOpen(string name){
		ifstream ifp(DataDir + name + ".txt");
		if(!ifp){ throw "cannot open file"; }
		return ifp;
	}


	//�쐬����t�@�C���̖��O��g���ƁA���ˊp�x�ŕԂ�
	string getDataName(){
		return WorkingDir + getWaveData();
	}

	//�g�̏���Ԃ�
	string getWaveData(){
		string str = "(WL=" + to_s(Inv_Nano_S(lambda_s)) + "nm,AOI=" + to_s(wave_angle) + "deg)";
		return str;
	}

	string getWorkingDirPass(){
		return WorkingDir;
	}

	void setWorkingDirPass(string wk){
		//WorkingDir = wk;
		DataDir += wk;
		cout << DataDir + WorkingDir << endl;
	}

	string getDataDirPass(){
		return DataDir;
	}

	string MakeDir(string name){
		if(std::filesystem::create_directory((DataDir + name).c_str()) == 0)
			 cout << "makeNEWdir" <<endl;
		return name + "/"; 
	}

	//nano prefix in simulation
	double Nano_S(double a){
		return mField->nanoToCell(a);
	}

	//inverse Nano_S
	double Inv_Nano_S(double a){
		return mField->cellToNano(a);
	}

	//(1 - ��0)/2 ��Ԃ�
	double NsCoef(){
		//�v�Z�p�萔�̐ݒ�
		double kx_s = k_s *          pow(2.0, -0.25);	//2��-4����1��
		double ky_s = k_s * sqrt(1 - pow(2.0, -0.5 ));
		double sin2_kx = pow(sin(kx_s*H_S/2), 2);
		double sin2_ky = pow(sin(ky_s*H_S/2), 2);
		double sin2_k  = pow(sin(k_s *H_S/2), 2);
		return (sin2_kx + sin2_ky - sin2_k)/(4*sin2_kx*sin2_ky);		//(1-��0)/2 = R_M, ( R_P = 1�Ƃ���Ƃ��������Ȃ� todo?)
	}


	//Maxwell�������p�̌W���̂ЂȌ` ��t = 1
	//ep_mu �Â���(E�̌W��->��, H�̌W��-> ��
	//sig  ��
	double MaxwellCoef(double ep_mu, double sig){
		return (1.0 - sig/ep_mu)/ (1.0 + sig/ep_mu);
	}

	double MaxwellCoef2(double ep_mu, double sig){
		return 1.0/ep_mu / (1.0 + sig/ep_mu);
	}
};

double fazzy_circle(double r, int x0, int y0, int x, int y);


#endif //_SOLVER_H