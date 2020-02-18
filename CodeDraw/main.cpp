#include<graphics.h>
#include<stdio.h>
#include<string.h>

#define MAX_LENGTH 1024

bool debug = false;

int ParameterI = 0;
char Parameter[MAX_LENGTH] = {};
int ParameterTempI = 0;
char ParameterTemp[MAX_LENGTH] = {};

int getNextParameter();					// 获取下一个参数

int main()
{
	bool noCmd = true;					// 是否为无效命令
	bool hasForm = false;				// 是否已经创建了窗体
	int cmd_i = 1;						// 第几条命令
	char cmd[MAX_LENGTH] = {};			// 存储命令
	char cmdOld[MAX_LENGTH] = {};		// 存储命令历史
	HWND hWnd;							// 窗体句柄
	int width = 640, height = 480;		// 默认的宽高


	printf("CodeDraw 1.0.0.0\n");
	printf("使用了 EasyX 库，他们的官网是：https://easyx.cn/\n");
	printf("访问项目地址:https://gitee.com/ruxia-tjy/CodeDraw\n");
	printf("\n[%d]:", cmd_i);
	// 读取命令
	scanf("%s", cmd);
	
	// 命令不为exit则执行循环，不退出程序
	while (strcmp(cmd, "exit")) {		
		// 读取参数		
		gets_s(Parameter);			

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
			// 获取窗体句柄
			hWnd = GetHWnd();
			hasForm = true;
			noCmd = false;
			if (debug) {
				printf("创建一个 %dx%d 大小的窗体\n", width, height);
				printf("等效代码：initgraph(%d,%d,SHOWCONSOLE);\n", width, height);
				printf("CodeDraw会自动获取创建窗体的句柄，对应代码为：GetHWnd();\n");
			}
		}
		
		// 命令位move，移动窗体
		if (!strcmp(cmd, "move")) {			
			if (hasForm) {
				int left = 0, top = 0;
				
				if (getNextParameter()) {				
					sscanf(ParameterTemp, "%d", &left);					
					if (getNextParameter()) {						
						sscanf(ParameterTemp, "%d", &top);						
					}
				}				
				MoveWindow(hWnd, left, top, width, height, FALSE);
				if (debug) {
					printf("移动窗体到 (%d,%d)\n",left,top);
					printf("等效代码：MoveWindow(句柄,%d,%d,%d,%d,FALSE)\n", left, top, width, height);
				}
			}
			noCmd = false;
		}

		// 清除屏幕内容
		if (!strcmp(cmd, "cls")) {
			if (hasForm) {
				cleardevice();
			}
			noCmd = false;
			if (debug) {
				printf("清除窗体内容\n");
				printf("等效代码：cleardevice()\n");
			}
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
					noCmd = false;
				}
				if (debug) {
					printf("关闭窗体\n");
					printf("等效代码：closegraph()\n");
				}
			}
		}

		if (!strcmp(cmd, "debug")) {
			debug = !debug;
			if (debug)
				printf("debug 已开启\n");
			else
				printf("debug 已关闭\n");
			noCmd = false;
		}
		
		if (noCmd)
			printf(" %s 命令不存在！\n", cmd);


		ParameterI = 0;
		cmd_i++;
		noCmd = true;
		printf("\n[%d]:", cmd_i);
		scanf("%s", cmd);
	}

	// 退出后，如果有窗体，关闭他
	if (hasForm)closegraph();
}

// 获取下个参数数组首坐标
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