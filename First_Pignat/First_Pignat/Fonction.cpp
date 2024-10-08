// Exemple d'utilisation de la biblioth�que NI DAQ
// BTS IRIS, Avril 2013
#include <iostream> // pour cin, cout
#include <Windows.h> // pour la fonction Sleep()
// prototypes des fonctions DAQmx, ce fichier doit �tre plac� dans le r�pertoire de votre projet
#include "NIDAQmx.h"
#include "EsMelangeur.h"
#include "head.h"
using namespace std;
// Fonction principale
int fonctiondep(void)
{

	cout << "..:: Debut programme ::.." << endl;

	CEsMelangeur melangeur;
	melangeur.TestAlgo();
	
	cout << "..:: Fin programme ::.." << endl;
	return 0;
}
