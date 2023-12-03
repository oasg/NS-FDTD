#ifndef _FIELD_INFO_H
#define _FIELD_INFO_H
#include<string>
#include"Object.h"

namespace TYPE{
    class Field{
    private:
        const int WIDTH;	//width[nm]
        const int HEIGHT;	//height[nm]
        const double H_U;	//The size of a cell[nm]
        const int H_S;		//Cell spacing (set to 1 for clarity)
        const int N_X;		//横の配列数
        const int N_Y;		//縦の配列数
        const int N_PML;	//PMLで用いるレイヤの厚さ
        const int N_PX;		//PMLレイヤを含めた横の配列数
        const int N_PY;		//PMLレイヤを含めた縦の配列数
        const int N_CELL;	//総セル数
    public:
        Field(int width, int height, double h_u, int pml);

        bool sig;	//吸収係数(色素)の有無

        //計算領域にしかアクセスできないインデックス関数(負の数をしていすればいける)
        int index(const int& i, const int& j){
            //return (i+N_PML)*N_PY + (j+N_PML);
            return i*N_PY + j;
        }

        //pml領域にもアクセスできるインデックス関数
        int pmlIndex(const int &i, const int &j){
            return i*N_PY + j;
        }

        //計算領域の情報,(ファイル名用)
        string getStringCellInfo(){
            return "(" +to_s(H_U) + "nm,"+ to_s(N_X) + "cell" +  ")";
        }

        //nm単位の物理量をシミュレーション値に変換
        double nanoToCell(const double &length){
            return length/H_U;
        }

        double cellToNano(const double &cell){
            return cell*H_U;
        }

        //ゲッター
        int getNx(){
            return N_X;
        }
        int getNy(){
            return N_Y;
        }
        int getNpx(){
            return N_PX;
        }
        int getNpy(){
            return N_PY;
        }
        int getNcel(){
            return N_CELL;
        }
        int getHu(){
            return H_U;
        }
        int getNpml(){
            return N_PML;
        }

        double sigmaX(const int &i, const int &j); //σx
        double sigmaY(const int &i, const int &j); //σy
    };
}


#endif //_FIELD_INFO_H