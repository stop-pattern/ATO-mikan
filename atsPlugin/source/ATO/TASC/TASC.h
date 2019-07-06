#ifndef _TASC_
#define _TASC_

#include "../header/ats.h"

class c_TASC {
public:
	void Control(State, int *, int *);	//毎フレーム制御
	Hand control;	//ハンドル制御値
	void setBeacon(int, Beacon);	//beacon set
	void setStatus(bool);	//status set
	void inEmergency(void);	//非常入力
	double Limit;	//目標速度
	bool isControl;	//制御中
private:
	double Location;	//目標位置
	double Distance;	//残距離
	Beacon P[5][3];	//TASC地上子
	Beacon passage;	//停通判定
};

#endif // !_TASC_