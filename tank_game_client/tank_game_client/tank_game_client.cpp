#include <winsock2.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>

// 버퍼의 크기
#define	BUF_SIZE 20

// 맵의 크기 정의
#define MAP_LINES 25
#define MAP_COLS 111

// 유저의 수 정의
#define USERS 2

// 지뢰의 수 정의
#define MINES 10

// 방향 상수 정의
#define EAST 0
#define WEST 1
#define SOUTH 2
#define NORTH 3

// 탱크 구조체
typedef struct tank {
	int x;		// x좌표
	int y;		// y좌표
	int vector;		// 방향
}TANK;

// 레이져 구조체
typedef struct laser {
	int tank_x;	// 발사된 x좌표
	int tank_y; // 발사된 y좌표
	int vector;	// 레이져 방향
}LASER;

// 지뢰 구조체
typedef struct mine {
	int x;
	int y;
}MINE;

void ErrorDisplay( const char *str )
{
	printf("<ERROR> %s!!!\n", str );
	exit(-1);
}

int main( int argc, char* argv[] )
{
	int	retval;		//recv(), send() 수행하면 return 값: 전송한 데이타 수

	// argv[0] : TCPClient
	// argv[1] : IP 주소
	// argv[2] : Port 번호
	if( argc != 3 )	
	{
		printf("<input format>TCPClient <IP> <Port>\n");
		exit(0);
	}

	// 윈속 DLL: open()
	WSADATA	wsa;
	retval = WSAStartup(  MAKEWORD(2, 2), &wsa );
	if( retval != 0 )	return -1;

	// 소켓 하나 만들기...
	SOCKET	ClientSocket;
	ClientSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( ClientSocket == INVALID_SOCKET )
	{
		ErrorDisplay("socket() error(INVALID_SOCKET)");
	}

	// connect() 함수를 이용해서 서버에 접속 시도...
	SOCKADDR_IN	ServerAddr;
	ZeroMemory( &ServerAddr, sizeof(ServerAddr)  );
	ServerAddr.sin_family			= AF_INET;
	ServerAddr.sin_port			= htons( atoi(argv[2]) );		// 서버 포트 번호...
	ServerAddr.sin_addr.s_addr		= inet_addr( argv[1] );		// 서버 IP 주소...
	
	retval = connect( ClientSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr) );
	if( retval == SOCKET_ERROR )
	{
		ErrorDisplay("connect() error(SOCKET_ERROR)");
	}

	// 1. 연결이 완료된 상태에서 게임화면 출력
	char map[MAP_LINES][MAP_COLS];	// 게임의 맵
	TANK tank[USERS];	// 탱크 구조체
	LASER laser[USERS];	// 레이저 구조체
	MINE mine[MINES];	// 지뢰 구조체
	int key = 0;		// 입력받은 키의 아스키코드 저장 변수
	int shoot = 0;		// 레이져 발사여부 저장 변수
	int hit = 0;		// 레이져를 맞은 탱크 저장 변수
	int i, j, k, l, m;	// 반복 제어 변수

	// 맵 초기화
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			if ((i == 0 || i == MAP_LINES - 1) && j % 2 == 0) {
				map[i][j] = 'x';
			}
			else if (j == 0 || j == MAP_COLS - 1) {
				map[i][j] = 'x';
			}
			else {
				map[i][j] = ' ';
			}
		}
	}

	// 메인화면 출력
	// TANK GAME 문자열 등록
	map[12][52] = 'T';
	map[12][53] = 'A';
	map[12][54] = 'N';
	map[12][55] = 'K';

	map[12][57] = 'G';
	map[12][58] = 'A';
	map[12][59] = 'M';
	map[12][60] = 'E';

	// PRESS ANY BUTTON 문자열 등록
	map[17][48] = 'P';
	map[17][49] = 'R';
	map[17][50] = 'E';
	map[17][51] = 'S';
	map[17][52] = 'S';

	map[17][54] = 'A';
	map[17][55] = 'N';
	map[17][56] = 'Y';

	map[17][58] = 'K';
	map[17][59] = 'E';
	map[17][60] = 'Y';
	map[17][61] = '.';
	map[17][62] = '.';
	map[17][63] = '.';

	// 맵 출력
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}

	// 대기상태
	_getch();
	while (_kbhit()) _getch();

	// 맵 초기화
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			if ((i == 0 || i == MAP_LINES - 1) && j % 2 == 0) {
				map[i][j] = 'x';
			}
			else if (j == 0 || j == MAP_COLS - 1) {
				map[i][j] = 'x';
			}
			else {
				map[i][j] = ' ';
			}
		}
	}

	// 3
	system("cls");
	map[13][56] = '3';
	// 맵 출력
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	Sleep(1000);

	// 2
	system("cls");
	map[13][56] = '2';
	// 맵 출력
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	Sleep(1000);

	// 1
	system("cls");
	map[13][56] = '1';
	// 맵 출력
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	Sleep(1000);

	// 맵 초기화
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			if ((i == 0 || i == MAP_LINES - 1) && j % 2 == 0) {
				map[i][j] = 'x';
			}
			else if (j == 0 || j == MAP_COLS - 1) {
				map[i][j] = 'x';
			}
			else {
				map[i][j] = ' ';
			}
		}
	}

	// 1번 탱크 위치 초기화
	tank[0].x = 4;
	tank[0].y = 2;
	tank[0].vector = EAST;

	// 2번 탱크 위치 초기화
	tank[1].x = MAP_COLS - 5;
	tank[1].y = MAP_LINES - 3;
	tank[1].vector = WEST;

	// 지뢰 위치 초기화
	srand((int)time(NULL));
	for (i = 0; i < MINES; i++) {
		do{
		mine[i].x = (rand() % (MAP_COLS - 20)) + 10;
		} while ((mine[i].x % 2) != 0);
		mine[i].y = (rand() % (MAP_LINES - 6)) + 3;
	}

	// 탱크 그리기
	for (i = 0; i < USERS; i++) {
		for (j = tank[i].x - 2; j <= tank[i].x + 2; j += 2) {
			for (k = tank[i].y - 1; k <= tank[i].y + 1; k++) {
				map[k][j] = 'o';
			}
		}
		switch (tank[i].vector) {
		case EAST:
			map[tank[i].y][(tank[i].x) + 4] = 'e';
			break;
		case WEST:
			map[tank[i].y][(tank[i].x) - 4] = 'e';
			break;
		case SOUTH:
			map[(tank[i].y) + 2][tank[i].x] = 'e';
			break;
		case NORTH:
			map[(tank[i].y) - 2][tank[i].x] = 'e';
			break;
		}
	}

	// 지뢰 그리기
	for (i = 0; i < MINES; i++)
		map[mine[i].y][mine[i].x] = 'v';

	// 맵 출력
	system("cls");
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}


	// 데이타를 송, 수신하기...
	int	buf[BUF_SIZE];		// 정수형 버퍼 선언
	int result[BUF_SIZE];	// 결과값 버퍼 선언

	while( 1 )
	{
		// 사용자로부터 데이타 입력...
		if (_kbhit()) { //키입력이 있는 경우
			for (i = 0; i < USERS; i++) {	// 이전의 위치에서 탱크를 지움
				for (j = tank[i].x - 2; j <= tank[i].x + 2; j += 2) {
					for (k = tank[i].y - 1; k <= tank[i].y + 1; k++) {
						map[k][j] = ' ';
					}
				}
				switch (tank[i].vector) {
				case EAST:
					map[tank[i].y][(tank[i].x) + 4] = ' ';
					break;
				case WEST:
					map[tank[i].y][(tank[i].x) - 4] = ' ';
					break;
				case SOUTH:
					map[(tank[i].y) + 2][tank[i].x] = ' ';
					break;
				case NORTH:
					map[(tank[i].y) - 2][tank[i].x] = ' ';
					break;
				}
			}
			key = _getch(); //키값을 받음
			if (key == 224 || key == 0) { //방향키인경우 
				key = _getch(); //키값을 받음
			}
			while (_kbhit()) _getch();	// 버퍼를 비움

			// 전송할 데이터 삽입
			buf[0] = tank[0].x;
			buf[1] = tank[0].y;
			buf[2] = tank[0].vector;
			buf[3] = tank[1].x;
			buf[4] = tank[1].y;
			buf[5] = tank[1].vector;
			buf[6] = key;

			// 서버로 데이터를 전송
			retval = send(ClientSocket, (char*)buf, sizeof(buf), 0);

			// 전송상태 검사
			if (retval == SOCKET_ERROR)
			{
				printf("<ERROR> send()(SOCKET_ERROR)!!!\n");
				break;
			}

			// 서버로부터 데이터를 수신
			retval = recv(ClientSocket, (char*)result, sizeof(result), 0);

			// 수신상태 검사
			if (retval == SOCKET_ERROR)
			{
				printf("<ERROR> recvn()(SOCKET_ERROR)!!!\n");
				break;
			}
			else if (retval == 0)	break;

			tank[0].x = result[0];
			tank[0].y = result[1];
			tank[0].vector = result[2];
			tank[1].x = result[3];
			tank[1].y = result[4];
			tank[1].vector = result[5];
			shoot = result[6];
			switch (shoot) {
			case 0:
				break;
			case 1:
				laser[0].tank_x = result[7];
				laser[0].tank_y = result[8];
				laser[0].vector = result[9];
			case 2:
				laser[1].tank_x = result[7];
				laser[1].tank_y = result[8];
				laser[1].vector = result[9];
			}
			
			// 탱크를 맵에 그리기
			for (i = 0; i < USERS; i++) {
				for (j = tank[i].x - 2; j <= tank[i].x + 2; j += 2) {
					for (k = tank[i].y - 1; k <= tank[i].y + 1; k++) {
						map[k][j] = 'o';
						// 탱크의 위치가 지뢰와 같은지 확인
						for (l = 0; l < MINES; l++) {
							if (j == mine[l].x && k == mine[l].y) {
								if (i == 0) shoot = 2;
								else shoot = 1;
								hit = 1;
							}
						}
					}
				}
				switch (tank[i].vector) {
				case EAST:
					map[tank[i].y][(tank[i].x) + 4] = 'e';
					break;
				case WEST:
					map[tank[i].y][(tank[i].x) - 4] = 'e';
					break;
				case SOUTH:
					map[(tank[i].y) + 2][tank[i].x] = 'e';
					break;
				case NORTH:
					map[(tank[i].y) - 2][tank[i].x] = 'e';
					break;
				}
			}

			// 지뢰 그리기
			for (i = 0; i < MINES; i++)
				map[mine[i].y][mine[i].x] = 'v';

			// 탱크가 지뢰를 밟았다면 게임 종료
			if (hit != 0) break;

			// 레이져를 맵에 그리기
			switch (shoot) {
			case 0:
				break;
			case 1:	// 플레이어1의 레이져
				switch (laser[shoot - 1].vector) {
				case EAST:
					for (k = laser[shoot - 1].tank_x + 6; k < MAP_COLS - 2; k += 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// 플레이어1의 레이져가 플레이어2에 맞았는지 확인
						if ((k == tank[shoot].x) && ((laser[shoot - 1].tank_y) == tank[shoot].y))
							hit = shoot;
					}
					break;
				case WEST:
					for (k = laser[shoot - 1].tank_x - 6; k > 1; k -= 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// 플레이어1의 레이져가 플레이어2에 맞았는지 확인
						if ((k == tank[shoot].x) && ((laser[shoot - 1].tank_y) == tank[shoot].y))
							hit = shoot;
					}
					break;
				case NORTH:
					for (k = laser[shoot - 1].tank_y - 3; k > 1 - 1; k--) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// 플레이어1의 레이져가 플레이어2에 맞았는지 확인
						if ((k == tank[shoot].y) && ((laser[shoot - 1].tank_x) == tank[shoot].x))
							hit = shoot;
					}
					break;
				case SOUTH:
					for (k = laser[shoot - 1].tank_y + 3; k < MAP_LINES - 1; k++) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// 플레이어1의 레이져가 플레이어2에 맞았는지 확인
						if ((k == tank[shoot].y) && ((laser[shoot - 1].tank_x) == tank[shoot].x))
							hit = shoot;
					}
					break;
				}
			case 2:	// 플레이어2의 레이져
				switch (laser[shoot - 1].vector) {
				case EAST:
					for (k = laser[shoot - 1].tank_x + 6; k < MAP_COLS - 2; k += 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// 플레이어2의 레이져가 플레이어1에 맞았는지 확인
						if ((k == tank[shoot - 2].x) && ((laser[shoot - 1].tank_y) == tank[shoot - 2].y))
							hit = shoot;
					}
					break;
				case WEST:
					for (k = laser[shoot - 1].tank_x - 6; k > 1; k -= 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// 플레이어2의 레이져가 플레이어1에 맞았는지 확인
						if ((k == tank[shoot - 2].x) && ((laser[shoot - 1].tank_y) == tank[shoot - 2].y))
							hit = shoot;
					}
					break;
				case NORTH:
					for (k = laser[shoot - 1].tank_y - 3; k > 1 - 1; k--) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// 플레이어1의 레이져가 플레이어2에 맞았는지 확인
						if ((k == tank[shoot - 2].y) && ((laser[shoot - 1].tank_x) == tank[shoot - 2].x))
							hit = shoot;
					}
					break;
				case SOUTH:
					for (k = laser[shoot - 1].tank_y + 3; k < MAP_LINES - 1; k++) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// 플레이어1의 레이져가 플레이어2에 맞았는지 확인
						if ((k == tank[shoot - 2].y) && ((laser[shoot - 1].tank_x) == tank[shoot - 2].x))
							hit = shoot;
					}
					break;
				}
			}

			// 발사된 레이져 출력
			if (shoot == 1 || shoot == 2) {
				system("cls");
				for (i = 0; i < MAP_LINES; i++) {
					for (j = 0; j < MAP_COLS; j++) {
						printf("%c", map[i][j]);
					}
					printf("\n");
				}
				Sleep(50);	// 출력 후 바로 사라지는 것을 방지
			}
			
			// 탱크가 맞았다면 게임 종료
			if (hit != 0) break;

			// 그렸던 레이져를 제거
			if (shoot == 1 || shoot == 2) {
				for (i = 0; i < USERS; i++) {
					switch (laser[i].vector) {
					case EAST:
						for (k = laser[i].tank_x + 6; k < MAP_COLS - 2; k += 2) {
							map[laser[i].tank_y][k] = ' ';
						}
						break;
					case WEST:
						for (k = laser[i].tank_x - 6; k > 1; k -= 2) {
							map[laser[i].tank_y][k] = ' ';
						}
						break;
					case NORTH:
						for (k = laser[i].tank_y - 3; k > 1 - 1; k--) {
							map[k][laser[i].tank_x] = ' ';
						}
						break;
					case SOUTH:
						for (k = laser[i].tank_y + 3; k < MAP_LINES - 1; k++) {
							map[k][laser[i].tank_x] = ' ';
						}
						break;
					}
				}
				// 레이저 소멸
				for(i=0; i<USERS; i++){
					laser[i].tank_x = 0;
					laser[i].tank_y = 0;
				}

				// 탱크 다시 그리기
				for (i = 0; i < USERS; i++) {
					for (j = tank[i].x - 2; j <= tank[i].x + 2; j += 2) {
						for (k = tank[i].y - 1; k <= tank[i].y + 1; k++) {
							map[k][j] = 'o';
						}
					}
					switch (tank[i].vector) {
					case EAST:
						map[tank[i].y][(tank[i].x) + 4] = 'e';
						break;
					case WEST:
						map[tank[i].y][(tank[i].x) - 4] = 'e';
						break;
					case SOUTH:
						map[(tank[i].y) + 2][tank[i].x] = 'e';
						break;
					case NORTH:
						map[(tank[i].y) - 2][tank[i].x] = 'e';
						break;
					}
				}
			}
			
			// 맵 출력
			system("cls");

			for (i = 0; i < MAP_LINES; i++) {
				for (j = 0; j < MAP_COLS; j++) {
					printf("%c", map[i][j]);
				}
				printf("\n");
			}

			shoot = 0;
		}		
	}
	// 소켓 close...
	closesocket( ClientSocket );
	
	// 윈속 DLL: close()
	WSACleanup();

	// 맵 초기화
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			if ((i == 0 || i == MAP_LINES - 1) && j % 2 == 0) {
				map[i][j] = 'x';
			}
			else if (j == 0 || j == MAP_COLS - 1) {
				map[i][j] = 'x';
			}
			else {
				map[i][j] = ' ';
			}
		}
	}

	// 승리 메시지 출력
	map[13][51] = 'P';
	map[13][52] = 'L';
	map[13][53] = 'A';
	map[13][54] = 'Y';
	map[13][55] = 'E';
	map[13][56] = 'R';
	map[13][59] = 'W';
	map[13][60] = 'I';
	map[13][61] = 'N';
	map[13][62] = '!';
	switch (shoot) {
	case 1:
		map[13][57] = '1';
		break;
	case 2:
		map[13][57] = '2';
		break;
	}

	// 맵 출력
	system("cls");
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	exit(0);
	_getch();
	return 0;
}