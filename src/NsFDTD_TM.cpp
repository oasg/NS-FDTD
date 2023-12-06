


//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>
#include"NsFDTD_TM.h"
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//---------------�R���X�g���N�^, �f�X�g���N�^ -----------------//
NsFDTD_TM::NsFDTD_TM(std::shared_ptr<TYPE::Field> field, std::shared_ptr<FazzyModel> model)
:FDTD_TM(field,model)
{
	cout << "NsFDTD_TM Constructor" << endl;
};

NsFDTD_TM::~NsFDTD_TM(){
	cout << "NsFDTD_TM Destructor" << endl;
};


bool NsFDTD_TM::calc() {
	//std::lock_guard<std::mutex> lock(field_mutex);
//	CalcE();	//�d��̌v�Z
	CalcE_PML();
	NsScatteredWave(wave_angle);	//�U���g�̓���
	//pointLightSource(Ez);
//	CalcH();	//����̌v�Z
	CalcH_PML();
	
	//absorbing();					//�z�����E

	//ButtonFactory::setButton("EZ", norm(EZ(mField->getNx()/2+10, mField->getNy()/2)));
	if (time > maxStep) {
		//MiePrint(Ez, "time"+to_s(maxStep)+"_PML"+to_s(mField->getNpml()) + "_NsTM_");
		//capture(to_s(time));
		//capture(to_s(Inv_Nano_S(lambda_s)));
//		if ((int)Inv_Nano_S(lambda_s) == 380 | (int)Inv_Nano_S(lambda_s) == 440 | (int)Inv_Nano_S(lambda_s) == 550 | (int)Inv_Nano_S(lambda_s) == 700)
//			capture(to_s(wave_angle) + "deg_" + to_s(Inv_Nano_S(lambda_s)) + "nm");
		return EndTask();
	}

	return true;
};

//----------------�I�����̎d��------------------------//
bool NsFDTD_TM::EndTask(){
	cout << "End Task" << endl;

	string label = "";
	NTFFindexform("", NTFF::NTFFDATA | NTFF::TOTAL );	// label -> "" �ɂ������Ǔ������m�F���ĂȂ�.
	
	//�I�������̊m�F
	if( !Terminate() )
		return false;

	ReStart();
	return true;
}

//--------------�v�Z�W���̐ݒ�---------------------//
void NsFDTD_TM::field(){		
	super::field();										//�U�d���̐ݒ�
	setWorkingDirPass(MakeDir("Ns"));
	R_M = NsCoef();		//�������Z�p�̌v�Z�萔�̐ݒ�
	R_P = 1.0-R_M;

	for(int i=0; i<mField->getNpx(); i++){
		for(int j=0; j<mField->getNpy(); j++){
			double mu = MU_0_S;
			
			double u_ez = sin(w_s/sqrt(EPSEZ(i,j)/EPSILON_0_S)*DT_S/2)/ sin(k_s*DT_S/2);
			double u_hx = sin(w_s/sqrt(EPSHX(i,j)/EPSILON_0_S)*DT_S/2)/ sin(k_s*DT_S/2);
			double u_hy = sin(w_s/sqrt(EPSHY(i,j)/EPSILON_0_S)*DT_S/2)/ sin(k_s*DT_S/2);
 			
			CEZ(i,j)   = 1;							//(1 - tanh(a*h_s)) / (1 + tanh(a*H_S));
			CEZLH(i,j) = u_ez*sqrt(mu/EPSEZ(i,j));		//u' ��(��/��)
			CHXLY(i,j) = u_hx*sqrt(EPSHX(i,j)/mu);		//u' ��(��/��)
			CHYLX(i,j) = u_hy*sqrt(EPSHY(i,j)/mu);		//u' ��(��/��)
		}
	}
	cout << "Ns_TM_Field" << endl;

	
	PMLfield();
}

void NsFDTD_TM::PMLfield() {
	double mu = MU_0_S;

#ifdef _OPENMP
#pragma omp parallel for
#endif
	for (int i = 0; i < mField->getNpx(); i++) {
		for (int j = 0; j < mField->getNpy(); j++) {
			double sig_x = mField->sigmaX(i, j);			//��x, ��x*, ��y, ��y* �@�@<- B-PML�̌W��
			double sig_xx = mu / EPSILON_0_S * sig_x;
			double sig_y = mField->sigmaY(i, j);
			double sig_yy = mu / EPSILON_0_S * sig_y;

			if (SIG(i, j) != 0) {
				sig_x = SIG(i, j);
				sig_y = SIG(i, j);
				sig_xx = mu / EPSILON_0_S * sig_x;
				sig_yy = mu / EPSILON_0_S * sig_y;
			}
			
			double ax = sig_x * DT_S / (2 * EPSEZ(i, j));
			double ay = sig_y * DT_S / (2 * EPSEZ(i, j));
			double axx = sig_xx * DT_S / (2 * mu);
			double ayy = sig_yy * DT_S / (2 * mu);
/*
			BEZXP(i, j) = 1 + tanh(ax);
			BEZXM(i, j) = 1 - tanh(ax);
			BEZYP(i, j) = 1 + tanh(ay);
			BEZYM(i, j) = 1 - tanh(ay);
			BHXP(i, j) = 1 + tanh(axx);
			BHXM(i, j) = 1 - tanh(axx);
			BHYP(i, j) = 1 + tanh(ayy);
			BHYM(i, j) = 1 - tanh(ayy);
*/
			BEZXP(i, j) = 1 + (tanh(ax) / (1 + tanh(ax)*tanh(axx)));
			BEZXM(i, j) = 1 - (tanh(ax) / (1 + tanh(ax)*tanh(axx)));
			BEZYP(i, j) = 1 + (tanh(ay) / (1 + tanh(ay)*tanh(ayy)));
			BEZYM(i, j) = 1 - (tanh(ay) / (1 + tanh(ay)*tanh(ayy)));
			BHXP(i, j) = 1 + (tanh(axx) / (1 + tanh(ax)*tanh(axx)));
			BHXM(i, j) = 1 - (tanh(axx) / (1 + tanh(ax)*tanh(axx)));
			BHYP(i, j) = 1 + (tanh(ayy) / (1 + tanh(ay)*tanh(ayy)));
			BHYM(i, j) = 1 - (tanh(ayy) / (1 + tanh(ay)*tanh(ayy)));

			BEZXP(i, j) = 1 / BEZXP(i, j);
			BEZYP(i, j) = 1 / BEZYP(i, j);
			BHXP(i, j) = 1 / BHXP(i, j);
			BHYP(i, j) = 1 / BHYP(i, j);
		}
	}
	cout << "PML_field" << endl;
}

void NsFDTD_TM::absorbing(){
	absorbing_stRL(Hy, 0, LEFT);						//����
	absorbing_nsRL(Hy, 0, LEFT);

	absorbing_stRL(Hy, mField->getNpx()-2, RIGHT);		//�E��
	absorbing_nsRL(Hy, mField->getNpx()-2, RIGHT);

	absorbing_stTB(Hx, 0, BOTTOM);					//����
	absorbing_nsTB(Hx, 0, BOTTOM);

	absorbing_stTB(Hx, mField->getNpy()-2, TOP);			//���
	absorbing_nsTB(Hx, mField->getNpy()-2, TOP);
}

/* Field�̌W���ݒ�̕���
	double u_ez = sin(w_s/sqrt(EPSEZ(i,j)/EPSILON_0_S)*DT_S/2)/ sin(k_s*DT_S/2);
	double u_hx = sin(w_s/sqrt(EPSHX(i,j)/EPSILON_0_S)*DT_S/2)/ sin(k_s*DT_S/2);
	double u_hy = sin(w_s/sqrt(EPSHY(i,j)/EPSILON_0_S)*DT_S/2)/ sin(k_s*DT_S/2);
*/
/*
//MorphoModel���������,�����I�ɔz�u����Ă��邩�̊m�F�p, field��SetEPS�̌�ɔz�u
	cout << "confirm" << endl;
	int s=0;
	double bf = 1.0;
	for(int i=0; i < mField->getNy(); i++){
		if(N_S(mField->getNx()/2, i) == bf){
			s++;
		}
		else{
			cout << bf << "=" << s << endl;
			s = 1;
		}
		bf = N_S(mField->getNx()/2, i);
	}
	cout << "confirm end" << endl;
	*/

/*
	//�v�Z�p�萔�̐ݒ� �ԈႦ�Ă����ۂ�
	double kx_s = 1/sqrt(sqrt(2.0)) * k_s;
	double ky_s = sqrt(1 - 1/sqrt(2.0) ) * k_s;

*/

	/*
	double sig = 0;	//���d��=0�ōl����
	double a = 0;	//�� = ��/(2��) ���, ���̓�=0�Ƃ��Ă���̂Ń���0
	//double u = sqrt( (_pow(sin(sqrt(w_s*w_s - a*a)*DT_S/2 ),2)+_pow( sinh(a*DT_S/2),2) )/ (_pow(sin(k_s*DT_S/2),2)*cosh(a*DT_S))  );

	// a = 0, tanh(0) = sinh(0) = 0, cosh(0) = 1�@��p���čœK������
	//double u = sin(w_s*DT_S/2)/ sin(k_s*DT_S/2);
	*/

/*
	if(time <= maxStep){
		OpenData(name);
		time = maxStep+1;
	}
	else{
		//SaveData(name);		//�V�~�����[�g�����f�[�^�͕ۑ�
	}
*/