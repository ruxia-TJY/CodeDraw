#include<graphics.h>
#include<stdio.h>
#include<string.h>

#define MAX_LENGTH 1024

bool debug = false;

int ParameterI = 0;
char Parameter[MAX_LENGTH] = {};
int ParameterTempI = 0;
char ParameterTemp[MAX_LENGTH] = {};

int getNextParameter();					// ��ȡ��һ������

int main()
{
	bool noCmd = true;					// �Ƿ�Ϊ��Ч����
	bool hasForm = false;				// �Ƿ��Ѿ������˴���
	int cmd_i = 1;						// �ڼ�������
	char cmd[MAX_LENGTH] = {};			// �洢����
	char cmdOld[MAX_LENGTH] = {};		// �洢������ʷ
	HWND hWnd;							// ������
	int width = 640, height = 480;		// Ĭ�ϵĿ��


	printf("CodeDraw 1.0.0.0\n");
	printf("ʹ���� EasyX �⣬���ǵĹ����ǣ�https://easyx.cn/\n");
	printf("������Ŀ��ַ:https://gitee.com/ruxia-tjy/CodeDraw\n");
	printf("\n[%d]:", cmd_i);
	// ��ȡ����
	scanf("%s", cmd);
	
	// ���Ϊexit��ִ��ѭ�������˳�����
	while (strcmp(cmd, "exit")) {		
		// ��ȡ����		
		gets_s(Parameter);			

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
			// ��ȡ������
			hWnd = GetHWnd();
			hasForm = true;
			noCmd = false;
			if (debug) {
				printf("����һ�� %dx%d ��С�Ĵ���\n", width, height);
				printf("��Ч���룺initgraph(%d,%d,SHOWCONSOLE);\n", width, height);
				printf("CodeDraw���Զ���ȡ��������ľ������Ӧ����Ϊ��GetHWnd();\n");
			}
		}
		
		// ����λmove���ƶ�����
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
					printf("�ƶ����嵽 (%d,%d)\n",left,top);
					printf("��Ч���룺MoveWindow(���,%d,%d,%d,%d,FALSE)\n", left, top, width, height);
				}
			}
			noCmd = false;
		}

		// �����Ļ����
		if (!strcmp(cmd, "cls")) {
			if (hasForm) {
				cleardevice();
			}
			noCmd = false;
			if (debug) {
				printf("�����������\n");
				printf("��Ч���룺cleardevice()\n");
			}
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
					noCmd = false;
				}
				if (debug) {
					printf("�رմ���\n");
					printf("��Ч���룺closegraph()\n");
				}
			}
		}

		if (!strcmp(cmd, "debug")) {
			debug = !debug;
			if (debug)
				printf("debug �ѿ���\n");
			else
				printf("debug �ѹر�\n");
			noCmd = false;
		}
		
		if (noCmd)
			printf(" %s ������ڣ�\n", cmd);


		ParameterI = 0;
		cmd_i++;
		noCmd = true;
		printf("\n[%d]:", cmd_i);
		scanf("%s", cmd);
	}

	// �˳�������д��壬�ر���
	if (hasForm)closegraph();
}

// ��ȡ�¸���������������
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