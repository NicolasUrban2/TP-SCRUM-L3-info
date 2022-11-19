#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstring>

using namespace std;

void convertirPDF(string file);
string recupererText(string file);

int main(int argvc, char *argv[])
{
	return 0;
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