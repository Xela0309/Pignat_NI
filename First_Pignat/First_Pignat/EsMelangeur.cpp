#include <iostream> 
#include "head.h"
#include "NIDAQmx.h"
using namespace std;
#include <Windows.h> // pour la fonction Sleep()
#include "EsMelangeur.h"

CEsMelangeur::CEsMelangeur()
	: m_arret(false), m_capteur_bas_R1(false), m_capteur_bas_R2(false), m_capteur_bas_R3(false), 
	m_capteur_niveau_bas(false), m_capteur_niveau_haut(false), m_evacuation(false), 
	m_malaxeur(false), m_manuel_auto(false), m_marche(false), m_poids(0), 
	m_tache_ecriture_tor(NULL), m_tache_lecture_ana(NULL), m_tache_lecture_tor(NULL),
	m_vanne_lubrifiant(false), m_vanne_plastifiant(false), m_vanne_pvc_base(false), 
	m_vanne_pvc_base_FD(false), m_vanne_vidange(false), m_voyant_rouge(false)
{


}

int32 CEsMelangeur::initEsMelangeur()
{

	int32 erreur_lecture_ana, erreur_lecture_tor, erreur_ecriture_tor;

	erreur_lecture_ana = DAQmxCreateTask("", &m_tache_lecture_ana);
	erreur_lecture_tor = DAQmxCreateTask("", &m_tache_lecture_tor);
	erreur_ecriture_tor = DAQmxCreateTask("", &m_tache_ecriture_tor);

	if (erreur_lecture_ana || erreur_lecture_tor || erreur_ecriture_tor < 0) {
		cout << "Erreur creation de la tache..." << endl;
		return -1;
	}

	erreur_lecture_tor = DAQmxCreateDIChan(m_tache_lecture_tor, "Dev1/port1", "", DAQmx_Val_ChanForAllLines);
	erreur_lecture_ana = DAQmxCreateAIVoltageChan(m_tache_lecture_ana, "Dev1/ai0", "", DAQmx_Val_RSE,0.0,10.0, DAQmx_Val_Volts,NULL);
	erreur_ecriture_tor = DAQmxCreateDOChan(m_tache_ecriture_tor, "Dev1/port0", "", DAQmx_Val_ChanForAllLines);

	if (erreur_lecture_ana  < 0) {
		cout << "Erreur de creation Chan ana" << endl;
		return -2;
	}
	if (erreur_lecture_tor < 0) {
		cout << "Erreur de creation Chan lect tor " << endl;
		return -2;
	}
	if (erreur_ecriture_tor < 0) {
		cout << "Erreur de creation Chan ecr tor" << endl;
		return -2;
	}



	erreur_lecture_ana = DAQmxStartTask(m_tache_lecture_ana);
	erreur_lecture_tor = DAQmxStartTask(m_tache_lecture_tor);
	erreur_ecriture_tor = DAQmxStartTask(m_tache_ecriture_tor);

	if (erreur_lecture_ana || erreur_lecture_tor || erreur_ecriture_tor < 0) {
		cout << "Erreur au demarrage de la tache..." << endl;
		return -3;
	}

	return int32();
}

int32 CEsMelangeur::fermerEsMelangeur()
{
	int32 erreur_lecture_ana, erreur_lecture_tor, erreur_ecriture_tor;

	erreur_lecture_ana = DAQmxStopTask(m_tache_lecture_ana);
	erreur_lecture_tor = DAQmxStopTask(m_tache_lecture_tor);
	erreur_ecriture_tor = DAQmxStopTask(m_tache_ecriture_tor);
	
	if (erreur_lecture_ana || erreur_lecture_tor || erreur_ecriture_tor < 0) {
		cout << "Erreur lors de l'arret de la tache..." << endl;
		return -1;
	}

	erreur_lecture_ana = DAQmxClearTask(m_tache_lecture_ana);
	erreur_lecture_tor = DAQmxClearTask(m_tache_lecture_tor);
	erreur_ecriture_tor = DAQmxClearTask(m_tache_ecriture_tor);

	if (erreur_lecture_ana || erreur_lecture_tor || erreur_ecriture_tor < 0) {
		cout << "Erreur lors de la suppression de la tache..." << endl;
		return -2;
	}
	return int32();
}

int32 CEsMelangeur::lireEntrees()
{
	uInt32 capteur = 0;
	float64 tension = 0;

	int32 erreur_lecture_ana, erreur_lecture_tor;


	erreur_lecture_tor = DAQmxReadDigitalScalarU32(m_tache_lecture_tor, 0, &capteur, NULL);
	erreur_lecture_ana = DAQmxReadAnalogScalarF64(m_tache_lecture_ana, 10.0, &tension, NULL);
	
	if (erreur_lecture_tor < 0) {
		cout << "Erreur de lecture de la tension tor " << endl;
		return -1;
	}

	if (erreur_lecture_ana < 0) {
		cout << "Erreur de lecture de la tension ana " << endl;
		return -1;
	}

	if (capteur >= 128)
	{
		m_manuel_auto = true;
		capteur = capteur - 128;
	}
	else
	{
		m_manuel_auto = false;
	}
	if (capteur >= 64)
	{
		m_arret = true;
		capteur = capteur - 64;
	}
	else
	{
		m_arret = false;
	}
	if (capteur >= 32)
	{
		m_marche = true;
		capteur = capteur - 32;
	}
	else
	{
		m_marche = false;
	}
	if (capteur >= 16)
	{
		m_capteur_niveau_haut = true;
		capteur = capteur - 16;
	}
	else
	{
		m_capteur_niveau_haut = false;
	}
	if (capteur >= 8)
	{
		m_capteur_niveau_bas = true;
		capteur = capteur - 8;
	}
	else
	{
		m_capteur_niveau_bas = false;
	}
	if (capteur >= 4)
	{
		m_capteur_bas_R3 = true;
		capteur = capteur - 4;
	}
	else
	{
		m_capteur_bas_R3 = false;
	}
	if (capteur >= 2)
	{
		m_capteur_bas_R2 = true;
		capteur = capteur - 2;
	}
	else
	{
		m_capteur_bas_R2 = false;
	}
	if (capteur >= 1)
	{
		m_capteur_bas_R1 = true;
		capteur = capteur - 1;
	}
	else
	{
		m_capteur_bas_R1 = false;
	}

	m_poids = static_cast<int>(tension * -200.0);

	//afficherEntrees();

	return int32();
}

void CEsMelangeur::afficherEntrees()
{
	cout << "\nCapteur bas R1 : " << m_capteur_bas_R1 
		<< "\nCapteur bas R2 : " << m_capteur_bas_R2
		<< "\nCapteur bas R3 : " << m_capteur_bas_R3
		<< "\nCapteur niveau bas : " << m_capteur_niveau_bas
		<< "\nCapteur niveau haut : " << m_capteur_niveau_haut
		<< "\nMarche : " << m_marche
		<< "\nArret : "  << m_arret
		<< "\nManuel/Auto : " << m_manuel_auto
		<< "\nPoids : " << m_poids
		<< endl;
}

int32 CEsMelangeur::majSorties()
{
	uInt32 valeur = 0;

	if (m_voyant_rouge == true)
	{
		valeur += 128;
	}
	if (m_evacuation == true)
	{
		valeur += 64;
	}
	if (m_malaxeur == true)
	{
		valeur += 32;
	}
	if (m_vanne_vidange == true)
	{
		valeur += 16;
	}
	if (m_vanne_lubrifiant == true)
	{
		valeur += 8;
	}
	if (m_vanne_plastifiant == true)
	{
		valeur += 4;
	}
	if (m_vanne_pvc_base_FD == true)
	{
		valeur += 2;
	}
	if (m_vanne_pvc_base == true)
	{
		valeur += 1;
	}

	DAQmxWriteDigitalScalarU32(m_tache_ecriture_tor,1, 1, valeur, NULL);

	return int32();
}

void CEsMelangeur::afficherSorties()
{
	cout << "\nVanne PVC base : " << m_vanne_pvc_base
		<< "\nVanne PVC base FD : " << m_vanne_pvc_base_FD
		<< "\nVanne plastifiant : " << m_vanne_plastifiant
		<< "\nVanne lubrifiant : " << m_vanne_lubrifiant
		<< "\nVanne vidange : " << m_vanne_vidange
		<< "\nMalaxeur : " << m_malaxeur
		<< "\nEvacuation : " << m_evacuation
		<< "\nVoyant rouge : " << m_voyant_rouge
		<< endl;
}

void CEsMelangeur::changerEtatSorties()
{
	char choix;

	while (true)
	{
		cout << "Voulez-vous changer l'etat des sorties ? (o/n) : ";
		cin >> choix;

		if (choix == 'o')
		{
			cout << "Vanne PVC base : ";
			cin >> m_vanne_pvc_base;
			cout << "Vanne PVC base FD : ";
			cin >> m_vanne_pvc_base_FD;
			cout << "Vanne plastifiant : ";
			cin >> m_vanne_plastifiant;
			cout << "Vanne lubrifiant : ";
			cin >> m_vanne_lubrifiant;
			cout << "Vanne vidange : ";
			cin >> m_vanne_vidange;
			cout << "Malaxeur : ";
			cin >> m_malaxeur;
			cout << "Evacuation : ";
			cin >> m_evacuation;
			cout << "Voyant rouge : ";
			cin >> m_voyant_rouge;

			majSorties();
			afficherSorties();
		}
		else
		{
			break;
		}


	}

}

int CEsMelangeur::getPoids()
{
	return m_poids;
}

bool CEsMelangeur::getCapteurBasR1()
{
	return m_capteur_bas_R1;
}

bool CEsMelangeur::getCapteurBasR2()
{
	return m_capteur_bas_R2;
}

bool CEsMelangeur::getCapteurBasR3()
{
	return m_capteur_bas_R3;
}

bool CEsMelangeur::getCapteurNiveauBas()
{
	return m_capteur_niveau_bas;
}

bool CEsMelangeur::getCapteurNiveauHaut()
{
	return m_capteur_niveau_haut;
}

bool CEsMelangeur::getMarche()
{
	return m_marche;
}

bool CEsMelangeur::getArret()
{
	return m_arret;
}

bool CEsMelangeur::getManuelAuto()
{
	return m_manuel_auto;
}

void CEsMelangeur::setVannePVCBase(bool etat)
{
	m_vanne_pvc_base = etat;
}

void CEsMelangeur::setVannePVCBaseFD(bool etat)
{
	m_vanne_pvc_base_FD = etat;
}

void CEsMelangeur::setVannePlastifiant(bool etat)
{
	m_vanne_plastifiant = etat;
}

void CEsMelangeur::setVanneLubrifiant(bool etat)
{
	m_vanne_lubrifiant = etat;
}

void CEsMelangeur::setVanneVidange(bool etat)
{
	m_vanne_vidange = etat;
}

void CEsMelangeur::setMalaxeur(bool etat)
{
	m_malaxeur = etat;
}

void CEsMelangeur::setEvacuation(bool etat)
{
	m_evacuation = etat;
}

void CEsMelangeur::setVoyantRouge(bool etat)
{
	m_voyant_rouge = etat;
}

std::string CEsMelangeur::texteErreur(int32 code)
{
	return std::string();
}

void CEsMelangeur::TestAlgo()
{
	cout << "Rentrer le temps de refroidissement" << endl;
	cin >> m_temps_refroidissement;
	cout << "Rentrer le temps de malaxage" << endl;
	cin >> m_temps_malaxage;
	cout << "Rentrer la masse de PVC de base" << endl;
	cin >> m_masse_pvc_base;
	cout << "Rentrer la masse de plastifiant" << endl;
	cin >> m_masse_plastifiant;
	cout << "Rentrer la masse de lubrifiant" << endl;
	cin >> m_masse_lubrifiant;
	cout << "Rentrer la duree de refroidissement" << endl;
	cin >> m_durée_refroid;
	cout << "Rentrer la duree de malaxage" << endl;
	cin >> m_durée_malax;

	initEsMelangeur();

	m_nEtat = 0;
	cout << "Test de l'algo du Professeur" << endl;
	while (true)
	{
		lireEntrees();
		switch (m_nEtat)
		{
		case 0:
			if (m_marche == true)
			{
				m_nEtat = 1;
			}
			else
			{
				m_nEtat = 0;
			}
			break;
		case 1:
			cout << "Etat 1" << endl;
			if (m_poids >= m_masse_pvc_base)
			{
				m_nEtat = 2;
			}
			else
			{
				m_nEtat = 1;
			}
			break;
		case 2:
			cout << "Etat 2" << endl;
			if (m_poids >= m_masse_pvc_base + m_masse_plastifiant ) 
			{
				m_nEtat = 3;
			}
			else
			{
				m_nEtat = 2;
			}
			break;
		case 3:
			cout << "Etat 3" << endl;
			if (m_poids >= m_masse_pvc_base + m_masse_plastifiant + m_masse_lubrifiant)
			{
				m_nEtat = 4;
			}
			else
			{
				m_nEtat = 3;
			}
			break;
		case 4:
			cout << "Etat 4" << endl;
			if (m_durée_malax == m_temps_malaxage)
			{
				m_nEtat = 5;
			}
			else
			{
				m_nEtat = 4;
			}
			break;
		case 5:
			cout << "Etat 5" << endl;
			if (m_poids <= 10)
			{
				m_nEtat = 6;
			}
			else
			{
				m_nEtat = 5;
			}
			break;
		case 6:
			cout << "Etat 6" << endl;
			if (m_durée_refroid == m_temps_refroidissement)
			{
				m_nEtat = 7;
			}
			else
			{
				m_nEtat = 6;
			}
			break;
		case 7:
			cout << "Etat 7" << endl;
			if (m_capteur_niveau_bas == false)
			{
				m_nEtat = 8;
			}
			else
			{
				m_nEtat = 7;
			}
			break;
		case 8:
			cout << "FIN" << endl;
		}

	
		if (m_nEtat == 1)
		{
			m_vanne_pvc_base = true;
		}
		else
		{
			m_vanne_pvc_base = false;
		}
		if (m_nEtat == 2)
		{
			m_vanne_plastifiant = true;
		}
		else
		{
			m_vanne_plastifiant = false;
		}
		if (m_nEtat == 3)
		{
			m_vanne_lubrifiant = true;
		}
		else
		{
			m_vanne_lubrifiant = false;
		}
		if (m_nEtat == 4)
		{
			m_durée_malax += 1 ;
		}
		if (m_nEtat == 5)
		{
			m_vanne_vidange = true;
		}
		else
		{
			m_vanne_vidange = false;
		}
		if (m_nEtat == 6)
		{
			m_durée_refroid += 1;
		}
		if (m_nEtat == 7)
		{
			m_evacuation = true;
		}
		else
		{
			m_evacuation = false;
		}
		if (m_nEtat == 3 || m_nEtat == 4)
		{
			m_malaxeur = true;
		}
		else
		{
			m_malaxeur = false;
		}
		majSorties();
	}
	fermerEsMelangeur();
}
