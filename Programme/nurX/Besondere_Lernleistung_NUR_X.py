import math

def getbeschl(PR):
	mM = 7.349e22 #in kg
	mE = 5.972e24 #in kg
	mR = 5000 #in kg
	G = 6.67408e-11 #in m^3/s^2kg; Gravitationskonstante
	rEM = 3844e5 #in m; Abstand Erde-Mond

	#Berechnung der Kräfte und der Beschleunigung
	FE =-1*G*mR*mE/(PR**2) #negativ, da die Erde auf der x-Achse "hinter" dem Raumschiff liegt
	FM = G*mR*mM/((rEM-PR)**2) 
	FRES = FE+FM #resultierende Kraft auf das Raumschiff
	aR = FRES/mR #resultierende Beschleunigung

	return aR, FRES,FE,FM


#deklarationen/initialisierungen
n= 1000000 #Anzahl an maximalen Wiederholungen
h = 1 #Schrittweite

t=[0]*n #Zeit
PR=[0]*n #Punt des Raumschiffes
v=[0]*n #Geschwindigkeit des Raumschiffes

rM = 1737e3 #Radius Mond m
rE = 6371e3 # Radius Erde m 
rEM = 3844e5 #Abstand Erde-Mond


#Anfangswerte
t[0]=0
PR[0]=6371e3	# Startpunkt des Raumschiffs (Radius der Erde)

print("Startgeschwindigkeit: ")
v[0]=float(input()) #Eingabe der Startgeschwindigkeit
print("\n\n")

aR, FRES,FE,FM = getbeschl(PR[0])
ausg = open ("Ausgabe.dat","w")
ausg.write(str(t[0])+"\t"+str(PR[0])+"\t"+str(v[0])+"\t"+str(aR)+"\t"+str(FRES)+"\t"+str(FE)+"\t"+str(FM)+"\n") #Anfangswerte in die Datei schreiben

#Schleife für einzelne Berechnung am jeweiligen Punkt
for i in range(1,n,1):
	aR, FRES,FE,FM =getbeschl(PR[i-1])
	print("FRES = ", FRES,"\naR = ",aR)

	#k berechnet x 
	#l berechnet v
	#kräfte bei zwischenschritten als Unterprogramm
	
	#RungeKutta4
	kx1 = v[i-1]
	lx1 = aR
		
	kx2 = v[i-1]+0.5*h*lx1
	lx2 = aR
	
	kx3 = v[i-1]+0.5*h*lx2
	lx3 = aR
	
	kx4 = v[i-1]+ h * lx3
	lx4 = aR
	
	t[i] = t[i-1]+h
	PR[i] = PR[i-1]+ h/6.0 * (kx1 + 2 * kx2 + 2* kx3 + kx4)
	v[i] = v[i-1]+ ((h/6.0) * (lx1 + 2 * lx2 + 2* lx3 + lx4))
	aR, FRES,FE,FM = getbeschl(PR[i-1])
	
	
	print("DURCHGANG NUMMER: ", i)
	#Test ob Raumschiff Mond oder Erde erreicht hat
	if (PR[i]<=rE):
		print("Auf Erde aufgeschlagen!")
		break
	elif(PR[i]>(rEM-rM)):
		print("Auf Mond aufgeschlagen!")
		break

	#werte in datei
	ausg.write(str(t[i])+"\t"+str(PR[i])+"\t"+str(v[i])+"\t"+str(aR)+"\t"+str(FRES)+"\t"+str(FE)+"\t"+str(FM)+"\n")
	

ausg.close()

#Bremsvorgang mit reinbringe#
#if anweisung ob lagrange --> Zündung bremsdüsen