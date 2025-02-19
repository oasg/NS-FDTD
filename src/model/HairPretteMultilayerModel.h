#pragma once
#include "FazzyModel.h"
#include"type/Field.h"
#include<memory>
#include <random>
#include "PerlinNoise.hpp"
class HairPretteMultilayerModel : public FazzyModel
{

public:
	HairPretteMultilayerModel(std::shared_ptr<TYPE::Field> f);
	string mkdir(string root) override;
	double calcEPS(const double&, const double&, enum INTEG = D_XY) override;
	double calcSIG(const double&, const double&, const double lam, enum INTEG = D_XY) override;
	bool update(int) override {
		//return true;
		return false;
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

class HairPretteMultilayerPerlinModel : public FazzyModel
{

public:
	HairPretteMultilayerPerlinModel(std::shared_ptr<TYPE::Field> f);
	string mkdir(string root) override;
	double calcEPS(const double&, const double&, enum INTEG = D_XY) override;
	double calcSIG(const double&, const double&, const double lam, enum INTEG = D_XY) override;
	bool update(int) override {
		//return true;
		return false;
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
	std::vector<siv::PerlinNoise> perlins;
	const double noise_scale;           // 噪声缩放
    const double noise_strength;         // 噪声强度

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