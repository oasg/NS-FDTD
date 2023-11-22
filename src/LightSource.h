#ifndef _LIGHT_SOURCE_H
#define _LIGHT_SOURCE_H

#include "Object.h"
#include "PhysicalConstant.h"
#include "Field.h"

class LightSource{
	const int angle;
	const double lambda_s;	//波長
	const double k_s;			//波数
	const double w_s;			//角周波数
	const double T_s;			//周期
public:
	LightSource(int ang, double lambda);
	int getAngle();
	double getAngleRad();
	Complex lightSource(int time);

	//void pointSource(complex<double> *p);
	//void linearSource(complex<double> *p);
};

inline int LightSource::getAngle(){
	return angle;
}

inline double LightSource::getAngleRad(){
	return M_PI*angle/180.0;
}

/*
//点光源の入射
void LightSource::pointSource(complex<double> *p){
	double ray_coef = exp(-0.0001*(time-500)*(time-500));
	p[index(mField->getNx()/2, mField->getNy()/2)] = ray_coef*polar(1.0, w_s*time);
}

//線光源の入射
void LightSource::linearLightSource(complex<double> *p){

	double ray_coef = exp(-0.0001*(time-500)*(time-500));
	for(int i=1; i<mField->getNy()-1; i++)
		p[index(5,i, +1)]  +=ray_coef*polar(1.0, w_s*time);	//todo 境界上にも入れていいのか?
}

*/

#endif //_LIGHT_SOURCE_H