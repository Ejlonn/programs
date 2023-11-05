#include <stdio.h>
#include <stdlib.h>
#include <string.h> // added for comparing user input and existing data.

#define STRING_LONG 20
#define STRING_SHORT 10
#define MAX_USERS 50

/// this code contains an user interface, a main menu, a registration and login menu and a sub menu after successfully logging in
//  submenu contains the main program, main menu contains the registration and login options.
//  int main functions as checking the user state and operate the program by calling the right function

// bu kodda bir kullanıcı arayüzü, bir ana menü, bir kayıt ol ve giriş yap menüsü, bir alt menü(giriş yaptıktan sonraki menü) bulunuyor.
// alt menu ana programı içeriyor, ana menü ise kayıt ol ve giriş yap seçeneklerini içeriyor
// int main fonksiyonu kullanıcı giriş yapmış mı yapmamış mı veya çıkmak mı istiyor koşullarını kontrol edip kullanıcı durumuna göre-
// ana menü veya alt menüyü çağırıyor.

struct login // basic user input, includes their first name,lastname a username and a password.
{
    char fname[STRING_LONG];
    char lname[STRING_LONG];
    char username[STRING_SHORT];
    char password[STRING_SHORT];
    int balance;
};

void withdraw();                 // for withdraw operation.
void deposit();                  // deposit operation
void transfer();                 // transfer operation
int currentBalance();            // This is for the sync directly from file and it helps for real time data storing without closing program to synchronize
int handleMainMenu();            // Registration and login menu
int handleSubMenu();             // after login menu, where program actually works
void registration();             // to create an user, it also checks if there's a user already registered with the same username.
int login();                     // checks for if user exists and compares with input, then directs to the submenu
char fname[] = {"newlogin.txt"}; // this is the name of the file that stores users' data. You can change the name of file here

enum // To check if users logged in or out, and helps for exit.
{
    LOGGED_OUT,
    LOGGED_IN,
    EXIT,
};

int main()
{

    int user_state = LOGGED_OUT;

    while (user_state != EXIT)
    { // we check if user is logged in or out so that we can call the correct function if the return value is EXIT, we break the program

        if (user_state == LOGGED_OUT)
            user_state = handleMainMenu();
        else
        {
            user_state = handleSubMenu();
        }
    }

    return 0;
}

int handleMainMenu()
{ //

    int op;
    while (op != 3)
    {
        printf("\nKaya Bank'a hosgeldiniz..!\n\n");
        printf("1: Kayit ol.\n");
        printf("2: Giris yap.\n");
        printf("3: Guvenli cikis.\n");
        printf("\nLutfen seciminizi yapiniz:");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            registration();
            break;
        case 2:
            if (login() == 1) // this checks if function login is true, then returns logged in to main menu
                return LOGGED_IN;
            else
            {
                return 0;
            }
            break;
        case 3:
            printf("\nBizi tercih ettiginiz icin Tesekkur ederiz..!\n");
            return EXIT; // returns exit, and close the program
            break;
        default:
            printf("Gecersiz islem! Lutfen gecerli bir secim yapiniz..!\n");
            break;
        }
    }
}

int handleSubMenu() // this is the menu if you are logged in, you make your operations here and where the main program lives.
{

    struct login currentUser;
    FILE *fp = fopen(fname, "r+"); // we read file here again so we can see our current balance via currentBalance function
    if (fp == NULL)
    { // to prevent wrong balance output(operations do their job successfully but it wasn't shown
      // at this menu)
        printf("Dosya açilamadi!\n");
        return EXIT;
    }

    if (fread(&currentUser, sizeof(currentUser), 1, fp) != 1)
    {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return EXIT;
    }

    int op;

    while (1) // we declare a infinite loop but if user wants to exit, we return EXIT to main menu via case 9, so that this menu can used continously.
    {

        printf("\nMerhaba sn. %s %s", currentUser.fname, currentUser.lname);
        printf("\nKaya Bank'a hosgeldiniz.\n");
        printf("Islemler\n1:Para Cekme\n2:Para Yatirma\n3:Havale Yapma\n4:Bakiye Sorgulama\n9:Kart Iade\n\n");
        printf("Islemi Seciniz: ");
        if (scanf("%d", &op))
        {
            getchar();
            switch (op)
            {
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
                return EXIT; // this is the break condition.
                break;
            default:
                printf("Hatali tuslama yaptiniz.\n");
                break;
            }
        }
    }
}

void registration()
{ // the registration function, it creates a file named has fname(declared as global variable)
  // and checks if user exists or not

    FILE *log;
    log = fopen(fname, "a+");
    struct login l;

    if (!log)
    {
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

    struct login existing_users[MAX_USERS]; // we create an array of struct and max size is MAX USERS, this is for creating and listing the current
                                            // users, if we want we can see how many users we have.
    int num_users = 0;

    while (fread(&existing_users[num_users], sizeof(struct login), 1, log) == 1)
    {
        num_users++;
    }

    int isUserExist = 0;

    for (int i = 0; i < num_users; i++)
    {
        if (strcmp(l.username, existing_users[i].username) == 0)
        {                    // we compare here if user is already exist and declare isUserExist=1 and
            isUserExist = 1; // we check at next if statement and return them to main menu
            break;
        }
    }

    if (isUserExist)
    {
        printf("\nBu kullanici adi zaten kullaniliyor!\n\n");
        fclose(log);
        return;
    }
    else
    { // after succesfull signing up- we write their data to "fname" file and ask them deposit some money so they can operate, or they can do it
      // at the subMenu
        int deposit;
        printf("\nBasariyla kayit oldunuz.\n");
        printf("Kullanici adiniz: %s\n", l.username);
        printf("Sifreniz:%s\n", l.password);
        printf("Artik kullanici adi ve sifreniz ile giris yapabilirsiniz..!\n");
        printf("\nBankamizda islem yapabilmek icin once para yatirmalisiniz!!\n");
        printf("Lutfen yatirmak istediginiz miktari giriniz:");
        scanf("%d", &deposit);
        l.balance = deposit;
        printf("\n%d TL yatirdiniz! Bizi tercih ettiğiniz için teşekkür ederiz!", l.balance);
        fwrite(&l, sizeof(l), 1, log);
        fclose(log);
    }
}

int login() // check for user input and the data existing at file, we open the file on reading mode and strcmp it with the input
            // if the input data is not matching with the data that exists in the file, we return LOGGED_OUT so they can
            // sign up or close the program.
{

    FILE *log = fopen(fname, "r");
    struct login linput, lfile;

    if (log == NULL)
    { // checks if the file exists and returns an error message that wants them to sign up.
        printf("\nDosya Bulunamadi!! Lutfen once kayit olunuz!!\n");
        return 0; // giriş başarısız.
    }
    printf("\nKullanici adiniz:");
    scanf("%s", linput.username);
    printf("\nSifreniz:");
    scanf("%s", linput.password);
    while (fread(&lfile, sizeof(lfile), 1, log)) // we compare user input and existing data here.
    {
        if (strcmp(linput.username, lfile.username) == 0 && strcmp(linput.password, lfile.password) == 0)
        {
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
    return LOGGED_OUT; // giriş başarısız.
}

void withdraw()
{

    FILE *fp = fopen(fname, "r+"); // open the file as read and write mode(r+)
    if (fp == NULL)
    {
        printf("Dosya açilamadi!\n");
        return;
    }

    struct login wdraw;

    // Kullanıcının bakiyesini dosyadan oku             // we read the users balance from file here.
    if (fread(&wdraw, sizeof(wdraw), 1, fp) != 1)
    {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return;
    }
    int cost;

    printf("Guncel bakiyeniz: %d\t", wdraw.balance);
    printf("Lutfen cekmek istediginiz miktari giriniz: ");
    scanf("%d", &cost);
    if (cost > wdraw.balance)
    { // checks for the conditions to prevent wrong user input
        printf("Girdiginiz deger bakiyenizden fazla olamaz.!\n");
        fclose(fp);
    }
    else if (cost > 200000)
    { // checks for the conditions to prevent wrong user input
        printf("Bankamiz gunluk cekme limiti 200.000TL'dir !!");
        fclose(fp);
    }
    else
    {
        wdraw.balance -= cost; // this is the place where operation took place, and rewrite the balance data.
        fseek(fp, 0, SEEK_SET);
        fwrite(&wdraw, sizeof(wdraw), 1, fp);
        fflush(fp); // this is for synchronising the data in the file immediately so we can see our current balance at the subMenu.(operation 4)
        fclose(fp);
        printf("%d TL Yatirdiniz. Guncel Bakiyeniz: %d", cost, wdraw.balance);
    }
}
void deposit()
{ // similar to wdraw function but operation is different

    FILE *fp = fopen(fname, "r+");
    if (fp == NULL)
    {
        printf("Dosya açilamadi!\n");
        return;
    }

    struct login deposit;

    // Kullanıcının bakiyesini dosyadan oku
    if (fread(&deposit, sizeof(deposit), 1, fp) != 1)
    {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return;
    }
    int cost;

    printf("Guncel bakiyeniz: %d\t", deposit.balance);
    printf("Lutfen yatirmak istediginiz miktari giriniz: ");
    scanf("%d", &cost);
    if (cost < 0)
    { // checks for the conditions to prevent wrong user input
        printf("Girdiginiz deger 0 dan dusuk olamaz.!\n");
        fclose(fp);
    }
    else if (cost > 200000)
    { // checks for the conditions to prevent wrong user input
        printf("Bankamiz gunluk yatirma limiti 200.000TL'dir !!");
        fclose(fp);
    }
    else
    {
        deposit.balance += cost;
        fseek(fp, 0, SEEK_SET);
        fwrite(&deposit, sizeof(deposit), 1, fp);
        fflush(fp);
        fclose(fp);
        printf("%d TL cektiniz! Guncel bakiyeniz: %d", cost, deposit.balance);
    }
}

void transfer()
{ // same as wdraw function, just the change of names

    FILE *fp = fopen(fname, "r+");
    if (fp == NULL)
    {
        printf("Dosya açilamadi!\n");
        return;
    }

    struct login transfer;

    // Kullanıcının bakiyesini dosyadan oku
    if (fread(&transfer, sizeof(transfer), 1, fp) != 1)
    {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return;
    }
    int cost;

    printf("Guncel bakiyeniz: %d\t", transfer.balance);
    printf("Lutfen transfer etmek istediginiz miktari giriniz: ");
    scanf("%d", &cost);
    if (cost > transfer.balance)
    {
        printf("Girdiginiz deger bakiyenizden fazla olamaz.!\n");
        fclose(fp);
    }
    else if (cost > 200000)
    {
        printf("Bankamiz gunluk transfer limiti 200.000TL'dir !!");
        fclose(fp);
    }
    else
    {
        transfer.balance -= cost;
        fseek(fp, 0, SEEK_SET);
        fwrite(&transfer, sizeof(transfer), 1, fp);
        fflush(fp);
        fclose(fp);
        printf("%d TL Gonderdiniz! Guncel bakiyeniz: %d", cost, transfer.balance);
    }
}
int currentBalance()
{ // this is for the synchronising the balance data so that we can see it real time
  // without this function we do operations successfully but can't see it correctly
  // in the sub menu.

    FILE *fp = fopen(fname, "r+");
    if (fp == NULL)
    {
        printf("Dosya açilamadi!\n");
        return EXIT_FAILURE;
    }

    struct login currentBalance;

    // Kullanıcının bakiyesini dosyadan oku
    if (fread(&currentBalance, sizeof(currentBalance), 1, fp) != 1)
    {
        printf("Kullanici bilgileri okunamadi!\n");
        fclose(fp);
        return EXIT;
    }
    printf("Guncel Bakiyeniz %d TL'dir.", currentBalance.balance);
    fclose(fp);
    return currentBalance.balance;
}
