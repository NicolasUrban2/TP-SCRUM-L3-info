#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <sstream>

using namespace std;

void convertirPDF(string file);
string recupererText(string file);
string chercherTitre(string texte);
string chercherAbstract(string texte);

int main(int argvc, char *argv[])
{
	string texte;
	texte = recupererText("CORPUS_TRAIN/ACL2004-HEADLINE");
	string titre;
	titre = chercherTitre(texte);
	cout<<titre<<endl;
	string abstract;
	abstract = chercherAbstract(texte);
	cout<<abstract<<endl;
	return 0;
}

/**
 * 	Récupère la ligne de "texte" en dessous du premier mot "abstract"
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