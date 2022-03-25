#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning (disable:4996)

//fisier binar relativ
typedef struct {
	char is;
	int cod; //is si cod reprezinta informatiile vitale la folosirea fis. binare relative
	char nume[30];
	char prenume[30];
	char dep[20];
	char prof[20];
	int varsta;
	float sal[3];
	struct data {
		int zi;
		int luna;
		int an;
	} data_ang;
} ang;

int nr_art(FILE* f)
{
	int l, m;

	l = ftell(f);//poz curenta
	fseek(f, 0, SEEK_END);//te duce la finalul fisierului

	m = ftell(f) / sizeof(ang);

	fseek(f, l, 0);//ma repozitionez de unde am plecat
	return m;
}

void populare()
{
	FILE* f;

	ang a;
	int n, i, er;
	char nume_f[20];

	printf("\nnume fisier:");
	gets_s(nume_f);
	f = fopen(nume_f, "wb+");

	printf("n="); scanf_s("%d", &n);//reprezinta codul
	while (!feof(stdin))//<----sfarsitul introducerii fiind marcat standard - se citeste pana cand introduceti CTRL+Z
	{
		//aici n >= 1
		if (n > nr_art(f)) /// A1 A2 A3 A4 ..........A300 
		{
			fseek(f, 0, SEEK_END);
			a.is = 0;
			for (i = nr_art(f); i < n; i++)
				fwrite(&a, sizeof(ang), 1, f);

			a.is = 1;
			a.cod = n;
			printf("nume="); getc(stdin); gets_s(a.nume);
			printf("prenume="); gets_s(a.prenume);
			printf("dep="); gets_s(a.dep);
			printf("prof="); gets_s(a.prof);
			printf("varsta="); scanf_s("%d", &a.varsta);
			for (i = 0; i < 3; i++)
			{
				printf("sal din luna %d: ", i + 1);
				scanf_s("%f", &a.sal[i]);
			}
			printf("data angajarii zi, luna si an:  ");//03 05 2019
			scanf("%d %d %d", &a.data_ang.zi, &a.data_ang.luna, &a.data_ang.an);
			fseek(f, (n - 1) * sizeof(ang), 0); //atentie aici....e (n-1)....daca n=1 => 0*sizeof(ang) ...adica ne pozitionam la byte-ul 0
			fwrite(&a, sizeof(ang), 1, f);
		}
		else {
			fseek(f, (n - 1) * sizeof(ang), 0);
			fread(&a, sizeof(ang), 1, f);
			if (a.is == 0) {
				a.is = 1;
				a.cod = n;
				printf("nume="); getc(stdin); gets_s(a.nume);
				printf("prenume="); gets_s(a.prenume);
				printf("dep="); gets_s(a.dep);
				printf("prof="); gets_s(a.prof);
				printf("varsta="); scanf_s("%d", &a.varsta);
				for (i = 0; i < 3; i++)
				{
					printf("sal din luna %d: ", i + 1);
					scanf_s("%f", &a.sal[i]);
				}
				printf("data angajaraii zi, luna si an: ");
				scanf("%d %d %d", &a.data_ang.zi, &a.data_ang.luna, &a.data_ang.an);
				fseek(f, (n - 1) * sizeof(ang), 0);//repozitionare pentru ca dupa fseek-ul de la linia 75 este un fread care va trece
				//la urmatorul angajat din fisier
				fwrite(&a, sizeof(ang), 1, f);
			}
			else printf("inreg. exista\n");
		}
		printf("\nn="); scanf_s("%d", &n);
	}
	fclose(f);
}

void adaugare()
{
	FILE* f;
	ang a;
	int n, i, er;
	char nume_f[20];
	int nr = sizeof(ang);

	printf("\nnume fisier:");
	gets_s(nume_f);
	f = fopen(nume_f, "rb+");//va deschide fisierul in format de citire si scriere....dar ce exista in fisier se pastreaza

	if (f == NULL) f = fopen(nume_f, "wb+");
	printf("\nn="); scanf_s("%d", &n);

	while (!feof(stdin))
	{
		if (n > nr_art(f))
		{
			fseek(f, 0, 2);
			a.is = 0;
			for (i = nr_art(f); i < n; i++)
				fwrite(&a, sizeof(ang), 1, f);
			fseek(f, (n - 1) * sizeof(ang), 0);
			a.is = 1;
			a.cod = n;
			printf("nume="); getc(stdin); gets_s(a.nume);
			printf("prenume="); gets_s(a.prenume);
			printf("dep="); gets_s(a.dep);
			printf("prof="); gets_s(a.prof);
			printf("varsta="); scanf_s("%d", &a.varsta);
			for (i = 0; i < 3; i++)
			{
				printf("sal din luna %d: ", i + 1);
				scanf_s("%f", &a.sal[i]);
			}
			printf("data angajarii zi, luna si an: ");
			scanf("%d %d %d", &a.data_ang.zi, &a.data_ang.luna, &a.data_ang.an);
			fwrite(&a, sizeof(ang), 1, f);
		}
		else {
			fseek(f, (n - 1) * sizeof(ang), 0);
			fread(&a, sizeof(ang), 1, f);
			if (a.is == 0) {
				a.is = 1;
				a.cod = n;
				printf("nume="); getc(stdin); gets_s(a.nume);
				printf("prenume="); gets_s(a.prenume);
				printf("dep="); gets_s(a.dep);
				printf("prof="); gets_s(a.prof);
				printf("varsta="); scanf_s("%d", &a.varsta);
				for (i = 0; i < 3; i++)
				{
					printf("sal din luna %d: ", i + 1);
					scanf_s("%f", &a.sal[i]);
				}
				printf("data angajarii zi, luna si an: ");
				scanf("%d %d %d", &a.data_ang.zi, &a.data_ang.luna, &a.data_ang.an);
				fseek(f, (n - 1) * sizeof(ang), 0);
				//fseek(f, ftell(f) - sizeof(ang), 1);
				fwrite(&a, sizeof(ang), 1, f);
			}
			else printf("inregistrarea exista\n");
		}
		printf("\nn="); scanf_s("%d", &n);
	}
	fclose(f);
}
void afisare()
{
	FILE* f, * g;
	ang a;
	char nume_f[20];
	printf("\nnume fisier:");
	gets_s(nume_f);
	if (!(f = fopen(nume_f, "rb"))) printf("nu exista fisierul");
	else {
		printf("\n===========================afisarea inregistrarilor din fisier=============");
		g = fopen("lista_fis.txt", "w");
		printf("\n ");
		fprintf(g, " LISTA SALARII \n ");
		fprintf(g, "==============================================================================================================\n ");
		fprintf(g, " COD || NUME || PRENUME || DEPARTAMENT || PROFESIE || VARSTA || LUNA 1 || LUNA 2 || LUNA 3 || DATA \n ");
		fprintf(g, "===================================================================================================================\n ");
		printf(" LISTA SALARII \n ");
		printf("=========================================================================================================================\n ");
		printf(" COD || NUME || PRENUME || DEPARTAMENT || PROFESIE || VARSTA || LUNA 1 || LUNA 2 || LUNA 3 || DATA \n ");
		printf("=========================================================================================================================\n ");
		fread(&a, sizeof(ang), 1, f);
		while (!feof(f))//pana cand ajungi la sf fisierului
		{
			if (a.is == 1)
			{
				printf("\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f || %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
				fprintf(g, "\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f|| %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
			}
			fread(&a, sizeof(ang), 1, f);
		}
		fclose(f); fclose(g);
	}
}

void afisare_poz()
{
	FILE* f, * g;
	ang a;
	int n;
	char nume_f[20];
	printf("\nnume fisier:");
	gets_s(nume_f);
	if (!(f = fopen(nume_f, "rb"))) printf("nu exista fisierul");
	else {
		printf("\nafisarea inregistrarilor a caror pozitie o citesc de la tastatura");
		g = fopen("lista_poz.txt", "w");
		printf("\n ");
		fprintf(g, " LISTA SALARII \n ");
		fprintf(g, "=============================================================================================\n ");
		fprintf(g, " COD || NUME || PRENUME || VARSTA || LUNA 1 ||LUNA 2 || LUNA 3\n ");
		fprintf(g, "==============================================================================================\n ");
		printf("\nn="); scanf("%d", &n);
		while (!feof(stdin))
		{
			if (n > nr_art(f)) printf("depaseste lungimea fis.");
			else
			{
				fseek(f, (n - 1) * sizeof(ang), 0);
				fread(&a, sizeof(ang), 1, f);
				if (a.is == 1)
				{
					printf("\n%5d|| %-20s || %-20s ||%5d ||%5.2f||%5.2f||%5.2f", n, a.nume, a.prenume, a.varsta, a.sal[0], a.sal[1], a.sal[2]);
					fprintf(g, "\n%5d|| %-20s || %-20s ||%5d ||%5.2f||%5.2f||%5.2f", n, a.nume, a.prenume, a.varsta, a.sal[0], a.sal[1], a.sal[2]);
				}
				else printf("inregistrarea nu exista");
			}
			printf("\nn="); scanf("%d", &n);
		}
		fclose(f); fclose(g);
	}
}

void afisare_peBazaAnului()
{
	FILE* f, * g;
	ang a;
	int an;
	char nume_f[20];
	printf("\nnume fisier:");
	gets_s(nume_f);
	if (!(f = fopen(nume_f, "rb"))) printf("nu exista fisierul");
	else {
		printf("\nafisarea inregistrarilor pe baza anului");
		g = fopen("lista_angPeBazaAnului.txt", "w");
		printf("\n ");
		fprintf(g, " LISTA SALARII \n ");
		fprintf(g, "=============================================================================================\n ");
		fprintf(g, " COD || NUME || PRENUME || VARSTA || LUNA 1 ||LUNA 2 || LUNA 3\n ");
		fprintf(g, "==============================================================================================\n ");
		printf("\nan="); scanf("%d", &an);
		while (!feof(stdin))
		{
			fread(&a, sizeof(ang), 1, f);
			while (!feof(f))//pana cand ajungi la sf fisierului
			{
				if (a.is == 1 && a.data_ang.an==an)
				{
					printf("\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f || %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
					fprintf(g, "\n%5d|| %-20s || %-20s || %-15s || %-15s || %5d || %-7.2f || %-7.2f || %-7.2f|| %2d/%2d/%4d", a.cod, a.nume, a.prenume, a.dep, a.prof, a.varsta, a.sal[0], a.sal[1], a.sal[2], a.data_ang.zi, a.data_ang.luna, a.data_ang.an);
				}
				fread(&a, sizeof(ang), 1, f);
			}
			rewind(f);
			printf("\nan="); scanf("%d", &an);
		}
		fclose(f); fclose(g);
	}
}

void main() {
	//#define SEEK_CUR    1
	//#define SEEK_END    2
	//#define SEEK_SET    0

	//printf("\n\POPULARE FISIER BINAR\n\n");
	//populare();
	//printf("\n\ADAUGARE IN FISIER BINAR\n\n");
	//adaugare();

	printf("\n\nAFISARE DIN FISIER BINAR\n\n");
	afisare();
	//printf("\n\AFISARE DIN FISIER BINAR PE BAZA CODULUI\n\n");
	//afisare_poz();
	printf("\n\AFISARE DIN FISIER BINAR PE BAZA ANULUI\n\n");
	afisare_peBazaAnului();
}