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
	aR = (FRES/mR) #resultierende Beschleunigung

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

#bremsantw=bool(False)


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
	
	k1 = v[i-1]
	l1 = aR

	k2 = v[i-1]+0.5*h*l1
	l2 = aR
	
	k3 = v[i-1]+0.5*h*l2
	l3 = aR

	k4 = v[i-1]+ h * l3
	l4 = aR

	t[i] = t[i-1]+h
	PR[i] = PR[i-1]+ h/6.0 * (k1 + 2 * k2 + 2* k3 + k4)
	v[i]  = v[i-1] + h/6.0 * (l1 + 2 * l2 + 2* l3 + l4)
	#aR, FRES,FE,FM = getbeschl(PR[i-1])
	
	
	print("DURCHGANG NUMMER: ", i)
	#Test ob Raumschiff Mond oder Erde erreicht hat
	if (PR[i]<=rE):
		print("Auf Erde aufgeschlagen!")
		break

	elif(PR[i]>(rEM-rM)):
		print("Auf Mond aufgeschlagen!")
		break
		
	#if (math.fabs(FRES)<=0.1 and bremsantw==False):
	#	print ("LAGRANGEPUNKT ERREICHT\n Soll der Bremsvorgang eingeleitet werden?\n\n[1] für Ja\n[2] für Nein\n")
	#	bremsen=int(input())
		
	#	while (bremsen <= 0 or bremsen >= 3):#Falls ein ungültiger Wert eingegeben wurde wird diese Schleife ausgeführt
	#		print("Bitte geben Sie [1] oder [2] ein!")
	#		bremsen=int(input())

	#	bremsantw=True # wird auf True gesetzt, damit nicht erneut gefragt wird, falls bereits geantwortet wurde

	

	#if (lagrinp==1):

	
	#werte in datei
	ausg.write(str(t[i])+"\t"+str(PR[i])+"\t"+str(v[i])+"\t"+str(aR)+"\t"+str(FRES)+"\t"+str(FE)+"\t"+str(FM)+"\n")
	

ausg.close()

