import sys

if len(sys.argv) != 3:
    print("\nTo make this program work you need to provide two arguments : a text file and a number n.\nThis program will then compute the n words that appears the most in the text file (except the words in stop_words.txt and the worlds of one letter).\n\n")
else:
    dico = {}
    fichier = open(sys.argv[1])
    lignes = fichier.readlines()
    fichier.close()
    for ligne in lignes:
        ligneCourante = ligne.lower()
        caracteres = list(ligneCourante)
        for i in range(len(caracteres)):
            lettre = caracteres[i]
            if (not(lettre.isalnum())):
                caracteres[i] = ' '
        ligneCourante = "".join(caracteres)
        mots = ligneCourante.split(' ')
        for m in mots:
            if (len(m) > 1):
                if (m in dico):
                    dico[m]+= 1
                else:
                    dico[m] = 1
    fichier = open("stop_words.txt")
    lignes = fichier.readlines()
    fichier.close()
    for ligne in lignes:
        ligneCourante = ligne.lower()
        caracteres = list(ligneCourante)
        for i in range(len(caracteres)):
            lettre = caracteres[i]
            if (not(lettre.isalnum())):
                caracteres[i] = ' '
        ligneCourante = "".join(caracteres)
        mots = ligneCourante.split(' ')
        for m in mots:
            if (m in dico):
                _ = dico.pop(m)
    l = []
    for mot in dico:
        l.append([mot, dico[mot]])
    def cle(C):
        return C[1]
    l = (sorted(l, key=cle, reverse=True))
    l = l[0:int(sys.argv[2])]
    for (a,b) in l:
        print(a + " : " + str(b))