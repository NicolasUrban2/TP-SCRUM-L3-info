# Online Python compiler (interpreter) to run Python online.
# Write Python 3 code in this online editor and run it.
import os
import linecache

def main(arg):
	return 0

#Récupère le contenu du PDF file (sans l'extension .pdf) et le retourne sous forme d'un string

def parser(directory):
	dir = os.opendir(directory)
	dp = os.readdir(dir)

	removeCommande = "rm -r "+directory+"sorties/"
	commande = directory+"sorties/"
	os.system(removeCommande)
	os.mkdir(commande)

	while(dp):
		currentFile = dp.d_name
		extension = currentFile.find(".pdf")
		if(extension):

			currentFile = currentFile.substr(0, currentFile.find(".pdf"))
			texte = recupererText(directory+currentFile)
			titre = chercherTitre(texte)
			absract = chercherAbstract(texte)

			txtPath = directory+"sorties/"+currentFile+".txt"
			if(txtPath):
				txtPath<<currentFile<<"\n"<<titre<<"\n"<<abstract<<endl
				
def recupererText(file):
	pdfFile = file+".pdf"
	txtFile = file+".txt"
	convertirPDF(pdfFile)
	flux = txtFile
	if(flux):
		ligne = ""
		while(linecache.getline(flux, ligne)):
			content +=ligne
			content +="\n"
		
		return content
	
	return 0


 #Crée la version txt du PDF file (avec l'extension .pdf)
 
def chercherTitre(texte):
	titre = texte.substr(0, texte.find("\n"))
	return titre

def chercherAbstract(texte):
	firstAbstract = texte.find("Abstract")
	if(not firstAbstract):
		return "Aucun resume"
	abstract = texte.substr(firstAbstract+9, texte.find("\n",firstAbstract+9)-(firstAbstract+9))
	return abstract


def convertirPDF(file):
	commande = "pdftotext "
	commande += file
	os.system(commande)
