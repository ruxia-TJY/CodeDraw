#include<graphics.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LENGTH 1024

#define BK 1
#define LINE 2
#define FILL 3

// 本程序不改变文字背景色，只改变文字颜色
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

// 要转换为小写的数组目标
#define LOWERTARGET_CMD 1
#define LOWERTARGET_PARAMETER 2
#define LOWERTARGET_PARAMETERTEMP 3

// 是否开启debug模式
bool debug = true;

int ParameterI = 0;
char Parameter[MAX_LENGTH] = {};
int ParameterTempI = 0;
char ParameterTemp[MAX_LENGTH] = {};

char cmd[MAX_LENGTH] = {};							// 存储命令
char cmdOld[MAX_LENGTH] = {};						// 存储命令历史

HWND ChWnd = GetConsoleWindow();					// 控制台句柄
RECT Console;

int getNextParameter();								// 获取下一个参数
void setcolorbystring(char color[],int target);		// 通过字符串设置颜色
bool IsStringColor(char color[]);					// 检查数组是否是颜色
void setColor(int target);							// 对某项设置颜色
void moveConsole(int left, int top);				// 移动控制台
void lowerArray(int target);						// 将数组转换为小写
void setConsoleColor(int Mode);						// 设置控制台文本颜色
void putConstStr(int StrID);

int main()
{
	bool invalidCmd = true;				// 是否为无效命令
	bool hasForm = false;				// 是否已经创建了窗体
	int cmd_i = 1;						// 第几条命令	
	HWND hWnd;							// 窗体句柄
	int width = 640, height = 480;		// 默认的宽高
	
	GetWindowRect(ChWnd, &Console);		// 获取控制台位置，大小

	setConsoleColor(C_WHITE);
	printf("CodeDraw 1.0.0.0\n");
	printf("使用了 EasyX 库，他们的官网是：https://easyx.cn/\n");
	printf("gitee地址:https://gitee.com/ruxia-tjy/CodeDraw\n");
	printf("github地址:https://github.com/ruxia-TJY/CodeDraw\n");
	
	if (debug) {
		setConsoleColor(C_H_RED);
		printf("\n<%d>:", cmd_i);
	}		
	else {
		setConsoleColor(C_H_BLUE);
		printf("\n[%d]:", cmd_i);
	}	
	setConsoleColor(C_WHITE);
	// 读取命令
	scanf("%s", cmd);
	// 将cmd数组转换为小写
	lowerArray(LOWERTARGET_CMD);

	// 命令不为exit则执行循环
	while (strcmp(cmd, "exit")) {		
		// 读取参数		
		gets_s(Parameter);
		lowerArray(LOWERTARGET_PARAMETER);
		if (debug) {			
			putConstStr(CSTR_PARSECMD);			
			printf("%s\n", cmd);
			putConstStr(CSTR_GETPARAMETER);
			printf("%s\n", Parameter);
		}		

		// 命令为form，创建窗体
		if (!strcmp(cmd, "form")) {			
			if (getNextParameter()) {
				sscanf(ParameterTemp, "%d", &width);
				if (getNextParameter()) {
					sscanf(ParameterTemp, "%d", &height);					
				}
			}
			// 创建窗体
			initgraph(width, height, SHOWCONSOLE);
			// 排列窗体与控制台
			MoveWindow(ChWnd, 0, 0, 600, 400, TRUE);
			GetWindowRect(ChWnd, &Console);
			hWnd = GetHWnd(); 
			MoveWindow(hWnd, Console.right, Console.top, width, height, FALSE);
			
			// 控制台设置焦点
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
				printf("\n\t1.创建%dx%d窗体", width, height);
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tinitgraph(%d,%d,SHOWCONSOLE)\n", width, height);
				setConsoleColor(C_WHITE);
				printf("\t2.移动控制台至(%d,%d)",Console.left,Console.top);
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tMoveWindow(控制台句柄,%d,%d,%d,%d,TRUE)\n", Console.left, Console.top, Console.right - Console.left, Console.bottom - Console.top);
				setConsoleColor(C_WHITE);
				printf("\t3.移动窗体至(%d,%d)", Console.right,Console.top);
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tMoveWindow(窗体句柄,%d,%d,%d,%d,FALSE)\n", Console.right, Console.top, width, height);
				setConsoleColor(C_WHITE);
				printf("\t4.控制台得到焦点");
				setConsoleColor(C_H_MAGENTA);
				printf("\t\tSetFocus(控制台句柄)\n");				
			}
			hasForm = true;
			invalidCmd = false;
		}
		
		// 命令为move，移动窗体或控制台
		if (!strcmp(cmd, "move")) {			
			int left = 0, top = 0;

			// 如果移动命令行
			if (getNextParameter() && !strcmp(ParameterTemp, "console")) {
				if (getNextParameter()) {
					sscanf(ParameterTemp, "%d", &left);
					if (getNextParameter()) {
						sscanf(ParameterTemp, "%d", &top);
						moveConsole(left, top);
					}
				}
				if (debug) {
					printf("解析参数:<int>左：%d<int>顶：%d\n", left, top);
					printf("执行操作:\n");
					printf("\t1.移动控制台至(%d,%d)\t\tMoveWindow(控制台句柄,%d,%d,%d,%d ,FALSE)\n", left, top, left, top, Console.right - Console.left, Console.bottom - Console.top);
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
					printf("解析参数:<int>左：%d,<int>顶：%d\n", left, top);					
					printf("执行操作:\n"); 
					if(hasForm)
						printf("\t1.移动窗体至(%d,%d)\t\tMoveWindow(窗体句柄,%d,%d,%d,%d,FALSE)\n", left, top, left, top, width, height);
					else
						printf("\t无窗体，未执行任何操作\n");
				}
			}						
			invalidCmd = false;
		}

		// 命令为cls,清除窗体内容
		if (!strcmp(cmd, "cls")) {
			if (hasForm) {
				cleardevice();
			}
			invalidCmd = false;		
		}

		// 命令为close，关闭窗体
		if (!strcmp(cmd, "close")) {
			// 如果有窗体，确认
			if (hasForm) {
				printf("确定删除？(y/n):");
				char temp = getchar();
				// 输入y或按下Enter键，执行命令
				if (temp == 'y' || temp == '\n') {
					closegraph();
					hasForm = false;					
				}
				if (debug) {
					printf("关闭窗体\n");
					printf("等效代码：closegraph()\n");
				}
			}
			invalidCmd = false;
		}

		// 命令为setcolor,设置颜色
		if (!strcmp(cmd, "setcolor")) {
			if (hasForm) {
				// 获取设置目标
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

		// 命令为line,画线
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
								printf("参数不足！");
						}
						else
							printf("参数不足！");
					}
					else
						printf("参数不足！");
				}
				else
					printf("参数不足！");
			}
			invalidCmd = false;
		}

		// 命令为circle,画圆
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

		// 命令为rectangle，画矩形
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

		// 打开debug模式
		if (!strcmp(cmd, "debug")) {
			debug = !debug;
			if (debug)
				printf("debug 已开启\n");
			else
				printf("debug 已关闭\n");
			invalidCmd = false;
		}
		
		// 命令为clear,清空命令行
		if (!strcmp(cmd, "clear")) {
			system("cls");
			invalidCmd = false;
		}

		// 如果是无效命令，输出提示
		if (invalidCmd)printf(" %s 命令不存在！\n", cmd);

		// 初始化变量
		ParameterI = 0;
		cmd_i++;
		invalidCmd = true;
		// 获取下一个命令
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

	// 退出后，如果有窗体，关闭他
	if (hasForm)closegraph();
}

// 获取下个参数
int getNextParameter()
{	
	memset(ParameterTemp, 0, sizeof(ParameterTemp));
	ParameterTempI = 0;
	
	// 清除前面空格
	while (Parameter[ParameterI] == 32 && Parameter[ParameterI] != '\0')
		ParameterI++;
	
	// 如果下标小于数组长度，说明存在参数，返回1
	if (ParameterI < strlen(Parameter)) {		
		// 将参数导入ParameterTemp数组中
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

// 通过字符设置颜色
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

// 是否是颜色常量
// color:颜色
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

// 对某项设置颜色
// target 目标
void setColor(int target)
{
	// 设置颜色有三种方式
	// setbkcolor rgb 255 0 0		RGB
	// setbkcolor RED				文字
	// setbkcolor ff0000			十六进制
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
						printf("无B值");
				}
				else
					printf("无G,B值\n");
			}
			else
				printf("无R,G,B值\n");
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

// 移动控制台
void moveConsole(int left, int top)
{	
	MoveWindow(ChWnd, left, top, Console.right - Console.left, Console.bottom - Console.top, FALSE);
	GetWindowRect(ChWnd, &Console);
}

// 将目标数组转换为小写
// target:目标
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

// 设置控制台文本颜色
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
			printf("解析命令:");
			break;
		case CSTR_GETPARAMETER:
			printf("获取参数:");
			break;
		case CSTR_PARSEPARAMETER:
			printf("解析参数:");
			break;
		case CSTR_DOCMD:
			printf("执行命令:");
			break;
		case CSTR_WIDTH:
			printf("宽:");
			break;
		case CSTR_HEIGHT:
			printf("高:");
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