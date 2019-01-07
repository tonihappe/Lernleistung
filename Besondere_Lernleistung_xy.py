import math

def getbeschl(PRx,PRy,PMx,PMy):
	mM = 7.349e22 #in kg
	mE = 5.972e24 #in kg
	mR = 5000 #in kg
	G = 6.67408e-11 #Gravitationskonstante m^3/s^2kg
	
	#Abstände/Entfernungen
	rEM = 3844e5 #Abstand Erde-Mond NOCH ZU ÄNDERN WENN SICH MOND BEWEGT
	
	rEX = PRx#-PEx da die Erde auf dem Ursprung liegt
	rEY = PRy#-PEy da die Erde auf dem Ursprung liegt
	rER = math.sqrt(rEX*rEX+rEY*rEY)

	rMX = PRx-PMx
	rMY = PRy-PMy
	rMR = math.sqrt(rMX*rMX+rMY*rMY)

	#Berechnungen der Kräfte der Erde/des Mondes
	FER =-1*G*mR*mE/(rER**2) #negativ, da erde "hinter" raumschiff liegt auf x-Achse
	FMR = G*mR*mM/(rMR**2)

	#Zerlegung der Teilkräfte --> Bestimmung der notwendigen Winkel
	betaREF=math.tan(rEX/rEY) #############################################################################rEY division durch 0
	alphaMER= math.pi-betaREF
	FEx = math.cos(alphaMER)*FER
	FEy = math.sin(alphaMER)*FER

	betaFRM = math.tan(rMX/rMY) #umbenennen?
	alphaRME = math.pi-betaFRM
	FMx = math.cos(alphaRME)*FMR
	FMy = math.sin(alphaRME)*FMR

	#FRES 
	gammah = math.acos((FEx*FMx+FEy*FMy) / (math.sqrt((FEx**2)+(FEy**2)) + math.sqrt((FMx**2)+(FMy**2))))
	gammacoss = math.pi-gammah

	FRES=math.sqrt((FMR**2)+(FER**2)-2*FMR*FER*math.cos(gammacoss))
	#FRES Zerlegung
	alphaRES=math.asin((FMR*math.sin(alphaRME))/FRES)
	
	FRx = FRES * math.cos(alphaRES)
	FRy = FRES * math.sin(alphaRES)

	aR = FRES/mR
	return aR,aRx,aRy,FRES,FRx,FRy




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
v0=int(input()) #Startgeschwindigkeit
print("Startrichtung (in Grad --> 0° = x-Achse): ")
winkelv0 = int(input()) #Winkel der Startgeschwindigkeit
print("\n\n")
#Zerlegung der Startgeschwindigkeit in x und y Komponente
vx[0] = v0*math.cos(winkelv0)
vy[0] = v0*math.sin(winkelv0)

aR,aRx[0],aRy[0],FRES,FRx[0],FRy[0] = getbeschl(PRx[0],PRy[0],PMx[0],PMy[0])
ausg = open ("Ausgabe.dat","w")
ausg.write(str(t[0])+"\t"+str(PR[0])+"\t"+str(v[0])+"\t"+str(a[0])+"\n")

#Schleife für einzelne Berechnung am jeweiligen Punkt
for i in range(1,n,1):
	aR,aRx[i-1],aRy[i-1],FRES,FRx[i-1],FRy[i-1] = getbeschl(PRx[i-1],PRy[i-1],PMx[i-1],PMy[i-1])
	print("FRES = ", FRES,"\naR = ",aR)

	#k berechnet x 
	#l berechnet v
	#kräfte bei zwischenschritten als Unterprogramm
	####################################################################################################################################################################
	kx1 = vx[i-1]
	lx1 = aRx
		
	kx2 = vx[i-1]+0.5*h*lx1
	lx2 = aRx
	
	kx3 = vx[i-1]+0.5*h*lx2
	lx3 = aRx
	
	kx4 = vx[i-1]+ h * lx3
	lx4 = aRx
	
	t[i] = t[i-1]+h
	PRx[i] = PRx[i-1]+ h/6 * (kx1 + 2 * kx2 + 2* kx3 + kx4)
	vx[i] = vx[i-1]+ ((h/6) * (lx1 + 2 * lx2 + 2* lx3 + lx4))
	aR,aRx[i],aRy,FRES,FRx,FRy = getbeschl(PR[i-1])
	
	print("DURCHGANG NUMMER: ", i)
	if (PRx[i]<=rE):
		print("Auf Erde aufgeschlagen!")
		break
	elif(PRx[i]>(rEM-rM)):
		print("Auf Mond aufgeschlagen!")
		break

	#werte in datei
	ausg.write(str(t[i])  +" \t "+str(PRx[i])+ " \t " + str(vx[i]) + " \t " + str(aRx[i])+"\n")
	#force.write(str(FRx[i-1])+"\t"+str(FRy[i-1])+"\n")

ausg.close()