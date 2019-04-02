
/*							Customer Banking Account                                         */
/*	Here, you can create a new account, update information of an existing account, 
	view and manage transactions, check the details of an existing account, 
	remove existing account and view customers’ list.

	Bugs free.
	Implemented in a simple way 
	Improvments may be added 
*/


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAXNO 500
#define T 100

struct tranzactie{
	unsigned tip; //1 pentru introducere 2//pentru scoatere 3// pentru trimitere 4// daca ai primit bani
	float transfer; // valoare monetara
	time_t data;
};

struct customer{
	char nume[50];
	char data_nastere[50];
	char telefon[15];
	char pin[10];
	//tipuri de conturi
	float bani;
	char iban[10];
	struct tranzactie tran[T];
	unsigned nrTran;
};

 
struct customer conturi[MAXNO];
int noAcc;




void menu();
void new_acc();
int validareTelefon(char* numar);
void generareIBAN();
void checkAcc();
void updateAcc();
void deleteExistingAccount();

void makeTransaction();
void introducereValuta(int* index);  //functie ajutatoare makeTransaction
void extrageValuta(int* index);		 //functie ajutatoare makeTransaction
void transferaBani(int* index); 
int checkIBAN(char* string);  //verifica daca iban exista in baza de date daca exista intoarce index daca nu -1
void afisareTranzactii(int* index); //functia afiseaza tranzactiile unei persoane


void menu(int* choice){

	printf("\n\n");

	printf("\t\t\tCustomer Banking Account\n\n");

	printf("\t\tWelcome to main menu\n\n");

	printf("\t 1.Create new account\n"
		"\t 2.Update information of existing account\n"
		"\t 3.For transactions\n"
		"\t 4.Check the details of an existing account\n"
		"\t 5.Removing existing account\n"
		"\t 6.Vier customer's list\n"
		"\t 7.Exit\n");

	printf("\n\n\n");
	printf("\t\tEnter your choice:");
	scanf_s("%d", choice);

}




void deleteExistingAccount(){
	char password[10];
	char  nume[50];
	unsigned nrIncercari = 0;
	unsigned choice = 0;


	system("cls");
	printf("\n\n\t\t Stergere Cont Bancar \n\n");
	_sleep(1000);
	if (noAcc > 0){
		printf("\n\t Introduceti numele dumneavoastra:");
		fscanf(stdin, "%s", nume);
		int valid = validareNume(nume); //validare e folosit ca si index 
		if (valid == -1){
			while (valid == -1){
				if (nrIncercari == 3) {
					printf("\n\t Numarul de incercari permise a fost depasit\n");
					exit(EXIT_FAILURE);
				}
				printf("\n\t Numele introdus este gresit.Reintroduceti:");
				fscanf(stdin, "%s", nume);
				valid = validareNume(nume);
				++nrIncercari;
			}
		}

		printf("\n\t Introduceti parola dumneavoastra:");
		scanf("%s", password);
		fflush(stdin);
		if (strcmp(conturi[valid].pin, password) != 0){
			printf("\n\t PAROLA INTRODUSA ESTE GRESITA!");
			_sleep(1000);
			exit(EXIT_FAILURE);
		}
		
		//stergerea propriu zisa
		for (int j = valid; j < noAcc - 1; j++){
			struct customer aux = conturi[j];
			conturi[j] = conturi[j + 1];
			conturi[j + 1] = aux;
		}
		noAcc--;

		printf("\n\t Stergerea a avut succes\n");
		_sleep(3000);
	}
	else{
		printf("\n\t Nu exista conturi bancare");
		_sleep(3000);
	}

	

}








//validare in 2 pasi ca la update
void makeTransaction(){
	char password[10];
	char  nume[50];
	unsigned nrIncercari = 0;
	unsigned choice = 0;


	system("cls");
	printf("\n\n\t\t Realizare Tranzactie \n\n");
	printf("\n\t Introduceti numele dumneavoastra:");
	fscanf(stdin, "%s", nume);
	int valid = validareNume(nume); //validare e folosit ca si index 
	if (valid == -1){
		while (valid == -1){
			if (nrIncercari == 3) {
				printf("\n\t Numarul de incercari permise a fost depasit\n");
				exit(EXIT_FAILURE);
			}
			printf("\n\t Numele introdus este gresit.Reintroduceti:");
			fscanf(stdin, "%s", nume);
			valid = validareNume(nume);
			++nrIncercari;
		}
	}

	printf("\n\t Introduceti parola dumneavoastra:");
	scanf("%s", password);
	fflush(stdin);
	if (strcmp(conturi[valid].pin, password) != 0){
		printf("\n\t PAROLA INTRODUSA ESTE GRESITA!");
		_sleep(1000);
		exit(EXIT_FAILURE);
	}		
	
	printf("\t 1.Introducere monetar card\n"
		"\t 2.Retragere monetar\n"
		"\t 3.Transfera bani intr-un alt cont\n"
		"\t 4.Exit\n");
	printf("\n\t Introduceti varianta dorita:");
	fscanf(stdin,"%u", &choice);
	
	do{
		switch (choice)
		{
		case 1:{
				   introducereValuta(&valid);
				   break;
		}
		case 2:{
				   extrageValuta(&valid);
				   break;
		}
		case 3:{
				   transferaBani(&valid);
				   break;
		}
		default:
			break;
		}

		printf("\n\t Reintroduceti");
		scanf("%u", &choice);
		fflush(stdin);
	} while (choice != 4);


}


//introduce bani la indexul cu tare in structura
void introducereValuta(int* index){

	float val;
	time_t timer;
	//dupa ce s-a introdus suma 
	
	printf("\n\t Introduceti suma:"); scanf("%f", &val);
	if (val>0)
	conturi[*index].bani += val;
	else{
		printf("Nu se poate introduce suma mentionata");
		exit(EXIT_FAILURE);
	}
	conturi[*index].tran[conturi[*index].nrTran].tip = 1; //adica de aduagare bani
	conturi[*index].tran[conturi[*index].nrTran].transfer = val; //cat se adaug
	time(&timer);
	conturi[*index].tran[conturi[*index].nrTran].data = timer;
	conturi[*index].nrTran++;


}


//extrage bani de pe card
void extrageValuta(int* index){
	float val;
	time_t timer;
	//dupa ce s-a introdus suma 

	printf("\n\t Introduceti suma pe care doriti sa o retrageti:"); scanf("%f", &val);
	if (val<=conturi[*index].bani)
		conturi[*index].bani -= val;
	else{
		printf("Nu se poate extrage suma mentionata");
		exit(EXIT_FAILURE);
	}
	conturi[*index].tran[conturi[*index].nrTran].tip = 1; //adica de aduagare bani
	conturi[*index].tran[conturi[*index].nrTran].transfer = val; //cat se adaug
	time(&timer);
	conturi[*index].tran[conturi[*index].nrTran].data = timer;
	conturi[*index].nrTran++;
}


//introduce un iban valid si transfera bani in contul specificat
void transferaBani(int* tindex){
	int rindex;
	char iban[50];
	
	printf("\n\t Introduceti IBAN:");
	scanf("%s", iban);
	fflush(stdin);

	if ((rindex = checkIBAN(iban)) == -1){
		printf("\n\n\t IBANUL INTRODUS ESTE GRESIT ");
		_sleep(10000);
		exit(EXIT_FAILURE);
	}
	else{
		float suma = 0;
		time_t timer;
		printf("\n\t Introduceti suma pe care doriti sa o transferati:");
		scanf("%f", &suma);
		fflush(stdin);
		if (suma <=conturi[*tindex].bani){
			conturi[*tindex].bani -= suma; //scazi de la transmitator
			conturi[rindex].bani += suma; //adaugi la receiver
			conturi[*tindex].tran[conturi[*tindex].nrTran].tip = 3;
			conturi[*tindex].tran[conturi[*tindex].nrTran].transfer = suma;
			conturi[rindex].tran[conturi[rindex].nrTran].tip = 4;
			conturi[rindex].tran[conturi[rindex].nrTran].tip = suma;
			time(&timer);
			conturi[*tindex].tran[conturi[*tindex].nrTran].data = timer;
			conturi[rindex].tran[conturi[rindex].nrTran].data = timer;
			conturi[*tindex].nrTran++;
			conturi[rindex].nrTran++;
		}
	}
}


//cauta iban 
int checkIBAN(char* string){

	if (strlen(string) != 10)
		return -1;
	else{
		for (int i = 0; i < noAcc;i++)
		if (strcmp(conturi[i].iban, string) == 0)
			return i;

	}
	
	return -1;

}




//no password check
//validare nume 3 permise
void checkAcc(){
	int space;
	char  nume[50];
	unsigned nrIncercari = 0;
	system("cls"); 
	printf("\n\n\t\t Check details of an account\n\n");
	printf("\n\t Introduceti numele dumneavoastra:");
	fscanf(stdin, "%s", nume);
	int valid = validareNume(nume);
	if (valid == -1){
		while (valid == -1){
			if (nrIncercari == 3) {
				printf("\n\t Numarul de incercari permise a fost depasit\n");
				exit(EXIT_FAILURE);
			}
			printf("\n\t Numele introdus este gresit.Reintroduceti:");
			fscanf(stdin, "%s", nume);
			valid = validareNume(nume);
			++nrIncercari;
		}

	}
		
	printf("\n\t\t Data nastere: %s", conturi[valid].data_nastere);
	printf("\n\t\t Numar telefon: %s", conturi[valid].telefon);
	printf("\n\t\t Suma disponibila: %f", conturi[valid].bani);
	printf("\n\t\t IBAN: %s", conturi[valid].iban);
	afisareTranzactii(&valid);  //functie speciala pentru afisare tranzactii

	printf("\n\tApasati orice buton pentru a va intoarce la meniu:");
	scanf("%d", &space);
	fflush(stdin);

	

}



void afisareTranzactii(int* index){
	int k;

	if (conturi[*index].nrTran > 0){
		printf("\n\t Tranzactii efectuate");
		for (k = 0; k < conturi[*index].nrTran; k++){
			printf("\n\t\t Tranzactie nr. %d :",k);
			switch (conturi[*index].tran[k].tip){
			case 1:
				printf(" indroducere ");
				break;
			case 2:
				printf(" retragere ");
				break;
			case 3:
				printf(" tranzactie ");
				break;
			default:
				break;
			}

			printf(" suma:%f ", conturi[*index].tran[k].transfer);
			printf(" data:%s", ctime(&conturi[*index].tran[k].data));

		}
	}

}

//valideaza un nume in cauta in DB
int validareNume(char* nume){
	int i=0;
	while (i < noAcc){
		if (strcmp(nume, conturi[i].nume) == 0)
			return i;
		i++;

	}
	return (-1);
}


void new_acc(){

	system("cls");
	int status = 1; //status introducere account
	int validTel = 0;
	float sum = 0;
	char flg[3];
	unsigned variantaCont = 0;
	

	printf("\n\n\t\tCreate new account\n\n");
	printf("\t Introduceti numele: ");
	scanf("%s", conturi[noAcc].nume);
	fflush(stdin);
	printf("\n");

	printf("\t Introduceti data nasterii: ");
	scanf("%s", conturi[noAcc].data_nastere);
	fflush(stdin);
	printf("\n");  

	printf("\t Introduceti telefon: ");
	do{
	scanf("%s", conturi[noAcc].telefon);
	if ((validTel = validareTelefon(conturi[noAcc].telefon)) == 0)
	printf("\n\t Gresit.Reintroduceti:");
	} while (!validTel);
	fflush(stdin);
	printf("\n");  

	printf("\t Introducere suma monetara in conturi ?(da/nu):");

	fscanf(stdin,"%s",flg);
	if (strcmp(flg, "da") == 0){

		while (strcmp(flg, "da") == 0){
			printf("\n\t\t Introduceti suma dorita: ");
			scanf("%f", &sum);
			if (sum > 0){
				conturi[noAcc].bani += sum;
			}

			printf("\n\t\t\Doriti sa reintroduceti?:(da/nu)");
			fscanf(stdin, "%s", flg);

		}
	}
	fflush(stdin);
	printf("\n\t Introduceti parola:");
	scanf("%s", conturi[noAcc].pin); 



	//generare iban
	generareIBAN(); //functia generaza un IBAN 
	noAcc++;
}


int validareTelefon(char* numar){
	unsigned flag = 1;
	int i;
	int lungime = strlen(numar);


	if (lungime != 10)
		flag = 0;


	for (i = 0; i < lungime; i++)
	if (!isdigit(numar[i]))
		flag = 0;

	return flag;

}


void generareIBAN(){

	strcpy(conturi[noAcc].iban, "RO");
	srand((unsigned int)time(0));
	for (int i = 2; i < 10; i++)
		conturi[noAcc].iban[i] = rand() % 26 + 'A';
}   







//validare in 2 pasi nume plus password
//trebuie la numar introdusa validarea de numar
void updateAcc(){
	char password[10];
	char  nume[50];
	unsigned nrIncercari = 0;
	unsigned choice = 0;


	system("cls");
	printf("\n\n\t\t Update \n\n");
	printf("\n\t Introduceti numele dumneavoastra:");
	fscanf(stdin, "%s", nume);
	int valid = validareNume(nume);
	if (valid == -1){
		while (valid == -1){
			if (nrIncercari == 3) {
				printf("\n\t Numarul de incercari permise a fost depasit\n");
				exit(EXIT_FAILURE);
			}
			printf("\n\t Numele introdus este gresit.Reintroduceti:");
			fscanf(stdin, "%s", nume);
			valid = validareNume(nume);
			++nrIncercari;
		}
	}

	printf("\n\t Introduceti parola dumneavoastra:");
	scanf("%s", password);
	fflush(stdin);
	if (strcmp(conturi[valid].pin, password) != 0){
		printf("\n\t PAROLA INTRODUSA ESTE GRESITA!");
		_sleep(1000);
		exit(EXIT_FAILURE);
	}

	printf("\t 1.Modificare nume\n"
		"\t 2.Modificare numar telefon\n"
		"\t 3.Modificare parola\n"
		"\t 4.Modificare data de nastere\n"
		"\t 5.Exit\n");
	printf("\n\n\n");
	printf("\t\t Enter your choice:");
	scanf("%u", &choice);
	do{
		switch (choice)
		{
		case 1:{
				   printf("\n\t Introduceti noul nume:");
				   fscanf(stdin, "%s", conturi[valid].nume);
				   fflush(stdin);

				   break;
		}
		case 2:{
				   printf("\n\t Introduceti noul numar de telefon:");
				   fscanf(stdin, "%s", conturi[valid].telefon);
				   fflush(stdin);

				   break;
				   
		}
		case 3:{
				   printf("\n\t Introduceti noua parola:");
				   fscanf(stdin, "%s", conturi[valid].pin);
				   fflush(stdin);

				   break;

		}
		case 4:{
				   printf("\n\t Introduceti noua data de nastere:");
				   fscanf(stdin, "%s", conturi[valid].data_nastere);
				   fflush(stdin);

				   break;
		}
		case 5:{	

				   break;
		}
		default:
			break;
		}
		
		
		printf("\n\t\t Enter your choice:");
		scanf("%u", &choice);
		fflush(stdin);

	} while (choice != 5);


}


int main(){
	
	int choice=0;


	while (choice != 7){
	
		menu(&choice);

		switch (choice)
		{
		case 1:
			new_acc();
			break;
		case 2:
			updateAcc();
			break;
		case 3:
			makeTransaction();
			break;
		case 4:
			checkAcc();
			break;
		case 5:
			deleteExistingAccount();
			break;
		case 7:{
				   printf("\n\t\t\tAplicattion Close\n");
				   
		}
		default:
			break;
		}

		_sleep(1000);
		system("cls");
	}


}
