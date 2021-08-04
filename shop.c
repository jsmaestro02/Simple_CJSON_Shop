#include <stdio.h>
#include "cJSON.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#ifdef _WIN32
#include <conio.h>
#else
#define clrscr() printf("\e[1;1H\e[2J")
#endif

void anyKey();
void signUp(void);
void authenticate(void);
int enterChoice(char *message);
void updateSeller(char *username, char *seller, char *product, int price, int multitude);
void dashboard(char *username);
void userParse(FILE *json_file, char *username, char *pass, char *firstname, char *lastname, char *role);
int listUser();
void listBought(char *username);
void listSold(char *username);
void banUser();
void unbanUser();
void showProducts();
void newProduct(char *username);
void buy(char *username);
void updateUserBuy(char *username, char *product, int multitude, int price, int new_money);
void updateMultitude(char *product, int new_multitude);	
int getMoney(char *username);

char categories[4][30] = {"Clothes", "Electronics", "Vehicles", "Groceries"};

int main(void)
{
	int choice;
	char *filename = (char *) malloc(100 * sizeof(char));
	
	char message[] = "1) Login\n2) Sign Up\n\n99) Exit\n";
	FILE *json_file;
		
	while((choice = enterChoice(message)) != 99)
	{
		//MainMenu
		switch(choice)
		{
			//Login
			case 1:
				clrscr();
				authenticate();
				break;
			//Sign Up
			case 2:
				clrscr();
				signUp();
				break;	
		}
	}

	return 0;
}


int enterChoice(char *message)
{
	clrscr();
	printf("  ____    _\n / ___|  | |__     ___    _ __\n \\___ \\  | \'_ \\   / _ \\  | \'_ \\ \n  ___) | | | | | | (_) | | |_) |\n |____/  |_| |_|  \\___/  | .__/\n                         |_|\n");
	printf("%s", message);
	int choice;
	scanf("%d", &choice);
	return choice;

}


void anyKey()
{
	printf("\n\nPress any key to continue...\n");
	getchar();
	getchar();
}


void dashboard(char *user)
{
	FILE *json_file;
	char username[30], pass[30], firstname[30], lastname[30], role[30];
	int money, choice;
	char *filename = (char *) malloc(100 * sizeof(char));
	sprintf(filename, "users/%s.json", user);
	json_file = fopen(filename, "r+");
	
	userParse(json_file, username, pass, firstname, lastname, role);
	
	//Admin Menu	
	if(strcmp(role, "admin") == 0)
	{
		char message[] = "1)Show Info\n2)List of Users\n3)Ban Users\n4)Unban Users\n\n0)Logout\n";
adminmenu:
		while((choice = enterChoice(message)) != 0)
		{
			switch(choice)
			{
				//Show Info
				case 1:
					clrscr();
					printf("Username: %s\n", username);
					printf("Firstname: %s\n", firstname);
					printf("Lastname: %s\n", lastname);
					printf("Role: %s\n", role);
					anyKey();
					break;

				//List of Users
				case 2:
					clrscr();
					listUser();
					break;

				//Ban User
				case 3:
					clrscr();
					banUser();
					break;

				//Unban User
				case 4:
					clrscr();
					unbanUser();
					break;
				

				default:
					goto adminmenu;
			}
		}

	}

	//Customer Menu
	else if(strcmp(role, "customer") == 0)
	{
		char message[] = "1)Show Info\n2)Show Products\n3)List Bought Items\n0)Logout\n";
cusmenu:	
		while((choice = enterChoice(message)) != 0)
		{
			switch(choice)
			{
				//Show Info
				case 1:
					money = getMoney(username);
					clrscr();
					printf("Username: %s\n", username);
					printf("Firstname: %s\n", firstname);
					printf("Lastname: %s\n", lastname);
					printf("Money: %d\n", money);
					anyKey();
					break;

				//Show Products 
				case 2:
					money = getMoney(username);
					clrscr();
					showProducts();
					int choice2;
					printf("\n1)Buy an item\n0)Go Back\n");
					scanf("%d", &choice2);
					if(choice2 == 1)
					{
						buy(username);
					}

					break;

                case 3:
                    clrscr();
                    listBought(username);


				default:
					goto cusmenu;
			}
		}

		

	}

	//Retailer Menu
	else if(strcmp(role, "retailer") == 0)
	{
		char message[] = "1)Show Info\n2)Show Categories\n3)New Product\n4)Sell History\n\n0)Logout\n";
retmenu:	
		while((choice = enterChoice(message)) != 0)
		{
			switch(choice)
			{
				//Show Info
				case 1:
					clrscr();
					printf("Username: %s\n", username);
					printf("Firstname: %s\n", firstname);
					printf("Lastname: %s\n", lastname);
					anyKey();
					break;

				//Show Categories 
				case 2:
					clrscr();
					int i;
					int catcount = sizeof(categories)/sizeof(categories[0]);
					for(i=0; i<catcount; i++)
					{
						printf("%d. %s\n", i+1, categories[i]);
					}
					anyKey();
					break;

				//New Product
				case 3:
					clrscr();
					newProduct(username);
					break;

				//Sell History
				case 4:
					clrscr();
					listSold(username);
                    break;

				default:
					goto retmenu;
			}
		}



	}
	clrscr();
	printf("GoodBye %s\n", username);
	anyKey();
}


void signUp()
{
	FILE *json_file;
	int rolenum;
	int money;
	cJSON *user;
	char *filename = (char *) malloc(100 * sizeof(char));
	char username[30], pass[30], first[30], last[30], role[30];
	
	puts("Enter Username: ");
	scanf("%s", username);

	puts("Enter Password: ");
	scanf("%s", pass);
	
	puts("Enter First Name: ");
	scanf("%s", first);

	puts("Enter Last Name: ");
	scanf("%s", last);
	
roleget:	
	clrscr();
	puts("Select Role: ");
	printf("1) Admin\n2)Retailer\n3)Customer\n");
	scanf("%d", &rolenum);
	switch(rolenum)
	{
		case 1:
			strcpy(role, "admin");
			break;
		case 2:
			strcpy(role, "retailer");
			break;
		case 3:
			strcpy(role, "customer");
			break;
		default:
			puts("Invalid Input");
			anyKey();
			goto roleget;
	}

	if(strcmp(role, "admin") != 0)
	{
		puts("Enter Your Staring Money: ");
		scanf("%d", &money);
	}

	sprintf(filename, "users/%s.json", username);
	json_file = fopen(filename, "r+");
	
	if(json_file == 0)
	{
		json_file = fopen(filename, "w+");
		user = cJSON_CreateObject(); 
		
		const char *buy_history[1] = {""};
		int buy_count[1] = {0};
		int prices[1] = {0};
		cJSON *price, *history, *count;
    
		cJSON_AddStringToObject(user, "username", username);
		cJSON_AddStringToObject(user, "password", pass);
		cJSON_AddStringToObject(user, "firstname", first);
		cJSON_AddStringToObject(user, "lastname", last);
		cJSON_AddStringToObject(user, "role", role);
		cJSON_AddNumberToObject(user, "allowed", 1);
		history = cJSON_CreateStringArray(buy_history, 1);
		count = cJSON_CreateIntArray(buy_count, 1);
		price = cJSON_CreateIntArray(prices, 1);
		cJSON_AddItemToObject(user, "history", history);
		cJSON_AddItemToObject(user, "count", count);
		cJSON_AddItemToObject(user, "price", price);
		
        if(strcmp(role, "admin") != 0)
		{
			cJSON_AddNumberToObject(user, "money", money);
		}

		if(strcmp(role, "retailer") == 0)
		{
            cJSON *buyer;
		    const char *buyers[1] = {""};
		    buyer = cJSON_CreateStringArray(buyers, 1);
		    cJSON_AddItemToObject(user, "buyer", buyer);
		}
		char *string = cJSON_PrintUnformatted(user);
		cJSON_Delete(user);
		fwrite(string, 10, 100, json_file);
		puts("Signed Up Successfully");		
	}

	else
	{
		puts("User Already Exists");
	}
	anyKey();	
	fclose(json_file);
}


void authenticate(void)
{
	cJSON *user, *passPtr;
	char *username = (char *) malloc(100 * sizeof(char));
	char *pass = (char *) malloc(100 * sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	FILE *save_file;
	
	puts("Enter Username: ");
	scanf("%s", username);
	puts("Enter Password: ");
	scanf("%s", pass);
	sprintf(filename, "users/%s.json", username);
	save_file = fopen(filename, "r+");
	if(save_file == NULL)
	{
		puts("User doesn't exist");
		anyKey();
	}
	else
	{

		char *json_data = calloc(1000, sizeof(char));
		fread(json_data, 10, 100, save_file);
		user = cJSON_Parse(json_data);	
		passPtr = cJSON_GetObjectItem(user, "password");
		if(strcmp(pass, passPtr->valuestring) == 0)
		{
			if(cJSON_GetObjectItem(user, "allowed")->valueint == 1)
			{
				printf("Welcome %s\n", username);
				anyKey();
				dashboard(username);
			}
			else
			{
				puts("You are banned from logging in. Please contact your admin");
				anyKey();
			}

		}
		else
		{
			puts("Invalid User/Password");
			anyKey();
		}
	}
}


void userParse(FILE *json_file, char *username, char *pass, char *firstname, char *lastname, char *role)
{
	cJSON *user;
	cJSON *usernamePtr, *passPtr, *firstnamePtr, *lastnamePtr, *rolePtr, *moneyPtr;
	char *json_data = calloc(1000, sizeof(char));
	fread(json_data, 10, 100, json_file);
	user = cJSON_Parse(json_data);
	

	//getting the info of the user	
	usernamePtr = cJSON_GetObjectItem(user, "username");
	strcpy(username, usernamePtr->valuestring);

	passPtr = cJSON_GetObjectItem(user, "password");
	strcpy(pass, passPtr->valuestring);

	firstnamePtr = cJSON_GetObjectItem(user, "firstname");
	strcpy(firstname, firstnamePtr->valuestring);

	lastnamePtr = cJSON_GetObjectItem(user, "lastname");
	strcpy(lastname, lastnamePtr->valuestring);

	rolePtr = cJSON_GetObjectItem(user, "role");
	strcpy(role, rolePtr->valuestring);
	
		
	cJSON_Delete(user);
}


int listUser()
{
	struct dirent *de; 
	FILE *json_file;
	cJSON *user, *usernamePtr, *rolePtr;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
  
    	DIR *dr = opendir("users");
  
    	while ((de = readdir(dr)) != NULL)
	{			
		sprintf(filename, "users/%s", de->d_name);
		json_file = fopen(filename, "r+");
		if(json_file != 0)
		{
			fread(json_data, 10, 100, json_file);
			user = cJSON_Parse(json_data);

			usernamePtr = cJSON_GetObjectItem(user, "username");
			rolePtr = cJSON_GetObjectItem(user, "role");

			printf("%s : %s\n", usernamePtr->valuestring, rolePtr->valuestring);
		}
	}	
	
	anyKey();  
	
	cJSON_Delete(user);	
    	closedir(dr);	
}


void banUser()
{
	char username[30];
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *user, *rolePtr;
	puts("Enter Username: ");
	scanf("%s", username);
	
	sprintf(filename, "users/%s.json", username);
	json_file = fopen(filename, "r+");
	if(json_file == 0)
	{
		puts("User doesn't exist.");
	}
	else
	{
		fread(json_data, 10, 100, json_file);
		user = cJSON_Parse(json_data);

		rolePtr = cJSON_GetObjectItem(user, "role");
		if(strcmp(rolePtr->valuestring, "admin") == 0) {
			printf("You can't change an admin's permission");
		}
		else
		{
			cJSON *perm, *sub;
			user = cJSON_Parse(json_data);
			cJSON_DeleteItemFromObject(user, "allowed");
			cJSON_AddNumberToObject(user, "allowed", 0);
			char *string = cJSON_PrintUnformatted(user);
			printf("%s has been banned successfully.", username);
			anyKey();
			cJSON_Delete(user);
			json_file = fopen(filename, "w");
			fputs(string, json_file);
		}
			
	}
	fclose(json_file);

}


void unbanUser()
{
	char username[30];
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *user, *rolePtr;
	puts("Enter Username: ");
	scanf("%s", username);
	
	sprintf(filename, "users/%s.json", username);
	json_file = fopen(filename, "r+");
	if(json_file == 0)
	{
		puts("User doesn't exist.");
	}
	else
	{
		fread(json_data, 10, 100, json_file);
		user = cJSON_Parse(json_data);

		rolePtr = cJSON_GetObjectItem(user, "role");
		if(strcmp(rolePtr->valuestring, "admin") == 0)
		{
			printf("You can't change an admin's permission");
		}
		else
		{
			cJSON *perm, *sub;
			user = cJSON_Parse(json_data);
			cJSON_DeleteItemFromObject(user, "allowed");
			cJSON_AddNumberToObject(user, "allowed", 1);
			char *string = cJSON_PrintUnformatted(user);
			printf("%s has been unbanned successfully.", username);
			anyKey();
			cJSON_Delete(user);
			json_file = fopen(filename, "w");
			fputs(string, json_file);
		}
			
	}
	fclose(json_file);
}


void showProducts(void)
{
	struct dirent *de; 
	FILE *json_file;
	cJSON *product, *name, *price, *category, *seller, *multitude;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
  
    	DIR *dr = opendir("products");

    	while ((de = readdir(dr)) != NULL)
	{			
		sprintf(filename, "products/%s", de->d_name);
		json_file = fopen(filename, "r+"); if(json_file != 0)
		{
			fread(json_data, 10, 100, json_file);
			product = cJSON_Parse(json_data);

			name = cJSON_GetObjectItem(product, "product name");
			category = cJSON_GetObjectItem(product, "category");
			seller = cJSON_GetObjectItem(product, "seller");
			multitude = cJSON_GetObjectItem(product, "multitude");
			price = cJSON_GetObjectItem(product, "price");
			if(multitude->valueint >= 0)
			{
				printf("%s: %d$\n\tcategory: %s\n\tseller: %s\n\tmultitude: %d\n",name->valuestring, price->valueint,
						category->valuestring, seller->valuestring, multitude->valueint);
			}
		}
	}	
	
	anyKey();  
	
	cJSON_Delete(product);	
    	closedir(dr);	

}


void newProduct(char *username)
{

	FILE *json_file;
	cJSON *product;
	char *filename = (char *) malloc(100 * sizeof(char));
	char pname[30], category[30]; 
	long int price, multitude;
	
	puts("Enter the name of the product: ");
	scanf("%s", pname);
	
	puts("Enter the price of the product: ");
	scanf("%ld", &price);
	

	puts("How many do you have: ");
	scanf("%ld", &multitude);
	
catget:	
	clrscr();
	puts("Which category does it belong to: ");
	scanf("%s", category);


	sprintf(filename, "products/%s.json", pname);
	json_file = fopen(filename, "r+");
	
	if(json_file == 0)
	{
		json_file = fopen(filename, "w+");
		product = cJSON_CreateObject(); 
		
		cJSON_AddStringToObject(product, "product name", pname);
		cJSON_AddStringToObject(product, "category", category);
		cJSON_AddStringToObject(product, "seller", username);
		cJSON_AddNumberToObject(product, "multitude", multitude);
		cJSON_AddNumberToObject(product, "price", price);

		char *string = cJSON_PrintUnformatted(product);
		cJSON_Delete(product);
		fwrite(string, 10, 100, json_file);
		puts("Product Registered Successfully");		
	}

	else
	{
		puts("Product already exists.");
	}
	anyKey();	
	fclose(json_file);
}


void buy(char *username)
{
	int count;
	char pname[50];
	puts("Which product do you want?");
	scanf("%s", pname);
	puts("How many?");
	scanf("%d", &count);

	//get user balance	
	cJSON *user, *moneyPtr;
	int money;
	char *user_filename = (char *) malloc(100 * sizeof(char));
	sprintf(user_filename, "users/%s.json", username);
	FILE *json_file_user;
  	json_file_user = fopen(user_filename, "r+");
	char *json_data_user = calloc(1000, sizeof(char));
	fread(json_data_user, 10, 100, json_file_user);
	user = cJSON_Parse(json_data_user);
	moneyPtr = cJSON_GetObjectItem(user, "money");
	money = moneyPtr->valueint;
	
	//get multitude and price
	cJSON *product;
	FILE *json_file_product;
	cJSON *pricePtr, *multitudePtr, *sellerPtr;
	char *product_filename = (char *) malloc(100 * sizeof(char));
	char *json_data_product = calloc(1000, sizeof(char));
	char *seller;
	int multitude, price;
	sprintf(product_filename, "products/%s.json", pname);
	json_file_product = fopen(product_filename, "r+");
	fread(json_data_product, 10, 100, json_file_product);
	product = cJSON_Parse(json_data_product);

	pricePtr = cJSON_GetObjectItem(product, "price");
	multitudePtr = cJSON_GetObjectItem(product, "multitude");
	sellerPtr = cJSON_GetObjectItem(product, "seller");
	price = pricePtr->valueint;
	multitude = multitudePtr->valueint;
	seller = sellerPtr->valuestring;
	
	//check if user can buy the item	
	if(json_file_product != 0)
	{
		if(multitude > 0 && multitude >= count)
		{
			//able to buy
			if(money >= count * price)
			{
				updateUserBuy(username, pname, count, count * price, money-(count * price));
				updateMultitude(pname, multitude - count);
                updateSeller(username, seller, pname, count * price, count);
			}	
		}
		//out of stock
		else
		{
			puts("Out of Stock");
		}
	}
	//unlisted product name
	else
	{
		puts("Product doesn't exist");
	}
	cJSON_Delete(user);
}


void updateUserBuy(char *username, char *product, int multitude, int price, int new_money)
{
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *user;
	
	sprintf(filename, "users/%s.json", username); json_file = fopen(filename, "r+");
	fread(json_data, 10, 100, json_file);
	user = cJSON_Parse(json_data);

	cJSON *count, *history, *prices, *money, *bought, *item;
	user = cJSON_Parse(json_data);

    cJSON_DeleteItemFromObject(user, "money");
	cJSON_AddNumberToObject(user, "money", new_money);
    

    history = cJSON_GetObjectItem(user, "history");
    count = cJSON_GetObjectItem(user, "count");
    prices = cJSON_GetObjectItem(user, "price");
    cJSON_AddItemToArray(history, cJSON_CreateString(product));
    cJSON_AddItemToArray(count, cJSON_CreateNumber(multitude));
    cJSON_AddItemToArray(prices, cJSON_CreateNumber(price));
    
    char *string = cJSON_PrintUnformatted(user);
	cJSON_Delete(user);
	json_file = fopen(filename, "w");
	fputs(string, json_file);
	
	fclose(json_file);
}


void updateSeller(char *username, char *seller, char *product, int price, int multitude)
{
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *user;
	
	sprintf(filename, "users/%s.json", seller); 
    json_file = fopen(filename, "r+");
	fread(json_data, 10, 100, json_file);
	user = cJSON_Parse(json_data);

	cJSON *buyer, *count, *history, *prices, *money, *bought, *item;
	user = cJSON_Parse(json_data);

    history = cJSON_GetObjectItem(user, "history");
    buyer = cJSON_GetObjectItem(user, "buyer");
    count = cJSON_GetObjectItem(user, "count");
    prices = cJSON_GetObjectItem(user, "price");
    cJSON_AddItemToArray(history, cJSON_CreateString(product));
    cJSON_AddItemToArray(buyer, cJSON_CreateString(username));
    cJSON_AddItemToArray(count, cJSON_CreateNumber(multitude));
    cJSON_AddItemToArray(prices, cJSON_CreateNumber(price));
    
    char *string = cJSON_PrintUnformatted(user);
	cJSON_Delete(user);
	json_file = fopen(filename, "w");
	fputs(string, json_file);
	
	fclose(json_file); }


void updateMultitude(char *pname, int new_multitude)	
{
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *product, *multitude;
	
	sprintf(filename, "products/%s.json", pname);
	json_file = fopen(filename, "r+");
	fread(json_data, 10, 100, json_file);
	product= cJSON_Parse(json_data);
		
	cJSON_DeleteItemFromObject(product, "multitude");
	cJSON_AddNumberToObject(product, "multitude", new_multitude);
	

	char *string = cJSON_PrintUnformatted(product);
	cJSON_Delete(product);
	json_file = fopen(filename, "w");
	fputs(string, json_file);
	
	fclose(json_file);
}


int getMoney(char *username)
{
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *user, *money;
	
	sprintf(filename, "users/%s.json", username);
	json_file = fopen(filename, "r+");
	fread(json_data, 10, 100, json_file);
	user = cJSON_Parse(json_data);
		
	money = cJSON_GetObjectItem(user, "money");
		

	cJSON_Delete(user);
	fclose(json_file);
	return money->valueint;	
}


void listBought(char *username) 
{
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *user, *price, *bought, *count;
   
	
	sprintf(filename, "users/%s.json", username);
	json_file = fopen(filename, "r+");
	fread(json_data, 10, 100, json_file);
	user = cJSON_Parse(json_data);
	
    bought = cJSON_GetObjectItem(user, "history");
	count = cJSON_GetObjectItem(user, "count");
	price = cJSON_GetObjectItem(user, "price");
    int n = cJSON_GetArraySize(count);
    int i;
    for(i=1; i<n; i++)
    {
        printf("%s x%d : %d$\n", cJSON_GetArrayItem(bought, i)->valuestring, cJSON_GetArrayItem(count, i)->valueint, cJSON_GetArrayItem(price, i)->valueint);
    } 

    anyKey();
        

	cJSON_Delete(user);
	fclose(json_file);
}


void listSold(char *username) 
{
	FILE *json_file;
	char *json_data = calloc(1000, sizeof(char));
	char *filename = (char *) malloc(100 * sizeof(char));
	cJSON *user, *buyer, *price, *bought, *count;
   
	
	sprintf(filename, "users/%s.json", username);
	json_file = fopen(filename, "r+");
	fread(json_data, 10, 100, json_file);
	user = cJSON_Parse(json_data);
	
    bought = cJSON_GetObjectItem(user, "history");
	count = cJSON_GetObjectItem(user, "count");
	buyer = cJSON_GetObjectItem(user, "buyer");
	price = cJSON_GetObjectItem(user, "price");
    int n = cJSON_GetArraySize(count);
    int i;
    for(i=1; i<n; i++)
    {
        printf("%s->%s x%d : %d$\n", cJSON_GetArrayItem(bought, i)->valuestring, cJSON_GetArrayItem(buyer, i)->valuestring, cJSON_GetArrayItem(count, i)->valueint, cJSON_GetArrayItem(price, i)->valueint);
    } 

    anyKey();
        

	cJSON_Delete(user);
	fclose(json_file);
}
