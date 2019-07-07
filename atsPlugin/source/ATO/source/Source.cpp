#include "../header/define.h"
#include "../header/Header.h"
#include "../ATO/ATO.h"
#include"../TASC/TASC.h"

int signal;	//信号インデックス
double speed;	//現在速度
double distance;	//現在位置
double accelaration;	//加減速度

bool door;	//ドア状態
int ATCstatus;	//ATC状態
int LimitSpeed;	//制限速度
bool Key_S, Key_A1, Key_A2, Key_B1, Key_B2, Key_C1, Key_C2, Key_D, Key_E, Key_F, Key_G, Key_H, Key_I, Key_J, Key_K, Key_L;	//ATSKey
bool Key;	//atsKey

int MasCon_Key;	//マスコンキー
int ATC_SW;	//ATC切り替えSW
int lag_cnt;	//ラグ用カウンター
bool isLoad;	//ラグ制御伝達用

Spec specific;	//車両性能
State Stat;	//車両状態(前フレーム)
Hand manual;	//手動ハンドル位置
Hand handle;	//ハンドル制御値

c_ATO ATO;	//ATO
c_TASC TASC;	//TASC