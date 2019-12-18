#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define gold_key 553

char *encrypt_int(int a){  // this is for encrypt integer only
	a ^= gold_key;  //hash by gold key
	char *re = (char*)malloc(100005 * sizeof(char));  // this is another way to create a string in c
	int i;
	srand(time(NULL));
	int first_length = rand()%20 + 1;  //gen. the number of useless letters before #
	for(i = 0 ;i<first_length;i++){
		re[i] = (char) (rand()%90 + 36);  //start to gen. useless letters, no # will be generate
	}
	re[i] = '#';
	i++;
	int p = 0;
	int tmp[1005];
	while(a>0){  //start to convert dec to hex in numberic state
		tmp[p] = a%16;
		a/=16;
		p++;
	}
	while(p--){  //convert numberic state to string state
		if(tmp[p] >= 10){
			re[i] = tmp[p] - 10 + 'A';
		}
		else{
			re[i] = tmp[p] + '0';
		}
		i++;
	}
	re[i] = '*';
	i++;
	int second_length = rand()%20 + 1;
	while(second_length--){  //gen. the number of useless letters after *
		re[i] = (char) (rand()%90 + 36);
		i++;
	}
	re[i] = '\0';
	return re;
}

int decrypt_int(char str[]){  // this is for decrypt integer only
	int l = strlen(str);
	char tmp[100005];
	int i;
	int p = 0;
	for(i=0;i<l;i++){
		if(str[i] == '#'){  // finding # 
			while(str[i + p + 1] != '*'){  //get the 16 based number until meet *
				tmp[p] = str[i + p + 1];
				p++;
			}
			break;
		}
	}
	int cnt = 0;
	for(i=0;i<p;i++){  //converting to dec from hex
		if('0' <= tmp[i] && tmp[i] <= '9'){   
			cnt += (1<<(4 * (p - i - 1))) * (tmp[i] - '0');  
		}
		else{
			cnt += (1<<(4 * (p - i - 1)))  * (tmp[i] - 'A' + 10);
		}
	}
	return cnt ^ gold_key;  //hash the dec number back to orginal data
}


