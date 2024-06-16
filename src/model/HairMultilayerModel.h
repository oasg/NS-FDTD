#pragma once
#include "FazzyModel.h"
#include"type/Field.h"
#include<memory>
class HairMultilayerModel : public FazzyModel
{

public:
	HairMultilayerModel(std::shared_ptr<TYPE::Field> f);
	string mkdir(string root) override;
	double calcEPS(const double&, const double&, enum INTEG = D_XY) override;
	double calcSIG(const double&, const double&, const double lam, enum INTEG = D_XY) override;
	bool update(int) override {
		//return true;
		return true;
	}
	void Initialize() override
	{
	}
private:
    const double ep1, ep2;
	//const int alpha;
    const double cmc_width;
    const double cuticle_width;
    const int num_layers;

	//double alphaR, r, rn, cwidth, cn, cmc, mn, length, ln, lx, ly;
	/*
	alpha:傾き		alphaR:傾き(ラジアン)
	r:毛皮質範囲の半径(μm)							rn:毛皮質範囲の半径(nmシミュレーション値)
	cwidth:キューティクル厚さ(μm)					cn:キューティクル厚さ(nmシミュレーション値)
	cmc:CMC幅(μm)									mn:CMC幅(nmシミュレーション値)
	length:キューティクル長さ(μm)					ln:キューティクル長さ(nmシミュレーション値)
	ly:キューティクル範囲(nmシミュレーション値)		lx:x方向長さ(nmシミュレーション値)
	*/
};