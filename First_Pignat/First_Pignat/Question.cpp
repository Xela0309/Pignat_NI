
#include <iostream> 
#include <Windows.h> 
#include "head.h"
#include "NIDAQmx.h"
using namespace std;
#include <Windows.h> // pour la fonction Sleep()

int Q2a1()
{
	TaskHandle tacheLectureAN0;

	int32 erreur = 0;

	uInt32 valAN0 = 0;

	erreur = DAQmxCreateTask("", &tacheLectureAN0);

	if (erreur < 0) {

		cout << "Erreur creation de la tache..." << endl;

		return -1;
	}

	erreur = DAQmxCreateDIChan(tacheLectureAN0, "Dev1/port0", "", DAQmx_Val_ChanForAllLines);
	if (erreur < 0) {
		cout << "Erreur de creation DIchan" << endl;
		return -2;
	}

	erreur = DAQmxStartTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur au demarrage de la tache..." << endl;
		return -3;
	}

	erreur = DAQmxReadDigitalScalarU32(tacheLectureAN0, 0, &valAN0, NULL);
	if (erreur < 0) {
		cout << "Erreur de lecture de la tension " << endl;
		return -4;
	}

	cout << valAN0 << endl ;


	erreur = DAQmxStopTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur lors de l'arret de la tache..." << endl;
		return -5;
	}

	erreur = DAQmxClearTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur lors de la suppression de la tache..." << endl;
		return -6;
	}
}

int Q2a2()
{
	TaskHandle tacheLectureAN0;

	int32 erreur = 0;

	erreur = DAQmxCreateTask("", &tacheLectureAN0);

	if (erreur < 0) {

		cout << "Erreur creation de la tache..." << endl;

		return -1;
	}

	erreur = DAQmxCreateDOChan(tacheLectureAN0, "Dev1/port0", "", DAQmx_Val_ChanForAllLines);
	if (erreur < 0) {
		cout << "Erreur de creation de la voie analogique en entree..." << endl;
		return -2;
	}

	erreur = DAQmxStartTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur au demarrage de la tache..." << endl;
		return -3;
	}


	erreur = DAQmxWriteDigitalScalarU32(tacheLectureAN0, 1, 1, 255, NULL);
	if (erreur < 0) {
		cout << "Erreur de creation de la voie analogique en entree..." << endl;
		return -2;
	}

	Sleep(2000);


	erreur = DAQmxWriteDigitalScalarU32(tacheLectureAN0, 1, 1, 0, NULL);
	if (erreur < 0) {
		cout << "Erreur de creation de la voie analogique en entree..." << endl;
		return -2;
	}

	erreur = DAQmxStopTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur lors de l'arret de la tache..." << endl;
		return -5;
	}

	erreur = DAQmxClearTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur lors de la suppression de la tache..." << endl;
		return -6;
	}
}

int Q2a3()
{

	TaskHandle tacheLectureAN0;

	int32 erreur = 0;

	erreur = DAQmxCreateTask("", &tacheLectureAN0);

	if (erreur < 0) {

		cout << "Erreur creation de la tache..." << endl;

		return -1;
	}

	erreur = DAQmxCreateDOChan(tacheLectureAN0, "Dev1/port0", "", DAQmx_Val_ChanForAllLines);
	if (erreur < 0) {
		cout << "Erreur de creation de la voie analogique en entree..." << endl;
		return -2;
	}

	erreur = DAQmxStartTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur au demarrage de la tache..." << endl;
		return -3;
	}


	erreur = DAQmxWriteDigitalScalarU32(tacheLectureAN0, 1, 1, 20, NULL);
	if (erreur < 0) {
		cout << "Erreur de creation de la voie analogique en entree..." << endl;
		return -2;
	}

	Sleep(2000);

	erreur = DAQmxWriteDigitalScalarU32(tacheLectureAN0, 1, 1, 16, NULL);
	if (erreur < 0) {
		cout << "Erreur de creation de la voie analogique en entree..." << endl;
		return -2;
	}

	Sleep(2000);

	erreur = DAQmxWriteDigitalScalarU32(tacheLectureAN0, 1, 1, 0, NULL);
	if (erreur < 0) {
		cout << "Erreur de creation de la voie analogique en entree..." << endl;
		return -2;
	}

	erreur = DAQmxStopTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur lors de l'arret de la tache..." << endl;
		return -5;
	}

	erreur = DAQmxClearTask(tacheLectureAN0);
	if (erreur < 0) {
		cout << "Erreur lors de la suppression de la tache..." << endl;
		return -6;
	}
}