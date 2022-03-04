#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#pragma warning(disable:4996) //pentru a dezactiva _CRT_SECURE_NO_WARNINGS

enum CULORI {
	ALB,
	NEGRU = 10,
	VERDE = 20,
	ROSU,
	ROZ
};

const char* transformaEnumInSirDeCaractere(CULORI x) {
	switch (x)
	{
	case ALB:
		return "alb";
	case NEGRU:
		return "negru";
	case VERDE:
		return "verde";
	case ROSU:
		return "rosu";
	default:
		return "culoarea introdusa nu a fost interpretata inca";
	}
}

CULORI transformaSirDeCaractereInEnum(const char* pointerSir) {
	if (strcmp(pointerSir, "alb") == 0) {
		return ALB;
	}
	else if (strcmp(pointerSir, "negru") == 0) {
		return NEGRU;
	}
	else if (strcmp(pointerSir, "verde") == 0) {
		return VERDE;
	}
	else if (strcmp(pointerSir, "rosu") == 0) {
		return ROSU;
	}

	return ROZ;
}

//104 bytes
struct client {
	char nume[100];//100 bytes
	int varsta;//4 bytes
};

//100 bytes
union user
{
	int varsta;//4 bytes
	char nume[100];// 100 bytes
	char gen;//1 byte
};

struct telefon {
	CULORI culoareTelefon;
	char* marca;
	double pret;
	int anFabricatie;
	char version;
	client proprietar;
};

void scrieClientLaConsola(client c) {
	printf("client = %s\n", c.nume);
	printf("varsta = %d\n\n", c.varsta);
}

void citesteClientDeLaConsola(client& c) {
	//uneori mai ramane blocat in buffer un \n
	//getchar(); folosit pt eliberare buffer (se citeste un caracter de la consola)
	printf("client = ");
	gets_s(c.nume);
	//scanf("%s", c.nume); //citeste pana la primul spatiu

	printf("varsta = ");
	scanf("%d", &c.varsta);
}

void scrieClientInFisierText(FILE* fisier, client c) {
	fprintf(fisier, "%s\n", c.nume);
	fprintf(fisier, "%d\n", c.varsta);
}

void citireClientDinFisierText(FILE* fisier, client &c) {
	fgets(c.nume, 100, fisier);
	fscanf(fisier, "%d", &c.varsta);
}

void scrieTelefonLaConsola(telefon t) {
	printf("culoare telefon = %s\n", transformaEnumInSirDeCaractere(t.culoareTelefon));

	printf("marca = %s\n", t.marca);
	printf("pret = %.2lf\n", t.pret);
	printf("an fabricatie = %d\n", t.anFabricatie);
	printf("version = %c\n", t.version);

	scrieClientLaConsola(t.proprietar);
	//linia de mai sus este echivalenta cu:
	//	printf("client = %s\n", t.proprietar.nume);
	//  printf("varsta = %d\n\n", t.proprietar.varsta);
}
void main() {
	//pentru a declara un sir de caractere pe stack ai face asa: char aux[100];
	//unde aux este o variabila care are ca valoarea adresa primului element din vector...adica aux[0]
	//si ca sa pui ceva in aux: strcpy(aux,"un sir de caractere oarecare");

	printf("%s\n", transformaEnumInSirDeCaractere(ROZ));
	printf("%d\n", transformaSirDeCaractereInEnum("verdreagergee"));

	client c1;
	c1.varsta = 30;
	strcpy(c1.nume, "Ion Popescu");

	user u1;
	u1.varsta = 20;
	printf("u1.varsta=%d\n", u1.varsta);
	strcpy(u1.nume, "Ana Popescu");
	printf("u1.nume=%s\n", u1.nume);
	printf("u1.varsta=%d\n", u1.varsta);
	u1.varsta = 30;
	u1.gen = 'D';
	printf("u1.gen=%c\n", u1.gen);
	printf("u1.varsta=%d\n", u1.varsta);


	// scriere in fisiere
	//w-creare fisier si scriere in el, daca exista fisierul exista se sterg toate informatiile din el
	//wb-este "w" in mod binar
	//r-citire din fisier, daca fisierul nu exista se intoarce NULL la fopen()
	//rb-este "r" in mod binar, daca fisierul nu exista se intoarce NULL la fopen()
	//a-adaugare informatii la sf fisierului, daca NU exista fisierul se face unul nou
	//ab- este "a" in mod binar
	//r+ sau rb+ - fisier deschis pentru citire si scriere/ citire si scriere in mod binar, daca fisierul nu exista se intoarce NULL la fopen()
	//w+ sau wb+ - fisier deschis pentru citire si scriere in mod binar. 
	//a+ si ab+ - fisier deschis pentru citire si adaugare la final

	FILE* fisierClienti = fopen("clienti.txt", "w");
	if (fisierClienti == NULL) {
		printf("Eroare: Nu s a putut deschide pentru scriere fisierul clienti.txt");
		exit(1000);
	}

	client c2;
	c2.varsta = 20;
	strcpy(c2.nume, "Laurentiu Popescu");

	client c3;
	citesteClientDeLaConsola(c3);

	printf("\n\n----SCRIERE CLIENT LA CONSOLA----\n\n");
	scrieClientLaConsola(c1);
	scrieClientLaConsola(c2);
	scrieClientLaConsola(c3);

	printf("\n\n----SCRIERE CLIENT IN FISIER TEXT----\n\n");

	scrieClientInFisierText(fisierClienti, c1);
	scrieClientInFisierText(fisierClienti, c2);
	scrieClientInFisierText(fisierClienti, c3);
	fclose(fisierClienti);

	printf("\n\n----CITIRE CLIENT DIN FISIER TEXT----\n\n");
	fisierClienti = fopen("clienti.txt", "r");
	if (fisierClienti == NULL) {
		printf("Eroare: Nu s a putut deschide pentru citire fisierul clienti.txt");
		exit(2000);
	}
	client c4, c5, c6;
	citireClientDinFisierText(fisierClienti, c4);
	fgetc(fisierClienti);//pentru a elibera buffer-ul de \n
	citireClientDinFisierText(fisierClienti, c5);
	fgetc(fisierClienti);//pentru a elibera buffer-ul de \n
	citireClientDinFisierText(fisierClienti, c6);
	scrieClientLaConsola(c4);
	scrieClientLaConsola(c5);
	scrieClientLaConsola(c6);

	fclose(fisierClienti);

	printf("\n\n----SCRIE TELEFON LA CONSOLA----\n\n");
	telefon t1, t2, t3;
	t1.anFabricatie = 2021;
	t2.anFabricatie = 2022;

	t1.culoareTelefon = VERDE;
	t2.culoareTelefon = NEGRU;

	t1.pret = 2600.99;
	t2.pret = 3599.99;

	t1.version = 'X';
	t2.version = '8';

	t1.proprietar = c1;
	t2.proprietar = c2;

	//pt alocare memorie pe heap:
	//1) aloci memorie
	t1.marca = (char*)malloc(strlen("Apple") + 1);//se aloca memorie pe heap pt 6 bytes : 5+1 - unde +1 este acel \0
	//2) copiezi informatia la acea zona de memorie
	strcpy(t1.marca, "Apple");

	t2.marca = (char*)malloc(strlen("Huawei") + 1);
	strcpy(t2.marca, "Huawei");

	scrieTelefonLaConsola(t1);
	scrieTelefonLaConsola(t2);


}