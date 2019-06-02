#include <winsock2.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <stdio.h>

// ������ ũ��
#define	BUF_SIZE 20

// ���� ũ�� ����
#define MAP_LINES 25
#define MAP_COLS 111

// ������ �� ����
#define USERS 2

// ������ �� ����
#define MINES 10

// ���� ��� ����
#define EAST 0
#define WEST 1
#define SOUTH 2
#define NORTH 3

// ��ũ ����ü
typedef struct tank {
	int x;		// x��ǥ
	int y;		// y��ǥ
	int vector;		// ����
}TANK;

// ������ ����ü
typedef struct laser {
	int tank_x;	// �߻�� x��ǥ
	int tank_y; // �߻�� y��ǥ
	int vector;	// ������ ����
}LASER;

// ���� ����ü
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
	int	retval;		//recv(), send() �����ϸ� return ��: ������ ����Ÿ ��

	// argv[0] : TCPClient
	// argv[1] : IP �ּ�
	// argv[2] : Port ��ȣ
	if( argc != 3 )	
	{
		printf("<input format>TCPClient <IP> <Port>\n");
		exit(0);
	}

	// ���� DLL: open()
	WSADATA	wsa;
	retval = WSAStartup(  MAKEWORD(2, 2), &wsa );
	if( retval != 0 )	return -1;

	// ���� �ϳ� �����...
	SOCKET	ClientSocket;
	ClientSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( ClientSocket == INVALID_SOCKET )
	{
		ErrorDisplay("socket() error(INVALID_SOCKET)");
	}

	// connect() �Լ��� �̿��ؼ� ������ ���� �õ�...
	SOCKADDR_IN	ServerAddr;
	ZeroMemory( &ServerAddr, sizeof(ServerAddr)  );
	ServerAddr.sin_family			= AF_INET;
	ServerAddr.sin_port			= htons( atoi(argv[2]) );		// ���� ��Ʈ ��ȣ...
	ServerAddr.sin_addr.s_addr		= inet_addr( argv[1] );		// ���� IP �ּ�...
	
	retval = connect( ClientSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr) );
	if( retval == SOCKET_ERROR )
	{
		ErrorDisplay("connect() error(SOCKET_ERROR)");
	}

	// 1. ������ �Ϸ�� ���¿��� ����ȭ�� ���
	char map[MAP_LINES][MAP_COLS];	// ������ ��
	TANK tank[USERS];	// ��ũ ����ü
	LASER laser[USERS];	// ������ ����ü
	MINE mine[MINES];	// ���� ����ü
	int key = 0;		// �Է¹��� Ű�� �ƽ�Ű�ڵ� ���� ����
	int shoot = 0;		// ������ �߻翩�� ���� ����
	int hit = 0;		// �������� ���� ��ũ ���� ����
	int i, j, k, l, m;	// �ݺ� ���� ����

	// �� �ʱ�ȭ
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

	// ����ȭ�� ���
	// TANK GAME ���ڿ� ���
	map[12][52] = 'T';
	map[12][53] = 'A';
	map[12][54] = 'N';
	map[12][55] = 'K';

	map[12][57] = 'G';
	map[12][58] = 'A';
	map[12][59] = 'M';
	map[12][60] = 'E';

	// PRESS ANY BUTTON ���ڿ� ���
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

	// �� ���
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}

	// ������
	_getch();
	while (_kbhit()) _getch();

	// �� �ʱ�ȭ
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
	// �� ���
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
	// �� ���
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
	// �� ���
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}
	Sleep(1000);

	// �� �ʱ�ȭ
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

	// 1�� ��ũ ��ġ �ʱ�ȭ
	tank[0].x = 4;
	tank[0].y = 2;
	tank[0].vector = EAST;

	// 2�� ��ũ ��ġ �ʱ�ȭ
	tank[1].x = MAP_COLS - 5;
	tank[1].y = MAP_LINES - 3;
	tank[1].vector = WEST;

	// ���� ��ġ �ʱ�ȭ
	srand((int)time(NULL));
	for (i = 0; i < MINES; i++) {
		do{
		mine[i].x = (rand() % (MAP_COLS - 20)) + 10;
		} while ((mine[i].x % 2) != 0);
		mine[i].y = (rand() % (MAP_LINES - 6)) + 3;
	}

	// ��ũ �׸���
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

	// ���� �׸���
	for (i = 0; i < MINES; i++)
		map[mine[i].y][mine[i].x] = 'v';

	// �� ���
	system("cls");
	for (i = 0; i < MAP_LINES; i++) {
		for (j = 0; j < MAP_COLS; j++) {
			printf("%c", map[i][j]);
		}
		printf("\n");
	}


	// ����Ÿ�� ��, �����ϱ�...
	int	buf[BUF_SIZE];		// ������ ���� ����
	int result[BUF_SIZE];	// ����� ���� ����

	while( 1 )
	{
		// ����ڷκ��� ����Ÿ �Է�...
		if (_kbhit()) { //Ű�Է��� �ִ� ���
			for (i = 0; i < USERS; i++) {	// ������ ��ġ���� ��ũ�� ����
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
			key = _getch(); //Ű���� ����
			if (key == 224 || key == 0) { //����Ű�ΰ�� 
				key = _getch(); //Ű���� ����
			}
			while (_kbhit()) _getch();	// ���۸� ���

			// ������ ������ ����
			buf[0] = tank[0].x;
			buf[1] = tank[0].y;
			buf[2] = tank[0].vector;
			buf[3] = tank[1].x;
			buf[4] = tank[1].y;
			buf[5] = tank[1].vector;
			buf[6] = key;

			// ������ �����͸� ����
			retval = send(ClientSocket, (char*)buf, sizeof(buf), 0);

			// ���ۻ��� �˻�
			if (retval == SOCKET_ERROR)
			{
				printf("<ERROR> send()(SOCKET_ERROR)!!!\n");
				break;
			}

			// �����κ��� �����͸� ����
			retval = recv(ClientSocket, (char*)result, sizeof(result), 0);

			// ���Ż��� �˻�
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
			
			// ��ũ�� �ʿ� �׸���
			for (i = 0; i < USERS; i++) {
				for (j = tank[i].x - 2; j <= tank[i].x + 2; j += 2) {
					for (k = tank[i].y - 1; k <= tank[i].y + 1; k++) {
						map[k][j] = 'o';
						// ��ũ�� ��ġ�� ���ڿ� ������ Ȯ��
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

			// ���� �׸���
			for (i = 0; i < MINES; i++)
				map[mine[i].y][mine[i].x] = 'v';

			// ��ũ�� ���ڸ� ��Ҵٸ� ���� ����
			if (hit != 0) break;

			// �������� �ʿ� �׸���
			switch (shoot) {
			case 0:
				break;
			case 1:	// �÷��̾�1�� ������
				switch (laser[shoot - 1].vector) {
				case EAST:
					for (k = laser[shoot - 1].tank_x + 6; k < MAP_COLS - 2; k += 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// �÷��̾�1�� �������� �÷��̾�2�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot].x) && ((laser[shoot - 1].tank_y) == tank[shoot].y))
							hit = shoot;
					}
					break;
				case WEST:
					for (k = laser[shoot - 1].tank_x - 6; k > 1; k -= 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// �÷��̾�1�� �������� �÷��̾�2�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot].x) && ((laser[shoot - 1].tank_y) == tank[shoot].y))
							hit = shoot;
					}
					break;
				case NORTH:
					for (k = laser[shoot - 1].tank_y - 3; k > 1 - 1; k--) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// �÷��̾�1�� �������� �÷��̾�2�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot].y) && ((laser[shoot - 1].tank_x) == tank[shoot].x))
							hit = shoot;
					}
					break;
				case SOUTH:
					for (k = laser[shoot - 1].tank_y + 3; k < MAP_LINES - 1; k++) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// �÷��̾�1�� �������� �÷��̾�2�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot].y) && ((laser[shoot - 1].tank_x) == tank[shoot].x))
							hit = shoot;
					}
					break;
				}
			case 2:	// �÷��̾�2�� ������
				switch (laser[shoot - 1].vector) {
				case EAST:
					for (k = laser[shoot - 1].tank_x + 6; k < MAP_COLS - 2; k += 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// �÷��̾�2�� �������� �÷��̾�1�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot - 2].x) && ((laser[shoot - 1].tank_y) == tank[shoot - 2].y))
							hit = shoot;
					}
					break;
				case WEST:
					for (k = laser[shoot - 1].tank_x - 6; k > 1; k -= 2) {
						map[laser[shoot - 1].tank_y][k] = 'z';
						// �÷��̾�2�� �������� �÷��̾�1�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot - 2].x) && ((laser[shoot - 1].tank_y) == tank[shoot - 2].y))
							hit = shoot;
					}
					break;
				case NORTH:
					for (k = laser[shoot - 1].tank_y - 3; k > 1 - 1; k--) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// �÷��̾�1�� �������� �÷��̾�2�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot - 2].y) && ((laser[shoot - 1].tank_x) == tank[shoot - 2].x))
							hit = shoot;
					}
					break;
				case SOUTH:
					for (k = laser[shoot - 1].tank_y + 3; k < MAP_LINES - 1; k++) {
						map[k][laser[shoot - 1].tank_x] = 'z';
						// �÷��̾�1�� �������� �÷��̾�2�� �¾Ҵ��� Ȯ��
						if ((k == tank[shoot - 2].y) && ((laser[shoot - 1].tank_x) == tank[shoot - 2].x))
							hit = shoot;
					}
					break;
				}
			}

			// �߻�� ������ ���
			if (shoot == 1 || shoot == 2) {
				system("cls");
				for (i = 0; i < MAP_LINES; i++) {
					for (j = 0; j < MAP_COLS; j++) {
						printf("%c", map[i][j]);
					}
					printf("\n");
				}
				Sleep(50);	// ��� �� �ٷ� ������� ���� ����
			}
			
			// ��ũ�� �¾Ҵٸ� ���� ����
			if (hit != 0) break;

			// �׷ȴ� �������� ����
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
				// ������ �Ҹ�
				for(i=0; i<USERS; i++){
					laser[i].tank_x = 0;
					laser[i].tank_y = 0;
				}

				// ��ũ �ٽ� �׸���
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
			
			// �� ���
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
	// ���� close...
	closesocket( ClientSocket );
	
	// ���� DLL: close()
	WSACleanup();

	// �� �ʱ�ȭ
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

	// �¸� �޽��� ���
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

	// �� ���
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