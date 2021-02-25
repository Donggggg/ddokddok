#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

int sdoku[9][9] = {0};

void IN_sdoku(){
	for(int i = 0; i<9; i++){
		for(int j = 0; j<9;j++){
			scanf("%d", &sdoku[i][j]);
		}
	}
}

int vertical(int n, int x){
	for(int i=0; i<9; i++){
		if(sdoku[i][x] == n){
			return FALSE;
		}
	}
	return TRUE;
}

int horizontal(int n, int y){
	for(int i=0; i<9; i++){
		if(sdoku[y][i] == n){
			return FALSE;
		}
	}
	return TRUE;
}

int box(int n, int x, int y){
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			if(sdoku[(y/3)*3+i][(x/3)*3+j] == n){
				return FALSE;
			}
		}
	}
	return TRUE;
}

int isComplete(){
	for(int i = 0; i<9; i++){
		for(int j = 0; j<9; j++){
			if(sdoku[i][j] == 0)
				return FALSE;
		}
	}
	return TRUE;
}

void PrintSdoku(){
	for(int i = 0; i<9; i++){
		for(int j = 0; j<9; j++){
			printf("%d ", sdoku[i][j]);
		}
		printf("\n");
	}
}

int Sdoku(int n, int m){ 
	for(int i = n; i < 9; i++)
		for(int j = i!=n?0:m; j < 9; j++) {
			if(!sdoku[i][j]){
				for(int k=1; k<=9;k++){
					if(vertical(k,j) && horizontal(k,i) && box(k,j,i)){
						sdoku[i][j] = k;
						if(!Sdoku(i,j)) sdoku[i][j]=0;
					}
				}
				if(!sdoku[i][j])	return 0;
			}
		}
	return 1;
}

int main(){
	IN_sdoku();
	Sdoku(0,0);
	if(isComplete())
		PrintSdoku();
	return 0;
}