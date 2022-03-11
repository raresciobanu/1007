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
		return "default";
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
	printf("varsta = %d\n", c.varsta);
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

void citireClientDinFisierText(FILE* fisier, client& c) {
	fgets(c.nume, 100, fisier);
	c.nume[strcspn(c.nume, "\n")] = 0;//se va inlocui \n cu 0 - adica sfarsit de sir de caractere
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

void citireTelefonDeLaConsola(telefon& t) {
	char aux[100];
	printf("culoare telefon (alb / negru / verde / rosu / roz) = ");
	scanf("%s", aux);
	t.culoareTelefon = transformaSirDeCaractereInEnum(aux);

	printf("marca = ");
	scanf("%s", aux);
	t.marca = (char*)malloc(strlen(aux) + 1);//+1 este pt \0
	strcpy(t.marca, aux);

	printf("pret=");
	scanf("%lf", &t.pret);

	printf("an fabricatie=");
	scanf("%d", &t.anFabricatie);


	printf("version=");
	scanf(" %c", &t.version);//daca tu introduci de la tastatura pt an fabricatie 2020 (in spate inseamna 2020\n) => 
	//automat t.version=\n ...ca sa evitam aceasta problema (sa ignore acel \n) punem un spatiu inainte de %c

	getchar(); //folosit pt a elibera bufferul...cand citesti versiunea ( se citeste de fapt de ex: 9\n) => ramane in buffer \n

	citesteClientDeLaConsola(t.proprietar);
}

void scrieTelefonInFisierText(FILE* fisier, telefon t) {
	fprintf(fisier, "%s\n", transformaEnumInSirDeCaractere(t.culoareTelefon));

	fprintf(fisier, "%s\n", t.marca);
	fprintf(fisier, "%.2lf\n", t.pret);
	fprintf(fisier, "%d\n", t.anFabricatie);
	fprintf(fisier, "%c\n", t.version);

	scrieClientInFisierText(fisier, t.proprietar);
}

void citireTelefonDinFisierText(FILE* fisier, telefon& t) {
	char aux[100];
	fscanf(fisier, "%s", aux);
	t.culoareTelefon = transformaSirDeCaractereInEnum(aux);

	fscanf(fisier, "%s", aux);
	t.marca = (char*)malloc(strlen(aux) + 1);//+1 este pt \0
	strcpy(t.marca, aux);

	fscanf(fisier, "%lf", &t.pret);

	fscanf(fisier, "%d", &t.anFabricatie);


	fscanf(fisier, " %c", &t.version);//daca tu introduci de la tastatura pt an fabricatie 2020 (in spate inseamna 2020\n) => 
	//automat t.version=\n ...ca sa evitam aceasta problema (sa ignore acel \n) punem un spatiu inainte de %c

	fgetc(fisier); //folosit pt a elibera bufferul...cand citesti versiunea ( se citeste de fapt de ex: 9\n) => ramane in buffer \n

	citireClientDinFisierText(fisier, t.proprietar);
}

void scrieClientInFisierBinar(FILE* fisier, client c) {
	//functia fwrite - primeste ca input ===> fwrite(addressData, sizeData, numbersData, pointerToFile);
	//addressData - adresa variabilei citite (cu "&" in fata)
	//sizeData - dimensiunea variabilei salvate (sizeof)
	//numbersData - numarul de variabile salvate
			//int arr[3] = {101, 203, 303};
			// 
			//fwrite(arr, sizeof(int), 2, fp); //va scrie primele 2 elemente din vector
	//pointerToFile - fisierul unde se salveaza variabila

	//DACA scrierea are erori atunci fwrite va retuna un numar mai mic decat variabila numbersData;

	//fwrite(&t, sizeof(l), 1, fisier); //Daca clientul nu ar fi avut char* puteam sa scriem direct asa.

	//fwrite(&c, sizeof(c), 1, fisier);// <-echivalentul a ce am scris mai jos

	int dim = strlen(c.nume) + 1;
	fwrite(&dim, sizeof(dim), 1, fisier);
	fwrite(&c.nume, dim, 1, fisier);

	fwrite(&c.varsta, sizeof(c.varsta), 1, fisier);
}

void citireClientDinFisierBinar(FILE* fisier, client &c) {
	//functia fread - primeste ca input===> fread(addressData, sizeData, numbersData, pointerToFile);
	//descrierea parametrilor este la fel ca la fwrite

	//fread(&l, sizeof(l), 1, fisier); //daca nu aveam char* in structura client scriam asa.

	//fread(&c, sizeof(c), 1, fisier);// <-echivalentul a ce am scris mai jos
	int dim = 0;
	fread(&dim, sizeof(dim), 1, fisier);
	fread(&c.nume, dim, 1, fisier);

	fread(&c.varsta, sizeof(c.varsta), 1, fisier);
}

void scrieTelefonInFisierBinar(FILE* fisier, telefon t) {
	char aux[100];
	strcpy(aux, transformaEnumInSirDeCaractere(t.culoareTelefon));

	int dim = strlen(aux) + 1; //calculam dim culorii +1 (pt \0)
	fwrite(&dim, sizeof(dim), 1, fisier);
	fwrite(aux, dim, 1, fisier);

	dim = strlen(t.marca) + 1; //calculam dim marcii +1 (pt \0)
	fwrite(&dim, sizeof(dim), 1, fisier);
	fwrite(t.marca, dim, 1, fisier);

	fwrite(&t.pret, sizeof(t.pret), 1, fisier);

	fwrite(&t.anFabricatie, sizeof(t.anFabricatie), 1, fisier);

	fwrite(&t.version, sizeof(t.version), 1, fisier);

	scrieClientInFisierBinar(fisier, t.proprietar);
}

void citireTelefonDinFisierBinar(FILE* fisier, telefon &t) {
	char aux[100];
	int dim = 0;
	fread(&dim, sizeof(dim), 1, fisier);
	fread(aux, dim, 1, fisier);
	t.culoareTelefon = transformaSirDeCaractereInEnum(aux);


	fread(&dim, sizeof(dim), 1, fisier);
	fread(aux, dim, 1, fisier);
	t.marca = (char*)malloc(strlen(aux) + 1);//+1 pt \0
	strcpy(t.marca, aux);

	fread(&t.pret, sizeof(t.pret), 1, fisier);

	fread(&t.anFabricatie, sizeof(t.anFabricatie), 1, fisier);

	fread(&t.version, sizeof(t.version), 1, fisier);

	citireClientDinFisierBinar(fisier, t.proprietar);
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

	printf("\n\n----CITIRE TELEFON DE LA CONSOLA----\n\n");

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



	citireTelefonDeLaConsola(t3);


	printf("\n\n----SCRIE TELEFON LA CONSOLA----\n\n");
	scrieTelefonLaConsola(t1);
	printf("\n");
	scrieTelefonLaConsola(t2);
	printf("\n");
	scrieTelefonLaConsola(t3);

	printf("\n\n----SCRIE TELEFON IN FISIER TEXT----\n\n");
	FILE* fisierTelefoane = fopen("telefoane.txt", "w");
	if (fisierClienti == NULL) {
		printf("Eroare: Nu s a putut deschide pentru scriere fisierul telefoane.txt");
		exit(900);
	}

	scrieTelefonInFisierText(fisierTelefoane, t1);
	scrieTelefonInFisierText(fisierTelefoane, t2);
	scrieTelefonInFisierText(fisierTelefoane, t3);


	fclose(fisierTelefoane);
	
	printf("\n\n----CITIRE TELEFON DIN FIISER TEXT----\n\n");
	fisierTelefoane = fopen("telefoane.txt", "r");
	if (fisierClienti == NULL) {
		printf("Eroare: Nu s a putut deschide pentru citire fisierul telefoane.txt");
		exit(330);
	}

	telefon t4, t5, t6;

	citireTelefonDinFisierText(fisierTelefoane, t4);
	citireTelefonDinFisierText(fisierTelefoane, t5);
	citireTelefonDinFisierText(fisierTelefoane, t6);

	scrieTelefonLaConsola(t4);
	scrieTelefonLaConsola(t5);
	scrieTelefonLaConsola(t6);


	fclose(fisierTelefoane);

	printf("\n\n----SCRIE TELEFON IN FISIER BINAR----\n\n");
	FILE *fisierTelefoaneBinar = fopen("telefoaneBinar.bin", "wb");
	if (fisierTelefoaneBinar == NULL) {
		printf("Eroare: Nu s a putut deschide pentru scriere fisierul telefoaneBinar.bin");
		exit(530);
	}
	scrieTelefonInFisierBinar(fisierTelefoaneBinar, t4);	
	scrieTelefonInFisierBinar(fisierTelefoaneBinar, t5);
	scrieTelefonInFisierBinar(fisierTelefoaneBinar, t6);

	fclose(fisierTelefoaneBinar);

	printf("\n\n----CITIRE TELEFON DIN FIISER BINAR----\n\n");

	fisierTelefoaneBinar = fopen("telefoaneBinar.bin", "rb");
	if (fisierTelefoaneBinar == NULL) {
		printf("Eroare: Nu s a putut deschide pentru citire fisierul telefoaneBinar.bin");
		exit(730);
	}

	telefon t10, t11, t12;
	citireTelefonDinFisierBinar(fisierTelefoaneBinar, t10);
	citireTelefonDinFisierBinar(fisierTelefoaneBinar, t11);
	citireTelefonDinFisierBinar(fisierTelefoaneBinar, t12);
	

	scrieTelefonLaConsola(t10);
	printf("\n");
	scrieTelefonLaConsola(t11);
	printf("\n");
	scrieTelefonLaConsola(t12);

	fclose(fisierTelefoaneBinar);

	free(t1.marca);
	free(t2.marca);
	free(t3.marca);
	free(t4.marca);
	free(t5.marca);
	free(t6.marca);
	free(t10.marca);
	free(t11.marca);
	free(t12.marca);
}