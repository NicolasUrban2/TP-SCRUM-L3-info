# Online Python compiler (interpreter) to run Python online.
# Write Python 3 code in this online editor and run it.
import os
import linecache

def main(arg):
	return 0

#Récupère le contenu du PDF file (sans l'extension .pdf) et le retourne sous forme d'un string

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
 

def convertirPDF(file):
	commande = "pdftotext "
	commande += file
	os.system(commande)
