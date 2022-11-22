#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <direct.h>

using namespace std;

void convertirPDF(string file);
string recupererText(string file);
string chercherTitre(string texte, int *posFin);
string chercherAbstract(string texte);
void parser(string directory);

int main(int argvc, char *argv[])
{
	if(argvc != 2)
	{
		cerr<<"Nombre d'arguments incorrect"<<endl;
		return -1;
	}
	parser(argv[1]);
	return 0;
}

/**
 * 	Récupère le nom, le titre et le résumé de tous les articles PDF dans le répertoire et les ressort dans des .txt dans un sous répertoire
 *	[directory] doit avoir un '/' à la fin
 */ 

void parser(string directory)
{
	string currentFile;
	DIR* dir = opendir(directory.c_str());
	struct dirent * dp;
	dp = readdir(dir);

	string removeCommand = "rm -r "+directory+"sorties/";
	string commande = directory+"sorties/";
	system(removeCommand.c_str());	// Supprime le sous répertoire sorties/ s'il existe déjà (n'est pas bloquant sinon)
	//cerr<<removeCommand<<endl;
	_mkdir(commande.c_str());	// Crée le sous répertoire sorties/ (ne fonctionnait pas avec une commande mkdir dans system())
	//cerr<<commande.c_str()<<endl;
	
	// Pour chaque fichier dans le répertoire directory
	while(dp)
	{
		currentFile = dp->d_name;
		size_t extension = currentFile.find(".pdf");
		if(extension!=string::npos)		// Traite les fichiers .pdf uniquement
		{
			currentFile = currentFile.substr(0, currentFile.find(".pdf"));	// Récupère le nom du fichier sans l'extension
			string texte;
			texte = recupererText(directory+currentFile);	// Récupère le texte du PDF

			string titre;
			int posFinTitre;
			titre = chercherTitre(texte, &posFinTitre);	// Récupère le titre du PDF

			string abstract;
			abstract = chercherAbstract(texte);	// Récupère le résumé du PDF

			// Ecrit les résultats dans un fichier .txt du même nom que le PDF dans le sous répertoire sorties/
			string txtPath = directory+"sorties/"+currentFile+".txt";
			ofstream flux(txtPath.c_str());
			if(flux)
			{
				flux<<currentFile<<"\n"<<titre<<"\n"<<abstract<<endl;
			}
			else
			{
				cerr<<"Erreur lors de l'ouverture du fichier "<<txtPath<<endl;
			}
		}
		dp=readdir(dir);
	}
	closedir(dir);
}

/**
 * 	Récupère la ligne de "texte" en dessous du premier mot "Abstract"
 */ 

string chercherAbstract(string texte)
{
	string abstract;
	size_t firstAbstract = texte.find("Abstract");
	if(firstAbstract==string::npos)
	{
		return "Aucun resume";	
	}
	abstract = texte.substr(firstAbstract+9, texte.find("\n",firstAbstract+9)-(firstAbstract+9));
	return abstract;
}

/**
 * 	Récupère la première ligne de "texte" (qui représente en général le titre)
 */

string chercherTitre(string texte, int *posFin)
{
	string titre;
	size_t posTitre = 0;

	// Cherche la position du titre du journal. S'il est avant le titre, on cherche le titre après
	size_t posJournal = texte.find("Journal");
	size_t posFinJournal = 0;
	if(posJournal != string::npos)
	{
		if(posJournal < 10)	// Si le journal est au début (moins de 10 caractères au début du fichier)
		{
			posFinJournal = texte.find("\n", posJournal);
			posFinJournal += 2;	// Prend en compte le fait qu'il y a généralement 2 sauts de lignes après
			posTitre = posFinJournal;
		}
	}

	// Cherche la position de la date de soumission de la même façon que le journal mais après ce dernier
	size_t posSubmit = texte.find("Submitted");
	size_t posFinSubmit = 0;
	if(posSubmit != string::npos)
	{
		if(posSubmit < 100)	// Si la date de soumission est vers le début
		{
			posFinSubmit = texte.find("\n", posSubmit);
			posFinSubmit += 2;
			if(posFinSubmit > posFinJournal)
			{
				posTitre = posFinSubmit;
			}
		}
	}

	// Cherche la source 
	size_t posSource = texte.find("From:");
	size_t posFinSource = 0;
	if(posSource != string::npos)
	{
		if(posSource < 50)	// Si la source est vers le début
		{
			posFinSource = texte.find("\n", posSource);
			posFinSource += 2;
			if(posFinSource > posFinJournal || posFinSource > posFinSubmit)
			{
				posTitre = posFinSource;
			}
		}
	}
	int posFinTitre = texte.find("\n", posTitre+1)-(posTitre);
	titre = texte.substr(posTitre, posFinTitre);
	posFin = &posFinTitre;
	return titre;
} 

/**
 * 	Récupère le contenu du PDF file (sans l'extension .pdf) et le retourne sous forme d'un string
 */ 

string recupererText(string file)
{
	string pdfFile = file+".pdf";
	string txtFile = file+".txt";
	string content;
	convertirPDF(pdfFile);
	ifstream flux(txtFile);
	if(flux)
	{
		string ligne;
		while(getline(flux, ligne))
		{
			content +=ligne;
			content +="\n";
		}
		flux.close();	// Ferme le flux avant de supprimer le fichier (le texte a été récupéré, il faut nettoyer l'environnement)
		remove(txtFile.c_str());	// Supprime le fichier txt
		cerr<<txtFile.c_str()<<endl;
		return content;
	}
	return NULL;
}

/**
 * 	Crée la version txt du PDF file (avec l'extension .pdf)
 */ 

void convertirPDF(string file)
{
	string commande = "pdftotext ";
	commande += file;
	char commandeChar[commande.length()+1];
	strcpy(commandeChar, commande.c_str());
	system((char *) commandeChar);
}