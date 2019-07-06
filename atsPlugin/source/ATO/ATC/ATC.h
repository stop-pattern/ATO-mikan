#ifndef _ATC_
#define _ATC_

#include "../header/ats.h"

#define BRAKE_HALF 1500
#define FORWARDNOTICE 1900

class c_ATC{
public:
	void Control(State, int *, int *);	//毎フレーム制御
	void SignalChange(void);	//信号設定
	Hand control;
	void notice(int, int);	//前方予告
	int Limit;	//制限速度
	void set02(void);	//状態管理
private:
	int limit_panel;	//現示index
	long int brake_cnt;	//ハーフブレーキ用カウンタ
	bool changeSignal;	//現示変化
	bool isNotice;	//前方予告
	int notice_panel;	//前方予告index
	long int notice_time;	//前方予告用カウンタ
	bool isNoSignal;	//02受信
};


#endif // !_ATC_
