liste1 = []
liste2 = []

with open("D:/Documents/L3 Informatique/Génie logiciel/TP/Torres.txt", "r", encoding="utf8",) as f:
    liste1 = [line.strip() for line in f]
print(liste1)

res = " ".join(str(x) for x in liste1) #transformer en string
with open("D:/Documents/L3 Informatique/Génie logiciel/TP/Torres2.txt", 'w', encoding='utf-8') as f:
    f.write(res)

for i in range(0, len(liste1)):  #découper les fichiers textes
    if (liste1[i] == " "):
        head, sep, tail = liste1[i+1].partition(' ')
        liste2.append(head)
print()

