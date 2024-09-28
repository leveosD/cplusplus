//������ ��������� ��������� ������ �� ����� � 
#include <iostream>
#include <stdio.h>

int main() {
	using std::cout;
	using std::cin;

	FILE* bin;
	FILE* text;
	fopen_s(&bin, "binary", "rb"); //������ �������� ����
	fopen_s(&text, "text.txt", "w"); //���������� � ���������
	if (bin == NULL || text == NULL)
		return 0;
	int max_arr[100]; //������ � ���������� ����������� ������ ������ �����(>0)
	int cur_arr[100]; //������ � ������� ����� ������������� �����
	memset(cur_arr, 0, 100 * sizeof(int));
	memset(max_arr, 0, 100 * sizeof(int)); //��������� ��� ������� ������
	int n;
	int i = 0;
	int max_size = 0;
	while (fread(&n, sizeof(n), 1, bin)) { //������ �� �����
		cout << n << std::endl;
		if (n > 0) { //���� ����� >0 ���������� � ������� ������
			cur_arr[i] = n;
			i++;
		}
		else {
			if (i > max_size) { //���� ��� ������������� ����� ���������� � �� ������ �����������
				max_size = i;
				for (int k = 0; k < max_size; k++)
					max_arr[k] = cur_arr[k]; // �� ���������� � ���������� ������� ������
			}
			i = 0;
			memset(cur_arr, 0, 100 * sizeof(int)); //������ �������� �������
		}
	}
	//���� ����� ����������� �� ����� ������� ������,�� ���������� ���
	if (i > max_size) {
		max_size = i;
		for (int k = 0; k < max_size; k++)
			max_arr[k] = cur_arr[k];
	}
	cout << "----------\n";
	//����� ����������� ������� �� ����� � � ����
	for (int k = 0; k < max_size; k++) {
		cout << max_arr[k] << std::endl;
		int len = 0; //����� ����� � ��������
		int x = max_arr[k];
		while (x > 0) {
			x = (int)(x / 10);
			len++;
		}
		char* str = new char[len + 1]; //������ ��� ������ �����
		sprintf_s(str, len + 1, "%d", max_arr[k]); //�������� ���� ����� � ��������
		fwrite(str, len, 1, text); // � ���������� �� � ����
		fputs("\n", text);
	}
	//��������� ��� �����
	fclose(text);
	fclose(bin);
} 