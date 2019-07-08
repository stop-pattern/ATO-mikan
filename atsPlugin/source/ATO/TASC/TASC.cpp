#include "../header/define.h"
#include "../header/Header.h"
#include "TASC.h"


void c_TASC::Control(State S, int * panel, int * sound) {
	this->control.P = 0;
	panel[static_cast<int>(ATC_Panel::TASC_braking)] = false;
	if (ATCstatus & static_cast<int>(ATC_Status::TASC_ON)) {
		//P�ݒ�
		this->Distance = this->Location - S.Z;
		this->Limit = sqrt(this->Distance * DECELERATION_BRAKE);
		if (this->Limit >= MAX_SPEED || isnan(this->Limit)) {
			this->Limit = MAX_SPEED;
		}

		//B����
		if (ATCstatus & static_cast<int>(ATC_Status::TASC_control)) {
			if (this->Limit * 0.85 < speed) {	//TASC���씻��
				ATCstatus |= static_cast<int>(ATC_Status::TASC_doing);

				int sign = (this->Limit - speed > 0) - (this->Limit - speed < 0);
				if (abs(this->Limit - speed) <= 8) {}
				if (this->Limit > speed) {
					this->control.B++;
				}
				else if (this->Limit < speed) {
					if (rand() % 2) this->control.B--;
				}

				/*
				//�n���h������
				int sign = (this->Limit - speed > 0) - (this->Limit - speed < 0);

				if (this->control.B == 0) {	//�n���h��0
					if (abs(this->Limit - speed) >= 1) this->control.B = 1;
				}
				else {
					//�n���h��+
					if (sign > 0) {	//P�ȉ�
						if ((1 / (this->control.B) * 1 / abs(this->Limit - speed)) >= 1) if ((rand() % 3) == 0) this->control.B += sign;
					}
					else {	////P�ȏ�
						if (rand() % 2) this->control.B += sign;
					}
				}
				//*/
			}
			else {
				if (rand() % 2) this->control.B--;
				//this->control.B = 0;
			}

			if (abs(speed) < 5) {
				if (this->control.B > (specific.B + 1) / 2) {	// (this->control.B > specific.E / 2)
					if (rand() % 2) this->control.B--;
				}
				else if (this->control.B < (specific.B + 1) / 2) {	// (this->control.B < specific.E / 2)
					if (rand() % 2) this->control.B++;
				}

				if (abs(speed) < 1.5 && this->control.B > 1) {
					if (rand() % 2) this->control.B--;
				}

				if (abs(speed) == 0) {
					ATCstatus |= static_cast<int>(ATC_Status::TASC_stopping);
				}
			}
			/*
			if (ATCstatus & static_cast<int>(ATC_Status::TASC_doing)) {
				if (this->control.B == 0) {
					this->control.B++;
				}
			}//*/
		}


		//�]���h�~B
		if (ATCstatus & static_cast<int>(ATC_Status::TASC_stopping)) {
			this->isControl = false;
			ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
			ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
			this->control.B = 4;
			if (manual.B > this->control.B) {
				this->control.B = 0;
				ATCstatus &= ~static_cast<int>(ATC_Status::TASC_stopping);
			}
		}
	}

	//�͈͊O����
	if (this->control.B < 0) {
		this->control.B = 0;
	}
	else if (this->control.B >= (specific.B + 1)) {	// (this->control.B >= specific.E)
		this->control.B = specific.B;
	}
}

void c_TASC::setBeacon(int index, Beacon b) {
	if (index >= 0 && index < 5) {
		if (index == 4)	this->isControl = true;	//����on
		this->setStatus(true);
		c_TASC::P[index][index / 1000] = b;
		Location = Stat.Z + b.Data % 1000 + DOOR_MARGIN;
	}
	else if (index == -1) {
		c_TASC::passage = b;
	}
}

void c_TASC::setStatus(bool in) {
	if (in == true) {
		ATCstatus |= static_cast<int>(ATC_Status::TASC_control);
	}
	else {
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
		this->Location = 0;
		for (size_t i = 0; i < 5; i++) {
			for (size_t j = 0; j < 3; j++) {
				P[i][j].Data = 0;
				P[i][j].Num = 0;
				P[i][j].Sig = 0;
				P[i][j].X = 0;
			}
		}
	}
}

void c_TASC::inEmergency(void) {
	ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
	ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
	ATCstatus &= ~static_cast<int>(ATC_Status::TASC_stopping);
	this->control.P = 0;
	this->control.B = 0;
}
