#pragma once
#ifndef HEADER
#define HEADER

#include <string>
#include <math.h>
#include "ats.h"
#include "../ATO/ATO.h"
#include "../TASC/TASC.h"
#include "../ATC/ATC.h"
#include "../INI/Getini.h"

#define LAG	200	//判定ラグ[ms]


extern int signal;	//信号インデックス
extern double speed;	//現在速度[km/h]
extern double distance;	//現在位置[m]
extern double accelaration;	//加減速度[km/h/s]

extern bool door;	//ドア状態
extern int ATCstatus;	//ATC状態
extern int LimitSpeed;	//制限速度
extern bool Key_S, Key_A1, Key_A2, Key_B1, Key_B2, Key_C1, Key_C2, Key_D, Key_E, Key_F, Key_G, Key_H, Key_I, Key_J, Key_K, Key_L;	//ATSKey
extern bool Key;	//atsKey

extern int MasCon_Key;	//マスコンキー
extern int ATC_SW;	//ATC切り替えSW
extern int lag_cnt;	//ラグ用カウンター
extern bool isLoad;	//ラグ制御伝達用

extern Spec specific;	//車両性能
extern State Stat;	//車両状態(前フレーム)
extern Hand manual;	//手動ハンドル位置
extern Hand handle;	//ハンドル制御値

extern c_ATO ATO;	//ATO
extern c_TASC TASC;	//TASC
extern c_ATC ATC;	//ATC
extern c_INI INI;	//INI

void reload(void);	//駅ジャンプ時再読み込み処理
void SetStatus();	//ATO/TASCスイッチ操作
void setKey(int);	//マスコンキー操作

#endif // !HEADER