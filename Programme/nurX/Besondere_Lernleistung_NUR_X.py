import math

def getbeschl(PR):
	mM = 7.349e22 #in kg
	mE = 5.972e24 #in kg
	mR = 5000 #in kg
	G = 6.67408e-11 #Gravitationskonstante m^3/s^2kg
	rEM = 3844e5 #Abstand Erde-Mond

	FE =-1*G*mR*mE/(PR**2) #negativ, da erde "hinter" raumschiff liegt auf x-Achse
	FM = G*mR*mM/((rEM-PR)**2)
	FRES = FE+FM
	aR = FRES/mR
	return aR, FRES,FE,FM




#deklarationen/initialisierungen
n= 1000000
rM = 3476e3 #Radius Mond m
rE = 6371e3 # Radius Erde m 
rEM = 3844e5 #Abstand Erde-Mond
h = 1 #Schrittweite
t=[0]*n
PR=[0]*n
v=[0]*n
#a=[0]*n



#Anfangswerte
t[0]=0
PR[0]=6371e3	# Startpunkt des R (Radius der Erde)

print("Startgeschwindigkeit: ")
v[0]=int(input()) #Startgeschwindigkeit
print("\n\n")

aR, FRES,FE,FM = getbeschl(PR[0])
ausg = open ("Ausgabe.dat","w")
ausg.write(str(t[0])+"\t"+str(PR[0])+"\t"+str(v[0])+"\t"+str(aR)+"\t"+str(FRES)+"\t"+str(FE)+"\t"+str(FM)+"\n")

#Schleife für einzelne Berechnung am jeweiligen Punkt
for i in range(1,n,1):
	aR, FRES,FE,FM =getbeschl(PR[i-1])
	print("FRES = ", FRES,"\naR = ",aR)

	#k berechnet x 
	#l berechnet v
	#kräfte bei zwischenschritten als Unterprogramm
	
	kx1 = v[i-1]
	lx1 = aR
		
	kx2 = v[i-1]+0.5*h*lx1
	lx2 = aR
	
	kx3 = v[i-1]+0.5*h*lx2
	lx3 = aR
	
	kx4 = v[i-1]+ h * lx3
	lx4 = aR
	
	t[i] = t[i-1]+h
	PR[i] = PR[i-1]+ h/6 * (kx1 + 2 * kx2 + 2* kx3 + kx4)
	v[i] = v[i-1]+ ((h/6) * (lx1 + 2 * lx2 + 2* lx3 + lx4))
	aR, FRES,FE,FM = getbeschl(PR[i-1])
	
	print("DURCHGANG NUMMER: ", i)
	if (PR[i]<=rE):
		print("Auf Erde aufgeschlagen!")
		break
	elif(PR[i]>(rEM-rM)):
		print("Auf Mond aufgeschlagen!")
		break

	#werte in datei
	ausg.write(str(t[0])+"\t"+str(PR[0])+"\t"+str(v[0])+"\t"+str(aR)+"\t"+str(FRES)+"\t"+str(FE)+"\t"+str(FM)+"\n")
	#force.write(str(FRx[i-1])+"\t"+str(FRy[i-1])+"\n")

ausg.close()