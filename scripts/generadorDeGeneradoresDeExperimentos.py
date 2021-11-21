#vamos a generar 4 carpetas con distintos niveles de uniformidad:
#esto primero es para el experimento de inserciones con distintas uniformidades
import os
from generExp import generarExperimento

uniformidades = [1,2,3]

def experimentoDistintasUniformidades(u):
	for k in uniformidades:
		generarExperimento(k,"a","experimento_inestabilidad_"+str(k))

experimentoDistintasUniformidades(uniformidades)		
