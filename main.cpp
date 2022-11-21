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
string chercherTitre(string texte);
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
			titre = chercherTitre(texte);	// Récupère le titre du PDF

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

string chercherTitre(string texte)
{
	string titre;
	titre = texte.substr(0, texte.find("\n"));
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