#define _USE_MATH_DEFINES
#include "Object.h"
#include "FDTD_TE.h"
#include "Vec.h"
#include <math.h> 
using namespace std;

FDTD_TE::FDTD_TE(std::shared_ptr<TYPE::Field> field, std::shared_ptr<FazzyModel> model)
:Solver(field,model)
{
	cout << "FDTD_TE Constructor" << endl;
	Hz  = new complex<double>[3*mField->getNcel()];		//Hz(i+0.5, j+0.5) �ｿｽ�ｿｽ Hz(i,j)�ｿｽ�ｿｽ�ｿｽﾓ厄ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ �ｿｽﾌ茨ｿｽm�ｿｽ�ｿｽ
	Ex  = new complex<double>[3*mField->getNcel()];		//Ex(i+0.5,j) �ｿｽ�ｿｽ Ex(i,j)�ｿｽ�ｿｽ�ｿｽﾓ厄ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	Ey  = new complex<double>[3*mField->getNcel()];		//Ey(i,j+0.5) �ｿｽ�ｿｽ Ey(i,j)�ｿｽ�ｿｽ�ｿｽﾓ厄ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	Hzx	= new complex<double>[3*mField->getNcel()];		//Ey(i,j+0.5) �ｿｽ�ｿｽ Ey(i,j)�ｿｽ�ｿｽ�ｿｽﾓ厄ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	Hzy	= new complex<double>[3*mField->getNcel()];		//Ey(i,j+0.5) �ｿｽ�ｿｽ Ey(i,j)�ｿｽ�ｿｽ�ｿｽﾓ厄ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ

//計算用定数配列
	C_HZLH  = new double[mField->getNcel()];	//CHzlh(i+0.5, j+0.5) �ｿｽ�ｿｽ CHZLH(i,j)
	C_EX    = new double[mField->getNcel()];	//CEx(i+0.5, j) �ｿｽ�ｿｽ CEX(i,j)
	C_EY    = new double[mField->getNcel()];	//CEy(i, j+0.5) �ｿｽ�ｿｽ CEY(i,j)
	C_EXLY  = new double[mField->getNcel()];
	C_EYLX  = new double[mField->getNcel()];
	C_HZX   = new double[mField->getNcel()];
	C_HZY   = new double[mField->getNcel()];
	C_HZXLX = new double[mField->getNcel()];
	C_HZYLY = new double[mField->getNcel()];

	EPS_HZ  = new double[mField->getNcel()];
	EPS_EX  = new double[mField->getNcel()];
	EPS_EY  = new double[mField->getNcel()];

	B_EXp = new double[mField->getNcel()];
	B_EXm = new double[mField->getNcel()];
	B_EYp = new double[mField->getNcel()];
	B_EYm = new double[mField->getNcel()];
	B_HZXp = new double[mField->getNcel()];
	B_HZXm = new double[mField->getNcel()];
	B_HZYp = new double[mField->getNcel()];
	B_HZYm = new double[mField->getNcel()];

	//�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	for(int i=0; i<3*mField->getNcel(); i++)
		Ex[i] = Ey[i] = Hz[i] = Hzx[i] = Hzy[i] = 0;

	//todo �ｿｽ關費ｿｽﾍ擾ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾈゑｿｽ�ｿｽﾄゑｿｽ�ｿｽ�ｿｽ?
	// C_HZX[i] = C_HZY[i] = C_HZXLX[i] = C_HZYLY[i] = C_HZLH[i] = C_EX[i] = C_EY[i] = C_EXLY[i] = C_EYLX[i] = 0;
}

FDTD_TE::~FDTD_TE(){
	delete[] Hz;
	delete[] Ex;
	delete[] Ey;
	delete[] Hzx;
	delete[] Hzy;

	delete[] C_HZLH;
	delete[] C_EX;
	delete[] C_EY;
	delete[] C_EXLY;
	delete[] C_EYLX;
	delete[] C_HZX;
	delete[] C_HZY;
	delete[] C_HZXLX;
	delete[] C_HZYLY;

	delete[] EPS_HZ;
	delete[] EPS_EX;
	delete[] EPS_EY;

	delete[] B_EXp;
	delete[] B_EXm;
	delete[] B_EYp;
	delete[] B_EYm;
	delete[] B_HZXp;
	delete[] B_HZXm;
	delete[] B_HZYp;
	delete[] B_HZYm;

	cout << "FDTD_TE Destructor" << endl;
}


void FDTD_TE::draw(std::shared_ptr<GUI::ImageBuffer> img){
	super::draw(Ey,img);

}

void FDTD_TE::OpenData(string prefix){
	string str = prefix+ "data/" + getWaveData();	//�ｿｽf�ｿｽ[�ｿｽ^�ｿｽI�ｿｽ[�ｿｽv�ｿｽ�ｿｽ
	cout << str << endl;

	open_data(Hz, str +"Hz");
	open_data(Ex, str +"Ex");
	open_data(Ey, str +"Ey");
}

void FDTD_TE::SaveData(string prefix){
	string str = MakeDir(prefix+ "data");
	cout << str << endl;
	save_data(Hz, str +  "/" + getWaveData() + "Hz");
	save_data(Ex, str + "/" + getWaveData()  + "Ex");
	save_data(Ey, str + "/" + getWaveData()  + "Ey");
}

void FDTD_TE::Initialize(){
	super::Initialize();
	//�ｿｽﾌ域初�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	for(int i=0; i<3*mField->getNcel(); i++)
			Ex[i] = Ey[i] = Hz[i] = Hzx[i] = Hzy[i] = 0;
}

void FDTD_TE::field(){
	setWorkingDirPass(mModel->mkdir(getDataDirPass()));	//�ｿｽf�ｿｽ[�ｿｽ^�ｿｽ�ｿｽﾛ托ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽf�ｿｽB�ｿｽ�ｿｽ�ｿｽN�ｿｽg�ｿｽ�ｿｽ�ｿｽﾌ設抵ｿｽ
	setWorkingDirPass(MakeDir("TE"));
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=0; i<mField->getNpx(); i++){
		for(int j=0; j<mField->getNpy(); j++){
			EPSEX(i,j) = mModel->calcEPS(i+0.5,j    , D_Y);
			EPSEY(i,j) = mModel->calcEPS(i    ,j+0.5, D_X);
			EPSHZ(i,j) = 0.5*(mModel->calcEPS(i+0.5,j+0.5, D_X) + mModel->calcEPS(i+0.5,j+0.5, D_Y));
			N_S(i,j)   = sqrt(mModel->calcEPS(i+0.5,j+0.5) / EPSILON_0_S );		//�ｿｽ�ｿｽ�ｿｽﾜ暦ｿｽ n = �ｿｽ�ｿｽ(�ｿｽ�ｿｽ/ �ｿｽ�ｿｽ0) (�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾅゑｿｽ �ｿｽ�ｿｽ = �ｿｽ�ｿｽ0�ｿｽﾆゑｿｽ�ｿｽﾄゑｿｽ�ｿｽ�ｿｽﾌゑｿｽ)

			if (mField->sig == true)
				SIG(i, j) = mModel->calcSIG(i, j, lambda_s);	// Hair/incidence�ｿｽ�ｿｽ�ｿｽf�ｿｽ�ｿｽ�ｿｽﾌゑｿｽ(�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽj�ｿｽ�ｿｽ�ｿｽF�ｿｽf�ｿｽl�ｿｽ�ｿｽ�ｿｽL)
		}
	}
}

//�ｿｽU�ｿｽ�ｿｽ�ｿｽg
void FDTD_TE::NsScatteredWave(int ang){
	double rad = ang*M_PI/180;	//�ｿｽ�ｿｽ�ｿｽW�ｿｽA�ｿｽ�ｿｽ�ｿｽﾏ奇ｿｽ
	double _cos = cos(rad), _sin = sin(rad);	//�ｿｽ�ｿｽ�ｿｽ�ｿｽv�ｿｽZ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾆ趣ｿｽ�ｿｽﾔゑｿｽ�ｿｽ�ｿｽ�ｿｽ閧ｻ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ,�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾄゑｿｽ�ｿｽ�ｿｽ
	//double a = (1-exp(-_pow(0.01*time,2)));		//�ｿｽs�ｿｽA�ｿｽ�ｿｽ�ｿｽﾉ難ｿｽ�ｿｽﾋゑｿｽ�ｿｽ�ｿｽ�ｿｽﾌゑｿｽh�ｿｽ�ｿｽ�ｿｽﾗの係�ｿｽ�ｿｽ
	double a = ray_coef;
	double n, u0, u1, _n;
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=mField->getNpml(); i<mField->getNpx() - mField->getNpml(); i++){
		for(int j=mField->getNpml(); j<mField->getNpy() - mField->getNpml(); j++){
			if(EPSEY(i,j) == EPSILON_0_S && EPSEX(i,j) == EPSILON_0_S) continue;
			double ikx = k_s*(i*_cos + j*_sin);

			n  = sqrt(EPSEY(i,j)/EPSILON_0_S);				//todo �ｿｽ�ｿｽ = 0�ｿｽ�ｿｽ�ｿｽO�ｿｽ�ｿｽﾌ話
			u0 = sin(w_s/n*H_S/2.0)  / sin(k_s*H_S/2.0);
			u1 = sin(w_s/n*DT_S/2.0) / sin(k_s*n*H_S/2.0);
			_n = u0/u1;
			EY(i,j, +1) += a*_cos*(1.0/(_n*n) - 1.0)*(polar(1.0, ikx - w_s*(time+DT_S)) - polar(1.0, ikx - w_s*time));

			n  = sqrt(EPSEX(i,j)/EPSILON_0_S);				//todo �ｿｽ�ｿｽ = 0�ｿｽ�ｿｽ�ｿｽO�ｿｽ�ｿｽﾌ話
			u0 = sin(w_s/n*H_S/2.0)  / sin(k_s*H_S/2.0);
			u1 = sin(w_s/n*DT_S/2.0) / sin(k_s*n*H_S/2.0);
			_n = u0/u1;
			EX(i,j, +1) += a*_sin*(1.0/(_n*n) - 1.0)*(polar(1.0, ikx - w_s*(time+DT_S)) - polar(1.0, ikx - w_s*time));

		}
	}
}

//�ｿｽ�ｿｽ�ｿｽﾋ波
void FDTD_TE::IncidentWave(int ang){
	double rad = ang*M_PI/180;	//�ｿｽ�ｿｽ�ｿｽW�ｿｽA�ｿｽ�ｿｽ�ｿｽﾏ奇ｿｽ
	double _cos = cos(rad), _sin = sin(rad);	//�ｿｽ�ｿｽ�ｿｽ�ｿｽv�ｿｽZ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾆ趣ｿｽ�ｿｽﾔゑｿｽ�ｿｽ�ｿｽ�ｿｽ閧ｻ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ,�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾄゑｿｽ�ｿｽ�ｿｽ
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=0; i<mField->getNx(); i++){
		for(int j=0; j<mField->getNy(); j++){
			double ikx = k_s*(i*_cos + j*_sin);
			EY(i,j, +1) += _cos*polar(1.0, ikx - w_s*(time+DT_S));
			EX(i,j, +1) += _sin*polar(1.0, ikx - w_s*(time+DT_S));
		}
	}
}

void FDTD_TE::IncidentWaveH(int ang){
	double rad = ang*M_PI/180;	//�ｿｽ�ｿｽ�ｿｽW�ｿｽA�ｿｽ�ｿｽ�ｿｽﾏ奇ｿｽ
	double _cos = cos(rad), _sin = sin(rad);	//�ｿｽ�ｿｽ�ｿｽ�ｿｽv�ｿｽZ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾆ趣ｿｽ�ｿｽﾔゑｿｽ�ｿｽ�ｿｽ�ｿｽ閧ｻ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ,�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾄゑｿｽ�ｿｽ�ｿｽ
#ifdef _OPENMP
#pragma omp parallel for
#endif
	for(int i=0; i<mField->getNx(); i++){
		for(int j=0; j<mField->getNy(); j++){
			double ikx = k_s*(i*_cos + j*_sin);
			HZ(i,j, +1) += k_s/(w_s*MU_0_S) * polar(1.0, ikx - w_s*(time+DT_S))*(_sin - _cos);
		}
	}
}


void FDTD_TE::NTFFindexform(string label, NTFF::output flag){
	cout << "NTF start" << endl;

	int cx = mField->getNpx()/2;
	int cy = mField->getNpy()/2;

	double r0 = 1.0e6;
    complex<double> iu( 0.0, 1.0 );											// imaginary unit
    complex<double> Coef = sqrt( iu*k_s/(8*M_PI*r0) ) * exp( iu*k_s*r0 );	// common coefficient

    int offset = 5;		    // closed line offset
	int lt,rt, tp, bm;		//�ｿｽﾂ曲面の場所
	tp = mField->getNy() - mField->getNpml() - offset;			//�ｿｽ繧ｩ�ｿｽ�ｿｽ-5
	bm = mField->getNpml() + offset;			//�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ5
	rt = mField->getNx() - mField->getNpml() - offset;		//�ｿｽE�ｿｽ�ｿｽ�ｿｽ�ｿｽ-5
	lt = mField->getNpml() + offset;			//�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ5

	double sum = 0;
	const int max_angle = 360;	//�ｿｽﾇの角�ｿｽx�ｿｽﾜで包ｿｽ�ｿｽz�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾟるか, 180�ｿｽ�ｿｽ360
	double strength[max_angle];
    for ( int phi=0; phi<max_angle; phi++ ) {
        double rad = (phi-90) * M_PI/180.0;
//		rad = M_PI - rad;							//todo �ｿｽﾈゑｿｽ�ｿｽ�ｿｽTE�ｿｽ�ｿｽ180�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ轤ｷ
        Vec2<double> r( cos(rad), sin(rad) );	        // eye vector
          
        complex<double> Nx( 0, 0 );
        complex<double> Ny( 0, 0 );
        complex<double> Lz( 0, 0 );
        Vec2<double> r2;
        complex<double> C_EX, C_EY, C_HZ;

        // (left,bottom) -> (right,bottom)
        for ( int i=lt; i<rt; i++ ) {
            r2    = Vec2<double>( i-cx + 0.5, bm-cy);	//�ｿｽ�ｿｽ�ｿｽS�ｿｽ�ｿｽ�ｿｽ�ｿｽZ�ｿｽ�ｿｽ�ｿｽﾜでの具ｿｽ�ｿｽ�ｿｽ
            C_HZ  = 0.5*( HZ(i,bm, +1) + HZ(i,bm-1, +1) );
            C_EX  = EX(i,bm, +1);
            Nx   -= C_HZ * exp( iu * k_s * In_prod(r,r2) );
            Lz   -= C_EX * exp( iu * k_s * In_prod(r,r2) );
        }

        // (right,bottom) -> (right,top)
        for ( int j=bm; j<tp; j++ ) {
			r2   = Vec2<double>( rt-cx, j-cy + 0.5 );     
            C_HZ  = 0.5*( HZ(rt,j, +1) + HZ(rt+1,j, +1) );  
            C_EY  = 0.25 * ( EY(rt,j, +1) + EY(rt-1,j, +1) + EY(rt,j+1, +1) + EY(rt-1,j+1, +1));  
            Ny -= C_HZ * exp( iu * k_s * In_prod(r,r2) );
            Lz -= C_EY * exp( iu * k_s * In_prod(r,r2) );
        }

        // (right,top) -> (left,top)
        for ( int i=lt; i<rt; i++ ) {
            r2    = Vec2<double>( i-cx+0.5, tp-cy );
            C_HZ  = 0.5*(HZ(i,tp, +1) + HZ(i,tp+1, +1));
            C_EX  = 0.25 * ( EX(i,tp, +1) + EX(i,tp-1, +1) + EX(i+1,tp, +1) + EX(i+1,tp-1, +1));
            Nx += C_HZ * exp( iu * k_s * In_prod(r,r2) );
            Lz += C_EX * exp( iu * k_s * In_prod(r,r2) );
        }            

        // (left,top) -> (left,bottom)
        for ( int j=bm; j<tp; j++ ) {
            r2    = Vec2<double>( lt-cx, j-cy+0.5 );         
            C_HZ  = 0.5*(HZ(lt,j, +1) + HZ(lt-1,j, +1));
            C_EY  = 0.25 * ( EY(lt,j, +1) + EY(lt-1,j, +1) + EY(lt,j+1, +1) + EY(lt-1,j+1, +1));  
            Ny += C_HZ * exp( iu * k_s * In_prod(r,r2) );
            Lz += C_EY * exp( iu * k_s * In_prod(r,r2) );
        }
            
        // Get Ephi
        complex<double> Ephi = Coef * ( -Z_0_S*( -Nx*sin(rad) + Ny*cos(rad) ) + Lz );
          
		strength[phi] = norm(Ephi);
		sum += norm(Ephi);
    }

	//NTFF�ｿｽﾌ鯉ｿｽ�ｿｽﾊゑｿｽ�ｿｽo�ｿｽ�ｿｽ
	if( (flag & NTFF::NTFFDATA) == NTFF::NTFFDATA ){
		ofstream fp = WriteOpen(MakeDir("NTFF") + label + getWaveData());
		for(int i=0; i<max_angle; i++)
			fp << strength[i] << endl;
	}

	//NTFF�ｿｽﾌ鯉ｿｽ�ｿｽﾊの托ｿｽ�ｿｽa�ｿｽ�ｿｽ�ｿｽo�ｿｽ�ｿｽ
	if( (flag & NTFF::TOTAL) == NTFF::TOTAL){
		ofstream fp= WriteOpen(MakeDir("NTFF") + label + "WaveAngleStrength", DATAFILE::ADD); //false�ｿｽﾍ追記�ｿｽ�ｿｽ�ｿｽ[�ｿｽh
		fp << "(" <<(int)Inv_Nano_S(lambda_s) << "," << wave_angle << ")  " << sum << endl;		//�ｿｽg�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾆの托ｿｽ�ｿｽa�ｿｽ�ｿｽ�ｿｽo�ｿｽ�ｿｽ
	}

	//�ｿｽ�ｿｽ�ｿｽﾋ暦ｿｽ�ｿｽ�ｿｽ�ｿｽo�ｿｽ�ｿｽ
	if( (flag & NTFF::REFLEC) == NTFF::REFLEC){
		ofstream fp = WriteOpen(MakeDir("Reflection") + label + getWaveData());
		for(int i = 0; i < max_angle; i++)
			fp << strength[i] / sum << endl;
	}
	cout << "NTF finish" << endl;
}
