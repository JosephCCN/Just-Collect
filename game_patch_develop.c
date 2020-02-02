#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<wchar.h>
#include <stdarg.h>
#include<unistd.h>
#include "Encryption.h"
#define cls system("cls");
#define pause system("pause");
#define ESC 27
#define admin_user 3
#define random_code 100
#define random_chance 95
#define max_stuff_val 100
#define frequency 600
#define duration 30
#define max_level 3 //remember to change if a new level of ore is added
#define type_of_core 4 //remember to change if a new core is added

/**
list of command code:
end game ..... 0
user ......... 1
admin .... ... 2
developer .....3
**/

/********special function***********/

void gotoxy(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void color(int ForgC){
     WORD wColor;
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi)){
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}
/*********special function end ********/

/*******struct part******************/
typedef struct {
	int x , y;
}  MAP;

typedef struct {
	int x , y;
	int weapon_val;
	int defence;
	int health;
	int lv;
} PLAYER;

typedef struct{
	float time_spend;
	bool hurt;
} CI;

char play_map[1000][1000];
int core_lv_divide[1000][1000]; //only for map 2 i.e. mining area
char ore_list[101][100][100] = {

{},
{"Blue Ore" , "Red Ore"},  //lv.1
{"IDK Ore"},   //lv.2
{"il7 Ore"},   //lv.3

}; //lv , string array

int ore_in_each_lv[100] = {0 , 2 , 1 , 1}; //remember to change if a new core is added
char ore_code[100][100] = {"You find a Easter EGG" , "Blue Ore" , "Red Ore" , "IDK Ore" , "il7 Ore"};
int ore_inventory[100];

PLAYER coop;
int total_monster_kill;
int monster_counting;
int skillpoint[5];
char main_character;
char boundary_skin = '#';
int text_color;
char narrow;
char up , down , left , right;
bool admin_checked = false;
bool string_cmp(char* , char* );

/*******struct part end***************/

int alp[26]; //the alpabet
char player_ac[1000];
int map_lower_x , map_lower_y;
int mode = 1;

void string_input(char str[] , int max_len , bool hide){
	int pt = 0;
	while(1){
		char ch = getch();
		if(ch == ESC){
			strcpy(str , "GoToHomePage_Code:100");
			return;
		}
		ch = tolower(ch);
		if((33 > ch || ch > 127) && ch != '\r' && ch != '\b' && ch < 0) continue;
		if(ch == '\r'){
			putchar('\n');
			break;
		}
		if(ch == '\b' && pt > 0){
			str[pt - 1] = '\0';
			pt--;
			printf("\b \b");
		}
		else if(max_len > pt){
			str[pt] = ch;
			pt++;
			if(hide) putchar('*');
			else putchar(ch);
		}
	}
}

int find_lv(int v){
	v = v / 100;
	float a = (-1 + sqrt(1 + 8 * v))/2; //quard equation
	int re = a;
	return re;
}


int admin_page_class(int line){
	printf("Please select:\n");
	printf("Check all players information\n");
	printf("Search player information\n");
	printf("Change player data\n");
	printf("Delete player account\n");
	printf("Exit");
	char cmd;
	int services = 5;
	
	do{
		gotoxy(30 , line);
		putchar('<');
		cmd = getch();
		cmd = tolower(cmd);
		if(cmd == 'w' && line > 1){
			gotoxy(30 , line);
			putchar(' ');
			line--;
		}
		else if(cmd == 's' && line < services){
			gotoxy(30 , line);
			putchar(' ');
			line++;
		}
	}while(cmd != '\r');
	cls;
	return line;
}

void admin_page(){
	cls;
	int choose = 1;
	do{
		choose = admin_page_class(choose);
		if(choose == 1){  //check all player information
			printf("Username..........Password\n");
			FILE *file;
			file = fopen("player_list.txt" , "r");
			char name[10005];
			while(fscanf(file , "%s" , name) != EOF){
				printf("%s" , name);
				FILE *fp;
				strcat(name , ".txt");
				fp = fopen(name , "r");
				char password[10005];
				fscanf(fp , "%s\n" , password);
				if(string_cmp(password , "ADMIN")){
					fscanf(fp , "%s\n" , password);
				}
				fclose(fp);
				for(int i=0;i<30 - strlen(name) - strlen(password);i++){
					putchar('.');
				}
				puts(password);
			} 
			fclose(file);
			pause;
			cls;
		}
		else if(choose == 2){
			printf("Please enter which player you want to search:");
			char input[10005];
			scanf("%s" , input);
			char tmp[10005];
			strcpy(tmp , input);
			strcat(input , ".txt");
			cls;
			if(!fopen(input , "r")){
				printf("No such player..");
				Sleep(500);
				cls;
				continue;
			}
			FILE *fp;
			fp = fopen(input , "r");
			printf("Player %s:\n" , tmp);
			char password[1005];
			fscanf(fp , "%s\n" , password);
			if(string_cmp(password , "ADMIN")){
				fscanf(fp , "%s\n" , password);
				printf("Password: %s\n" , password);
				puts("Account Identity: Admin");
			}
			else{
				printf("Password: %s\n" , password);
				puts("Account Identity: Player");
			}
			
			int k;
			fscanf(fp , "%d\n" , &k);
			printf("Exp:%d   Lv.:%d\n" , k , find_lv(k));
			puts("Alphabet:");
			for(int i=0;i<26;i++){
				int num;
				fscanf(fp , "%d\n" , &num);
				printf("%c...%d\n" , 'A' + i,num);
			}
			int a , b , c , d  ,e;
			puts("Hunting part:");
			fscanf(fp , "%d\n%d\n%d\n%d" , &a , &b , &c , &d);
			printf("Attack Power:%d Armor:%d Health:%d\nTotal Monster killed:%d\n" , a , b , c , d);
			fclose(fp);
			puts("Mining Part:");
			strcat(tmp , "_2.txt");
			fp = fopen(tmp , "r");
			for(int i=0;i<type_of_core;i++){
				int k;
				fscanf(fp , "%d\n" , &k);
				printf("%s\t%d\n" , ore_code[i+1] , k);
			}
			fscanf(fp , "%d %d\n%d" , &a , &b , &c);
			fscanf(fp , "%d" , &k);
			printf("Attack: %d\n" , k);
			fscanf(fp , "%d" , &k);
			printf("Health: %d\n" , k);
			fscanf(fp , "%d" , &k);
			printf("Armor: %d\n" , k);
			fscanf(fp , "%d" , &k);
			printf("Quantity: %d\n" , k);
			fclose(fp);
			
			puts("Settings:");
			char setting[1005];
			sprintf(setting , "setting_%s.txt" , player_ac);
			fp = fopen(setting , "r");
			while(fscanf(fp , "%s" , input) != EOF){
				printf("%s\n" , input);
			}
			fclose(fp);
			pause;
			cls;
		}
		else if(choose == 3){
			printf("I am so lazy, therefore I will make it later\n");
			pause;
			cls;
		} 
		else if(choose == 4){
			char name[10005];
			printf("Please input a player username:");
			scanf("%s" , name);
			char s1[10005] , s[10005];
			strcpy(s, name);
			strcpy(s1 , name);
			strcat(s1 , "_2.txt");
			strcat(s , ".txt");
			if(access(s , F_OK) == -1){
				printf("No such player\n");
				pause;
				cls;
				continue;
			}
			int k = remove(s);
			int p = remove(s1);
			if(k == 0 && p == 0){
				FILE *fp;
				fp = fopen("player_list.txt" , "r");
				char lt[10000][105];
				int i = 0;
				while(fscanf(fp , "%s\n" , lt[i]) != EOF){
					if(strcmp(lt[i] , name) != 0){
						i++;
					}
				}
				fclose(fp);
				fp = fopen("player_list.txt" , "w");
				while(i--){
					fprintf(fp , "%s\n" , lt[i]);
				}
				fclose(fp);
				printf("Delete Success\n");
			}
			else{
				printf("Error\n");
			}
			pause;
			cls;
		}
	}while(choose != 5);
	puts("Bye...");
	Sleep(500);
	cls;
}



PLAYER exp_up(PLAYER player , int v){
	int a = find_lv(player.lv);
	player.lv += v;
	int b = find_lv(player.lv);
	player.defence += max(b - a , 0)*10;
	player.health += max(b - a , 0) * 30;
	player.weapon_val += max(b - a , 0) * 20;
	return player;
}

bool string_cmp(char s1[] , char s2[]){
	int len , i;
	int l1 = strlen(s1);
	int l2 = strlen(s2);
	if(l1 > l2){
		len = l2;
	}
	else{
		len = l1;
	}
	for(i=0;i<len;i++){
		if(s1[i] != s2[i]){
			return false;
		}
	}
	if(l1 > l2){
		int l = l1;
		for(;i<l;i++){
			if(s1[i] != ' ' && s1[i] != '\0' && s1[i] != '\n'){
				return false;
			}
		}
	}
	else{
		int l = l2;
		for(;i<l;i++){
			if(s2[i] != ' ' && s2[i] != '\0' && s2[i] != '\n'){
				return false;
			}
		}
	}
	return true;
}

bool check_admin(char s[]){
	return admin_checked;
}

void register_f(){
	char user_name[100];
	char password[100];
	char file_name[105];
	char password_sec[105];
	int i;
	
	printf("please enter you user name(only record the first 100 characters):\n");
	string_input(user_name , 100 , false);
	sprintf(file_name , "%s.txt" , user_name);
	
	if(fopen(file_name , "r")){
		printf("user already registered!!\n");
		pause;
		return ;
	}
	
	FILE * file = fopen(file_name , "w");
	printf("please enter your password(only record the first 100 characters):\n");
	string_input(password , 100 , false);
	fprintf(file , "%s\n0\n" , password);
	for(i=0;i<26;i++){
		fprintf(file,"0\n");
	}
	fprintf(file , "10\n10\n100\n0\n0"); //AD defence health total_monster_kill monster_counting
	fclose(file);
	printf("Done\n");
	Sleep(500);
	char filename2[1000];
	sprintf(filename2 , "%s_2.txt" , user_name);
	file = fopen(filename2 , "w");
	for(int i=0;i<type_of_core;i++){
		fprintf(file , "%d " , ore_inventory[i + 1]);
	}
	fprintf(file , "\n1 1\n10\n0 0 0 0"); //the first 3 numbers are for mission sys , the next 4 is for skill point
	fclose(file);
	FILE *fp;
	fp = fopen("player_list.txt" , "a");
	fprintf(fp , "\n%s" , user_name);
	fclose(fp);
	
	char setting[1005];
	sprintf(setting , "setting_%s.txt" , user_name);
	fp = fopen(setting , "w");
	fprintf(fp , "main_character_skin=@\nboundary_skin=#\ntext_color=255\nUp=w\nDown=s\nLeft=a\nRight=d\n");
	fclose(fp);
}

bool login(){
	char user_name[100];
	char file_name[120];
	char password[100];
	int i;
	
	printf("please enter your user name:\n");
	string_input(user_name , 100 , false);
	sprintf(file_name , "%s.txt" , user_name);
	
	
	
	if(!fopen(file_name , "r")){
		printf("No such user\n");
		Sleep(500);
		return false;
	}
	
	
	
	printf("please enter your password:\n");
	string_input(password , 100 , true);
	
	
	
	FILE *file = fopen(file_name , "r");
	char comfirm_password[105];
	fgets(comfirm_password , 100 , file);
	
	if(string_cmp(comfirm_password , "ADMIN")){
		fgets(comfirm_password , 100 , file);
		admin_checked = true;
	}
	
	
	if(!string_cmp(password , comfirm_password)){
		admin_checked = false;
		printf("Wrong password!!\n");
		Sleep(500);
		return false;
	}
	
	
	
	printf("Login success!!\n");
	strcpy(player_ac , user_name);
	/*printf("%s" , player_ac);
	pause;*/
	fscanf(file , "\n");
	fscanf(file , "%d\n" , &coop.lv);
	for(i=0;i<26;i++){
		fscanf(file , "%d" , &alp[i]);
	}
	fscanf(file , "\n%d\n%d\n%d\n%d\n%d" , &coop.weapon_val , &coop.defence , &coop.health , &total_monster_kill , &monster_counting);
	fclose(file);
	char filename2[1000];
	sprintf(filename2 , "%s_2.txt" , user_name);
	file = fopen(filename2 , "r");
	for(int i=0;i<type_of_core;i++){
		fscanf(file , "%d" , &ore_inventory[i + 1]);
	}
	int a;
	for(int i=0;i<3;i++) fscanf(file , "%d" , &a);
	fscanf(file , "%d %d %d %d" , &skillpoint[1] , &skillpoint[2] , &skillpoint[3] , &skillpoint[4]);
	fclose(file);
	Sleep(500);
	
	char setting[2005];
	sprintf(setting , "setting_%s.txt" , user_name);
	file = fopen(setting , "r");
	fscanf(file , "main_character_skin=%c\n" , &main_character);
	fscanf(file , "boundary_skin=%c\n" , &boundary_skin);
	fscanf(file , "text_color=%d\n" , &text_color);
	fscanf(file , "Up=%c\n" , &up);
	fscanf(file , "Down=%c\n" , &down);
	fscanf(file , "Left=%c\n" , &left);
	fscanf(file , "Right=%c\n" , &right);
	fclose(file);
	return true;
}

bool login_class(){
	char cmd;
	int selector = 1;
	int i;
	printf("\tStone Studio\n");
	printf("\tpress enter to comfirm\n\tpress w to move upward and s for downward\n");
	printf("\t\tlogin\n");
	printf("\t\tregister\n");
	do{
		
		if(selector == 1){
			gotoxy(25 , 4);
			putchar(' ');
			gotoxy(25 , 3);
			putchar('<');
		}
		else{
			gotoxy(25 , 3);
			putchar(' ');
			gotoxy(25 , 4);
			putchar('<');
		}
		cmd = getch();
		if(cmd == 'w' || cmd == 'W'){
			if(selector == 2){
				selector = 1;
			}
		}
		else if(cmd == 's' || cmd == 'S'){
			if(selector == 1){
				selector = 2;
			}
		}
		else if(cmd == 'l'){
			strcpy(player_ac , "test");
			for(i=0;i<26;i++){
				alp[i] = 0;
			}
			coop.weapon_val = 10;
			coop.health = 100;
			coop.lv = 100;
			return true;
		}
	}while(cmd!='\r');
	cls;
	bool pass = false;
	
	switch(selector){
		case 1:
			pass = login();
			break;
		case 2:
			register_f();
			break;
	}
	return pass;
}


int ore_lv_chooser(int a){
	if(a <= 6000){
		return 1;
	}
	else if(a <= 9000){
		return 2;
	}
	else if(a <= 10000){
		return 3;
	}
}

MAP map_driver(int code , PLAYER player){
	srand(time(NULL));
	mode = code;
	//cls;
	int i , j , lim_i , lim_j , input;
	char map_name[100] , trash;
	sprintf(map_name , "Map_%d.txt" , code);
	FILE * file;
	file = fopen(map_name , "r");
	
	fscanf(file , "%d %d %d %d\n" , &map_lower_x , &map_lower_y , &lim_i , &lim_j); 
	for(i = 0 ; i<lim_i ; i++){
		for(j=0;j<lim_j ; j++){
			fscanf(file , "%c" , &play_map[i][j]);
			if(code == 2 && play_map[i][j] == ' ' && rand()%100 + 1 >= 95){ //the chance for generating core in map is 10%
				play_map[i][j] = '.';
				do{
					core_lv_divide[i][j] = ore_lv_chooser(rand()%10000 + 1);
				}while(core_lv_divide[i][j] > type_of_core);
				  //the lv. of core will never higher than player lv. 
			}
			else{
				core_lv_divide[i][j] = 0;
			}
			if(play_map[i][j] == '#'){
				play_map[i][j] = boundary_skin;
			}
		}
		fscanf(file , "\n");
	}
	fclose(file);
	/*for(i = 0 ; i<lim_i ; i++){
		for(j=0;j<lim_j ; j++){
			printf("%d" , core_lv_divide[i][j]);
		}
		printf("\n");
	}
	pause;
	cls;*/
	MAP re;
	re.x = lim_i;
	re.y = lim_j;
	switch(code){
		case 2:
			printf("Loading to Mining area...\n");
			Sleep(500);
			//cls;
			break;
		case 3:
			printf("Loading to Hunting area...\n");
			Sleep(500);
			//cls;
			break;
	}
	strcpy(play_map[lim_i+1] , "T: Command Prompt");
	strcpy(play_map[lim_i+2] , "P: Save the game");
	strcpy(play_map[lim_i+3] , "I: Open player information");
	strcpy(play_map[lim_i+4] , "M: Mission System");
	strcpy(play_map[lim_i+5] , "O: Skill Points");
	strcpy(play_map[lim_i+6] , "Q: Setting");
	re.x+=7;
	return re;
}

void map_runner(MAP information , PLAYER player){
	cls;
	int i , j;
	for(i = 0 ;i < information.x;i++){
		for(j = 0 ; j < information.y;j++){
			if(j == player.x && i == player.y){
				putchar(main_character);
			}
			else{
				printf("%c" , play_map[i][j]);
			}
		}
		printf("\n");
	}
}

void open_inventory(){
	int i;
	char Alapbet = 'A';
	for(i=0;i<13;i++){
		printf("%c:%d   " , Alapbet , alp[i]);
		Alapbet++;
	}
	printf("\n");
	for(i=13;i<26;i++){
		printf("%c:%d   " , Alapbet , alp[i]);
		Alapbet++;
	}
	printf("\n");
	for(int i=0;i<type_of_core;i++){
		printf("%s:%d\n" , ore_code[i + 1] , ore_inventory[i + 1]);
	}
	pause;
}

void updater(PLAYER player){
	int i;
	char file_name[1000];
	char password[1000];
	sprintf(file_name , "%s.txt" , player_ac);
	FILE *file;
	file = fopen(file_name , "r");
	fgets(password , 100 , file);
	if(admin_checked){
		fgets(password , 100 , file);
	}
	fclose(file);
	file = fopen(file_name , "w");
	if(admin_checked){
		fprintf(file , "ADMIN\n");
	}
	fprintf(file , "%s%d\n" , password , player.lv);
	for(i=0;i<26;i++){
		fprintf(file , "%d\n" , alp[i]);
	}
	fprintf(file , "%d\n%d\n%d\n%d\n%d" , player.weapon_val , player.defence , player.health , total_monster_kill , monster_counting);
	fclose(file);
	char filename2[1000];
	sprintf(filename2 , "%s_2.txt" , player_ac);
	file = fopen(filename2 , "r");
	for(int i=0;i<type_of_core;i++){
		int a;
		fscanf(file , "%d" , &a);
	}
	int a , b , c;
	fscanf(file , "%d %d\n%d" , &a , &b , &c);
	fclose(file);
	file = fopen(filename2 , "w");
	for(int i=0;i<type_of_core;i++){
		fprintf(file , "%d " , ore_inventory[i + 1]);
	}
	fprintf(file , "\n%d %d\n%d\n" , a , b , c);
	for(int i=1;i<=4;i++) fprintf(file , "%d " , skillpoint[i]);
	fclose(file);
	
	char str[1000];
	sprintf(str , "setting_%s.txt" , player_ac);
	file = fopen(str , "w");
	fprintf(file , "main_character_skin=%c\n" , main_character);
	fprintf(file , "boundary_skin=%c\n" , boundary_skin);
	fprintf(file , "text_color=%d\n" , text_color);
	fprintf(file , "Up=%c\nDown=%c\nLeft=%c\nRight=%c" , up , down , left , right);
	fclose(file);
	return ;
}

int choose_area(){
	char cmd;
	int select = 1;
	do{
		cls;
		if(select == 1){
			printf("\t\tMining Area<\n");
			printf("\t\tHunting Area\n");
		}
		else{
			printf("\t\tMining Area\n");
			printf("\t\tHunting Area<\n");
		}
		cmd = getch();
		if(cmd == 'W' || cmd == 'w'){
			select = 1;
		}
		else if(cmd == 'S' || cmd == 's'){
			select = 2;
		}
	}while(cmd != '\r' && cmd != ESC);
	if(cmd == ESC){
		return -99999;
	}
	return select;
}


void command_promte(PLAYER *player){
	char cmd[1000] , input;
	int quantity , i;
	printf("Command Prompt:\n");
	do{
		scanf("%s" , cmd);
		if(strcmp(cmd , "inventory") == 0){
			scanf("%s" , cmd);
			if(strcmp(cmd , "alp") == 0){
				scanf(" %c %d" , &input , &quantity);
				//printf("%c\n" , input);
				if('A' <= input && input <= 'Z'){
					alp[input - 'A']+=quantity;
					printf("Done\n");
				}
				else{
					printf("out of bound\n");
				}
			}
			else if(strcmp(cmd , "ore")== 0){
				char k[1005];
				int i;
				scanf("%s %d" , k , &quantity);
				strcat(k , " Ore");
				for(i=1;i<=type_of_core;i++){
					if(strcmp(k , ore_code[i]) == 0){
						break;
					}
				}
				if(i > type_of_core){
					printf("No such ore\n");
				}
				else{
					ore_inventory[i]+=quantity;
					puts("Done");
					
				}
			}
			else{
				printf("No such class\n");
			}
		}
		else if(strcmp(cmd , "color") == 0){
			int k;
			scanf("%d" , &k);
			if(1 <= k && k <= 255){
				text_color = k;
				color(k);
				printf("Done\n");
			}
			else{
				printf("out of bound\n");
			}
		}
		else if(strcmp(cmd , "admin_page") == 0){
			admin_page();
			printf("Command Prompt:\n");
		}
		else if(strcmp(cmd , "update") == 0){
			updater(*player);
			printf("Done\n");
		}
		else if(strcmp(cmd , "EGG") == 0){
			int input;
			scanf(" %d" , &input);
			if(input == 1317795){
				printf("You find the Easter egg...\n");
				//the animation will be make
			}
		}
		else if(strcmp(cmd , "exp_up") == 0){
			int exp;
			scanf(" %d" , &exp); 
			*player = exp_up(*player , exp);
			printf("Done\n");
		}
		else if(strcmp(cmd , "detail") == 0){
			PLAYER temp = *player;
			int a = temp.health;
			int b = temp.weapon_val;
			int c = temp.lv;
			int d = temp.defence;
			printf("Health:%d Attack:%d Armor:%d\nEXP:%d Lv.%d\nTotal Monster Killed:%d" , a , b , d , c  ,find_lv(c) , total_monster_kill);
		}
		else if(strcmp(cmd , "re-gen") == 0){
			int input;
			map_driver(mode , *player);
			printf("Done\n");
		}
		else if(strcmp(cmd , "ore_detail") == 0){
			char c[10000];
			scanf("%s" , c);
			if(strcmp(c , "divide") == 0){
				for(int i=0;i<20;i++){
					for(int j=0;j<100;j++){
						printf("%d" , core_lv_divide[i][j]);
					}
					printf("\n");
				}
				printf("Done\n");
			}
			else if(strcmp(c , "full_data") == 0){
				int checking[100] = {0,};
				for(int i=0;i<20;i++){
					for(int j=0;j<100;j++){
						checking[core_lv_divide[i][j]]++;
					}
				}
				for(int i=0;i<=max_level;i++){
					printf("%d:%d " , i , checking[i]);
				}
				printf("\nDone\n");
			}
			else{
				printf("Unknown Command\n");
			}
		}
		else if(strcmp(cmd , "end" ) == 0){
			printf("hard end game");
			exit(0);
		} 
		else if(strcmp(cmd , "ore_code") == 0){
			int k;
			scanf("%d" , &k);
			if(k <= type_of_core){
				printf("%s\nDone\n" , ore_code[k]);
			}
			else {
				printf("Out of bound...\n");
			}
		}
		else if(strcmp(cmd , "open_inventory") == 0){
			open_inventory();
			printf("Done\n");
		}
		else{
			printf("Unkown Command\n");
		}
	}while(!string_cmp(cmd , "exit"));
}


CI new_combat(){
	int i , j;
	for(i=0;i<=15;i++){
		gotoxy(10 , 10 + i);
		for(j=0;j<=30;j++){
			if(i == 0 || i == 15 || j == 0 || j == 30){
				printf("#");
			}
			else{
				printf(" ");
			}
		}
		printf("\n");
	}
	
	int randnum = rand()%10 + 3;
	int escap_map[100][100] = {0,};
	//printf("%d\n" , randnum);
	for(int i=0;i<randnum;i++){
		int y = rand()%8 + 1;
		int x = rand()%28 + 1;
		//printf("%d %d\n" , x , y);
		escap_map[y][x] = 1;
		if(x != 1){
			escap_map[y][x - 1] = 1;
		}
		else{
			escap_map[y - 1][x - 1] = 1;
		}
	}
	//pause;
	for(i=1;i<10;i++){
		gotoxy(11 , 11 + i);
		for(int j=1;j<30;j++){
			if(escap_map[i][j]){  //if escap_map[i][j] == 1
				putchar(' ');
			}
			else{  //if escap_map[i][j] == 0
				putchar('*');
			}
		}
	}
	Sleep(3000);
	//pause;
	for(i=1;i<10;i++){
		gotoxy(11 , 11 + i);
		for(int j=1;j<30;j++){
			putchar(' ');
		}
	}
	int x = rand()%28 + 1 , y = rand()%8 + 1;
	x+=11;
	y+=11;
	gotoxy(x , y);
	putchar('@');
	
	clock_t start = clock();
	clock_t mark;
	do{
		if(!kbhit()) continue;
		char cmd;
		cmd = getch();
		cmd = tolower(cmd);
		if(cmd == 'w' && y > 12){
			gotoxy(x , y);
			putchar(' ');
			y--;
			gotoxy(x , y);
			putchar('@');
		}
		else if(cmd == 'a' && x > 11){
			gotoxy(x , y);
			putchar(' ');
			x--;
			gotoxy(x , y);
			putchar('@');
		}
		else if(cmd == 's' && y < 20){
			gotoxy(x , y);
			putchar(' ');
			y++;
			gotoxy(x , y);
			putchar('@');
		}
		else if(cmd == 'd' && x < 39){
			gotoxy(x , y);
			putchar(' ');
			x++;
			gotoxy(x , y);
			putchar('@');
		}
		if(escap_map[y - 11][x - 11] == 1){
			mark = clock();
		}
	}while((clock() - start)/CLOCKS_PER_SEC <= 2);
	CI re;
	re.time_spend = (mark - start)/CLOCKS_PER_SEC; 
	cls;
	x-=11;
	y-=11;
	if(escap_map[y][x] == 0){
		re.hurt = true;
		return re;
	}
	else{
		re.hurt = false;
		return re;
	}
}


int new_mining(){
	
	int height[1005] = {0,};
	
	int have[100][100] = {0,};
	for(int i=0;i<=30;i++){
		height[i] = rand()%10 + 5;
	}
	
	for(int i=0;i<30;i++){
		gotoxy(50 , 10 + i);
		for(int j=0;j<=30;j++){
			if(i == 0 || i == 29 || j == 0 || j == 30){
				putchar(boundary_skin);
			}
			else if(30 - i <= height[j]){
				putchar('*');
				have[i][j] = 1;
			}
			else{
				putchar(' ');
			}
		}
		putchar('\n');
	}
	
	int sum = 0;
	int x = 65, y = 11;
	for(int i=0;i<5;i++){
		gotoxy(60 , 6);
		printf("Round: %d" , i + 1);
		long double dur = 1e-18;
		clock_t start = clock();
		gotoxy(x , y);
		putchar('&');
		while(have[y - 10][x - 50] == 0 && y < 39){
			if(kbhit()){
				int prev = x;
				char c = getch();
				c = tolower(c);
				if(c == 'a' && x > 52){
					x--;
				}
				else if(c == 'd' && x < 78){
					x++;
				}
				else if(c == 's'){
					y++;
				}
				if(prev != x){
					gotoxy(prev , y);
					putchar(' ');
					if((clock() - start)/CLOCKS_PER_SEC >= dur){
						start = clock();
						y++;
					}
					gotoxy(x , y);
					putchar('&'); 
					
				}
			}
			else if((clock() - start)/CLOCKS_PER_SEC >= dur){
				gotoxy(x , y);
				putchar(' ');
				y++;
				gotoxy(x , y);
				putchar('&'); 
				start = clock();
			}
		}
		
		gotoxy(x , y);
		putchar(' ');
		if(x - 1 > 52){
			gotoxy(x - 1, y);
			putchar(' ');
			gotoxy(x - 1, y - 1);
			putchar(' ');
			if(y + 1 < 38){
				gotoxy(x - 1, y + 1);
			putchar(' ');
			}
		}
		if(x + 1 < 78){
			gotoxy(x + 1, y);
			putchar(' ');
			gotoxy(x + 1, y - 1);
			putchar(' ');
			if(y + 1 < 38){
				gotoxy(x + 1, y + 1);
				putchar(' ');
			}
		}
		if(y + 1 < 38){
			gotoxy(x , y + 1);
			putchar(' ');
		}
		gotoxy(x , y - 1);
		putchar(' ');
		
		y = y - 10;
		x = x - 50;
		if(have[y][x] == 1){
			sum++;
			have[y][x] = 0;
		}	
		if(have[y][x + 1]  == 1){
			sum++;
			have[y][x + 1]  = 0;
		}
		if(have[y][x - 1]  == 1){
			sum++;
			have[y][x - 1] = 0;
		}
		if(have[y - 1][x - 1] == 1){
			sum++;
			have[y - 1][x - 1] = 0;
		}
		if(have[y - 1][x + 1] == 1){
			sum++;
			have[y - 1][x + 1] = 0;
		}
		if(have[y + 1][x] == 1){
			sum++;
			have[y + 1][x] = 0;
		}
		if(have[y - 1][x] == 1){
			sum++;
			have[y - 1][x] = 0;
		}
		if(have[y + 1][x - 1] == 1){
			sum++;
			have[y + 1][x - 1] = 0;
		}
		if(have[y + 1][x + 1] == 1){
			sum++;
			have[y + 1][x + 1] = 0;
		}
		x = 65;
		y = 11;
	}
	return sum;
}

bool getinto_event(PLAYER player){
	srand(time(NULL));
	
	if(mode == 2){  //mining
		int s = new_mining();
		int a = rand()%ore_in_each_lv[core_lv_divide[player.y][player.x]];
		int i;
		cls;
		for(i=1;i<=type_of_core;i++){
			if(strcmp(ore_list[core_lv_divide[player.y][player.x]][a] , ore_code[i]) == 0){
				ore_inventory[i] += s;
				break;
			}
			
		}
		gotoxy(60 ,10);
		printf("You have got %d %s\n" , s , ore_code[i]);
		Sleep(1500);
	}
	else if(mode == 3){
		printf("\t\tpress Y to ready..");
		char cmd;
		do{
			cmd = getch();
		}while(cmd != 'Y' && cmd != 'y');
		cls;
		printf("\t\tStart!!");
		Sleep(100);
		cls;
		int max_str_length = 5;  //max generate string length
		int max_damage = player.health * 0.2; //max damage monster can hit
		int i , j;
		int monster_health = rand()%player.weapon_val * 3 + player.weapon_val * 2;
		
		player.weapon_val+=skillpoint[1]*2;
		player.health += skillpoint[2]*5;
		player.defence += skillpoint[3]*2;
		
		while(monster_health > 0 && player.health > 0){
			CI k;
			k = new_combat();
			if(k.hurt){
				player.health -= max(0 , (rand()%max_damage + 1) - player.defence);
			}
			else{
				int time_spend = (int)(k.time_spend * 10);
				monster_health -= k.time_spend*player.weapon_val;
			}
			cls;
			gotoxy(11 , 10);
			printf("Health:%d" , player.health);
			gotoxy(11 , 11);
			printf("Monster Health:%d" , max(monster_health , 0));
			gotoxy(11 , 12);
			if(k.hurt){
				puts("You have taken damage!!");
			}
			else{
				puts("You hit the monster!!");
			}
			gotoxy(11 , 13);
			pause;
			cls;
		}
		cls;
		bool re = false;
		if(monster_health <= 0){
			printf("The monster is defeated.....\n");
			total_monster_kill++;
			monster_counting++;
			re = true;
		}
		else{
			printf("You are defeated....");
		}
		getch();
		return re;
	}
}


void open_information(PLAYER player){
	printf("\t\t Your information:\n");
	printf("\t\tHealth:%d\n" , player.health);
	printf("\t\tStrength:%d\n" , player.weapon_val);
	printf("\t\tArmor:%d\n" , player.defence);
	printf("\t\tLevel:%d\n" , find_lv(player.lv));
	printf("\t\tEXP:%d\n" , player.lv);
	printf("\t\tTotal monster you have killed:%d\n" , total_monster_kill);
	puts("Your inventory:");
	open_inventory();
	return ;
}

void trading(){
	
	void print_lines(){
		cls;
		printf("\t\tWelcome to the market!!\n");
		printf("Trade Alphabet\n");
		printf("Stay tuned");
		gotoxy(20 , 1);
		putchar('<');
	}
	
	int line = 1;
	int max_line = 2;
	char cmd;
	print_lines();
	do{
		cmd = getch();
		if(cmd == 'W' || cmd == 'w'){
			if(line > 1){
				gotoxy(20 , line);
				putchar(' ');
				line--;
				gotoxy(20 , line);
				putchar('<');
			}
		}
		else if(cmd == 's' || cmd == 'S'){
			if(line < max_line){
				gotoxy(20 , line);
				putchar(' ');
				line++;
				gotoxy(20 , line);
				putchar('<');
			}
		}
		else if(cmd == '\r'){
			if(line == 1){
				cls;
				char input;
				printf("Please typing what alphabet you want to trade:");
				scanf(" %c" , &input);
				input = tolower(input);
				if('a' <= input && input < 'z'){
					cls;
					int in;
					printf("How many you want to trade:");
					scanf("%d" , &in);
					if(alp[input - 'a' - 1] >= 100 * in){
						alp[input - 'a' - 1] -= 100 * in;
						alp[input - 'a' ]+=in;
					}
					else{
						printf("You don't have enough alphabet..");
						Sleep(500);
					}
				}
				else if(input == ESC){
					break;
				}
				else{
					cls;
					printf("Out of bound..");
					Sleep(500);
				}
			}
			print_lines();
		}
	}while(cmd != ESC);
	cls;
	printf("Bye..");
	Sleep(500);
	return;
}

void mission(PLAYER player){
	FILE *fp;
	char filename[1005];
	strcpy(filename , player_ac);
	strcat(filename , "_2.txt");
	fp = fopen(filename , "r");
	for(int i=0;i<type_of_core;i++){
		int a;
		fscanf(fp , "%d" , &a);
	}
	int quantity , input_ore_code;
	int has_kill , need_to_kill;
	char cmd;
	fscanf(fp , "%d %d\n" , &quantity , &input_ore_code);
	fscanf(fp , "%d" , &need_to_kill);
	fclose(fp);
	printf("Press ESC to exit mission system , Use A or D to select Yes or No , Press W and S to choose misssion , Press enter to comfirm selection\n");
	printf("You are required to submit %d %s , Process:%d/%d\n" , quantity , ore_code[input_ore_code] , ore_inventory[input_ore_code] , quantity);
	printf("Yes<  No");
	printf("\nYou are required to kill %d monsters , Process:%d/%d\n" , need_to_kill , monster_counting , need_to_kill);
	printf("Yes   No");
	int line = 1;
	int choice = 1;
	int pre_choice = 1;
	int pre_line = 1;
	do{
		cmd = getch();
		cmd = tolower(cmd);
		pre_choice = choice;
		pre_line = line;
		if(cmd == 'a'){
			
			choice = 1;
		}
		else if(cmd == 'd'){
			choice = 2;
		}
		else if(cmd == 's'){
			line = 2;
		}
		else if(cmd == 'w'){
			line = 1;
		} 
		else if(cmd == ESC){
			return;
		}
		if(pre_choice == 1){
			if(pre_line == 1){
				gotoxy(3 , 2);
			}
			else{
				gotoxy(3 , 4);
			}
		}
		else{
			if(pre_line == 1){
				gotoxy(8 , 2);
			}
			else{
				gotoxy(8 , 4);
			}
		}
		putchar(' ');
		if(choice == 1){
			if(line == 1){
				gotoxy(3 , 2);
			}
			else{
				gotoxy(3 , 4);
			}
		}
		else{
			if(line == 1){
				gotoxy(8 , 2);
			}
			else{
				gotoxy(8 , 4);
			}
		}
		putchar('<');
	}while(cmd!='\r');
	if(choice == 2){
		cls;
		puts("Bye...");
		Sleep(1000);
		return;
	}
	if(line == 1){  //mining ore
		if(ore_inventory[input_ore_code] >= quantity){
			ore_inventory[input_ore_code] -= quantity;
		}	
		else{
			cls;
			printf("You don't have enough ore...");
			Sleep(1000);
			return ;
		}
		int new_quty = rand()%10 + 1;
		int new_code = rand()%type_of_core + 1;
		fp = fopen(filename , "w");
		for(int i=1;i<=type_of_core;i++){
			fprintf(fp , "%d " , ore_inventory[i]);
		}
		fprintf(fp , "\n%d %d\n%d" , new_quty , new_code , need_to_kill);
		fclose(fp);
		cls;
		alp[0]+=10;
		updater(player);
		puts("Submitted");
		pause;
	}
	else{  // kill monster
		if(monster_counting >= need_to_kill){
			monster_counting -= need_to_kill;
			FILE * fp;
			fp = fopen(filename , "w");
			for(int i=1;i<=type_of_core;i++){
				fprintf(fp , "%d " , ore_inventory[i]);
			}	
			fprintf(fp , "\n%d %d\n%d" , quantity , input_ore_code , rand()%10 + 1);
			fclose(fp);
			alp[0]+=10;
			updater(player);
			puts("Submitted");
			pause;
		}
		else{
			cls;
			puts("You havn't kill enough monster...");
			pause;
		}
	}
}



void skill_point(PLAYER player){
	int lv = find_lv(player.lv);
	printf("Skill point remain:\t%d\n" , lv - skillpoint[1] - skillpoint[2] - skillpoint[3] - skillpoint[4]);
	printf("Attack:\t%d\n" , skillpoint[1]);
	printf("Health:\t%d\n" , skillpoint[2]);
	printf("Armor:\t%d\n" , skillpoint[3]);
	printf("Reward:\t%d\n" , skillpoint[4]);
	printf("Leave");
	gotoxy(20 , 1);
	putchar('<');
	int line = 1;
	while(1){
		char cmd;
		cmd = getch();
		cmd = tolower(cmd);
		if(cmd == 's' && line < 5){
			gotoxy(20, line);
			putchar(' ');
			line++;
			gotoxy(20, line);
			putchar('<');
		}
		else if(cmd == 'w' && line > 1){
			gotoxy(20, line);
			putchar(' ');
			line--;
			gotoxy(20, line);
			putchar('<');
		}
		else if(cmd == 'a'){
			skillpoint[line] = max(skillpoint[line] - 1 , 0);
			gotoxy(8 , line);
			for(int i=0;i<20;i++) putchar(' ');
			gotoxy(8 , line);
			printf("%d" , skillpoint[line]);
			gotoxy(24 , 0);
			for(int i=0;i<20;i++) putchar(' ');
			gotoxy(24 , 0);
			printf("%d" , lv - skillpoint[1] - skillpoint[2] - skillpoint[3] - skillpoint[4]);
			gotoxy(20 , line);
			putchar('<');
		}
		else if(cmd == 'd'){
			skillpoint[line]++;
			if(skillpoint[1] + skillpoint[2] + skillpoint[3] + skillpoint[4] > lv){
				skillpoint[line]--;
			}
			gotoxy(8 , line);
			for(int i=0;i<20;i++) putchar(' ');
			gotoxy(8 , line);
			printf("%d" , skillpoint[line]);
			gotoxy(24 , 0);
			for(int i=0;i<20;i++) putchar(' ');
			gotoxy(24 , 0);
			printf("%d" , lv - skillpoint[1] - skillpoint[2] - skillpoint[3] - skillpoint[4]);
			gotoxy(20 , line);
			putchar('<');
		}
		else if(cmd == '\r' && line == 5){
			cls;
			printf("Bye...");
			Sleep(1000);
			break;
		}
	}
}

void setting(){
	int x = 30, y = 0;
	char c;
	void ps(){
		printf("Main Character Skin:    %c\n" , main_character);
		printf("Boundary Skin:          %c\n" , boundary_skin);
		printf("Up:                     %c\n" , up );
		printf("Down:                   %c\n" , down);
		printf("Left:                   %c\n" , left);
		printf("Right:                  %c\n" , right);
		printf("Change Password");
	}
	ps();
	do{
		
		gotoxy(x , y);
		putchar('<');
		c = getch();
		c = tolower(c);
		if(c == 's' && y < 6){
			gotoxy(x , y);
			putchar(' ');
			y++;
		}
		else if(c == 'w' && y > 0){
			gotoxy(x , y);
			putchar(' ');
			y--;
		}
		else if(c == '\r'){
			if(y < 6){
				c = getch();
				c = tolower(c);
				if(c != 'q' && c != 't' && c != 'i' && c != 'm' && c != 'o' &&  c != 'p'){
					gotoxy(24 , y);
					putchar(' ');
					gotoxy(24 , y);
					putchar(c);
					switch(y){
						case 0:
							main_character = c;
							break;
						case 1:
							boundary_skin = c;
							break;
						case 2:
							up = c;
							break;
						case 3:
							down = c;
							break;
						case 4:
							left = c;
							break;
						case 5:
							right = c;
							break;
					}
				}
				
			}
			else if(y == 6){
				cls;
				char new_password[1001];
				printf("Please enter your new password:");
				string_input(new_password , 1000 , false);
				if(!string_cmp("GoToHomePage_Code:100" , new_password)){
					int i;
					char str[1000];
					char tmp[1000][1000];
					sprintf(str , "%s.txt" , player_ac);
					FILE *fp;
					fp = fopen(str , "r");
					for(i=0;fscanf(fp , "%s\n" , tmp[i]) != EOF;){
						i++;
					}
					fclose(fp);
					fp = fopen(str , "w");
					if(check_admin(player_ac)){
						strcpy(tmp[1] , new_password);
					}
					else{
						strcpy(tmp[0] , new_password);
					}
					for(int j=0;j<i;j++){
						fprintf(fp , "%s\n" , tmp[j]);
					}
					fclose(fp);
					printf("Done");
					Sleep(1000);
				}
				cls;
				ps();
			}
		}
	}while(c != ESC);
}

int main(){
	
	//initiallize
	srand(time(NULL));
	char consoletitle[200] = "Just Collect";
	SetConsoleTitle((wchar_t*)consoletitle);
	HANDLE buff = GetStdHandle(STD_OUTPUT_HANDLE);
	
	COORD sizeOfBuff;
	sizeOfBuff.X=300;
	sizeOfBuff.Y=300;
	SetConsoleScreenBufferSize(buff,sizeOfBuff);
	HWND hwnd = GetConsoleWindow();
	if( hwnd != NULL ){ SetWindowPos(hwnd ,0,0,0 ,1200,620 ,SWP_SHOWWINDOW|SWP_NOMOVE); }
	
	
	if(fopen("cheatison.txt" , "r")){
		new_mining();
		return 0;
	}
	
	while(!login_class()){
		cls;
	}
	
	color(text_color);
	
	/*********game start!!**********************/
	int i , j;
	int random;
	PLAYER player , saver;
	player = coop;
	player.x = 10;
	player.y = 10;
	int command_code = 1; //normal play
	char move_cmd;
	MAP map_information = map_driver(1 , player); //load map to the 2D array
	map_runner(map_information , player);  // output the map to screen
	
	while(command_code){
		int map_player_x = player.x, map_player_y = player.y;
		bool first_into_map = false;
		
		move_cmd = getch();
		move_cmd = tolower(move_cmd);
		
		random = rand()%random_code + 1;
		
		if(first_into_map){
			first_into_map = false;
		}
		
		/*if(move_cmd == 'm'){
			admin_page();
		}*/
		
		if(move_cmd == up){
			if(play_map[player.y - 1][player.x] == ' '){
				gotoxy(player.x , player.y);
				//Sleep(1000);
				printf(" ");
				//Sleep(500);
				gotoxy(player.x  , player.y - 1);
				putchar(main_character);
				player.y--;
				gotoxy(map_lower_x , map_lower_y);
			}
			else if(play_map[player.y - 1][player.x] == 'A'){
				cls;
				trading();
				map_runner(map_information , player);
			}
			else if(play_map[player.y - 1][player.x] == '.'){
				player.y--;
				cls;
				getinto_event(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y - 1][player.x] == 'C'){  //choosing what area to go
				cls;
				int select = choose_area();
				if(select == 1){  //mining
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(2 , player);
					map_runner(map_information , player);
					first_into_map = true;
				}
				else if(select == 2){  //hunting
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(3 , player);
					map_runner(map_information , player);
					first_into_map = true;
					/*cls;
					printf("Developing..");
					Sleep(500);
					cls;
					map_runner(map_information , player);*/
				}
				else{
					cls;
					map_runner(map_information , player);
				}
			}
			else if(play_map[player.y - 1][player.x] == 'O'){ //going back to main hall
				player.x = saver.x;
				player.y = saver.y;
				map_information = map_driver(1 , player);
				cls;
				map_runner(map_information , player);
				first_into_map = true;
			}
		}
		
		
		else if(move_cmd == down){
			if(play_map[player.y + 1][player.x] == ' '){
				gotoxy(player.x , player.y);
				printf(" ");
				gotoxy(player.x  , player.y + 1);
				putchar(main_character);
				player.y++;
				gotoxy(map_lower_x , map_lower_y);
			}
			else if(play_map[player.y + 1][player.x] == 'A'){
				cls;
				trading();
				map_runner(map_information , player);
			}
			else if(play_map[player.y + 1][player.x] == '.'){
				player.y++;
				cls;
				getinto_event(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y + 1][player.x] == 'C'){  //choosing what area to go
				cls;
				int select = choose_area();
				if(select == 1){  //mining
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(2 , player);
					map_runner(map_information , player);
					first_into_map = true;
				}
				else if(select == 2){  //hunting
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(3 , player);
					map_runner(map_information , player);
					first_into_map = true;
					/*cls;
					printf("Developing..");
					Sleep(500);
					cls;
					map_runner(map_information , player);*/
				}
				else{
					cls;
					map_runner(map_information , player);
				}
			}
			else if(play_map[player.y + 1][player.x] == 'O'){ //going back to main hall
				player.x = saver.x;
				player.y = saver.y;
				map_information = map_driver(1 , player);
				cls;
				map_runner(map_information , player);
				first_into_map = true;
			}
		}
		
		
		else if(move_cmd == left){
			if(play_map[player.y][player.x - 1] == ' '){
				gotoxy(player.x , player.y);
				printf(" ");
				gotoxy(player.x - 1, player.y);
				putchar(main_character);
				player.x--;
				gotoxy(map_lower_x , map_lower_y);
			}
			else if(play_map[player.y][player.x - 1] == 'A'){
				cls;
				trading();
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x - 1] == '.'){
				player.x--;
				cls;
				getinto_event(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x - 1] == 'C'){  //choosing what area to go
				cls;
				int select = choose_area();
				if(select == 1){  //mining
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(2 , player);
					map_runner(map_information , player);
					first_into_map = true;
				}
				else if(select == 2){  //hunting
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(3 , player);
					map_runner(map_information , player);
					first_into_map = true;
					/*cls;
					printf("Developing..");
					Sleep(500);
					cls;
					map_runner(map_information , player);*/
				}
				else{
					cls;
					map_runner(map_information , player);
				}
			}
			else if(play_map[player.y][player.x - 1] == 'O'){ //going back to main hall
				player.x = saver.x;
				player.y = saver.y;
				map_information = map_driver(1 , player);
				cls;
				map_runner(map_information , player);
				/*printf("k");
				pause;*/
				first_into_map = true;
			}
		}
		
		
		else if(move_cmd == right){
			if(play_map[player.y][player.x + 1] == ' '){
				gotoxy(player.x , player.y);
				printf(" ");
				gotoxy(player.x + 1 , player.y);
				putchar(main_character);
				player.x++;
				gotoxy(map_lower_x , map_lower_y);
			}
			else if(play_map[player.y][player.x + 1] == 'A'){
				cls;
				trading();
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x + 1] == '.'){
				player.x++;
				cls;
				getinto_event(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x + 1] == 'C'){  //choosing what area to go
				cls;
				int select = choose_area();
				if(select == 1){  //mining
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(2 , player);
					map_runner(map_information , player);
					first_into_map = true;
				}
				else if(select == 2){  //hunting
					cls;
					saver.x = player.x;
					saver.y = player.y;
					player.x = 10;
					player.y = 10;
					map_information = map_driver(3 , player);
					
					map_runner(map_information , player);
					first_into_map = true;
					/*cls;
					printf("Developing..");
					Sleep(500);
					cls;
					map_runner(map_information , player);*/
				}
				else{
					cls;
					map_runner(map_information , player);
					first_into_map = true;
				}
			}
			else if(play_map[player.y][player.x + 1] == 'O'){ //going back to main hall
				player.x = saver.x;
				player.y = saver.y;
				map_information = map_driver(1 , player);
				cls;
				map_runner(map_information , player);
				first_into_map = true;
			}
		}
		else if(move_cmd == ESC){  //exit
			cls;
			printf("Press Y to save the game and leave the game\n");
			char input = getch();
			if(input == 'y' || input == 'Y'){
				updater(player);
				printf("Saved");
				command_code = 0;
			}
			else{
				cls;
				map_runner(map_information , player);
			}
		}
		else if(move_cmd == 'p' || move_cmd == 'P'){ //update
			updater(player);
			cls;
			printf("\t\tJust updated");
			Sleep(500);
			cls;
			map_runner(map_information , player);
		}
		else if(move_cmd == 't' || move_cmd == 'T'){ //admin account only
			int k = 0;
			cls;
			if(!check_admin(player_ac)){
				printf("\t\tYou don't have premission!!");
				Sleep(1000);
			}
			else{
				command_promte(&player);
			}
			cls;
			map_runner(map_information , player);
		}
		else if(move_cmd == 'I' || move_cmd == 'i'){ //both information and inventory
			cls;
			open_information(player);
			cls;
			map_runner(map_information , player);
		}
		else if(move_cmd == 'M' || move_cmd == 'm'){
			cls;
			mission(player);
			cls;
			map_runner(map_information , player);
		}
		else if(move_cmd == 'O' || move_cmd == 'o'){
			cls;
			skill_point(player);
			cls;
			map_runner(map_information , player);
		}
		else if(move_cmd == 'Q' || move_cmd == 'q'){
			cls;
			setting();
			cls;
			map_runner(map_information , player);
		}
		if(move_cmd == 's' || move_cmd == 'S' || move_cmd == 'W' || move_cmd == 'w' || move_cmd == 'A' || move_cmd == 'a' || move_cmd == 'd' || move_cmd == 'D'){
			if(random >= random_chance && mode == 3 && !first_into_map){
				cls;
				printf("getting into event %d\n" , mode);
				Sleep(500);
				cls;
				/*printf("%d" , mode);
				pause;*/
				bool re = getinto_event(player);
				cls;
				if(re){
					player = exp_up(player , 50 + skillpoint[4] * 5);
				}
				map_runner(map_information , player);
			}
		}
	}
}
