#include "../header/define.h"
#include "../header/Header.h"
#include "ATO.h"

#define ATO_LAG 500	//ÉâÉO[ms]


void c_ATO::Control(State S, int * panel, int * sound) {
	//ì]ìÆñhé~
	if (ATCstatus & static_cast<int>(ATC_Status::ATO_stopping)) {
		if (manual.B > control.B) {
			ATCstatus &= ~static_cast<int>(ATC_Status::ATO_stopping);
		}
		else {
			this->control.P = 0;
			this->control.B = (specific.B + 1) / 2;	// specific.E / 2;
		}
	}

	//î≠é‘îªíË
	if (!(ATCstatus & static_cast<int>(ATC_Status::ATO_control))) {
		if (this->Departure()) {
			ATCstatus |= static_cast<int>(ATC_Status::ATO_control);
		}
	}

	//ATOêßå‰
	if (ATCstatus & static_cast<int>(ATC_Status::ATO_control)) {
		//íËë¨îªíË
		if (this->Limit > speed && LimitSpeed * 0.9 < speed) {
			this->CSCcnt = S.T;
		}
		else {
			if (S.T - this->CSCcnt > ATO_CSC_TIME) {
				this->isCSC = true;
				this->CSCspeed = int(speed);
			}
		}

		//å∏ë¨êßå‰
		if (this->Limit + this->margin < speed) {
			this->isCSC = false;
			if (this->control.B < specific.B) {
				if (rand() % 2) {
					this->control.P = 0;
					this->control.B++;
				}
			}
		}
		//â¡ë¨êßå‰
		else if (this->Limit > speed) {
			this->isCSC = false;
			if (this->control.P < specific.P) {
				if (rand() % 2) {
					this->control.P++;
					this->control.B = 0;
				}
			}
		}
		//ëƒçsêßå‰
		else {
			if (this->isCSC) {	//ATOíËë¨
				this->CSC();
			}
			this->control.P = 0;
			this->control.B = 0;
			//isCSC = true;
		}

		//éñëOå∏ë¨
		if (this->isFD) {
			if (this->Location > S.Z || this->TargetSpeed < speed) {
				this->Distance = this->Location - Stat.Z;
				double lim = sqrt(this->Distance * DECELERATION_BRAKE) * 10;	//FDâ∫ç~P
				if (isnan(lim)) lim = MAX_SPEED;
				if (lim < this->Limit) {
					this->Limit = lim;
				}
			}
			else isFD = false;
		}
	}

	/*
	if (abs(accelaration) > 10) {
		handle.B = specific.E;	//
	}
	*/
}


void c_ATO::CSC() {
	if (accelaration > 0.5) {
		this->control.B += 1;
		this->control.P = 0;
	}
	else if (accelaration < -0.5) {
		this->control.B = 0;
		this->control.P += 1;
	}
	else {
		//íËë¨à€éù
	}

	//îÕàÕäOîªíË
	if (this->control.B < 0) {
		this->control.B = 0;
	}
	if (this->control.P < 0) {
		this->control.P = 0;
	}
}


bool c_ATO::Departure() {
	if (Key_A1 == true && (Key_A2 == true || Key_B1 == true)) {
		if (door == false && Stat.V == 0 && manual.B == 0 && manual.P == 0 && manual.R == 1 && LimitSpeed >= 25) {
			this->control.P = 0;
			this->control.B = 0;
			return true;
		}
	}
	return false;
}

void c_ATO::SignalChange() {
	this->Limit = this->Reference_Speed[this->Mode][signal];
	this->margin = int((LimitSpeed - this->Limit) * 10) % 10 > 5 ? int(LimitSpeed - this->Limit) : int(LimitSpeed - this->Limit) + 1;
	this->isFD = false;
}


void c_ATO::ChangeMode(int in) {
	if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
		if (this->Mode > 0 && this->Mode < 3) {
			this->Mode += in;
			this->SignalChange();
		}
		else if (this->Mode >= 3) {
			this->Mode = 3;
		}
		else {
			this->Mode = 1;
		}
	}

}

void c_ATO::setPattern(Beacon b) {
	brake = b;
}

void c_ATO::inEmergency(void) {
	ATCstatus &= ~static_cast<int>(ATC_Status::ATO_control);
	ATCstatus &= ~static_cast<int>(ATC_Status::ATO_doing);
	ATCstatus &= ~static_cast<int>(ATC_Status::ATO_stopping);
	this->control.P = 0;
	this->control.B = 0;
	this->isCSC = false;
}

void c_ATO::Forward_Deceleration(int sig, int dist) {
	if (this->isFD == false || (this->isFD == true && this->TargetSpeed)) {
	}
	this->TargetSpeed = this->Reference_Speed[this->Mode][sig];
	if (this->TargetSpeed < LimitSpeed) {
		this->Location = Stat.Z + dist;
		this->isFD = true;
	}
}
