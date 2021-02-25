#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 10001
#define MAX 10000

int N; //행
int M; //열
int front;
int rear;
int maze[51][51];
int wall[51][51];
int visit[51][51];


//좌표
typedef struct xy{
	int x;
	int y;
}xy;

//정답입력
typedef struct Answer{
	char Yes_NO[2];
	int num;
}Answer;

xy queue[MAX_QUEUE_SIZE];

//방향 (상 하 좌 우)
int dx[4] = {0, 0, -1, 1};
int dy[4] = {1, -1, 0, 0};

int is_empty(){
	if (front == rear){
		return 1;
	}
	else
		return 0;
}

void enqueue(int x,int y){
	xy temp;
	temp.x = x;
	temp.y = y;
	rear = rear % MAX_QUEUE_SIZE;
	queue[rear++] = temp;
}

xy dequeue(){
	front = front % MAX_QUEUE_SIZE;
	return queue[front++];
}

//미로 벽뚫기 함수
void BreakWall(int x, int y){
	int nx;
	int ny;
	while(! is_empty()){
		xy pop = dequeue();
		for (int i = 0; i < 4; i++){
			nx = pop.x + dx[i];
			ny = pop.y + dy[i];

			//미로판을 아예 나갈 때
			if (nx < 1 || nx > M || ny < 1 || ny > N){
				continue;
			}
			
			//벽인 경우
			if (maze[nx][ny] == 0){
				if (wall[nx][ny] > wall[pop.x][pop.y] + 1){
					wall[nx][ny] = wall[pop.x][pop.y] + 1;
					enqueue(nx, ny);
				}
			}
			//길인 경우
			else if(maze[nx][ny] == 1){
				if (wall[nx][ny] > wall[pop.x][pop.y]){
					wall[nx][ny] = wall[pop.x][pop.y];
					enqueue(nx, ny);
				}
			}
		}
	}
}

//미로 최단거리 찾는 함수
void ShortDistance(int x, int y){
	int nx;
	int ny;
	while(! is_empty()){
		xy pop = dequeue();
		for (int i = 0; i < 4; i++){
			nx = pop.x + dx[i];
			ny = pop.y + dy[i];

			//미로판을 아예 나갈 때
			if (nx < 1 || nx > M || ny <1 || ny > N)
				continue;
			//벽인 경우
			if (maze[nx][ny] == 0)
				continue;
			//
			if (visit[nx][ny] == 0){
				visit[nx][ny] = visit[pop.x][pop.y] + 1;
				enqueue(nx, ny);
			}
		}
	}
}
int main(){
	srand(time(NULL));

	Answer answer;
	N = 10; //행
	M = 20; //열
	
	//wall배열 최댓값으로 초기화
	for (int x = 0; x < M; x++){
		for (int y = 0; y < N; y++){
			wall[x][y] = MAX;
		}
	}
	//난이도에 따라 미로 너비 조정

	//미로 받는 부분
	for (int x = 1; x <= M; x++){
		for (int y = 1; y <= N; y++){
			if (x == 1 && y == 1){
				maze[x][y] = 1;
			}
			else if (x == M && y == N){
				maze[x][y] = 1;
			}
			else{	
				maze[x][y] = rand() % 2;
			}
		}
	}

	//미로 출력
	for (int x = 1; x <= M; x++){
		for (int y = 1; y <= N; y++){
			printf("%d", maze[x][y]);
		}
		printf("\n");
	}

	enqueue(1, 1);
	visit[1][1] = 0;
	ShortDistance(1, 1);
	BreakWall(1, 1);

	//벽뚫기 없이 미로가 탈출가능한 경우
	if (wall[N][M] == 0){
		printf("탈출이 가능한가요? (O or X)\n");
		scanf("%c", answer.
	}
}
