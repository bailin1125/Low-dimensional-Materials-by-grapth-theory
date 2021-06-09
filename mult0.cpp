#include <omp.h>
#include <cstdio>
#include <cstring>
#include <iostream>
//#include "read_atom.hpp"
#include <unistd.h>
#include <string>
#include <time.h>
using namespace std;
#pragma warning(disable : 4996)
const int file_num =88159 ;
const int thred_num = 20;
int main(int argc, const char * argv[]) {
	const int start_a = 0;
	int i = 0, j = 0;
	int n = 0;
	//float start = omp_get_wtime();
	char now_name[25][100];
	char mingling1[200] = "./cut.out ";
	//��һ����ѭ��ʹ�õ���������
	//char mingling2[50] = "/atom.config";
	char mingling3[300] = "  /share/home/wangz/2d_search/ridus_cut/result/result_ionicandval/0103/my_1d/mz_config/";
	char temp[200];
	char cp_temp[100] = "cp ../database/BDM/files/";
	char rm_mingling[100] = "rm 2d_search/new_files/";
	char rm_temp[100];
	char file_namepath[100] = "right_file";
	FILE* name = fopen(file_namepath, "r");
	if (name == NULL)
	{
		cout << "i can not find the name file!" << endl;
		cout << "now the path is:" << file_namepath << endl;
		cin.get();
		return 0;
	}

	//cout << "start is:" << start_a << endl;
	for ( i = 0; i < start_a; i++)
	{
		fgets(temp, 200, name);
	}
	//��ȡ����Ӧ������������ʼ��������
#pragma omp parallel for private(n) schedule(dynamic)
	for (i = 0; i <14693 ; i++)
	{		
		//if (feof(name))
			//break;
#pragma omp critical
		{
			n = omp_get_thread_num();
			fscanf(name, "%s", now_name[n]);
			//cout << "the threads xuhao  is:" << n << endl;
			//cout << "the name is:" << now_name[n] << endl;
			//cin.get();
		}		
		
		if (i % 100 == 0)
		{
			cout << "has gone :"<<i << endl;
		}
			
		//int num = omp_get_thread_num();
		//cout << "threds num:" << num;
		//cout << "now the :" << i << "files" << endl;		
		//test
		//cout<<"now the name is:"<<now_name<<endl;
		//cin.get();
		char temp_mingling[300];
		char temp_3[100];
		strcpy(temp_mingling, mingling1);
		strcat(temp_mingling, now_name[n]);

		strcpy(temp_3, mingling3);
		strcat(temp_3, now_name[n]);
		strcat(temp_mingling, temp_3);
		//cout<<"now the mingling is:"<<temp_mingling<<endl;
		//cin.get();
		if (system(temp_mingling) !=0)
		{
			//char cp_mingling[300];
			//strcpy(cp_mingling, cp_temp);
			//strcat(cp_mingling, now_name[n]);
			//strcat(cp_mingling, mingling2);
			//strcat(cp_mingling, " 2d_search/files/");
			//strcat(cp_mingling, now_name[n]);
			//strcat(cp_mingling, ".config");
			////cout << "the cp mingling is:" << cp_mingling << endl;
			//system(cp_mingling);
			//cin.get();
		}
		/*else
		{
			strcpy(rm_temp, rm_mingling);
			strcat(rm_temp, now_name[n]);
			strcat(rm_temp, ".txt");
			system(rm_temp);
			cin.get();
		}*/
		
	}
	//float end = omp_get_wtime();
	//printf("has gone  :%lf:seconds\n",end-start);
	fclose(name);
	cout << "alla total work done!" << endl;
	//cin.get();
	return 0;
}












