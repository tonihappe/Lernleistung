import math

def getbeschl(PRx,PRy,PMx,PMy):
	mM = 7.349e22 #in kg
	mE = 5.972e24 #in kg
	mR = 5000 #in kg
	gamma = 6.67408e-11 #Gravitationskonstante m^3/s^2kg
	
	#Abstände/Entfernungen
	rEM = 3844e5 #Abstand Erde-Mond NOCH ZU ÄNDERN WENN SICH MOND BEWEGT
	
	rEX = PRx#-PEx da die Erde auf dem Ursprung liegt
	rEY = PRy#-PEy da die Erde auf dem Ursprung liegt
	rER = math.sqrt(rEX*rEX+rEY*rEY)

	rMX = PRx-PMx
	rMY = PRy-PMy
	rMR = math.sqrt(rMx*rMx+rMy*rMy)

	#Berechnungen der Kräfte der Erde/des Mondes
	FER =-1*gamma*mR*mE/(rER**2) #negativ, da erde "hinter" raumschiff liegt auf x-Achse
	FMR = gamma*mR*mM/((rMR-PR)**2)

	#Zerlegung der Kräfte --> Bestimmung der notwendigen Winkel
	betaREF=math.tan(rEX/rEY)
	alphaMER= math.pi-betaREF
	FEx = math.cos(alphaMER)*FER
	FEy = math.sin(alphaMER)*FER

	betaFRM = math.tan(rMX/rMY) #umbenennen?
	alphaRME = math.pi-betaFRM
	FMx = math.cos(alphaRME)*FMR
	FMy = math.sin(alphaRME)*FMR
	#############################################################################################################################################################################################

	#alt FRES = FER+FMR
	aR = FRES/mR
	return aR,FRES




#deklarationen/initialisierungen
n= 1000000
rM = 3476e3 #Radius Mond m
rE = 6371e3 # Radius Erde m 
rEM = 3844e5 #Abstand Erde-Mond NOCH ZU ÄNDERN WENN SICH MOND BEWEGT
h = 1 #Schrittweite
t=[0]*n
PRx=[0]*n
PRy=[0]*n
PEx=[0]*n
PEy=[0]*n
PMx=[0]*n
PMy=[0]*n
vx=[0]*n
vy=[0]*n
ax=[0]*n
ay=[0]*n

#Anfangswerte
t[0]=0
PRx[0]=6371e3	# Startpunkt des R (Radius der Erde)
PRy[0]=0

print("Startgeschwindigkeit: ")
v[0]=int(input()) #Startgeschwindigkeit
print("\n\n")

a[0],FRES = getbeschl(PR[0])
ausg = open ("Ausgabe.dat","w")
ausg.write(str(t[0])+"\t"+str(PR[0])+"\t"+str(v[0])+"\t"+str(a[0])+"\n")

#Schleife für einzelne Berechnung am jeweiligen Punkt
for i in range(1,n,1):
	aR, FRES =getbeschl(PRx[i-1])
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
	a[i],FRES = getbeschl(PR[i-1])
	
	print("DURCHGANG NUMMER: ", i)
	if (PR[i]<=rE):
		print("Auf Erde aufgeschlagen!")
		break
	elif(PR[i]>(rEM-rM)):
		print("Auf Mond aufgeschlagen!")
		break

	#werte in datei
	ausg.write(str(t[i])  +" \t "+str(PR[i])+ " \t " + str(v[i]) + " \t " + str(a[i])+"\n")
	#force.write(str(FRx[i-1])+"\t"+str(FRy[i-1])+"\n")

ausg.close()