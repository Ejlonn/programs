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

void withdraw();
void deposit();
void transfer();
int currentBalance();
int handleMainMenu();
int handleSubMenu();
void registration();
int login();
char fname[]={"newlogin.txt"};


enum
{
    LOGGED_OUT,
    LOGGED_IN,
    EXIT,
};



int main(){

int user_state = LOGGED_OUT;

while(user_state != EXIT){

    if(user_state == LOGGED_OUT)
        user_state = handleMainMenu();
    else
    {
        user_state = handleSubMenu();
    }
    
}

    return 0;
}

int handleMainMenu(){

int op;
while (op != 3)
{
    printf("\nKaya Bank'a hosgeldiniz..!\n\n");
    printf("1: Kayit ol.\n");
    printf("2: Giris yap.\n");
    printf("3: Guvenli cikis.\n");
    printf("\nLutfen seciminizi yapiniz:");
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

int handleSubMenu()
{

    struct login currentUser;
    FILE *fp = fopen(fname, "r+");
    if (fp == NULL) {
        printf("Dosya açilamadi!\n");
        return EXIT;
    }

    
    if (fread(&fp, sizeof(fp), 1, fp) != 1) {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return EXIT;
    }
    
    
    
    
    int op;
   
    while (1)
    {

    printf("\nMerhaba sn. %s %s",currentUser.fname,currentUser.lname);
    printf("\nKaya Bank'a hosgeldiniz.\n");
    printf("Islemler\n1:Para Cekme\n2:Para Yatirma\n3:Havale Yapma\n4:Bakiye Sorgulama\n9:Kart Iade\n\n");
	printf("Islemi Seciniz: ");
	if(scanf("%d",&op)){
    getchar();
    switch(op){
        case 1:
		    withdraw(currentUser.balance);          
            printf("\nDevam etmek icin bir tusa basiniz.");
            getchar();
            getchar();
			break;	
		case 2:
			deposit(currentUser.balance);
            printf("\nDevam etmek icin bir tusa basiniz.");
            getchar();
            getchar();
			break;
		case 3:
			transfer(currentUser.balance);
            printf("\nDevam etmek icin bir tusa basiniz.");
            getchar();
            getchar();
			break;	
        case 4:
            currentBalance();
            printf("\nDevam etmek icin lutfen bir tusa basiniz.");
            getchar();
            break;		
		case 9:
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
 

void registration() {
    FILE *log;
    log = fopen(fname, "a+");
    struct login l;

    if (!log) {
        printf("Dosya açilamadi!\n");
        return;
    }

    printf("\nIsminizi giriniz:");
    scanf("%s", l.fname);
    printf("\nSoyisminizi giriniz:");
    scanf("%s", l.lname);
    printf("\nKullanici adi giriniz:");
    scanf("%s", l.username);
    printf("\nSifre giriniz:");
    scanf("%s", l.password);

    struct login existing_users[MAX_USERS];
    int num_users = 0;

    while (fread(&existing_users[num_users], sizeof(struct login), 1, log) == 1) {
        num_users++;
    }

    int isUserExist = 0;

    for (int i = 0; i < num_users; i++) {
        if (strcmp(l.username, existing_users[i].username) == 0) {
            isUserExist = 1;
            break;
        }
    }

    if (isUserExist) {
        printf("\nBu kullanici adi zaten kullaniliyor!\n\n");
        fclose(log);
        return;
    } 
    else {
        int wdraw;
        printf("\nBasariyla kayit oldunuz.\n");
        printf("Kullanici adiniz: %s\n", l.username);
        printf("Sifreniz:%s\n", l.password);
        printf("Artik kullanici adi ve sifreniz ile giris yapabilirsiniz..!\n");
        printf("\nBankamizda islem yapabilmek icin once para yatirmalisiniz!!\n");
        printf("Lutfen yatirmak istediginiz miktari giriniz:");
        scanf("%d", &wdraw);
        l.balance = wdraw;
        printf("\n%d TL yatirdiniz! Bizi tercih ettiğiniz için teşekkür ederiz!", l.balance);
        fwrite(&l, sizeof(l), 1, log);
        fclose(log);
    }
}
int login()
{

    FILE *log = fopen(fname, "r");
    struct login linput, lfile;
    
    if(log == NULL){
        printf("\nDosya Bulunamadi!! Lutfen once kayit olunuz!!\n");
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
void withdraw(){
    
    FILE *fp = fopen(fname, "r+");
    if (fp == NULL) {
        printf("Dosya açilamadi!\n");
        return;
    }

    struct login wdraw;

    // Kullanıcının bakiyesini dosyadan oku
    if (fread(&wdraw, sizeof(struct login), 1, fp) != 1) {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return;
    }
    int cost;

    printf("Guncel bakiyeniz: %d\t",wdraw.balance);
	printf("Lutfen cekmek istediginiz miktari giriniz: ");
	scanf("%d",&cost);
    if (cost > wdraw.balance){
        printf("Girdiginiz deger bakiyenizden fazla olamaz.!\n");
        fclose(fp);
    }
    else if (cost > 200000){
        printf("Bankamiz gunluk cekme limiti 200.000TL'dir !!");
        fclose(fp);
    }
    else
    {
        wdraw.balance -= cost;
        fseek(fp, 0, SEEK_SET);
        fwrite(&wdraw,sizeof(wdraw),1,fp);        
        fflush(fp);   
        fclose(fp); 
        printf("%d TL Cektiniz. Guncel Bakiyeniz: %d",cost,wdraw.balance);
    }

}
void deposit(){	
   
    FILE *fp = fopen(fname, "r+");
    if (fp == NULL) {
        printf("Dosya açilamadi!\n");
        return;
    }

    struct login deposit;

    // Kullanıcının bakiyesini dosyadan oku
    if (fread(&deposit, sizeof(struct login), 1, fp) != 1) {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return;
    }
    int cost;

    printf("Guncel bakiyeniz: %d\t",deposit.balance);
	printf("Lutfen yatirmak istediginiz miktari giriniz: ");
	scanf("%d",&cost);
    if (cost < 0){
        printf("Girdiginiz deger 0 dan dusuk olamaz.!\n");
        fclose(fp);
    }
    else if (cost > 200000){
        printf("Bankamiz gunluk yatirma limiti 200.000TL'dir !!");
        fclose(fp);
    }
    else
    {
        deposit.balance += cost;
        fseek(fp, 0, SEEK_SET);
        fwrite(&deposit,sizeof(deposit),1,fp);
        fflush(fp);
        fclose(fp);
        printf("%d TL cektiniz! Guncel bakiyeniz: %d",cost,deposit.balance);
    }

}

void transfer(){

    
    FILE *fp = fopen(fname, "r+");
    if (fp == NULL) {
        printf("Dosya açilamadi!\n");
        return;
    }

    struct login transfer;

    // Kullanıcının bakiyesini dosyadan oku
    if (fread(&fp, sizeof(struct login), 1, fp) != 1) {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return;
    }
    int cost;

    printf("Guncel bakiyeniz: %d\t",transfer.balance);
	printf("Lutfen transfer etmek istediginiz miktari giriniz: ");
	scanf("%d",&cost);
    if (cost > transfer.balance){
        printf("Girdiginiz deger bakiyenizden fazla olamaz.!\n");
        fclose(fp);
    }
    else if (cost > 200000){
        printf("Bankamiz gunluk transfer limiti 200.000TL'dir !!");
        fclose(fp);
    }
    else
    {
        transfer.balance -= cost;
        fseek(fp, 0, SEEK_SET);
        fwrite(&transfer,sizeof(transfer),1,fp);
        fflush(fp);
        fclose(fp);
        printf("%d TL cektiniz! Guncel bakiyeniz: %d",cost,transfer.balance);
    }

}
int currentBalance(){

    FILE *fp = fopen(fname, "r+");
    if (fp == NULL) {
        printf("Dosya açilamadi!\n");
        return EXIT_FAILURE;
    }

    struct login currentBalance;

    // Kullanıcının bakiyesini dosyadan oku
    if (fread(&currentBalance, sizeof(struct login), 1, fp) != 1) {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return EXIT;
    }
    printf("Guncel Bakiyeniz %d TL'dir.",currentBalance.balance);
    fclose(fp);
    return currentBalance.balance;
}