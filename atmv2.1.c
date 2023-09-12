#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LONG 20
#define STRING_SHORT 10
#define MAX_USERS 50



/// bi tane kullanıcı yapısı, bi tane ana menu, bi tane giriş yapıldıktan sonraki(submenu)menü,
/// bir kayıt ve giriş menüsü yapılacak. Submenuye ana program eklenicek.

struct login
{
    char fname[STRING_LONG];
    char lname[STRING_LONG];
    char username[STRING_SHORT];
    char password[STRING_SHORT];
    int balance;
};

int withdraw(int *balance,int cost);
int deposit(int *balance,int cost);
int transfer(int *balance, int cost);
int handleMainMenu();
int handleSubMenu(int *balance,int cost);
void registration();
int login();
char fname[]={"login.dat"};


enum
{
    LOGGED_OUT,
    LOGGED_IN,
    EXIT,
};



int main(){

int balance= 1000;
 // global değişkenler
int *pB = &balance;
int cost;

int user_state = LOGGED_OUT;

while(user_state != EXIT){

    if(user_state == LOGGED_OUT)
        user_state = handleMainMenu();
    else
    {
        user_state = handleSubMenu(pB,cost);
    }
    
}

    return 0;
}

int handleMainMenu(){

int op;
while (op != 3)
{
    printf("Kaya Bank'a hosgeldiniz..!\n\n");
    printf("1: Kayit ol.\n");
    printf("2: Giris yap.\n");
    printf("3: Guvenli cikis.\n");
    printf("Lutfen seciminizi yapiniz:");
    scanf("%d",&op);

    switch (op)
    {
    case 1:
        registration();
        break;
    case 2:
        if(login() == 1)
        return LOGGED_IN;
        else{
            return 0;
        }
        break;
    case 3:
        printf("\nBizi tercih ettiginiz icin Tesekkur ederiz..!\n");
        return EXIT;
        break;
    default:
        printf("Gecersiz islem! Lutfen gecerli bir secim yapiniz..!\n");
        break;
        
    }
}
}

int handleSubMenu(int *balance, int cost)
{
    
    int op;
    while (1)
    {
    printf("\nKaya Bank'a hosgeldiniz.\n");
    printf("Islemler\n1:Para Cekme\n2:Para Yatirma\n3:Havale Yapma\n4:Bakiye Sorgulama\n5:Kart Iade\n\n\n");
	printf("Islemi Seciniz: ");
	if(scanf("%d",&op)){
    getchar();
    switch(op){
        case 1:
			withdraw(balance,cost);
            printf("\nDevam etmek icin bir tusa basiniz.");
            getchar();
            getchar();
			break;	
		case 2:
			deposit(balance,cost);
            printf("\nDevam etmek icin bir tusa basiniz.");
            getchar();
            getchar();
			break;
		case 3:
			transfer(balance,cost);
            printf("\nDevam etmek icin bir tusa basiniz.");
            getchar();
            getchar();
			break;			
		case 4: 
			printf("Hesabinizda %d TL Bulunmaktadir.\n",*balance);
            printf("\nDevam etmek icin bir tusa basiniz.");
            getchar();			
            getchar();
            break;
		case 5:
			printf("Guvenli Cikis yapildi. Tekrar bekleriz.");
			return EXIT;
            break;
		default: 
			printf("Hatali tuslama yaptiniz.\n");	
			break;

        }			
    }

    }
}    
 

void registration(){
    
    FILE *fp; 
    fp = fopen(fname,"a+b");
    struct login l;
    
    printf("\nIsminizi giriniz:");
    scanf("%s",(l.fname));
    printf("\nSoyisminizi giriniz:");
    scanf("%s",(l.lname));
    printf("\nKullanici adi giriniz.");
    scanf("%s",(l.username));
    printf("\nSifre giriniz:\n");
    scanf("%s",(l.password));

    struct login existing_users[MAX_USERS];
    int num_users = 0;
    while (fread(&existing_users[num_users],sizeof(struct login),1,fp) == 1)
    {
        num_users++;
    }
    
    int i;
    int isUserExist = 0;
    
    for(i=0; i< num_users ; i++){

        if(strcmp(l.username,existing_users[i].username)== 0)
        {
            isUserExist = 1;
            break;
        }
    }

    if(isUserExist){
        printf("\nBu kullanici adi zaten kullaniliyor!\n\n");
        fclose(fp);
    }
    else
    {

        fwrite(&l, sizeof(l), 1, fp);
        printf("\nBasariyla kayit oldunuz.\n");
        printf("Kullanici adiniz: %s\n",l.username);
        printf("Sifreniz:%s\n",l.password);
        printf("Artik kullanici adi ve sifreniz ile giris yapabilirsiniz..!\n");
        l.balance == 1000;
        printf("\nDevam etmek icin lutfen bir tusa basin.");
        getchar();

}
}
int login()
{

    FILE *log = fopen(fname, "r");
    struct login linput, lfile;
    
    if(log == NULL){
        printf("Dosya Bulunamadi!!");
        return 0; // giriş başarısız.
    }
    printf("\nKullanici adiniz:");
    scanf("%s",linput.username);
    printf("\nSifreniz:");
    scanf("%s",linput.password);
    while( fread(&lfile, sizeof(lfile), 1, log))
    {
        if (strcmp(linput.username, lfile.username) == 0 && strcmp(linput.password, lfile.password)==0){        
            printf("\nBasariyla giris yaptiniz!\n");
            fclose(log);
            return 1; // giriş başarılı.
        }
    }
    printf("\nKullanici adi veya sifre yanlis!!\n");
    printf("\nDevam etmek icin bir tusa basin...\n");
    getchar();
    getchar();
    fclose(log);
    return 0; // giriş başarısız.
}
int withdraw(int *balance,int cost){

		printf("Guncel bakiyeniz: %d\t",*balance);
		printf("Lutfen cekmek istediginiz miktari giriniz: ");
		scanf("%d",&cost);
        getchar();
	if (cost < 0){

		printf("Gecersiz bir tutar girdiniz!\n");
        fflush(stdout);
		}
	else if (cost > 200000){
		printf("Bankamiz gunluk cekme limitini astiniz!\n");
	
	}
	else{
		*balance -= cost;

		printf("%d TL Cektiniz. Guncel bakiyeniz: %d\n",cost,*balance);
	}
	return *balance;
	}	
int deposit(int *balance,int cost){	

		printf("Guncel bakiyeniz: %d\t",*balance);
		printf("Lutfen yatirmak istediginiz miktari giriniz: ");
		scanf("%d",&cost);
	if (cost < 0){

		printf("Gecersiz bir tutar girdiniz!\n");
		}
	else if (cost > 200000){
		printf("Bankamiz gunluk yatirma limitini astiniz!\n");
	
	}
	else{
		*balance += cost;
		printf("%d TL Yatirdiniz. Guncel bakiyeniz: %d\n",cost,*balance);
	}
	return *balance;
}
int transfer(int *balance,int cost){

		printf("Guncel bakiyeniz: %d\t",*balance);
		printf("Lutfen havale yapmak istediginiz miktari giriniz: ");
		scanf("%d",&cost);
	if (cost < 0 || cost>*balance){
		printf("Gecersiz bir tutar girdiniz!\n");
		}
	else if (cost > 200000){
		printf("Bankamiz gunluk yatirma limitini astiniz!\n");
}
	else {
		*balance -= cost;
		printf("%d TL Gonderdiniz. Guncel bakiyeniz: %d\n",cost,*balance);
	}
return *balance;	
}