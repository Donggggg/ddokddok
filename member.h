
typedef struct Info{
	char id[30]; //ID
	char password[30]; //비밀번호
	char name[20]; //이름
	char email[30]; //이메일
	char phone[15]; //번호
	char nickname[20]; //닉네임
	int member_number; //고유 회원번호
	int win; // 승리 횟수 저장  <미구현 예정>
	int lose; // 패배 횟수 저장 <미구현 예정>
	int manage; // 관리자 유무

} Info;

void inputInfo();
int login(char ID[255], char PW[255]);
Info login2(int log_num);
void searchIdPass(int check);
void manageMember();
int printMenu(Info member);
void startLogin(Info *player); //pritnMenu()로 입력받고 선택하는 함수
void logout(Info *player);
