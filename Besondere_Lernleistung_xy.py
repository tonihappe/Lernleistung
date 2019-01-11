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

	rMX = PMx-PRy
	rMY = PMy-PRy
	rMR = math.sqrt(rMX*rMX+rMY*rMY)

	#Berechnungen der Kräfte der Erde/des Mondes
	FER =-1*G*mR*mE/(rER**2) #negativ, da erde "hinter" raumschiff liegt auf x-Achse
	FMR = G*mR*mM/(rMR**2)

	#Zerlegung der Teilkräfte --> Bestimmung der notwendigen Winkel
	if (rEY==0): #um Division durch 0 zu verhindern als if Funktion
		betaREF = math.pi/2
	elif (rEX==0):
		betaREF = 0
	else:
		betaREF=math.tan(rEX/rEY)
	
	alphaMER= math.pi/2-betaREF #Pi halbe oder ganz pi??????
	FEx = math.cos(alphaMER)*FER
	FEy = math.sin(alphaMER)*FER

	if (rMY==0):
		betaFRM = math.pi/2
		
	elif (rMX==0):
		betaFRM = 0
	else:
		betaFRM = math.tan(rMX/rMY) #umbenennen?
	
	alphaRME = math.pi/2-betaFRM #Pi halbe oder ganz pi??????
	FMx = math.cos(alphaRME)*FMR
	FMy = math.sin(alphaRME)*FMR

	#FRES 
	testvar2 = (FEx*FMx+FEy*FMy)
	testvar3 =  (math.sqrt((FEx**2)+(FEy**2)) + math.sqrt((FMx**2)+(FMy**2)))
	testvar1 = testvar2 / testvar3
	gammah = math.acos( testvar1 )
	gammacoss = math.pi-gammah

	FRES=math.sqrt((FMR**2)+(FER**2)-2*FMR*FER*math.cos(gammacoss))
	#FRES Zerlegung
	alphaRES=math.asin((FMR*math.sin(alphaRME))/FRES)
	
	FRx = FRES * math.cos(alphaRES)
	FRy = FRES * math.sin(alphaRES)

	aR = FRES/mR
	aRx = FRx/mR
	aRy = FRy/mR
	return aR,aRx,aRy,FRES,FRx,FRy,rER,rMR




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
vx=[0]*n
vy=[0]*n
FRES=[0]*n
FRx=[0]*n
FRy=[0]*n
aRx=[0]*n
aRy=[0]*n

#Anfangswerte
t[0]=0
PMx=3844e5
PMy=0
#PRx[0]=6371e3	# Startpunkt des R (Radius der Erde)
#PRy[0]=0

print("Startgeschwindigkeit: ")
v0=int(input()) #Startgeschwindigkeit
print("Startrichtung (in Grad --> 0° = x-Achse): ")
winkelv0 = int(input()) #Winkel der Startgeschwindigkeit
print("\n\n")
#Zerlegung der Startgeschwindigkeit in x und y Komponente
if (winkelv0==0):
	vx[0]=v0
	vy[0]=0
else:
	vx[0] = v0*math.cos(winkelv0)
	vy[0] = v0*math.sin(winkelv0)

#Bestimmung des Startpunktes durch Radius der Erde und winkelv0
if (winkelv0==0):
	PRx[0]=rE
	PRy[0]=0
else:
	PRx[0] = rE*math.cos(winkelv0)
	PRy[0] = rE*math.sin(winkelv0)


aR,aRx[0],aRy[0],FRES[0],FRx[0],FRy[0],rER,rMR = getbeschl(PRx[0],PRy[0],PMx,PMy)
ausg = open ("Ausgabe.dat","w")
ausg.write(str(t[0])+"\t"+str(PRx[0])+"\t"+str(PRy[0])+"\t"+str(vx[0])+"\t"+str(vy[0])+"\t"+str(aRx[0])+"\t"+str(aRy[0])+"\n")

#Schleife für einzelne Berechnung am jeweiligen Punkt
for i in range(1,n,1):
	aR,aRx[i-1],aRy[i-1],FRES[i-1],FRx[i-1],FRy[i-1],rER,rMR = getbeschl(PRx[i-1],PRy[i-1],PMx,PMy)
	print("FRES = ", FRES[i-1],"\naR = ",aR)

	#k berechnet x 
	#l berechnet v
	#kräfte bei zwischenschritten als Unterprogramm
	
	kx1 = vx[i-1]
	lx1 = aRx[i-1]
	ky1 = vy[i-1]
	ly1 = aRy[i-1]
		
	kx2 = vx[i-1]+0.5*h*lx1
	lx2 = aRx[i-1]
	ky2 = vy[i-1]+0.5*h*ly1
	ly2 = aRy[i-1]
	
	kx3 = vx[i-1]+0.5*h*lx2
	lx3 = aRx[i-1]
	ky3 = vy[i-1]+0.5*h*ly2
	ly3 = aRy[i-1]
	
	kx4 = vx[i-1]+ h * lx3
	lx4 = aRx[i-1]
	ky4 = vy[i-1]+ h * ly3
	ly4 = aRy[i-1]
	
	t[i] = t[i-1]+h
	PRx[i] = PRx[i-1]+ h/6 * (kx1 + 2 * kx2 + 2* kx3 + kx4)
	PRy[i] = PRy[i-1]+ h/6 * (ky1 + 2 * ky2 + 2* ky3 + ky4)
	vx[i] = vx[i-1]+ ((h/6) * (lx1 + 2 * lx2 + 2* lx3 + lx4))
	vy[i] = vy[i-1]+ ((h/6) * (ly1 + 2 * ly2 + 2* ly3 + ly4))
	aR,aRx[i-1],aRy[i-1],FRES[i-1],FRx[i-1],FRy[i-1],rER,rMR = getbeschl(PRx[i-1],PRy[i-1],PMx,PMy)
	
	print("DURCHGANG NUMMER: ", i)
	#if (rER<=rE-1):
	#	print("Auf Erde aufgeschlagen!")
	#	break
	#elif(rMR>rM):
	#	print("Auf Mond aufgeschlagen!")
	#	break

	#werte in datei
	ausg.write(str(t[i-1])+"\t"+str(PRx[i-1])+"\t"+str(PRy[i-1])+"\t"+str(vx[i-1])+"\t"+str(vy[i-1])+"\t"+str(aRx[i-1])+"\t"+str(aRy[i-1])+"\n")
	#force.write(str(FRx[i-1])+"\t"+str(FRy[i-1])+"\n")

ausg.close()
