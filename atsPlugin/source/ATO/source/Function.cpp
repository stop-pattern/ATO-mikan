#include <Windows.h>
#include <string>
#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"


//駅ジャンプ時再読み込み処理
void reload(void) {
	if (ATCstatus & static_cast<int>(ATC_Status::ATC_ON)) {
		ATCstatus &= ~static_cast<int>(ATC_Status::ATC_brake);
	}
	if (ATCstatus & static_cast<int>(ATC_Status::ATO_ON)) {
		ATCstatus |= static_cast<int>(ATC_Status::ATO_stopping);
		ATCstatus &= ~static_cast<int>(ATC_Status::ATO_control);
		ATCstatus &= ~static_cast<int>(ATC_Status::ATO_doing);
	}
	if (ATCstatus & static_cast<int>(ATC_Status::TASC_ON)) {
		ATCstatus |= static_cast<int>(ATC_Status::TASC_stopping);
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
	}
}

//ATO/TASCスイッチ操作
void SetStatus() {
	if (ATCstatus & static_cast<int>(ATC_Status::ON)) {
		if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
			int mask = static_cast<int>(ATC_Status::ON) + static_cast<int>(ATC_Status::ATC_ON);
			ATCstatus &= mask;	//SW情報とATC情報以外を消去

			switch (MasCon_Key) {
			case static_cast<int>(Key::TRTA):
				ATCstatus |= static_cast<int>(ATC_Status::ATO_ON);
			case static_cast<int>(Key::TOB):
			case static_cast<int>(Key::TKK):
			case static_cast<int>(Key::SEB) :
				ATCstatus |= static_cast<int>(ATC_Status::TASC_ON);
			case static_cast<int>(Key::TOY) :
			case static_cast<int>(Key::KeyOff) :
			case static_cast<int>(Key::SOT) :
			case static_cast<int>(Key::JNR) :
			case static_cast<int>(Key::OER) :
				break;
			default:
				ATCstatus &= ~static_cast<int>(ATC_Status::ATC_ON);
				break;
			}
		}
	}
	else {
		ATCstatus &= static_cast<int>(ATC_Status::ATC_ON);	//ATC情報以外を消去
		ATCstatus &= ~static_cast<int>(ATC_Status::ON);	//SW=OFF
	}
}

//マスコンキー操作
void setKey(int in) {
	if (Stat.V == 0 && manual.P == 0 && manual.B == specific.E && manual.R == 0 && S == false) {
		ATCstatus &= static_cast<int>(ATC_Status::ON);	//SW状態のみ保持

		MasCon_Key += in;
		if (MasCon_Key < 0) {
			MasCon_Key = 0;
		}
		else if (MasCon_Key > 8) {
			MasCon_Key = 8;
		}

		if ((MasCon_Key >= 1 && MasCon_Key <= 4) || MasCon_Key == 8) {
			ATCstatus |= static_cast<int>(ATC_Status::ATC_ON);
		}
		else ATCstatus &= ~static_cast<int>(ATC_Status::ATC_ON);	//ATC情報以外を消去
		SetStatus();	//ATO/TASC状態更新
	}
}