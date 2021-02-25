#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct info{
	char name[30];
	char password[30];

} Info;


typedef struct Player{
	char name[30]; //한글 or 영어 최대 10자로 지정(영어만 or 한글만?)
	int score;
	int round; //난이도
	int miro_su; //미로, 스도쿠 판단
} Player;


void input_score(){
	Player input = {"",0,5}; //라운드는 5로 고정(예)
	FILE *fp = fopen("score.txt","ab+");
	if(fp==NULL){
		printf("파일 오류입니다.\n");
	}
	printf("당신의 이름을 입력하세요 : ");
	scanf("%s",input.name);
	printf("당신의 점수를 입력하세요 : ");
	scanf("%d",&input.score);
	fseek(fp,0,SEEK_END); //맨 끝으로 이동
	fwrite(&input,sizeof(Player),1,fp); //이진 파일로 저장
	fclose(fp);
	printf("\n입력이 완료되었습니다.\n\n");
}

void print_score(){
	Player output[100]={0,}, temp; //100명의 사람을 수용
	int N = 0; //갖고오는 사람 수
	int read_check; //제대로 읽어오는가?
	FILE *fp = fopen("score.txt","rb+");
	if(fp==NULL){
		printf("파일 오류입니다.\n");
		return;
	}
	printf("\n\n");
	rewind(fp);
	read_check = fread(&output[N],sizeof(Player),1,fp);
	while(read_check){
		printf("%-10s %-2d %-5d\n",output[N].name,output[N].round,output[N].score); //제대로 출력 확인 (주석)
		N+=1;
		read_check = fread(&output[N],sizeof(Player),1,fp);
	}

	for(int i=0; i<N-1; i++){ //버블정렬로 파일에서 입력받은 정보들을 소트
		for(int j=0; j<N-i-1; j++){
			if(output[j].score<output[j+1].score){
				strcpy(temp.name,output[j].name);
				temp.score = output[j].score;
				temp.round = output[j].round;
				strcpy(output[j].name,output[j+1].name);
				output[j].score = output[j+1].score;
				output[j].round = output[j+1].round;
				strcpy(output[j+1].name,temp.name);
				output[j+1].score = temp.score;
				output[j+1].round = temp.round;
			}
		}
	}


	printf("%19s===================랭킹===================\n","");
	printf("%19s순위          이름          난이도    점수\n","");
	printf("%19s==========================================\n","");
	for(int i=0; i<=19; i++){	
		printf("%19s %02d위   %-16s       %-3d     %-5d\n","",i+1,output[i].name,output[i].round,output[i].score);
		/*	if(output[i].name[0]>=0){ //영어 닉네임
			printf("%d 영",output[i].name[0]);
			printf("%19s %02d위   %-10s       %-3d     %-5d\n","",i+1,output[i].name,output[i].round,output[i].score);
			}
			else{ //한글 닉네임
			printf("%d 한",output[i].name[0]);
			printf("%19s %02d위   %-12s       %-3d     %-5d\n","",i+1,output[i].name,output[i].round,output[i].score);

			} */
	}

}

void input_info(){
	FILE *fp = fopen("info.txt","ab+");
	Info input, person_list[100];
	int N = 0; //갖고 올 회원 수

	rewind(fp); //회원 리스트에다가 기존 회원의 정보들 삽입
	read_check = fread(&person_list[N], sizeof(Info), 1, fp);
	while(read_check){ 
		N+=1;
		read_check = fread(&person_list[N],sizeof(INfo),1,fp);
	}

	while(1){
		printf("사용하실 닉네임을 입력하세요 : ");
		scanf("%s", input.name);

		for(int i = 0; i <= N; i++){
			if(strcmp(input.name, person_list[i].name) == 0){ //만약 닉네임이 겹치면
				printf("겹치는 닉네임이 존재합니다. 다시 입력해주세요\n");
				continue;
			}
		}
		break;
	}
	printf("패스워드를 입력하세요(6-12자) : ");
	scanf("%s", input.password);
	fseek(fp,0,SEEK_END); //맨 끝으로 이동
	fwrite(&input,sizeof(Info),1,fp); //이진 파일로 저장
	fclose(fp);

}

void login(){	
	FILE *fp = fopen("info.txt","ab+");
	Info input, person_list[100];
	int nick_check= 0, N = 0; //로그인 닉네임 체크, 갖고 올 회원 수
	char imsi_password[30];

	rewind(fp); //회원 리스트에다가 기존 회원의 정보들 삽입
	read_check = fread(&person_list[N], sizeof(Info), 1, fp);
	while(read_check){ 
		N+=1;
		read_check = fread(&person_list[N],sizeof(INfo),1,fp);
	}
	while(1){
		printf("닉네임을 입력하세요 : "); //닉네임 로그인 과정
		scanf("%s",&input.name);
		for(int i = 0; i <= N; i++){
			if(strcmp(input.name, person_list[i].name) == 0){ //만약 닉네임이 겹치면
				nick_check = 1;
				strcpy(imsi.password, person_list[i].name); //패스워드 임시 저장
				break;
			}
		}
		if(nick_check == 0){
			printf("존재하지 않는 닉네임입니다. 다시 입력하세요\n");
			continue;
		}
		while(1){ //패스워드 로그인 과정

			printf("패스워드를 입력하세요 : ");
			scanf("%s", &input.password);
			if(strcpy(input.password, imsi_password)==0){
				printf("로그인이 성공했습니다.\n");
				break;
			}
			else{
				printf("패스워드가 틀립니다. 다시 입력하세요\n");
				continue;
			}
		}
	}
}

int main(){
	int go;
	printf("\n===============================================\n");
	printf("1. 점수와 이름 등록 / 2. 랭킹 보기 / 3. 회원가입 / 4. 로그인 / 5. 종료\n");
	printf("입력 : ");
	scanf("%d",&go);
	while(go!=3){
		if(go==1)
			input_score();
		else if(go==2){
			print_score();
		}
		else if(go==3){
			input_info();
		}
		else if(go==4){

		}

		else 
			break;
		printf("\n===============================================\n");
		printf("1. 점수와 이름 등록 / 2. 랭킹 보기 / 3. 회원가입 / 4. 로그인 / 5. 종료\n");
		printf("입력 : ");
		scanf("%d",&go);


	}



}
