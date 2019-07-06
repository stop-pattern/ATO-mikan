#ifndef _ATO_
#define _ATO_

#include "../header/ats.h"

#define ATO_CSC_TIME 2500	//ATO定速判定時間


class c_ATO {
public:
	void Control(State, int *, int *);	//毎フレーム制御
	void CSC(void);	//ATO定速
	bool Departure(void);	//出発ボタン判定
	void SignalChange();	//現示変更
	void ChangeMode(int);	//運転モード変更
	void setPattern(Beacon); 	//地上子入力
	void inEmergency(void);	//非常入力
	void Forward_Deceleration(int, int);	//事前減速

	Hand control;	//ハンドル制御値
	double Limit;	//目標速度
private:
	bool isFD;	//ATO事前減速
	Beacon brake;	//ATO事前減速用地上子情報
	int TargetSpeed;	//目標速度
	double Location;	//目標位置
	double Distance;	//残距離

	bool isCSC;	//ATO定速
	int CSCspeed;	//定速維持速度
	long int CSCcnt;	//定速判定
	int Mode = 1;	//ATO運転モード
	int margin;	//制限猶予速度
	int button_buf;	//ボタン押下時ラグ判定用

	//ATO目標速度
	const int c_ATO::Reference_Speed[4][256] = {
	{0,25,45,65,100,0,0,0,0,0,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,120,0,30,0,0,0,45,40,35,30,25,20,15,10,5,0,0,0,25,40,65},
	{0,0,0,0,0,0,0,0,0,0,0,5,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,115,0,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,7,7,12,17,22,27,32,37,42,47,52,57,62,67,72,77,82,87,92,97,102,107,117,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,5,5,10,15,20,20,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	//ATO運転モード
	enum ATOmode {
		ATC追従,
		ATO平常,
		ATO回復,
		ATO遅速,
	};
};

#endif // _ATO_