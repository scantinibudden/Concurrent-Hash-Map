from creadorPalabras import WordGenerator
import os
import sys


"""
la idea es que haya una carpeta por cada nivel de inestabilidad de las muestras
No importa la letra insetable
"""
def generarExperimento(unstability, letter, folder):
	word_file = "/usr/share/dict/words"
	WORDS = open(word_file).read().splitlines()
	filtered = []
	for word in WORDS:
		if (ord(word[0])<=ord('d') and ord(word[0])>=ord('a')):
			filtered.append(word)
		
		
	superlong_word = "abcde" #hacer una mas larga todavia
	wg = WordGenerator(filtered)
	wg.superAmplifyDict(superlong_word)
	
	wg.splitByStart()

	for lis in wg.split:
		print(len(lis))
	
	now = os.getcwd()
	newfold = now + "/" +folder
	if not (os.path.exists(newfold)):
		os.mkdir(newfold)
	letasc = 97
	i = 0
	reps = unstability
	while(i<3):
		i = i + 1
		let = chr(letasc)
		if(let==letter):
			now = wg.getNwordsFrom(let, 20000,(unstability-reps)*20000)
		else:
			now = wg.getNwordsFrom(let, 20000,0)
		
		
		# escribirlas en el archivo
		f = open(newfold+"/"+let+str(i)+"2000", "w")
		f.writelines(["%s\n" % item  for item in now])
			
		if(let != letter or ((let==letter) & (reps==1))):
			letasc = letasc + 1
		else:
			reps = reps - 1

