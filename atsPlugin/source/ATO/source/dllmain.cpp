/*BVE ATSプラグインテンプレート
 *Author : Tetsu Otter
 *SDK : ver10.0.17763.0
 *DLLは、プロジェクト名で出力されます。任意のプロジェクト名を付与してください。
 *プロジェクト名変更時は、dllmain.defの "LIBRARY" に設定された "PITemp" という文字列を新たなプロジェクト名に変更してください。
*/

#include <string.h>
#include <math.h>
#include "../header/Header.h"
#include "../header/define.h"

#include "../ATO/ATO.h"
#include "../TASC/TASC.h"
#include "../ATC/ATC.h"
#include "../INI/Getini.h"

#include <fstream>


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	INI.getIni(INI.GetPath(hModule));

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

DE void SC Load() {
}
DE void SC Dispose() {
}
DE int SC GetPluginVersion() {
	return PI_VERSION;
}
DE void SC SetVehicleSpec(Spec s) {
	specific.A = s.A;
	specific.J = s.J;
	specific.B = s.B;
	specific.E = s.B + 1;
	specific.C = s.C;
	specific.P = s.P;
}
DE void SC Initialize(int b) {
	b = InitialPosition::Service;
	reload();
}
DE Hand SC Elapse(State S, int * panel, int * sound) {
	double spd = speed;
	speed = S.V;	//((S.Z - Stat.Z) * 1000) / ((S.T - Stat.T) * 3600000);
	accelaration = (speed - spd) / ((S.T - Stat.T) / 1000);
	//MasCon_Key = panel[92];
	//ATC_SW = panel[72];

	handle.P = manual.P;
	handle.B = manual.B;
	handle.R = manual.R;


	if (MasCon_Key != static_cast<int>(Key::KeyOff)) {
		if (S.T - lag_cnt >= LAG) {
			isLoad = true;
			lag_cnt = S.T;
		}
		else {
			isLoad = false;
		}
	}

	//ATO
	if (ATCstatus & static_cast<int>(ATC_Status::ATO_ON)) {
		panel[static_cast<int>(ATC_Panel::ATO_power)] = true;

		if (isLoad) {
			ATO.Control(S, panel, sound);	//制御関数
		}

		//ATO動作
		if (ATCstatus & static_cast<int>(ATC_Status::ATO_control)) {

			if (manual.B == 0 && manual.P == 0) {
				handle.P = ATO.control.P;
				panel[static_cast<int>(ATC_Panel::ATO_P)] = ATO.control.P + 1;
				panel[static_cast<int>(ATC_Panel::ATO_controling)] = true;
			}
			else {
				ATO.control.P = 0;
				panel[static_cast<int>(ATC_Panel::ATO_P)] = 0;
			}

			if (manual.P == 0 && manual.B < ATO.control.B) {
				handle.B = ATO.control.B;
				panel[static_cast<int>(ATC_Panel::ATO_B)] = ATO.control.B + 1;
			}
			else {
				panel[static_cast<int>(ATC_Panel::ATO_B)] = 0;
			}
		}
	}
	else {
		panel[static_cast<int>(ATC_Panel::Brake_notches)] = manual.B;
		panel[static_cast<int>(ATC_Panel::ATO_P)] = 0;
		panel[static_cast<int>(ATC_Panel::ATO_B)] = 0;
		panel[static_cast<int>(ATC_Panel::ATO_power)] = false;
		panel[static_cast<int>(ATC_Panel::ATO_controling)] = false;
	}
	panel[static_cast<int>(ATC_Panel::ATO_debug)] = int(ATO.Limit);


	//TASC
	if (ATCstatus & static_cast<int>(ATC_Status::TASC_ON)) {
		if (isLoad) {
			TASC.Control(S, panel, sound);	//制御関数
		}

		panel[static_cast<int>(ATC_Panel::TASC_release)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_braking)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_noches)] = 0;
		panel[static_cast<int>(ATC_Panel::TASC_failed)] = false;
		MasCon_Key == static_cast<int>(Key::SEB) ? panel[static_cast<int>(ATC_Panel::TASC_power_M)] = true : panel[static_cast<int>(ATC_Panel::TASC_power)] = true;

		if (ATCstatus & static_cast<int>(ATC_Status::TASC_control)) {
			MasCon_Key == static_cast<int>(Key::SEB) ? panel[static_cast<int>(ATC_Panel::TASC_controling_M)] = true : panel[static_cast<int>(ATC_Panel::TASC_controling)] = false;

			//TASC制御灯
			if(TASC.isControl) panel[static_cast<int>(ATC_Panel::TASC_braking)] = true;
			else panel[static_cast<int>(ATC_Panel::TASC_braking)] = false;

			//TASC動作
			if (true && ATCstatus & static_cast<int>(ATC_Status::TASC_doing)) {
				if (TASC.control.B > handle.B) {
					handle.P = 0;
					handle.B = TASC.control.B;
					panel[static_cast<int>(ATC_Panel::TASC_noches)] = TASC.control.B + 1;
				}
			}
		}
	}
	else {
		panel[static_cast<int>(ATC_Panel::TASC_power)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_release)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_braking)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_controling)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_noches)] = 0;
		panel[static_cast<int>(ATC_Panel::TASC_failed)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_power_M)] = false;
		panel[static_cast<int>(ATC_Panel::TASC_controling_M)] = false;
	}
	panel[static_cast<int>(ATC_Panel::TASC_debug)] = int(TASC.Limit);


	//ATCブレーキ
	if (ATCstatus & static_cast<int>(ATC_Status::ATC_ON)) {
		ATC.Control(S, panel, sound);	//制御関数

		if (ATCstatus & static_cast<int>(ATC_Status::ATC_brake)) {
			if (ATC.control.B >= handle.B || (ATC.Limit == 0 && abs(speed) < 1)) {
				handle.P = 0;
				handle.B = ATC.control.B;
				panel[static_cast<int>(ATC_Panel::ATC_braking)] = true;
				panel[static_cast<int>(ATC_Panel::ATO_P)] = 0;
				panel[static_cast<int>(ATC_Panel::ATO_B)] = 0;
				panel[static_cast<int>(ATC_Panel::TASC_noches)] = 0;
			}
			else panel[static_cast<int>(ATC_Panel::ATC_braking)] = false;
		}
		else panel[static_cast<int>(ATC_Panel::ATC_braking)] = false;
	}

	//TISunit
	if (S.I != 0) {
		if (S.I > 0) {	//電流+
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_0)] = 1;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_1)] = 1;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_2)] = 1;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_3)] = 1;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_4)] = 1;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_5)] = 1;
		}
		if (S.I < 0) {	//電流-
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_0)] = 2;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_1)] = 2;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_2)] = 2;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_3)] = 2;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_4)] = 2;
			if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_5)] = 2;
		}
	}
	else {	//電流0
		if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_0)] = 0;
		if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_1)] = 0;
		if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_2)] = 0;
		if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_3)] = 0;
		if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_4)] = 0;
		if (rand() % 10 == 0) panel[static_cast<int>(ATC_Panel::TIS_unit_5)] = 0;
	}


	panel[static_cast<int>(ATC_Panel::Brake_notches)] = handle.B == specific.E ? handle.B + 1 : handle.B;
	panel[static_cast<int>(ATC_Panel::Brake_notches_unic)] = handle.B == specific.E ? handle.B + 1 : handle.B;
	panel[static_cast<int>(ATC_Panel::Power_notches)] = handle.P;
	panel[static_cast<int>(ATC_Panel::Master_Controller_Key)] = MasCon_Key;
	panel[static_cast<int>(ATC_Panel::ORP_speed)] = int(TASC.Limit * 10);	//debug


	Stat = S;
	return handle;
}
DE void SC SetPower(int p) {
	manual.P = p;
}
DE void SC SetBrake(int b) {
	manual.B = b;
	if (b == specific.E) {
		ATO.inEmergency();
		TASC.inEmergency();
	}
}
DE void SC SetReverser(int r) {
	manual.R = r;
}


DE void SC KeyDown(int k) {
	switch (k) {
	case ATSKeys::S:
		Key_S = true;
		break;
	case ATSKeys::A1:
		Key_A1 = true;
		break;
	case ATSKeys::A2:
		Key_A2 = true;
		break;
	case ATSKeys::B1:
		Key_B1 = true;
		break;
	case ATSKeys::B2:
		Key_B2 = true;
		break;
	case ATSKeys::C1:
		Key_C1 = true;
		break;
	case ATSKeys::C2:
		Key_C2 = true;
		break;
	case ATSKeys::D:
		Key_D = true;
		break;
	case ATSKeys::E:
		Key_E = true;
		ATO.ChangeMode(-1);
		break;
	case ATSKeys::F:
		Key_F = true;
		ATO.ChangeMode(+1);
		break;
	case ATSKeys::G:
		Key_G = true;
		if (Key_S) {
			ATCstatus &= ~static_cast<int>(ATC_Status::ON);
			SetStatus();
		}
		break;
	case ATSKeys::H:
		Key_H = true;
		if (Key_S) {
			ATCstatus |= static_cast<int>(ATC_Status::ON);
			SetStatus();
		}
		break;
	case ATSKeys::I:
		Key_I = true;
		setKey(-1);
		break;
	case ATSKeys::J:
		Key_J = true;
		setKey(+1);
		break;
	case ATSKeys::K:
		Key_K = true;
		break;
	case ATSKeys::L:
		Key_L = true;
		break;
	default:
		break;
	}
}
DE void SC KeyUp(int k) {
	switch (k) {
	case ATSKeys::S:
		Key_S = false;
		break;
	case ATSKeys::A1:
		Key_A1 = false;
		break;
	case ATSKeys::A2:
		Key_A2 = false;
		break;
	case ATSKeys::B1:
		Key_B1 = false;
		break;
	case ATSKeys::B2:
		Key_B2 = false;
		break;
	case ATSKeys::C1:
		Key_C1 = false;
		break;
	case ATSKeys::C2:
		Key_C2 = false;
		break;
	case ATSKeys::D:
		Key_D = false;
		break;
	case ATSKeys::E:
		Key_E = false;
		break;
	case ATSKeys::F:
		Key_F = false;
		break;
	case ATSKeys::G:
		Key_G = false;
		break;
	case ATSKeys::H:
		Key_H = false;
		break;
	case ATSKeys::I:
		Key_I = false;
		break;
	case ATSKeys::J:
		Key_J = false;
		break;
	case ATSKeys::K:
		Key_K = false;
		break;
	case ATSKeys::L:
		Key_L = false;
		break;
	default:
		break;
	}
}
DE void SC HornBlow(int k) {

}

DE void SC DoorOpen() {
	door = true;
	if (ATCstatus & static_cast<int>(ATC_Status::ATC_ON)) {
		ATO.SignalChange();
		ATC.SignalChange();
	}
	if (ATCstatus & static_cast<int>(ATC_Status::ATO_ON)) {
		ATCstatus &= ~static_cast<int>(ATC_Status::ATO_control);
		ATCstatus &= ~static_cast<int>(ATC_Status::ATO_doing);
		ATCstatus |= static_cast<int>(ATC_Status::ATO_stopping);
	}
	if (ATCstatus & static_cast<int>(ATC_Status::TASC_ON)) {
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
		ATCstatus |= static_cast<int>(ATC_Status::TASC_stopping);
		TASC.setStatus(false);	//TASC制御解放
	}
}
DE void SC DoorClose() {
	door = false;
}

DE void SC SetSignal(int a) {
	signal = a;
	LimitSpeed = SpeedLimit[a];

	ATC.SignalChange();
	ATO.SignalChange();
}
DE void SC SetBeaconData(Beacon b) {
	switch (b.Num) {
		case static_cast<int>(ATC_Beacon::notice_force) :
			ATC.notice(b.Sig, b.Data);
			break;
		case static_cast<int>(ATC_Beacon::notice_link) :
			ATC.notice(b.Sig, b.Data);
			break;
		case static_cast<int>(ATC_Beacon::ORP) :
			break;
		case static_cast<int>(ATC_Beacon::TASC_P0) :
			TASC.setBeacon(0, b);
			break;
		case static_cast<int>(ATC_Beacon::TASC_P1) :
			TASC.setBeacon(1, b);
			break;
		case static_cast<int>(ATC_Beacon::TASC_P2) :
			TASC.setBeacon(2, b);
			break;
		case static_cast<int>(ATC_Beacon::TASC_P3) :
			TASC.setBeacon(3, b);
			break;
		case static_cast<int>(ATC_Beacon::TASC_P4) :
			TASC.setBeacon(4, b);
			break;
		case static_cast<int>(ATC_Beacon::TASC_passage) :
			TASC.setBeacon(-1, b);
		default:
			break;
	}
}


