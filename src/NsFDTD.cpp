#include "FDTD.h"

NsFDTD::NsFDTD(std::shared_ptr<TYPE::Field> field, std::shared_ptr<FazzyModel> model)
:FDTD(field,model){
		cout << "NsFDTD Constructor" << endl;
}

NsFDTD::~NsFDTD(){
	cout << "NsFDTD Destructor" << endl;
}

//NsFDTDのときの差分法
complex<double> NsFDTD::DxDy2(complex<double> *p, int i, int j, int t){
	return     p[index(i+1,j-1, t)] + p[index(i+1,j+1, t)] + p[index(i-1,j-1, t)] + p[index(i-1,j+1, t)]
	      -2.0*( p[index(i+1,j  , t)] + p[index(i-1,j  , t)] + p[index(i  ,j+1, t)] + p[index(i  ,j-1, t)] )
		  +4.0*p[index(i,j, t)];
}

complex<double> NsFDTD::D0_2(complex<double> *p, int i, int j, int t){
	return Dx2(p, i,j,t) + Dy2(p, i,j,t) + r_s*DxDy2(p, i,j,t);
}

void NsFDTD::field(){
	//屈折率の設定
	//計算用定数の設定
	kx_s = 1/sqrt(sqrt(2.0)) * k_s;
	ky_s = sqrt(1 - 1/sqrt(2.0) ) * k_s;

	double sin2_kx = pow(sin(kx_s*H_S/2), 2);
	double sin2_ky = pow(sin(ky_s*H_S/2), 2);
	double sin2_k  = pow(sin(k_s *H_S/2), 2);
	r_s = (sin2_kx + sin2_ky - sin2_k)/(4*sin2_kx*sin2_ky);  //(1-γ0)/2

	double w_b  = w_s*DT_S/2;
	double k_b  = k_s*H_S/2;
	//double kx_b = k_b * cos(PI/4);
	//double ky_b = k_b * sin(PI/4)

	for(int i=0; i<mField->getNx(); i++)
		for(int j=0; j<mField->getNy();j++)
			np[index(i,j)] = _pow(sin(w_b/n_s[index(i,j)]) / sin(k_b),2);
}

bool NsFDTD::calc(){
	time += DT_S;		//次のステップの計算
	//printf("%lf \n", time/T_s);

	//境界以外
	for(int i=1; i<mField->getNx()-1; i++){
		for(int j=1; j<mField->getNy()-1; j++){
			if( (i==1 || i==mField->getNx()-2) && (j==1 || j==mField->getNy()-2) ) //四隅を参照してしまう場所はS-FDTDで解く
				phi[index(i,j, +1)] = np[index(i,j)]*( Dx2(phi, i,j, 0) + Dy2(phi, i,j, 0) ) - phi[index(i,j, -1)] + 2.0*phi[index(i,j,0)];
			else 
				phi[index(i,j, +1)] = np[index(i,j)]*D0_2(phi, i,j, 0) - phi[index(i,j, -1)] + 2.0*phi[index(i,j,0)];
		}
	}

//吸収境界
	absorbing();

	//phi[index(mField->getNx()/2, mField->getNy()/2, +1)] += 5*(1-exp(-0.01*time*time))*sin(- w_s*time);
	pointLightSource(phi);
	if(time > 2500){
		MiePrint(phi, "Mie_Nsphi2");
		return false;
	}

	return true;
}


//-------------------------------------------------------//
//-----------↓---吸収境界こっから ↓--------------------//
//-------------------------------------------------------//
//左壁の吸収境界
void NsFDTD::absorbing_left(){
	double u1, u2;
	double w_b  = w_s*DT_S/2;
	double k_b  = k_s*H_S/2;
	double kx_b = kx_s*H_S/2;
	double ky_b = ky_s*H_S/2;

	for(int i=1; i<mField->getNx()-1; i++){

		u1 = tan(w_b/n_s[index(0,i)]) / tan(k_b);
		u2 = 2 * _pow(sin(w_b/n_s[index(0,i)]), 2) / _pow(sin(ky_b),2) * (1 - tan(kx_b)/tan(k_b));

		if(i==1 || i==mField->getNx()-2)	//四隅の横は一次元吸収境界
			phi[index(0,i, +1)]    = phi[index(1,i, 0)]    + (1- u1)/(1+u1)*(phi[index(0,i, 0)]    - phi[index(1,i, +1)]);

		else				//二次元吸収境界
			phi[index(0,i, +1)] = -phi[index(1,i, -1)]     
								 -  (1-u1)/(1+u1)*(phi[index(0,i, -1)] + phi[index(1,i, +1)])    
							     +       2/(1+u1)*(phi[index(0,i,  0)] + phi[index(1,i,  0)]) 
								 + u2*u2/(1+u1)/2*(   Dy2(phi,0,i, 0)  + Dy2(phi,1,i, 0)	);
												//  dy^2 φn     +   dy^2 φb
	}			
}

//右壁の吸収境界
void NsFDTD::absorbing_right(){

	double u1, u2;
	double w_b  = w_s*DT_S/2;
	double k_b  = k_s*H_S/2;
	double kx_b = kx_s*H_S/2;
	double ky_b = ky_s*H_S/2;

	for(int i=1; i<mField->getNx()-1; i++){
		u1 = tan(w_b/n_s[index(mField->getNx()-1,i)]) / tan(k_b);
		u2 = 2 * _pow(sin(w_b/n_s[index(mField->getNx()-1,i)]), 2) / _pow(sin(ky_b),2) * (1 - tan(kx_b)/tan(k_b));

		if(i == 1 || i == mField->getNx()-2)		// 四隅の横は一次元吸収境界
			phi[index(mField->getNx()-1,i, +1)] = phi[index(mField->getNx()-2,i, 0)] + (1- u1)/(1+u1)*(phi[index(mField->getNx()-1,i, 0)] - phi[index(mField->getNx()-2,i, +1)]);

		else						//それ以外は二次元吸収境界
			phi[index(mField->getNx()-1,i, +1)] = - phi[index(mField->getNx()-2,i, -1)] 
								     -   (1-u1)/(1+u1)*(phi[index(mField->getNx()-1,i, -1)] + phi[index(mField->getNx()-2,i, +1)]) 
								     +        2/(1+u1)*(phi[index(mField->getNx()-1,i,  0)] + phi[index(mField->getNx()-2,i,  0)]) 
								     + u2*u2/(1+u1)/2*(   Dy2(phi, mField->getNx()-1,i, 0) + Dy2(phi, mField->getNx()-2,i, 0)	);
												        //  dy^2 φn     +   dy^2 φb
	}
}

//上壁の吸収境界
void NsFDTD::absorbing_up(){

	double u1, u2;
	double w_b  = w_s*DT_S/2;
	double k_b  = k_s*H_S/2;
	double kx_b = kx_s*H_S/2;
	double ky_b = ky_s*H_S/2;

	for(int i=1; i<mField->getNx()-1; i++){
		u1 = tan(w_b/n_s[index(i,0)]) / tan(k_b);
		u2 = 2 * _pow(sin(w_b/n_s[index(i,0)]), 2) / _pow(sin(ky_b),2) * (1 - tan(kx_b)/tan(k_b));

		if(i==1 || i==mField->getNx()-2)	//四隅の横は一次元吸収境界
			phi[index(i,0, +1)]    = phi[index(i,1, 0)]    + (1- u1)/(1+u1)*(phi[index(i,0, 0)]    - phi[index(i,1, +1)]);

		else				//二次元吸収境界
			phi[index(i,0, +1)]  = -phi[index(i,1, -1)]    
								  -  (1-u1)/(1+u1)*(phi[index(i,0, -1)]+phi[index(i,1, +1)]) 
							      +       2/(1+u1)*(phi[index(i,0, 0)]+phi[index(i,1, 0)])     
								  + u2*u2/(1+u1)/2*( Dx2(phi, i,0, 0) + Dx2(phi, i,1, 0) 	);
												  //  dx^2 φn     +   dx^2 φb
	}		
}

//下壁の吸収境界
void NsFDTD::absorbing_down(){

	double u1, u2;
	double w_b  = w_s*DT_S/2;
	double k_b  = k_s*H_S/2;
	double kx_b = kx_s*H_S/2;
	double ky_b = ky_s*H_S/2;

	for(int i=1; i<mField->getNx()-1; i++){

		u1 = tan(w_b/n_s[index(i,mField->getNx()-1)]) / tan(k_b);
		u2 = 2 * _pow(sin(w_b/n_s[index(i,mField->getNx()-1)]), 2) / _pow(sin(ky_b),2) * (1 - tan(kx_b)/tan(k_b));

		if(i==1 || i==mField->getNx()-2)	//四隅の横は一次元吸収境界
			phi[index(i,mField->getNx()-1, +1)] = phi[index(i,mField->getNx()-2, 0)] + (1- u1)/(1+u1)*(phi[index(i,mField->getNx()-1, 0)] - phi[index(i,mField->getNx()-2, +1)]);



		else				//二次元吸収境界
			phi[index(i,mField->getNx()-1, +1)] = -phi[index(i,mField->getNx()-2, -1)] 
			                      - (1-u1)/(1+u1)*(phi[index(i,mField->getNx()-1, -1)]+phi[index(i,mField->getNx()-2, +1)]) 
							      +     2/(1+u1)*(phi[index(i,mField->getNx()-1,  0)]+phi[index(i,mField->getNx()-2,  0)]) 
								  + u2*u2/(1+u1)/2*( Dx2(phi, i,mField->getNx()-1, 0) + Dx2(phi, i,mField->getNx()-2, 0)	 );
													 //  dx^2 φn     +   dx^2 φb
	}		
}

//吸収境界
void NsFDTD::absorbing(){
	/*
	absorbing_left();
	absorbing_right();
	absorbing_up();
	absorbing_down();	
	*/
	absorbing_nsRL(phi, 0, LEFT);
	absorbing_nsRL(phi, mField->getNx()-1, RIGHT);
	absorbing_nsTB(phi, 0,    BOTTOM);
	absorbing_nsTB(phi, mField->getNy()-1, TOP);
}

//-------------------------------------------------------//
//-------------↑---吸収境界ここまで---↑----------------//
//-------------------------------------------------------//


void NsFDTD::Mie_Cylinder_Incidence(){
	//中心の円に,散乱波発生
	for(int i=0; i<mField->getNx(); i++)
		for(int j=0; j<mField->getNx(); j++)	
	//		if((i-mField->getNx()/2)*(i-mField->getNx()/2) + (j-mField->getNx()/2)*(j-mField->getNx()/2) <= lambda_s*lambda_s)
				phi[index(i,j, +1)] += (1-exp(-0.01*time*time))*(1/_pow(n_s[index(i,j)], 2) - 1) * ( sin(k_s*i - w_s*(time + DT_S) ) + sin(k_s*i - w_s*(time - DT_S)) - 2*sin(k_s*i - w_s*time)); 
	
}
