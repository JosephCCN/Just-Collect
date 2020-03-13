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
#include<limits.h>
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
int lv_of_ore[100] = {0 , 1 , 1 , 2 , 3};
int ore_inventory[100];

PLAYER coop;
int global_code;
int total_monster_kill;
int monster_counting;
int skillpoint[5];
char main_character = '@';
char boundary_skin = '#';
int player_health;
int text_color;
char narrow;
char up , down , left , right;
bool admin_checked = false;
bool master_comfirm = false;
bool string_cmp(char* , char* );

/*******struct part end***************/

int alp[26]; //the alpabet
char player_ac[1000];
int map_lower_x , map_lower_y;
int mode = 1;
bool cheatison = false;

void string_input(char *str , int max_len , bool hide){
	memset(str , '\0' , max_len);
	int pt = 0;
	while(1){
		//puts(str);
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
		else if(ch != ' ' && ch != '\b' && ch != '\n' && ch != '\r' && max_len > pt){
			str[pt] = ch;
			pt++;
			if(hide) putchar('*');
			else putchar(ch);
		}
	}
	/*for(int i=0;i<pt;i++){
		re[i] = str[i];
	}*/
}

int find_lv(int v){
	v = v / 100;
	float a = (-1 + sqrt(1 + 8 * v))/2; //quard equation MAX_LV = 6553
	int re = a;
	return re;
}


int admin_page_class(int line){
	printf("Please select:\n");
	printf("Check all players information\n");
	printf("Search player information\n");
	printf("Delete player account\n");
	printf("Add Admin\n");
	printf("Delete Admin\n");
	printf("Exit");
	char cmd;
	int services = 6;
	
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
				if(strcmp(password , "GJSOT") == 0 || strcmp(password , "SGYZKX") == 0){
					//puts("K");
					fscanf(fp , "%s\n" , password);
				}
				fclose(fp);
				//puts(password);
				for(int i=0;i<30 - strlen(name) - strlen(password);i++){
					putchar('.');
				}
				puts(decrypt_str(password));
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
			if(strcmp(password , "GJSOT") == 0){
				fscanf(fp , "%s\n" , password);
				printf("Password: %s\n" , decrypt_str(password) );
				puts("Account Identity: Operator");
			}
			else if(strcmp(password , "SGYZKX") == 0){
				fscanf(fp , "%s\n" , password);
				printf("Password: %s\n" , decrypt_str(password) );
				puts("Account Identity: Administrator");
			}
			else{
				printf("Password: %s\n" , decrypt_str(password) );
				puts("Account Identity: Player");
			}
			
			int k;
			fscanf(fp , "%s\n" , tmp);
			k = decrypt_int(tmp);
			printf("Exp:%d   Lv.:%d\n" , k , find_lv(k));
			puts("Alphabet:");
			for(int i=0;i<26;i++){
				int num;
				fscanf(fp , "%s\n" , tmp);
				num = decrypt_int(tmp);
				printf("%c...%d\n" , 'A' + i,num);
			}
			int a , b , c , d  ,e;
			puts("Hunting part:");
			fscanf(fp , "%s" , tmp);
			a = decrypt_int(tmp);
			fscanf(fp , "%s" , tmp);
			b = decrypt_int(tmp);
			fscanf(fp , "%s" , tmp);
			c = decrypt_int(tmp);
			fscanf(fp , "%s" , tmp);
			d = decrypt_int(tmp);
			fscanf(fp , "%s" , tmp);
			e = decrypt_int(tmp);
			printf("Attack Power:%d Armor:%d Health:%d\nTotal Monster killed:%d\n" , a , b , c , d);
			fclose(fp);
			puts("Mining Part:");
			strcat(tmp , "_2.txt");
			fp = fopen(tmp , "r");
			for(int i=0;i<type_of_core;i++){
				int k;
				fscanf(fp , "%s\n" , tmp);
				printf("%s\t%d\n" , ore_code[i+1] , decrypt_int(tmp));
			}
			for(int i=0;i<3;i++){
				fscanf(fp , "%s" , tmp);
			}
			fscanf(fp , "%s" , tmp);
			k = decrypt_int(tmp);
			printf("Attack: %d\n" , k);
			fscanf(fp , "%s" , tmp);
			k = decrypt_int(tmp);
			printf("Health: %d\n" , k);
			fscanf(fp , "%s" , tmp);
			k = decrypt_int(tmp);
			printf("Armor: %d\n" , k);
			fscanf(fp , "%s" , tmp);
			k = decrypt_int(tmp);
			printf("Reward: %d\n" , k);
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
			char name[10005];
			printf("Please input a player username:");
			scanf("%s" , name);
			if(strcmp(name , player_ac) == 0){
				puts("Cannot be yourself");
				pause;
				cls;
				continue;
			}
			char s1[10005] , s[10005] , s2[10005];
			strcpy(s, name);
			strcpy(s1 , name);
			strcat(s1 , "_2.txt");
			sprintf(s2,"setting_%s.txt" , name);
			strcat(s , ".txt");
			if(access(s , F_OK) == -1){
				printf("No such player\n");
				pause;
				cls;
				continue;
			}
			int k = remove(s);
			int p = remove(s1);
			int g = remove(s2);
			if(k == 0 && p == 0 && g == 0){
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
		else if(choose == 4){
			cls;
			if(!master_comfirm){
				printf("You have no permission to updrage user to admin");
				Sleep(1000);
				cls;
				continue;
			}
			printf("Please input user name:");
			char name[1000];
			char file_name[1000];
			string_input(name , 100 , false);
			sprintf(file_name , "%s.txt" , name);
			if(access(file_name , F_OK) == -1){
				printf("No such user");
				Sleep(1000);
				cls;
			}
			else if(string_cmp(player_ac , name)){
				printf("Cannot be yourself");
				Sleep(1000);
				cls;
			}
			else{
				int i = 0;
				FILE *fp = fopen(file_name , "r");
				char tmp[1000][1000];
				while(fscanf(fp , "%s\n" , tmp[i]) != EOF){
					i++;
				}
				fclose(fp);
				if(strcmp(tmp[0] , "GJSOT") == 0){
					fp = fopen(file_name , "w");
					char t[100] = "MASTER";
					fprintf(fp , "%s\n" , encrypt_str(t));
					for(int j=1;j<i;j++){
						fprintf(fp , "%s\n" , tmp[j]);
					}
					fclose(fp);
					printf("Done");
				}
				else if(strcmp(tmp[0] , "SGYZKX") == 0){
					printf("This user is the Master.");
				}
				else{
					fp = fopen(file_name , "w");
					char t[100] = "ADMIN";
					fprintf(fp , "%s\n" , encrypt_str(t));
					for(int j=0;j<i;j++){
						fprintf(fp , "%s\n" , tmp[j]);
					}
					fclose(fp);
					printf("Done");
				}
				Sleep(1000);
				cls;
			}
		}
		else if(choose == 5){
			cls;
			if(!master_comfirm){
				printf("You have no permission to degrade user to admin");
				Sleep(1000);
				cls;
				continue;
			}
			char str[1000];
			printf("Please input admin account:");
			string_input(str , 100 , false);
			char filename[1000];
			sprintf(filename , "%s.txt" , str);
			if(access(filename , F_OK) == -1){
				printf("No such user");
				Sleep(1000);
				cls;
			}
			else if(string_cmp(player_ac , str)){
				printf("Cannot be yourself");
				Sleep(1000);
				cls;
			}
			else{
				FILE *fp = fopen(filename , "r");
				int i = 0;
				char tmp[1000][1000];
				while(fscanf(fp , "%s\n" , tmp[i]) != EOF){
					i++;
				}
				fclose(fp);
				if(strcmp(tmp[0] , "GJSOT") == 0){
					fp = fopen(filename , "w");
					for(int j = 1;j<i;j++){
						fprintf(fp , "%s\n" , tmp[j]);
					}
					fclose(fp);
					printf("Done");
					Sleep(1000);
				}
				else if(strcmp(tmp[0] , "SGYZKX") == 0){
					fp = fopen(filename , "w");
					char t[100] = "ADMIN";
					fprintf(fp , "%s\n" , encrypt_str(t));
					for(int j = 1;j<i;j++){
						fprintf(fp , "%s\n" , tmp[j]);
					}
					fclose(fp);
					printf("Done");
					Sleep(1000);
				}
				else{
					printf("This is not an admin account");
					Sleep(1000);
				}
				cls;
			}
		}
	}while(choose != 6);
	puts("Bye...");
	Sleep(500);
	cls;
}



PLAYER exp_up(PLAYER player , int v){
	int a = find_lv(player.lv);
	player.lv += v;
	player.lv = min(INT_MAX , player.lv);
	player.lv = max(0 , player.lv);
	
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
	//printf("%d" , l1);
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
				//printf("%c" , s2[i]);
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
	if(string_cmp(user_name , "GoToHomePage_Code:100")){
		return;
	}
	sprintf(file_name , "%s.txt" , user_name);
	
	if(access( file_name, F_OK ) != -1 ){
		printf("user already registered!!\n");
		pause;
		return ;
	}
	
	FILE * file = fopen(file_name , "w");
	printf("please enter your password(only record the first 100 characters):\n");
	string_input(password , 100 , false);
	//printf("%s" , password);
	if(string_cmp(password , "GoToHomePage_Code:100")){
		return;
	}
	fprintf(file , "%s\n%s\n" , encrypt_str(password) , encrypt_int(0));
	for(i=0;i<26;i++){
		fprintf(file,"%s\n" , encrypt_int(0));
	}
	fprintf(file , "%s\n%s\n%s\n%s\n%s" , encrypt_int(10) , encrypt_int(10) , encrypt_int(100) , encrypt_int(0) , encrypt_int(0)); //AD defence health total_monster_kill monster_counting
	fclose(file);
	printf("Done\n");
	Sleep(500);
	char filename2[1000];
	sprintf(filename2 , "%s_2.txt" , user_name);
	file = fopen(filename2 , "w");
	for(int i=0;i<type_of_core;i++){
		fprintf(file , "%s\n" , encrypt_int(0));
	}
	fprintf(file , "%s\n%s\n%s\n%s\n%s\n%s\n%s" , encrypt_int(1) , encrypt_int(1) , encrypt_int(10) , encrypt_int(0) , encrypt_int(0) , encrypt_int(0) , encrypt_int(0)); //the first 3 numbers are for mission sys , the next 4 is for skill point
	fclose(file);
	FILE *fp;
	fp = fopen("player_list.txt" , "a");
	fprintf(fp , "%s\n" , user_name);
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
	fscanf(file , "%s\n" , comfirm_password);
	//puts(comfirm_password);
	if(strcmp(comfirm_password , "GJSOT") == 0){
		fscanf(file , "%s\n" , comfirm_password);
		admin_checked = true;
	}
	else if(strcmp(comfirm_password , "SGYZKX") == 0){
		//puts(decrypt_str(comfirm_password));
		admin_checked = true;
		master_comfirm = true;
		fscanf(file , "%s\n" , comfirm_password);
	}
	//puts(comfirm_password);
	//getch();
	if(!string_cmp(decrypt_str(comfirm_password) , comfirm_password)){
		admin_checked = false;
		printf("Wrong password!!\n");
		Sleep(500);
		return false;
	}
	
	
	
	printf("Login success!!\n");
	strcpy(player_ac , user_name);
	char tmp[2005] = {0,};
	/*printf("%s" , player_ac);
	pause;*/
	fscanf(file , "\n");
	fscanf(file , "%s\n" , tmp);
	coop.lv = decrypt_int(tmp);
	for(i=0;i<26;i++){
		fscanf(file , "%s" , tmp);
		alp[i] = decrypt_int(tmp);
	}
	fscanf(file , "\n%s" , &tmp);
	coop.weapon_val = decrypt_int(tmp);
	fscanf(file , "\n%s" , &tmp);
	coop.defence = decrypt_int(tmp);
	fscanf(file , "\n%s" , &tmp);
	coop.health = decrypt_int(tmp);
	fscanf(file , "\n%s" , &tmp);
	total_monster_kill = decrypt_int(tmp);
	fscanf(file , "\n%s" , &tmp);
	monster_counting = decrypt_int(tmp);
	fclose(file);
	
	
	char filename2[1000];
	sprintf(filename2 , "%s_2.txt" , user_name);
	file = fopen(filename2 , "r");
	for(int i=0;i<type_of_core;i++){
		fscanf(file , "%s" , tmp);
		ore_inventory[i + 1] = decrypt_int(tmp);
	}
	int a;
	for(int i=0;i<3;i++) fscanf(file , "%s" , tmp);
	fscanf(file , "%s" , tmp);
	skillpoint[1] = decrypt_int(tmp);
	fscanf(file , "%s" , tmp);
	skillpoint[2] = decrypt_int(tmp);
	fscanf(file , "%s" , tmp);
	skillpoint[3] = decrypt_int(tmp);
	fscanf(file , "%s" , tmp);
	skillpoint[4] = decrypt_int(tmp);
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
	MAP re;
	re.x = lim_i;
	re.y = lim_j;
	strcpy(play_map[lim_i+1] , "T: Command Prompt");
	strcpy(play_map[lim_i+2] , "P: Save the game");
	strcpy(play_map[lim_i+3] , "I: Open player information");
	strcpy(play_map[lim_i+4] , "M: Mission System");
	strcpy(play_map[lim_i+5] , "O: Skill Points");
	strcpy(play_map[lim_i+6] , "Q: Setting");
	if(mode == 2) {
		printf("Loading...\n");
		Sleep(500);
	}
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
			else if(mode == 2 && i == information.x - 6 && j >= 20){
				printf("\tPlayer Health:%d" , player_health);
				break;
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
	if(master_comfirm){
		char t[100] = "MASTER";
		fprintf(file , "%s\n" , encrypt_str(t));
	}
	else if(admin_checked){
		char t[100] = "ADMIN";
		fprintf(file , "%s\n" , encrypt_str(t));
	}
	fprintf(file , "%s%s\n" , password , encrypt_int(player.lv));
	for(i=0;i<26;i++){
		fprintf(file , "%s\n" , encrypt_int(alp[i]));
	}
	fprintf(file , "%s\n%s\n%s\n%s\n%s" , encrypt_int(player.weapon_val) , encrypt_int(player.defence) , encrypt_int(player.health) , encrypt_int(total_monster_kill) , encrypt_int(monster_counting));
	fclose(file);
	char filename2[1000];
	sprintf(filename2 , "%s_2.txt" , player_ac);
	file = fopen(filename2 , "r");
	for(int i=0;i<type_of_core;i++){
		char a[1005];
		fscanf(file , "%s" , a);
	}
	char a[1005];
	char b[1005];
	char c[1005];
	fscanf(file , "%s%s%s" , a , b , c);
	fclose(file);
	file = fopen(filename2 , "w");
	for(int i=0;i<type_of_core;i++){
		fprintf(file , "%s\n" , encrypt_int(ore_inventory[i + 1]));
	}
	fprintf(file , "%s\n%s\n%s\n" , a , b , c);
	for(int i=1;i<=4;i++) fprintf(file , "%s\n" , encrypt_int(skillpoint[i]));
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


void command_promte_admin(PLAYER *player){
	char cmd[1000] , input;
	int quantity , i;
	printf("Command Prompt For Admin:\n");
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
		else if(strcmp(cmd , "admin") == 0){
			admin_page();
			printf("Command Prompt For Admin:\n");
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


void command_promte_user(PLAYER *player){
	char cmd[1000] , input;
	int quantity , i;
	printf("Command Prompt For User:\n");
	do{
		scanf("%s" , cmd);
		if(strcmp(cmd , "color") == 0){
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

CI new_combat_2(PLAYER player){
	
	int combat_map[100][100] = {0,};
	int path_x = 65, path_y = 11;
	int upperbound_x = 79 , lowerbound_x = 51;
	while(path_y < 39){
		int turn = rand()%2;
		if((turn == 1 || path_x - lowerbound_x <= 1) && upperbound_x - path_x >= 1){  //right
			//puts("Right");
			int max_turn = min(upperbound_x - path_x , 39 - path_y );
			int turn_ratio;
			if(max_turn <= 3){
				turn_ratio = rand()%max_turn + 1;
			}
			else{
				turn_ratio = rand()%(max_turn - 3) + 3;
			}
			turn_ratio = min(5 , turn_ratio);
			for(int i=0;i<turn_ratio;i++){
				//printf("%d %d\n" , path_x , path_y);
				/*if(rand()%10 == 0){  //1/10 chance
					path_x++;
					i++;
				}*/ 
				combat_map[path_x][path_y] = 1;
				combat_map[path_x - 1][path_y] = 1;
				combat_map[path_x + 1][path_y] = 1;
				path_x++;
				path_y++;
			}
		}
		else{ //left
			//puts("Left");
			int max_turn = min(lowerbound_x - path_x , 39 - path_y );
			int turn_ratio;
			if(max_turn <= 3){
				turn_ratio = rand()%max_turn + 1;
			}
			else{
				turn_ratio = rand()%(max_turn - 3) + 3;
			}
			turn_ratio = min(5 , turn_ratio);
			for(int i=0;i<turn_ratio;i++){
				//printf("%d %d\n" , path_x , path_y);
				/*if(rand()%10 == 0){  //1/10 chance
					path_x--;
					i++;
				}*/
				combat_map[path_x][path_y] = 1;
				combat_map[path_x - 1][path_y] = 1;
				combat_map[path_x + 1][path_y] = 1;
				path_x--;
				path_y++;
			}
		}
	}
	
	//print map
	for(int i=0;i<30;i++){
		gotoxy(50 , 10 + i);
		for(int j=0;j<=30;j++){
			if(i == 0 || i == 29 || j == 0 || j == 30||combat_map[j + 50][i  + 10] == 0){
				putchar(boundary_skin);
			}
			else{
				putchar(' ');
			}
		}
		putchar('\n');
	}
	gotoxy(63 , 6);
	for(int i=0;i<5;i++){
		putchar(boundary_skin);
	}
	for(int i=7;i<10;i++){
		gotoxy(63 , i);
		putchar(boundary_skin);
		gotoxy(67 , i);
		putchar(boundary_skin);
	}
	gotoxy(64 , 10);
	for(int i=0;i<3;i++){
		putchar(' ');
	}
	
	
	double dur = find_lv(player.lv) * -0.0075 + 0.35;
	if(!cheatison) dur = max(0.2 , dur);
	else dur = max(0.15 , dur);
	
	gotoxy(55 , 5);
	printf("Speed %.3lf s per drop" , dur);
	
	//start play
	int x = 65 , y = 7;
	CI re;
	re.hurt = false;
	gotoxy(x , y);
	putchar(main_character);
	int line_report = 0;
	struct timeval interval , end;
	gettimeofday(&interval , NULL);
	while(y < 11){
		gettimeofday(&end , NULL);
		if(((end.tv_sec - interval.tv_sec) * 1e6 + end.tv_usec - interval.tv_usec) * 1e-6 >= dur){
			interval = end;
			gotoxy(x , y);
			putchar(' ');
			y++;
			gotoxy(x , y);
			putchar(main_character);
		}
	}
	
	gettimeofday(&interval , NULL);
	while(y < 39){
		//count++;
		if(combat_map[x][y] == 0){
			//gotoxy(0 , line_report);
			//printf("%d %d" , x , y);
			//line_report++;
			re.hurt = true;
			break;
		}
		if(kbhit()){
			char cmd = getch();
			cmd = tolower(cmd); 
			if(cmd == 'a'){
				gotoxy(x , y);
				putchar(' ');
				x--;
				gotoxy(x , y);
				putchar(main_character);
			}
			else if(cmd == 'd'){
				gotoxy(x , y);
				putchar(' ');
				x++;
				gotoxy(x , y);
				putchar(main_character);
			}
		}	
		gettimeofday(&end , NULL);
		if(((end.tv_sec - interval.tv_sec) * 1e6 + end.tv_usec - interval.tv_usec) * 1e-6 >= dur){
			interval = end;
			gotoxy(x , y);
			putchar(' ');
			y++;
			gotoxy(x , y);
			putchar(main_character);
		}
	}
	return re;
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


int new_mining(PLAYER player){
	
	
	
	for(int i=0;i<30;i++){
		gotoxy(50 , 10 + i);
		for(int j=0;j<=30;j++){
			if(i == 0 || i == 29 || j == 0 || j == 30){
				putchar(boundary_skin);
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
		int pt = 0;
		int m_x[100] = {0,};
		int m_y[100] = {0,};
		int mm[100][100] = {0,};
		int t = rand()%7 + 3;
		int t_y = 13;
		while(t_y < 39){
			int tp = 51 + rand()%29;
			gotoxy( tp , t_y);
			putchar('*');
			m_x[pt] = tp;
			m_y[pt] = t_y;
			mm[t_y][tp] = 1;
			pt++;
			t_y+=(30/t);
			
		}
		double dur;
		if(!cheatison) dur = 0.2;
		else dur = max(0.1 , find_lv(player.lv) * -0.0075 + 0.2);
		gotoxy(60 , 6);
		printf("Round: %d  Speed: %.3lf s per drop" , i + 1 , dur);
		struct timeval st , end;
		gotoxy(x , y);
		putchar('&');
		gettimeofday(&st , NULL);
		while(y < 39){
			if(mm[y][x] == 1){
				sum++;
				mm[y][x] = 0;
			}
			if(kbhit()){
				int prev = x;
				char c = getch();
				c = tolower(c);
				if(c == 'a' && x > 51){
					x--;
				}
				else if(c == 'd' && x < 79){
					x++;
				}
				if(prev != x){
					gotoxy(prev , y);
					putchar(' ');
					gotoxy(x , y);
					putchar('&'); 
				}
			}
			if(kbhit() && cheatison){
				int prev = x;
				char c = getch();
				c = tolower(c);
				if(c == 'a' && x > 51){
					x--;
				}
				else if(c == 'd' && x < 79){
					x++;
				}
				if(prev != x){
					gotoxy(prev , y);
					putchar(' ');
					gotoxy(x , y);
					putchar('&'); 
				}
			}
			if(kbhit() && cheatison){
				int prev = x;
				char c = getch();
				c = tolower(c);
				if(c == 'a' && x > 51){
					x--;
				}
				else if(c == 'd' && x < 79){
					x++;
				}
				if(prev != x){
					gotoxy(prev , y);
					putchar(' ');
					gotoxy(x , y);
					putchar('&'); 
				}
			}
			gettimeofday(&end , NULL);
			if(((end.tv_sec - st.tv_sec) * 1e6 + end.tv_usec - st.tv_usec) * 1e-6 >= dur){
					gotoxy(x , y);
					putchar(' ');
					y++;
					gotoxy(x , y);
					putchar('&'); 
					st = end;
			}
		}
		gotoxy(x , y);
		putchar(boundary_skin);
		for(int j=0;j<pt;j++){
			gotoxy(m_x[j] , m_y[j]);
			putchar(' ');
		}
		x = 65;
		y = 11;
	}
	return sum;
}

int getinto_event(PLAYER player , int type , int player_health){
	srand(time(NULL));
	
	if(type == 2){  //mining
		play_map[player.y][player.x] = ' ';
		int s = new_mining(player) + skillpoint[4] * 0.2;
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
		return 0;
	}
	else if(type== 3){
		printf("\t\t\t\tpress Y to ready..");
		char cmd;
		do{
			cmd = getch();
		}while(cmd != 'Y' && cmd != 'y');
		cls;
		printf("\t\t\t\tStart!!");
		Sleep(100);
		cls;
		int max_str_length = 5;  //max generate string length
		int max_damage = player.health * 1.5 + player.defence * 0.5; //max damage monster can hit
		if(cheatison) max_damage *= 2;
		int i , j;
		int monster_health = rand()%player.weapon_val * 5 + player.weapon_val * 2;
		if(cheatison) monster_health *= 1.3;
		
		player.health = player_health;
		player.weapon_val+=skillpoint[1]*2;
		player.health += skillpoint[2]*5;
		player.defence += skillpoint[3]*2;
		double crit_constant = 1.01395948;  //10^(log(4)/100)
		int crit_chance = min(40 , (int)(pow(crit_constant , skillpoint[4]) * 10));
		cls;
		gotoxy(65 , 10);
		printf("Health:%d" , max(player.health , 0));
		gotoxy(65 , 11);
		printf("Monster Health:%d" , max(monster_health , 0));
		gotoxy(65 , 12);
		pause;
		cls;
		while(monster_health > 0 && player.health > 0){
			CI k;
			k = new_combat_2(player);
			int damage = 0;
			bool crit = false;
			if(k.hurt){
				damage = (rand()%max_damage + player.defence * 0.5);
				if(cheatison) damage *= 2;
				damage-= player.defence;
				damage = max(0 , damage);
				player.health -= damage;
			}
			else{
				monster_health -= player.weapon_val;
				if((rand()%100) + 1 < crit_chance){
					monster_health -= player.weapon_val;
					crit = true;
				}
			}
			cls;
			gotoxy(65 , 10);
			printf("Health:%d" , max(player.health , 0));
			gotoxy(65 , 11);
			printf("Monster Health:%d" , max(monster_health , 0));
			gotoxy(65 , 12);
			if(k.hurt){
				printf("You have taken %d damage!!\n" ,damage);
			}
			else if(crit){
				puts("You do crit damage to monster!!");
			}
			else{
				puts("You hit the monster!!");
			}
			gotoxy(65 , 13);
			pause;
			cls;
		}
		cls;
		int re;
		if(monster_health <= 0){
			printf("The monster is defeated.....\n");
			total_monster_kill++;
			monster_counting++;
			re = player.health;
		}
		else{
			printf("You are defeated....");
			re = -1;
		}
		getch();
		return re;
	}
}


void open_information(PLAYER player){
	printf("\t\t Your information:\n");
	printf("\t\tHealth:%d (%d + %d)\n" , player.health + skillpoint[2] * 5, player.health , skillpoint[2] * 5);
	printf("\t\tStrength:%d  (%d + %d)\n" , player.weapon_val + skillpoint[1] * 2 , player.weapon_val , skillpoint[1] * 2);
	printf("\t\tArmor:%d (%d + %d)\n" , player.defence + skillpoint[3] * 2 , player.defence , skillpoint[3] * 2);
	printf("\t\tLevel:%d\n" , find_lv(player.lv));
	printf("\t\tEXP:%d\n" , player.lv);
	printf("\t\tTotal monster you have killed:%d\n" , total_monster_kill);
	puts("Your inventory:");
	open_inventory();
	return ;
}

void ENDING(PLAYER player){
	int line = 0;
	cls;
	char str1[1005] = "This is the end of the game\nBut this is not the end of your advanture,\ntry to find all the secret in this game\n"; 
	char str2[1005] = "It is not easy to finish a game alone,\nI need to thanks every one that helped to devlop the game.\n";
	char str3[1005] = "And this is your statistics:\n";
	char str4[1005] = "I hope you enjoy the game, press any key to continue";
	gotoxy(60 , 0);
	for(int i=0;i<strlen(str1);i++){
		putchar(str1[i]);
		if(str1[i] == '\n'){
			line++;
			gotoxy(60 , line);
			Sleep(200);
		}
		Sleep(100);
	}
	putchar('\n');
	line++;
	gotoxy(60 , line);
	for(int i=0;i<strlen(str2);i++){
		putchar(str2[i]);
		if(str2[i] == '\n'){
			line++;
			gotoxy(60 , line);
			Sleep(200);
		}
		Sleep(100);
	}
	putchar('\n');
	line++;
	gotoxy(60 , line);
	for(int i=0;i<strlen(str3);i++){
		putchar(str3[i]);
		Sleep(100);
	}
	line++;
	gotoxy(60 , line);
	printf("Health:%d\n" , player.health);
	line++;
	gotoxy(60 , line);
	printf("Strength:%d\n" , player.weapon_val);
	line++;
	gotoxy(60 , line);
	printf("Armor:%d\n" , player.defence);
	line++;
	gotoxy(60 , line);
	printf("Level:%d\n" , find_lv(player.lv));
	line++;
	gotoxy(60 , line);
	printf("EXP:%d\n" , player.lv);
	line++;
	gotoxy(60 , line);
	printf("Total monster you have killed:%d\n" , total_monster_kill);
	int i;
	char Alapbet = 'A';
	line++;
	gotoxy(60 , line);
	for(i=0;i<13;i++){
		printf("%c:%d   " , Alapbet , alp[i]);
		Alapbet++;
	}
	line++;
	gotoxy(60 , line);
	for(i=13;i<26;i++){
		printf("%c:%d   " , Alapbet , alp[i]);
		Alapbet++;
	}
	line++;
	gotoxy(60 , line);
	for(int i=0;i<type_of_core;i++){
		printf("%s:%d" , ore_code[i + 1] , ore_inventory[i + 1]);
		line++;
		gotoxy(60 , line);
	}
	line++;
	gotoxy(60 , line);
	for(int i=0;i<strlen(str4);i++){
		putchar(str4[i]);
		Sleep(100);
	}
	getch();
	cls;
}

void trading(PLAYER player){
	int  traderate = 10;
	if(cheatison) traderate = 100;
	while(1){
		cls;
		char input;
		printf("Please typing what alphabet you want to trade Or type 1 to leave:");
		scanf(" %c" , &input);
		if(input == '1'){
			break;
		}
		input = tolower(input);
		if('a' < input && input <= 'z'){
			cls;
			int in;
			printf("How many you want to trade:");
			scanf("%d" , &in);
			if(alp[input - 'a' - 1] >= traderate * in){
				alp[input - 'a' - 1] -= traderate * in;
				alp[input - 'a' ]+=in;
				printf("Done");
				Sleep(500);
				if(alp[25] >= 10){
					ENDING(player);
					alp[25]-=10;
					return;
				}
			}
			else{
				printf("You don't have enough alphabet..");
				Sleep(500);
			}
		}
	}
	cls;
	printf("Bye..");
	Sleep(500);
	
	return;
}

void mission(PLAYER* player){
	FILE *fp;
	char filename[1005];
	strcpy(filename , player_ac);
	strcat(filename , "_2.txt");
	fp = fopen(filename , "r");
	for(int i=0;i<type_of_core;i++){
		int a[1005];
		fscanf(fp , "%s" , a);
	}
	int quantity , input_ore_code;
	int has_kill , need_to_kill;
	char cmd;
	char tmp[1005];
	fscanf(fp , "%s" , tmp);
	quantity = decrypt_int(tmp);
	fscanf(fp , "%s" , tmp);
	input_ore_code = decrypt_int(tmp);
	fscanf(fp , "%s" , tmp);
	need_to_kill = decrypt_int(tmp);
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
		double base = 1.00695555;
		(*player).lv += quantity * lv_of_ore[input_ore_code] * min((int)pow(base , skillpoint[4]) , 3);
		int new_quty = rand()%10 + 1;
		int new_code = rand()%type_of_core + 1;
		fp = fopen(filename , "w");
		for(int i=1;i<=type_of_core;i++){
			fprintf(fp , "%s\n" , encrypt_int(ore_inventory[i]));
		}
		fprintf(fp , "\n%s\n%s\n%s" , encrypt_int(new_quty) , encrypt_int(new_code) , encrypt_int(need_to_kill));
		fclose(fp);
		cls;
		alp[0]+=10;
		updater(*player);
		puts("Submitted");
		pause;
	}
	else{  // kill monster
		if(monster_counting >= need_to_kill){
			monster_counting -= need_to_kill;
			FILE * fp;
			fp = fopen(filename , "w");
			for(int i=1;i<=type_of_core;i++){
				fprintf(fp , "%s\n" , encrypt_int(ore_inventory[i]));
			}	
			fprintf(fp , "\n%s\n%s\n%s" , encrypt_int(quantity) , encrypt_int(input_ore_code ), encrypt_int(rand()%10 + 1));
			fclose(fp);
			alp[0]+=10;
			updater(*player);
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

void setting(PLAYER player){
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
				if(c != 'q' && c != 't' && c != 'i' && c != 'm' && c != 'o' &&  c != 'p' && c != up && c != down && c != left && c != right){
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
							map_driver(mode , player);
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
						strcpy(tmp[1] , encrypt_str(new_password));
					}
					else{
						strcpy(tmp[0] ,encrypt_str(new_password));
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
	
	//char consoletitle[200] = "Just Collect";
	SetConsoleTitle("Just Collect");
	
	HANDLE buff = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD sizeOfBuff;
	sizeOfBuff.X=300;
	sizeOfBuff.Y=300;
	SetConsoleScreenBufferSize(buff,sizeOfBuff);
	HWND hwnd = GetConsoleWindow();
	if( hwnd != NULL ){ SetWindowPos(hwnd ,0,0,0 ,1200,620 ,SWP_SHOWWINDOW|SWP_NOMOVE); }
	
	
	if(access( "CRAZY.txt", F_OK ) != -1 ){
		FILE *fp = fopen("CRAZY.txt" , "r");
		int a;
		fscanf(fp , "%d" , &a);
		fclose(fp);
		if(a == 1){
			cheatison = true;
		}
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
	
	player_health = player.health;
	
	while(command_code){
		int map_player_x = player.x, map_player_y = player.y;
		bool first_into_map = false;
		
		move_cmd = getch();
		move_cmd = tolower(move_cmd);
		
		random = rand()%random_code + 1;
		
		if(first_into_map){
			first_into_map = false;
		}
		
		
		
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
				trading(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y - 1][player.x] == '.'){
				player.y--;
				cls;
				getinto_event(player , 2 , 0);
				random = 0;
				map_runner(map_information , player);
			}
			else if(play_map[player.y - 1][player.x] == 'C'){  //choosing what area to go
				cls;
				saver.x = player.x;
				saver.y = player.y;
				player.x = 10;
				player.y = 10;
				map_information = map_driver(2 , player);
				player_health = player.health;
				map_runner(map_information , player);
				first_into_map = true;
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
				trading(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y + 1][player.x] == '.'){
				player.y++;
				cls;
				getinto_event(player , 2 , 0);
				random = 0;
				map_runner(map_information , player);
			}
			else if(play_map[player.y + 1][player.x] == 'C'){  //choosing what area to go
				cls;
				saver.x = player.x;
				saver.y = player.y;
				player.x = 10;
				player.y = 10;
				map_information = map_driver(2 , player);
				player_health = player.health;
				map_runner(map_information , player);
				first_into_map = true;
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
				trading(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x - 1] == '.'){
				player.x--;
				cls;
				getinto_event(player , 2 , 0);
				random = 0;
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x - 1] == 'C'){  //choosing what area to go
				cls;
				saver.x = player.x;
				saver.y = player.y;
				player.x = 10;
				player.y = 10;
				map_information = map_driver(2 , player);
				player_health = player.health;
				map_runner(map_information , player);
				first_into_map = true;
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
				trading(player);
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x + 1] == '.'){
				player.x++;
				cls;
				getinto_event(player , 2 , 0);
				random = 0;
				map_runner(map_information , player);
			}
			else if(play_map[player.y][player.x + 1] == 'C'){  //choosing what area to go
				cls;
				saver.x = player.x;
				saver.y = player.y;
				player.x = 10;
				player.y = 10;
				map_information = map_driver(2 , player);
				player_health = player.health;
				map_runner(map_information , player);
				first_into_map = true;
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
				command_promte_user(&player);
			}
			else{
				command_promte_admin(&player);
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
			mission(&player);
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
			setting(player);
			cls;
			map_runner(map_information , player);
		}
		if(move_cmd == 's' || move_cmd == 'S' || move_cmd == 'W' || move_cmd == 'w' || move_cmd == 'A' || move_cmd == 'a' || move_cmd == 'd' || move_cmd == 'D'){
			if(random >= random_chance && mode == 2 && !first_into_map){
				cls;
				player_health = getinto_event(player , 3 , player_health);
				cls;
				if(player_health > 0){
					double base = 1.00695555;
					player = exp_up(player , 50 * min((int)pow(base , skillpoint[4]) , 5));
				}
				else{
					player.x = saver.x;
					player.y = saver.y;
					map_information = map_driver(1 , player);
					cls;
					first_into_map = true;
				}
				map_runner(map_information , player);
			}
		}
	}
}
