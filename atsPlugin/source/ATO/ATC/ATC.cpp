#include "../header/define.h"
#include "../header/Header.h"
#include "ATC.h"

void c_ATC::Control(State S, int * panel, int * sound) {
	//ATC現示・ベル
	if (changeSignal && signal > 9 && signal < 36) {
		for (size_t i = 101; i < 131; i++) {
			panel[i] = false;
		}
		//panel[LimitSpeed / 5 + 102] = true;
		sound[static_cast<int>(ATC_Sound::ATC_bell)] = SoundInfo::PlayOnce;
	}
	else {
		sound[static_cast<int>(ATC_Sound::ATC_bell)] = SoundInfo::PlayContinue;
	}
	panel[static_cast<int>(ATC_Panel::ATC_limit)] = this->Limit;
	panel[limit_panel] = true;

	//02判定
	if (this->isNoSignal) {
		//this->Limit = 0;
		//this->control.B = 0;
		panel[101] = true;
	}
	else {
		panel[101] = false;
		if (this->Limit == 0) {
			panel[131] = true;
			panel[132] = false;
		}
		else {
			panel[131] = false;
			panel[132] = true;
		}
	}

	//前方予告
	if (this->isNotice && (S.T - this->notice_time) / FORWARDNOTICE % 2) {
		panel[static_cast<int>(ATC_Panel::ATC_notice)] = true;
		panel[this->notice_panel] = true;
	}
	else {
		panel[static_cast<int>(ATC_Panel::ATC_notice)] = false;
		panel[this->notice_panel] = false;
	}


	//速度照査
	if (!(ATCstatus & static_cast<int>(ATC_Status::ATC_brake))) {
		this->brake_cnt = S.T;
	}
	if (Limit + 2 < speed) {
		ATCstatus |= static_cast<int>(ATC_Status::ATC_brake);
		this->control.B = int(specific.E / 2);
		if (Limit + 5 < speed) {
			if (S.T - brake_cnt > BRAKE_HALF) {
				this->control.B = specific.B;
			}
		}
	}
	else if (this->Limit == 0) {
		ATCstatus |= static_cast<int>(ATC_Status::ATC_brake);
		this->control.B = int(specific.E / 2);
		if (abs(speed) < 0.25) {
			this->control.B = specific.B;
		}
	}
	else {
		control.B = 0;
		ATCstatus &= ~static_cast<int>(ATC_Status::ATC_brake);
	}

	changeSignal = false;
	//}
}

void c_ATC::SignalChange() {
	if (signal <= 9 || signal >= 36) {
		this->set02();
	}
	else {
		this->isNoSignal = false;
	}
	this->isNotice = false;
	this->Limit = SpeedLimit[signal];
	this->limit_panel = signal + 92;
	this->notice_panel = 101;
	this->changeSignal = true;
}

void c_ATC::notice(int sig, int param) {
	if (sig != signal || param != 0) {
		this->isNotice = true;
		this->notice_panel = sig - 10 + 102;
		this->notice_time = Stat.T;
	}
	else this->isNotice = false;
}

void c_ATC::set02(void) {
	this->isNoSignal = true;
	//LimitSpeed = 0;
}

