#include<graphics.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LENGTH 1024

#define BK 1
#define LINE 2
#define FILL 3

// �����򲻸ı����ֱ���ɫ��ֻ�ı�������ɫ
#define C_RED 4
#define C_GREEN 2
#define C_BLUE 1
#define C_YELLOW 6
#define C_CYAN 3
#define C_MAGENTA 5
#define C_WHITE 7
#define C_H_RED 12
#define C_H_GREEN 10
#define C_H_BLUE 9
#define C_H_YELLOW 14
#define C_H_CYAN 11
#define C_H_MAGENTA 13
#define C_H_WHITE 15

// 
#define CSTR_PARSECMD 1
#define CSTR_GETPARAMETER 2
#define CSTR_PARSEPARAMETER 3
#define CSTR_DOCMD 4
#define CSTR_WIDTH 5
#define CSTR_HEIGHT 6
#define CSTR_LEFT 7
#define CSTR_TOP 8
#define CTYPE_INT 10
#define CTYPE_LONG 11
#define CTYPE_FLOAT 12
#define CTYPE_DOUBLE 13
#define CTYPE_BOOLEAN 14

// Ҫת��ΪСд������Ŀ��
#define LOWERTARGET_CMD 1
#define LOWERTARGET_PARAMETER 2
#define LOWERTARGET_PARAMETERTEMP 3

// �Ƿ���debugģʽ
bool debug = true;

int ParameterI = 0;
char Parameter[MAX_LENGTH] = {};
int ParameterTempI = 0;
char ParameterTemp[MAX_LENGTH] = {};

char cmd[MAX_LENGTH] = {};							// �洢����
char cmdOld[MAX_LENGTH] = {};						// �洢������ʷ

HWND ChWnd = GetConsoleWindow();					// ����̨���
RECT Console;

int getNextParameter();								// ��ȡ��һ������
void setcolorbystring(char color[],int target);		// ͨ���ַ���������ɫ
bool IsStringColor(char color[]);					// ��������Ƿ�����ɫ
void setColor(int target);							// ��ĳ��������ɫ
void moveConsole(int left, int top);				// �ƶ�����̨
void lowerArray(int target);						// ������ת��ΪСд
void setConsoleColor(int Mode);						// ���ÿ���̨�ı���ɫ
void putConstStr(int StrID);

int main()
{
	bool invalidCmd = true;				// �Ƿ�Ϊ��Ч����
	bool hasForm = false;				// �Ƿ��Ѿ������˴���
	int cmd_i = 1;						// �ڼ�������	
	HWND hWnd;							// ������
	int width = 640, height = 480;		// Ĭ�ϵĿ��
	
	GetWindowRect(ChWnd, &Console);		// ��ȡ����̨λ�ã���С

	setConsoleColor(C_WHITE);
	printf("CodeDraw 1.0.0.0\n");
	printf("ʹ���� EasyX �⣬���ǵĹ����ǣ�https://easyx.cn/\n");
	printf("gitee��ַ:https://gitee.com/ruxia-tjy/CodeDraw\n");
	printf("github��ַ:https://github.com/ruxia-TJY/CodeDraw\n");
	
	if (debug) {
		setConsoleColor(C_H_RED);
		printf("\n<%d>:", cmd_i);
	}		
	else {
		setConsoleColor(C_H_BLUE);
		printf("\n[%d]:", cmd_i);
	}	
	setConsoleColor(C_WHITE);
	// ��ȡ����
	scanf("%s", cmd);
	// ��cmd����ת��ΪСд
	lowerArray(LOWERTARGET_CMD);

	// ���Ϊexit��ִ��ѭ��
	while (strcmp(cmd, "exit")) {		
		// ��ȡ����		
		gets_s(Parameter);
		lowerArray(LOWERTARGET_PARAMETER);
		if (debug) {			
			putConstStr(CSTR_PARSECMD);			
			printf("%s\n", cmd);
			putConstStr(CSTR_GETPARAMETER);
			printf("%s\n", Parameter);
		}		

		// ����Ϊform����������
		if (!strcmp(cmd, "form")) {			
			if (getNextParameter()) {
				sscanf(ParameterTemp, "%d", &width);
				if (getNextParameter()) {
					sscanf(ParameterTemp, "%d", &height);					
				}
			}
			// ��������
			initgraph(width, height, SHOWCONSOLE);
			// ���д��������̨
			MoveWindow(ChWnd, 0, 0, 600, 400, TRUE);
			GetWindowRect(ChWnd, &Console);
			hWnd = GetHWnd(); 
			MoveWindow(hWnd, Console.right, Console.top, width, height, FALSE);
			
			// ����̨���ý���
			SetFocus(ChWnd);
			
			if (debug) {				
				putConstStr(CSTR_PARSEPARAMETER);				
				putConstStr(CTYPE_INT);
				putConstStr(CSTR_WIDTH);				
				printf("%d,", width);
				putConstStr(CTYPE_INT);
				putConstStr(CSTR_HEIGHT);
				setConsoleColor(C_WHITE);
				printf("%d\n", height);

				putConstStr(CSTR_DOCMD);
				printf("\n\t1.����%dx%d����", width, height);
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tinitgraph(%d,%d,SHOWCONSOLE)\n", width, height);
				setConsoleColor(C_WHITE);
				printf("\t2.�ƶ�����̨��(%d,%d)",Console.left,Console.top);
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tMoveWindow(����̨���,%d,%d,%d,%d,TRUE)\n", Console.left, Console.top, Console.right - Console.left, Console.bottom - Console.top);
				setConsoleColor(C_WHITE);
				printf("\t3.�ƶ�������(%d,%d)", Console.right,Console.top);
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tMoveWindow(������,%d,%d,%d,%d,FALSE)\n", Console.right, Console.top, width, height);
				setConsoleColor(C_WHITE);
				printf("\t4.����̨�õ�����");
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tSetFocus(����̨���)\n");				
			}
			hasForm = true;
			invalidCmd = false;
		}
		
		// ����Ϊmove���ƶ���������̨
		if (!strcmp(cmd, "move")) {			
			int left = 0, top = 0;

			// ����ƶ�������
			if (getNextParameter() && !strcmp(ParameterTemp, "console")) {
				if (getNextParameter()) {
					sscanf(ParameterTemp, "%d", &left);
					if (getNextParameter()) {
						sscanf(ParameterTemp, "%d", &top);
						moveConsole(left, top);
					}
				}
				if (debug) {
					printf("��������:<int>��%d<int>����%d\n", left, top);
					printf("ִ�в���:\n");
					printf("\t1.�ƶ�����̨��(%d,%d)\t\tMoveWindow(����̨���,%d,%d,%d,%d ,FALSE)\n", left, top, left, top, Console.right - Console.left, Console.bottom - Console.top);
				}
			}
			else {
				if (hasForm) {
					sscanf(ParameterTemp, "%d", &left);
					if (getNextParameter()) {
						sscanf(ParameterTemp, "%d", &top);
						MoveWindow(hWnd, left, top, width, height, FALSE);
					}
				}
				if (debug) {
					printf("��������:<int>��%d,<int>����%d\n", left, top);					
					printf("ִ�в���:\n"); 
					if(hasForm)
						printf("\t1.�ƶ�������(%d,%d)\t\tMoveWindow(������,%d,%d,%d,%d,FALSE)\n", left, top, left, top, width, height);
					else
						printf("\t�޴��壬δִ���κβ���\n");
				}
			}						
			invalidCmd = false;
		}

		// ����Ϊcls,�����������
		if (!strcmp(cmd, "cls")) {
			if (hasForm) {
				cleardevice();
			}
			invalidCmd = false;		
		}

		// ����Ϊclose���رմ���
		if (!strcmp(cmd, "close")) {
			// ����д��壬ȷ��
			if (hasForm) {
				printf("ȷ��ɾ����(y/n):");
				char temp = getchar();
				// ����y����Enter����ִ������
				if (temp == 'y' || temp == '\n') {
					closegraph();
					hasForm = false;					
				}
				if (debug) {
					printf("�رմ���\n");
					printf("��Ч���룺closegraph()\n");
				}
			}
			invalidCmd = false;
		}

		// ����Ϊsetcolor,������ɫ
		if (!strcmp(cmd, "setcolor")) {
			if (hasForm) {
				// ��ȡ����Ŀ��
				if (getNextParameter()) {
					if (!strcmp(ParameterTemp, "bk")) {
						setColor(BK);
						cleardevice();
					}
					else if (!strcmp(ParameterTemp, "line")) {
						setColor(LINE);
					}
					else if (!strcmp(ParameterTemp,"fill")) {
						setColor(FILL);
					}
				}				
			}
			invalidCmd = false;
		}

		// ����Ϊline,����
		if (!strcmp(cmd, "line")) {
			if (hasForm) {
				int x1, x2, y1, y2;
				if (getNextParameter()) {
					sscanf(ParameterTemp, "%d", &x1);
					if (getNextParameter()) {
						sscanf(ParameterTemp, "%d", &y1);
						if (getNextParameter()) {
							sscanf(ParameterTemp, "%d", &x2);
							if (getNextParameter()) {
								sscanf(ParameterTemp, "%d", &y2);
								line(x1, y1, x2, y2);
							}
							else
								printf("�������㣡");
						}
						else
							printf("�������㣡");
					}
					else
						printf("�������㣡");
				}
				else
					printf("�������㣡");
			}
			invalidCmd = false;
		}

		// ����Ϊcircle,��Բ
		if (!strcmp(cmd, "circle")) {
			if (hasForm) {
				int x, y, radius;
				int Mode = 0;
				if (getNextParameter()) {
					if (!strcmp(ParameterTemp, "fill"))
						Mode = 1;
					else if (!strcmp(ParameterTemp, "solid"))
						Mode = 2;
					else if (!strcmp(ParameterTemp, "clear"))
						Mode = 3;
					else
						sscanf(ParameterTemp, "%d", &x);
					if (!Mode) {
						if (getNextParameter()) {
							sscanf(ParameterTemp, "%d", &y);
							if (getNextParameter()) {
								sscanf(ParameterTemp, "%d", &radius);								
							}
						}
					}
					else {
						if (getNextParameter()) {
							sscanf(ParameterTemp, "%d", &x);
							if (getNextParameter()) {
								sscanf(ParameterTemp, "%d", &y);
								if (getNextParameter()) {
									sscanf(ParameterTemp, "%d", &radius);
								}
							}
						}
					}
					if (Mode == 0)
						circle(x, y, radius);
					else if (Mode == 1)
						fillcircle(x, y, radius);
					else if (Mode == 2)
						solidcircle(x, y, radius);
					else if (Mode == 3)
						clearcircle(x, y, radius);

				}
			}
			invalidCmd = false;
		}

		// ����Ϊrectangle��������
		if (!strcmp(cmd, "rectangle")) {
			if (hasForm) {
				int left, top, right, bottom;
				int Mode = 0;
				if (getNextParameter()) {
					if (!strcmp(ParameterTemp, "fill"))
						Mode = 1;
					else if (!strcmp(ParameterTemp, "solid"))
						Mode = 2;
					else if (!strcmp(ParameterTemp, "clear"))
						Mode = 3;
					else
						sscanf(ParameterTemp, "%d", &left);
					if (!Mode) {
						if (getNextParameter()) {
							sscanf(ParameterTemp, "%d", &top);
							if (getNextParameter()) {
								sscanf(ParameterTemp, "%d", &right);
								if (getNextParameter()) {
									sscanf(ParameterTemp, "%d", &bottom);									
								}
							}
						}
					}
					else {
						if (getNextParameter()) {
							sscanf(ParameterTemp, "%d", &left);
							if (getNextParameter()) {
								sscanf(ParameterTemp, "%d", &top);
								if (getNextParameter()) {
									sscanf(ParameterTemp, "%d", &right);
									if (getNextParameter()) {
										sscanf(ParameterTemp, "%d", &bottom);
									}
								}
							}
						}
					}
					if (Mode == 0)
						rectangle(left, top, right, bottom);
					else if (Mode == 1)
						fillrectangle(left, top, right, bottom);
					else if (Mode == 2)
						solidrectangle(left, top, right, bottom);
					else if (Mode == 3)
						clearrectangle(left, top, right, bottom);
				}
			}

			invalidCmd = false;
		}

		// ��debugģʽ
		if (!strcmp(cmd, "debug")) {
			debug = !debug;
			if (debug)
				printf("debug �ѿ���\n");
			else
				printf("debug �ѹر�\n");
			invalidCmd = false;
		}
		
		// ����Ϊclear,���������
		if (!strcmp(cmd, "clear")) {
			system("cls");
			invalidCmd = false;
		}

		// �������Ч��������ʾ
		if (invalidCmd)printf(" %s ������ڣ�\n", cmd);

		// ��ʼ������
		ParameterI = 0;
		cmd_i++;
		invalidCmd = true;
		// ��ȡ��һ������
		if (debug) {
			setConsoleColor(C_H_RED);
			printf("\n<%d>:", cmd_i);
		}
		else {
			setConsoleColor(C_H_BLUE);
			printf("\n[%d]:", cmd_i);
		}
		setConsoleColor(C_WHITE);
		scanf("%s", cmd);
		lowerArray(LOWERTARGET_CMD);
	}

	// �˳�������д��壬�ر���
	if (hasForm)closegraph();
}

// ��ȡ�¸�����
int getNextParameter()
{	
	memset(ParameterTemp, 0, sizeof(ParameterTemp));
	ParameterTempI = 0;
	
	// ���ǰ��ո�
	while (Parameter[ParameterI] == 32 && Parameter[ParameterI] != '\0')
		ParameterI++;
	
	// ����±�С�����鳤�ȣ�˵�����ڲ���������1
	if (ParameterI < strlen(Parameter)) {		
		// ����������ParameterTemp������
		while (Parameter[ParameterI] != 32 && Parameter[ParameterI] != '\0') {
			ParameterTemp[ParameterTempI] = Parameter[ParameterI];
			ParameterTempI++;
			ParameterI++;
		}
		return 1;
	}
	else
		return 0;
}

// ͨ���ַ�������ɫ
void setcolorbystring(char color[],int target)
{
	int Color;
	if (!strcmp(color, "black")) {
		Color = BLACK;
	}
	if (!strcmp(color, "blue")) {
		Color = BLUE;		
	}	
	if (!strcmp(color, "green")) {
		Color = GREEN;		
	}
	if (!strcmp(color, "cyan")) {
		Color = CYAN;
	}
	if (!strcmp(color, "red")) {
		Color = RED;
	}
	if (!strcmp(color, "maenta")) {
		Color = MAGENTA;
	}
	if (!strcmp(color, "brown")) {
		Color = BROWN;
	}
	if (!strcmp(color, "lightgray")) {
		Color = LIGHTGRAY;
	}
	if (!strcmp(color, "darkgray")) {
		Color = DARKGRAY;
	}
	if (!strcmp(color, "lightblue")) {
		Color = LIGHTBLUE;
	}
	if (!strcmp(color, "lightgreen")) {
		Color = LIGHTGREEN;
	}
	if (!strcmp(color, "lightcyan")) {
		Color = LIGHTCYAN;
	}
	if (!strcmp(color, "lightred")) {
		Color = LIGHTRED;
	}
	if (!strcmp(color, "lightmagenta")) {
		Color = LIGHTMAGENTA;
	}
	if (!strcmp(color, "yellow")) {
		Color = YELLOW;
	}
	if (!strcmp(color, "white")) {
		Color = WHITE;
	}

	if (target = BK) {
		setbkcolor(Color);
	}
	else if (target = LINE) {
		setlinecolor(Color);
	}
	else if (target = FILL) {
		setfillcolor(Color);
	}
}

// �Ƿ�����ɫ����
// color:��ɫ
bool IsStringColor(char color[])
{
	if (!strcmp(color, "black") ||
		!strcmp(color, "blue") ||
		!strcmp(color, "green") ||
		!strcmp(color, "cyan") ||
		!strcmp(color, "red") ||
		!strcmp(color, "maenta") ||
		!strcmp(color, "brown") ||
		!strcmp(color, "lightgray") ||
		!strcmp(color, "darkgray") ||
		!strcmp(color, "lightblue") ||
		!strcmp(color, "lightgreen") ||
		!strcmp(color, "lightcyan") ||
		!strcmp(color, "lightred") ||
		!strcmp(color, "lightmagenta") ||
		!strcmp(color, "yellow") ||
		!strcmp(color, "white")
		)
		return true;
	else
		return false;

}

// ��ĳ��������ɫ
// target Ŀ��
void setColor(int target)
{
	// ������ɫ�����ַ�ʽ
	// setbkcolor rgb 255 0 0		RGB
	// setbkcolor RED				����
	// setbkcolor ff0000			ʮ������
	if (getNextParameter()) {
		if (!strcmp(ParameterTemp, "rgb")) {
			int r, g, b;
			if (getNextParameter()) {
				sscanf(ParameterTemp, "%d", &r);
				if (getNextParameter()) {
					sscanf(ParameterTemp, "%d", &g);
					if (getNextParameter()) {
						sscanf(ParameterTemp, "%d", &b);
						setbkcolor(RGB(r, g, b));
					}
					else
						printf("��Bֵ");
				}
				else
					printf("��G,Bֵ\n");
			}
			else
				printf("��R,G,Bֵ\n");
		}
		else if (IsStringColor(ParameterTemp)) {
			setcolorbystring(ParameterTemp,target);
		}
		else {
			int color;
			sscanf(ParameterTemp, "%x", &color);
			setbkcolor(BGR(color));
		}		
	}
}

// �ƶ�����̨
void moveConsole(int left, int top)
{	
	MoveWindow(ChWnd, left, top, Console.right - Console.left, Console.bottom - Console.top, FALSE);
	GetWindowRect(ChWnd, &Console);
}

// ��Ŀ������ת��ΪСд
// target:Ŀ��
void lowerArray(int target)
{
	if (target == LOWERTARGET_CMD) {
		for (int i = 0; i < strlen(cmd); i++) {
			if (cmd[i] >= 'A' && cmd[i] <= 'Z') {
				cmd[i] += 32;
			}
		}
	}
	else if (target == LOWERTARGET_PARAMETER) {
		for (int i = 0; i < strlen(Parameter); i++) {
			if (Parameter[i] >= 'A' && Parameter[i] <= 'Z') {
				Parameter[i] += 32;
			}
		}
	}
}

// ���ÿ���̨�ı���ɫ
void setConsoleColor(int Mode)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Mode);
}

void putConstStr(int StrID)
{
	if (StrID >= CSTR_PARSECMD && StrID <= CSTR_TOP) {
		setConsoleColor(C_H_YELLOW);
		switch (StrID) {
		case CSTR_PARSECMD:
			printf("��������:");
			break;
		case CSTR_GETPARAMETER:
			printf("��ȡ����:");
			break;
		case CSTR_PARSEPARAMETER:
			printf("��������:");
			break;
		case CSTR_DOCMD:
			printf("ִ������:");
			break;
		case CSTR_WIDTH:
			printf("��:");
			break;
		case CSTR_HEIGHT:
			printf("��:");
			break;
		}
	}
	else if (StrID >= CTYPE_INT && StrID <= CTYPE_BOOLEAN) {
		setConsoleColor(C_H_CYAN);
		switch (StrID) {
		case CTYPE_INT:
			printf("<int>"); break;
		case CTYPE_LONG:
			printf("<long>"); break;
		case CTYPE_FLOAT:
			printf("<float>"); break;
		case CTYPE_DOUBLE:
			printf("<double>"); break;
		case CTYPE_BOOLEAN:
			printf("<boolean>"); break;
		}
	}
	
	setConsoleColor(C_WHITE);
}