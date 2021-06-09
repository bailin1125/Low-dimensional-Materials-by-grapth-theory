#include "CONNECTION.h"
#include <fstream>
#include <iostream>


using namespace std;


int classify_metal_maingroup(int &atomic_number)//0 rare gas,1 metal ,2 main group
{
	
	int j = 0, k = 0, m = 0;
	while (j < main_groupnum || k < metal_num || m < rare_gasnum)
	{
		if (rare_gas[m] == atomic_number)
		{
			return 0;
		}
		if (meatal_xuhao[k] == atomic_number)
		{
			return 1;
		}
		if (main_group_element[j] == atomic_number)
		{
			return 2;
		}


		if (meatal_xuhao[k] < atomic_number)
		{
			k++;
			continue;
		}
		if (rare_gas[m] < atomic_number)
		{
			m++;
			continue;
		}
		if (main_group_element[j] < atomic_number)
		{
			j++;
			continue;
		}
	}
	cout << "what's wrong!" << atomic_number << endl;
	cin.get();
	return -1;

}
bool CheckAllmetal(int*list, cell& cell_a,int edge_num) {
	/*
	@parmaters:原子的序号列表，针对的cell结构
	@output：是否是全部金属原子,且只有一种金属元素
	*/
	int type = cell_a.type[list[0] % cell_a.num];
	bool res = true;
	for (int i = 1; i < edge_num; i++) {
		if (list[i] == -1)
			break;
		if (cell_a.type[list[i] % cell_a.num]!=type ||classify_metal_maingroup(cell_a.type[list[i] % cell_a.num]) != 1) {
			res = false;
			break;
		}
	}	
	return res;
}


cell::cell(char *name,element*e,vector<vector<double>>&max_ion,double**dist,int flag) 
{
	complexMalloc = true;
	
	int i, j, k;
	//cout << "expand the :" << cengshu << "layer" << endl;
	char temp[300];
	double x_pian = 0.0;
	double y_pian = 0.0;
	double z_pian = 0.0;
	//strcpy(wenjian, "atom1.config");
	FILE *in;
	in = fopen(name, "rt");
	//system("pause");
	if (in == NULL)
	{
		printf("error of rading atom.config!\n");
		printf("the filename is :%s\n", name);
		//cin.get();
		return;
	}
	fscanf(in, "%d", &num);
	
	if(flag==1)
		positive = new int[num];
	type = (int *)malloc(num * sizeof(int));
	letice = (double **)malloc(3 * sizeof(double *));
	for (i = 0; i < 3; i++)
	{
		letice[i] = (double *)malloc(3 * sizeof(double));
	}
	p = (double **)malloc(num * sizeof(double *));
	for (i = 0; i < num; i++)
	{
		p[i] = (double *)malloc(3 * sizeof(double));
	}
	real_position = (double ***)malloc(yanshen * sizeof(double **));
	for (i = 0; i < yanshen; i++)
	{
		real_position[i] = (double **)malloc(num * sizeof(double *));
		for (k = 0; k < num; k++)
			real_position[i][k] = (double *)malloc(3 * sizeof(double));
	}

	p_real = new double **[yanshen];
	for (i = 0; i < yanshen; i++)
	{
		p_real[i] = new double *[num];
		for (k = 0; k < num; k++)
		{
			p_real[i][k] = new double[3];
		}
	}
	if (flag == 1)
	{
		ridus = new double*[num];
		for (i = 0; i < num; i++)
		{
			ridus[i] = new double[2];
		}
	}
		
	while (fgets(temp, 300, in) != NULL)
	{
		if (strstr(temp, "VECTOR") != NULL || strstr(temp, "vector") != NULL)
			break;
	}
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(in, "%lf", &letice[i][j]);
		}
		fgets(temp, 300, in);
	}	

	fgets(temp, 300, in);
	//cout << temp << endl;
	char line[10];
	for (i = 0; i < num; i++)
	{

		fscanf(in, "%d", &type[i]);
		fscanf(in, "%lf", &p[i][0]);
		fscanf(in, "%lf", &p[i][1]);
		fscanf(in, "%lf", &p[i][2]);
		if (flag == 1)
		{
			fscanf(in, "%s", line);
			if (line[0] == 'p')
			{
				positive[i] = 1;
				fscanf(in, "%lf", &ridus[i][0]);
			}
			else if (line[0] == 's')//读到s的时候说明是单质
			{
				fscanf(in, "%s", line);
				fscanf(in, "%lf", &ridus[i][0]);
			}
			else//如果是negative的话，就读两个ridus值
			{
				positive[i] = -1;
				fscanf(in, "%lf", &ridus[i][0]);//第一个是负价态对应的半径
				fscanf(in, "%lf", &ridus[i][1]);//对应正价态的半径
			}
		}
		

		fgets(temp, 300, in);
	}
	//int x_xishu = 0;
	//int y_xishu = 0;
	//int z_zishu = 0;

	//增加记录type的记录
	int temp_save[120] = { 0 };
	type_num = 0;
	for (i = 0; i < num; i++)
	{
		temp_save[type[i]]++;
	}
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_num++;
	}
	type_save = new int[type_num];
	j = 0;
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_save[j++] = i;
	}

	for (i = 0; i < yanshen; i++)
	{

		for (j = 0; j < num; j++)
		{
			//x_xishu = i/3;
			//y_xishu = i / 3;
			//z_zishu = (i % 9) / 3;

			real_position[i][j][0] = letice[0][0] * p[j][0] + letice[1][0] * p[j][1] + letice[2][0] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][0] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][0] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][0];
			real_position[i][j][1] = letice[0][1] * p[j][0] + letice[1][1] * p[j][1] + letice[2][1] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][1] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][1] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][1];
			real_position[i][j][2] = letice[0][2] * p[j][0] + letice[1][2] * p[j][1] + letice[2][2] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][2] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][2] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][2];
		}
	}
	for (i = 0; i < yanshen; i++)
	{
		for (j = 0; j < num; j++)
		{
			p_real[i][j][0] = (i % cengshu - ((cengshu - 1) / 2)) + p[j][0];
			p_real[i][j][1] = (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) + p[j][1];
			p_real[i][j][2] = -(i / (cengshu * cengshu) - ((cengshu - 1) / 2)) + p[j][2];
		}
	}

	if (flag == 2)
	{
		my_classify = new int[num];
		if_positive = new int [num];
		//开始按照之前的结果进行标定
		for (i = 0; i < num; i++)
		{
			my_classify[i] = classify_metal_maingroup(type[i]);
			
		}
		for (i = 0; i < num; i++)
		{
			//针对H和O的特殊情况
			if (type[i] == 1)
			{
				if_positive[i] = 1;
				for (j = 0; j < yanshen; j++)
				{
					for (k = 0; k < num; k++)
					{
						if (my_classify[k] == 1 && e[type[k]].electron_negativity<2.20)
						{
							double temp_dis = dis(real_position[13][i], real_position[j][k]);
							if (temp_dis <( max_ion[1][1]+max_ion[type[k]][0])*ridus_plus_factor)
							{
								if_positive[i] = -1;
								j = yanshen + 1;
								break;
							}
						}
					}
				}
				//cout << "judge:H" << if_positive[i] << endl;
			}
			/*else if (type[i] == 8)
			{
				if_positive[i] = -1;
				for (j = 0; j < num; j++)
				{
					if (type[j] == 9)
					{
						for (k = 0; k < yanshen; k++)
						{
							double temp_dis = dis(real_position[13][i],real_position[k][j]);
							if (temp_dis < OF_RULE)
							{
								if_positive[i] = 1;
								j = num + 1;
								break;
							}
						}
					}
				}
			}*/
			else
			{
				if (my_classify[i] == 0)
				{
					if_positive[i] = 0;
				}
				else if (my_classify[i] == 1)
				{
					if_positive[i] = 1;
				}
				else if (my_classify[i] == 2)
				{
					if_positive[i] = -1;
				}
			}
			//cout << atom_name[type[i]] << ":" << my_classify[i] <<"positive:"<< if_positive[i] << endl;

		}
	}	
	generateGrapth(num*yanshen,e,max_ion,dist);
	fclose(in);
	
}

cell::cell(char *name)
{
	int i, j, k;
	//cout << "expand the :" << cengshu << "layer" << endl;
	char temp[300];
	double x_pian = 0.0;
	double y_pian = 0.0;
	double z_pian = 0.0;
	//strcpy(wenjian, "atom1.config");
	FILE *in;
	in = fopen(name, "rt");
	//system("pause");
	if (in == NULL)
	{
		printf("error of rading atom.config!\n");
		printf("the filename is :%s\n", name);
		//cin.get();
		return;
	}
	fscanf(in, "%d", &num);	
	type = (int *)malloc(num * sizeof(int));
	letice = (double **)malloc(3 * sizeof(double *));
	for (i = 0; i < 3; i++)
	{
		letice[i] = (double *)malloc(3 * sizeof(double));
	}
	p = (double **)malloc(num * sizeof(double *));
	for (i = 0; i < num; i++)
	{
		p[i] = (double *)malloc(3 * sizeof(double));
	}
	real_position = (double ***)malloc(yanshen * sizeof(double **));
	for (i = 0; i < yanshen; i++)
	{
		real_position[i] = (double **)malloc(num * sizeof(double *));
		for (k = 0; k < num; k++)
			real_position[i][k] = (double *)malloc(3 * sizeof(double));
	}

	p_real = new double **[yanshen];
	for (i = 0; i < yanshen; i++)
	{
		p_real[i] = new double *[num];
		for (k = 0; k < num; k++)
		{
			p_real[i][k] = new double[3];
		}
	}
	
	while (fgets(temp, 300, in) != NULL)
	{
		if (strstr(temp, "VECTOR") != NULL || strstr(temp, "vector") != NULL)
			break;
	}
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(in, "%lf", &letice[i][j]);
		}
		fgets(temp, 300, in);
	}

	fgets(temp, 300, in);
	//cout << temp << endl;
	char line[10];
	for (i = 0; i < num; i++)
	{

		fscanf(in, "%d", &type[i]);
		fscanf(in, "%lf", &p[i][0]);
		fscanf(in, "%lf", &p[i][1]);
		fscanf(in, "%lf", &p[i][2]);
		fgets(temp, 300, in);
	}
	//int x_xishu = 0;
	//int y_xishu = 0;
	//int z_zishu = 0;

	//增加记录type的记录
	int temp_save[120] = { 0 };
	type_num = 0;
	for (i = 0; i < num; i++)
	{
		temp_save[type[i]]++;
	}
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_num++;
	}
	type_save = new int[type_num];
	j = 0;
	for (i = 0; i < 120; i++)
	{
		if (temp_save[i] != 0)
			type_save[j++] = i;
	}

	for (i = 0; i < yanshen; i++)
	{

		for (j = 0; j < num; j++)
		{
			//x_xishu = i/3;
			//y_xishu = i / 3;
			//z_zishu = (i % 9) / 3;

			real_position[i][j][0] = letice[0][0] * p[j][0] + letice[1][0] * p[j][1] + letice[2][0] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][0] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][0] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][0];
			real_position[i][j][1] = letice[0][1] * p[j][0] + letice[1][1] * p[j][1] + letice[2][1] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][1] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][1] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][1];
			real_position[i][j][2] = letice[0][2] * p[j][0] + letice[1][2] * p[j][1] + letice[2][2] * p[j][2] + (i % cengshu - ((cengshu - 1) / 2)) * letice[0][2] + (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) * letice[1][2] - (i / (cengshu * cengshu) - ((cengshu - 1) / 2)) * letice[2][2];
		}
	}
	for (i = 0; i < yanshen; i++)
	{
		for (j = 0; j < num; j++)
		{
			p_real[i][j][0] = (i % cengshu - ((cengshu - 1) / 2)) + p[j][0];
			p_real[i][j][1] = (i % (cengshu * cengshu) / cengshu - ((cengshu - 1) / 2)) + p[j][1];
			p_real[i][j][2] = -(i / (cengshu * cengshu) - ((cengshu - 1) / 2)) + p[j][2];
		}
	}	
	fclose(in);
}

save::save(int num)
{
	int i, j, k;
	sunbets = 0;
	save_list = new int *[num];
	for (i = 0; i < num; i++)
	{
		save_list[i] = new int[num];
	}
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			save_list[i][j] = -1;
		}
	}
}
save::~save(void) //�������ǲ��걸�ģ�˼����ô���������ص�����ֵ
{
	int i = 0, j = 0;
	delete[] save_list;
	//cout << "the object is being destroied!" << endl;
}


void random_choose_two(int *org, int num, int **in)
{
	//从给定的org当中，任取两个，填充到in里面
	int i = 0, j = 0;
	//int total = num * (num - 1) / 2;
	int count = 0;
	for (i = 0; i < num; i++)
	{
		for (j = i + 1; j < num; j++)
		{
			in[count][0] = org[i];
			in[count][1] = org[j];
			//cout << in[count][0] << "," << in[count][1] << endl;
			count++;
		}
	}
	//cin.get();
}
int judge_falg_file(char *path)
{
	//成功返回1，失败返回0
	ifstream fin;
	char real[200];
	strcpy(real, path);
	strcat(real, "/flag");
	fin.open(real, ios::in);
	string temp;
	fin >> temp;
	fin.close();
	if (temp == "Different")
		return 1;
	else if (temp.find("All") != -1)
		return 0;
	else
	{
		cout << "wrong!flag file!" << endl;
		cin.get();
		return 0;
	}
}

void read_radius(ionic_radius *****ir, string& file_name1, string& file_name2)
{

	int i, j, k, l, m;
	int line_in;
	int coor_in;
	int val_in, structure_in, spin_state_in;
	double radius_in;
	char c;
	char str[5];
	char str_temp[500];
	FILE *in;
	int max = -9999;
	char atom[120][3] = { "D","H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };

	for (i = 0; i < 120; i++)
	{
		for (j = 0; j < 20; j++)
		{
			for (k = 0; k < 15; k++)
			{
				for (l = 0; l < 3; l++)
				{
					for (m = 0; m < 3; m++)
					{
						ir[i][j][k][l][m].atomic_num = i;
						ir[i][j][k][l][m].coor_num = k;
						ir[i][j][k][l][m].val_state = j - 8;
						ir[i][j][k][l][m].radius = -9999;
						ir[i][j][k][l][m].structure_type = -9999;
						ir[i][j][k][l][m].spin_stat = -9999;
					}
				}
			}
		}
	}
	in = fopen(file_name1.c_str(), "r");
	if (in == NULL)
	{
		printf("input_radius does not exist!\n");
		return;
	}
	c = '0';
	for (line_in = 0; line_in < 503; line_in++)
	{
		fscanf(in, "%s", str);
		for (i = 0; i < 120; i++)
		{
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[1] == '\0')
			{
				break;
			}
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[2] == atom[i][2] && str[2] == '\0')
			{
				break;
			}
		}
		if (i == 120)
		{
			printf("ERROR :%s\n", str);
			cout << line_in << endl;
			cin.get();
			return;
		}
		fscanf(in, "%d", &val_in);
		fscanf(in, "%d", &coor_in);
		fscanf(in, "%d", &structure_in);
		fscanf(in, "%d", &spin_state_in);
		fscanf(in, "%lf", &radius_in);
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].atomic_num = i;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].val_state = val_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].coor_num = coor_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].spin_stat = spin_state_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].radius = radius_in;
		ir[i][val_in + 8][coor_in][structure_in][spin_state_in].structure_type = structure_in;
		/*for (i=0;i<15;i++)
		{
			if (max<ir[i][val_in+8][i].radius)
			{
				max=ir[i][val_in+8][i].radius;
			}
		}
		ir[i][val_in+8][0].radius=max*1.1;*/
	}
	fclose(in);
	//继续读取我补充的信息
	ifstream fin;
	fin.open(file_name2, ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file !" << file_name2 << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> str;
		//cout << str;
		for (i = 0; i < 120; i++)
		{
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[1] == '\0')
			{
				break;
			}
			if (str[0] == atom[i][0] && str[1] == atom[i][1] && str[2] == atom[i][2] && str[2] == '\0')
			{
				break;
			}
		}
		if (i == 120)
		{
			//printf("ERROR :%s\n", str);
			//cout << file_name2 << endl;
			//cin.get();
			return;
		}
		fin >> val_in;
		fin >> coor_in;
		fin >> radius_in;
		ir[i][val_in + 8][coor_in][0][0].atomic_num = i;
		ir[i][val_in + 8][coor_in][0][0].coor_num = coor_in;
		ir[i][val_in + 8][coor_in][0][0].val_state = val_in;
		ir[i][val_in + 8][coor_in][0][0].spin_stat = 0;
		ir[i][val_in + 8][coor_in][0][0].structure_type = 0;
		ir[i][val_in + 8][coor_in][0][0].radius = radius_in;

	}
	fin.close();


	//做完这个需要补充一点事情，就是取出一个最大值用来预估配位数，我们把不同元素和价态的最大半径放到一个位置，以后方便取用
	for (i = 0; i < 120; i++)
	{
		for (j = 0; j < 20; j++)
		{
			for (k = 0; k < 15; k++)
			{
				for (l = 0; l < 3; l++)
				{
					for (m = 0; m < 3; m++)
					{
						if (max < ir[i][j][k][l][m].radius)
						{
							max = ir[i][j][k][l][m].radius;
						}
					}
				}
			}
			//每种情况取出来的最大值在这里赋值,放到对应的元素和价态，0配位，0结构，0自旋的半径下
			max = ir[i][j][0][0][0].radius;
			max = -9999;
		}
	}
	return;
}

//这里建立一个重载版本
void read_element(element *e, string& file_element_r, string& file_colvance, string& file_electronic_negativity, string& file_first_ionization_energy)
{
	int i, j, k;
	char atom_name[120][3] = { "D","H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };

	//先初始化信息
	for (i = 0; i < 120; i++)
	{
		strcpy(e[i].name, atom_name[i]);
		e[i].atomic_num = i;
		e[i].num_common_val = 0;
		e[i].num_metal_radius = 0;
		e[i].num_unusual_val = 0;
	}
	i = 0, j = 0, k = 0;
	int line_in;
	ifstream fin;
	int atom_num_in;
	double vdw_r_min_in, vdw_r_max_in;
	double cov_r;
	int num_metal_r;
	double *temp;
	char str[5];
	char str_temp[500];
	char atom[120][3] = { "D","H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };
	int num_common_in;
	int num_unsual_in;
	//读取共价键范德华键和金属键

	fin.open(file_element_r.c_str(), ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file:" << file_element_r << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> atom_num_in;
		fin >> vdw_r_min_in;
		fin >> vdw_r_max_in;
		fin >> cov_r;
		fin >> num_metal_r;

		e[atom_num_in].metal_radius = new double[num_metal_r];
		double *temp = new double[num_metal_r];
		for (i = 0; i < num_metal_r; i++)
		{
			fin >> temp[i];
			//cout << temp[i] << endl;
		}
		e[atom_num_in].atomic_num = atom_num_in;
		e[atom_num_in].vdw_radius_max = vdw_r_max_in;
		e[atom_num_in].vdw_radius_min = vdw_r_min_in;
		e[atom_num_in].cov_radius = cov_r;
		e[atom_num_in].num_metal_radius = num_metal_r;
		for (i = 0; i < num_metal_r; i++)
		{
			e[atom_num_in].metal_radius[i] = temp[i];
		}
		free(temp);
	}
	fin.close();


	////然后开始读取价态相关信息
	//fin.clear();
	//fin.open(file_colvance.c_str(), ios::in);
	//if (!fin.is_open())
	//{
	//	cout << "i can not find the file !" << file_colvance.c_str() << endl;
	//	cin.get();
	//}
	//while (fin.peek() != EOF && fin.good())
	//{
	//	fin >> atom_num_in;
	//	//printf("%d\n", atom_num_in);
	//	//fin >> num_common_in;
	//	fin >> e[atom_num_in].num_common_val;
	//	e[atom_num_in].common_val = new int[e[atom_num_in].num_common_val];
	//	for (i = 0; i < e[atom_num_in].num_common_val; i++)
	//	{
	//		fin >> e[atom_num_in].common_val[i];
	//	}
	//	fin >> e[atom_num_in].num_unusual_val;
	//	e[atom_num_in].unusual_val = new int[e[atom_num_in].num_unusual_val];
	//	for (i = 0; i < e[atom_num_in].num_unusual_val; i++)
	//	{
	//		fin >> e[atom_num_in].unusual_val[i];
	//	}
	//	//delete[]e[atom_num_in].common_val;
	//}
	//fin.close();

	//开始读取电负性相关信息
	string temp_test;
	fin.clear();
	fin.open(file_electronic_negativity.c_str(), ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file !" << file_electronic_negativity << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> atom_num_in;
		//cout << atom_num_in << endl;
		fin >> str;
		//fin >> temp_test;
		//cout <<temp_test << endl;
		/*if (str[0] == '\0')
		{
			break;
		}*/
		fin >> e[atom_num_in].electron_negativity;
		//cout << e[atom_num_in].electron_negativity << endl;
		if (str[0] != atom[atom_num_in][0] || str[1] != atom[atom_num_in][1])
		{
			printf("ERROR atom name %s %s %d\n", str, atom[atom_num_in], atom_num_in);
			cout << file_electronic_negativity << endl;
			cin.get();
		}
	}
	fin.close();
	
	//如果是模仿mz的方法，这里需要补充
	if (IF_SAME_MZ == true) {
		string file_name = "/share/home/wangz/high/mz_ridus";
		ifstream fin;
		fin.open(file_name, ios::in);
		if (!fin.is_open()) {
			cout << "i can not find the file_name:" << file_name << endl;
			cin.get();
			return;
		}
		string temp;
		int index;
		double input[5] = { 0 };
		fin.getline(const_cast<char*>(temp.c_str()), 200);
		while (fin.good() && fin.peek() != EOF) {
			fin >> index;
			fin >> temp;
			for (int i = 0; i < 5; i++)
				fin >> input[i];
			e[index].vdw_radius_max = input[2];
			e[index].vdw_radius_min = input[2];
			//cout << temp << ":" << input[2] << endl;
		}
		fin.close();
		
	}
	////最后开始读取第一电离能的相关信息
	//fin.clear();
	//fin.open(file_first_ionization_energy.c_str(), ios::in);
	//if (!fin.is_open())
	//{
	//	cout << "i can not find the file !" << file_first_ionization_energy << endl;
	//	cin.get();
	//}
	//while (fin.peek() != EOF && fin.good())
	//{
	//	fin >> atom_num_in;
	//	//cout << atom_num_in << endl;
	//	fin >> str;
	//	/*if (str[0] = '\0')
	//	{
	//		break;
	//	}*/
	//	fin >> e[atom_num_in].first_ionization_energy;
	//	//cout << e[atom_num_in].first_ionization_energy << endl;
	//	if (str[0] != atom[atom_num_in][0] || str[1] != atom[atom_num_in][1])
	//	{
	//		printf("ERROR atom name %s %s %d\n", str, atom[atom_num_in], atom_num_in);
	//		cout << file_first_ionization_energy << endl;
	//		cin.get();
	//	}
	//}
	//fin.close();
	return;
}
void read_element(element *e, string &file_element_r)
{
	int i, j, k;
	char atom_name[120][3] = { "D", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };

	//先初始化信息
	for (i = 0; i < 120; i++)
	{
		//strcpy(e[i].name, atom_name[i]);
		e[i].atomic_num = i;
		//e[i].num_common_val = 0;
		e[i].num_metal_radius = 0;
		//e[i].num_unusual_val = 0;
	}
	i = 0, j = 0, k = 0;
	int line_in;
	ifstream fin;
	int atom_num_in;
	double vdw_r_min_in, vdw_r_max_in;
	double cov_r;
	int num_metal_r;
	double *temp;
	char str[5];
	char str_temp[500];
	int num_common_in;
	int num_unsual_in;
	//读取共价键范德华键和金属键

	fin.open(file_element_r.c_str(), ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file !" << file_element_r << endl;
		cin.get();
	}
	while (fin.peek() != EOF && fin.good())
	{
		fin >> atom_num_in;
		fin >> vdw_r_min_in;
		fin >> vdw_r_max_in;
		fin >> cov_r;
		fin >> num_metal_r;

		e[atom_num_in].metal_radius = new double[num_metal_r];
		double *temp = new double[num_metal_r];
		for (i = 0; i < num_metal_r; i++)
		{
			fin >> temp[i];
		}
		e[atom_num_in].atomic_num = atom_num_in;
		e[atom_num_in].vdw_radius_max = vdw_r_max_in;
		e[atom_num_in].vdw_radius_min = vdw_r_min_in;
		e[atom_num_in].cov_radius = cov_r;
		e[atom_num_in].num_metal_radius = num_metal_r;
		for (i = 0; i < num_metal_r; i++)
		{
			e[atom_num_in].metal_radius[i] = temp[i];
		}
		free(temp);
	}
	fin.close();

	return;
}
double dis(double *p1, double *p2)
{
	return sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}

//面向对象的构建原始的大数组
void cell::generateGrapth(int num, element*e, vector<vector<double>>&max_ionic_riuds, double**dist) {
	this->big_matrix = new int *[num]; //��ά���󣬴��������֮�����ϵ
	for (int i = 0; i < num; i++)
	{
		this->big_matrix[i] = new int[num];
	}
	//cout << "generate big mitrix!" << endl;
	for (int i = 0; i < num; i++)
	{
		for (int j = 0; j < num; j++)
		{
			this->big_matrix[i][j] = -1;
		}
	}
	if (IF_COUNT_RIDUS == true) {
		//默认倍数从0.8到1.3，每隔0.02统计一个
		int count_number = (end_times - start_times) / step_times;
		this->count.resize(count_number + 1);
		for (int i = 0; i < count_number + 1; i++)
			this->count[i] = 0;
	}
	
	full_expand_generate_graph(*this, this->big_matrix, num, e, max_ionic_riuds, dist);
}
void full_expand_generate_graph(cell &cell_a, int **expand_matrix, int num, element *e,vector <vector<double>> &max_ionic,double**dist)
{
	//建立重载版本的构建连接关系
	int i = 0, j = 0;
	int m = 0, n = 0;
	double temp = 0.0;

	//首先判断是不是进行mz方法的判断
	if (IF_SAME_MZ == true) {
		for (int i = 0; i < num; i++) {
			for (int j = i + 1; j < num; j++) {
				temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
				if (abs(temp) < 1e-3)
					continue;
				//检查没有共价键的情况
				if (e[cell_a.type[i%cell_a.num]].vdw_radius_max == -1 || e[cell_a.type[j%cell_a.num]].vdw_radius_max == -1)
				{
					//cout << "unknown covridus!:atomic :" << cell_a.type[i%cell_a.num] << "," << cell_a.type[j%cell_a.num] << endl;
					//cin.get();
					continue;
				}
				dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = (e[cell_a.type[i%cell_a.num]].vdw_radius_max + e[cell_a.type[j%cell_a.num]].vdw_radius_max) -1.3;
				dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = (e[cell_a.type[i%cell_a.num]].vdw_radius_max + e[cell_a.type[j%cell_a.num]].vdw_radius_max) -1.3;
				//cout << e[cell_a.type[i%cell_a.num]].cov_radius << "," << e[cell_a.type[i%cell_a.num]].cov_radius << endl;
				double rule = dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]];
				if (temp < rule)
				{
					//cout << e[cell_a.type[i%cell_a.num]].vdw_radius_max + e[cell_a.type[j%cell_a.num]].vdw_radius_max << endl;
					expand_matrix[i][j] = 1;
					expand_matrix[j][i] = 1;
				}
				else
				{
					//cout << temp << ";" << rule << endl;
					expand_matrix[i][j] = 0;
					expand_matrix[j][i] = 0;
				}
			}
		}
		return;
	}


	//首先尝试使用共价键连接原子
	for (i = 0; i < num; i++)
	{
		if(cell_a.my_classify[i%cell_a.num]==2)
		for (j = i + 1; j < num; j++)
		{
			if (cell_a.my_classify[j%cell_a.num] == 2)
			{
				temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
				if (abs(temp) < 1e-3)
					continue;
				//检查没有共价键的情况
				if (e[cell_a.type[i%cell_a.num]].cov_radius ==-1 || e[cell_a.type[j%cell_a.num]].cov_radius ==-1)
				{
					//cout << "unknown covridus!:atomic :" << cell_a.type[i%cell_a.num] << "," << cell_a.type[j%cell_a.num] << endl;
					//cin.get();
					continue;
				}
				dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = (e[cell_a.type[i%cell_a.num]].cov_radius + e[cell_a.type[j%cell_a.num]].cov_radius) / 100.0*val_radius_factor;
				dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = (e[cell_a.type[i%cell_a.num]].cov_radius + e[cell_a.type[j%cell_a.num]].cov_radius) / 100.0*val_radius_factor;
				double rule = (e[cell_a.type[i%cell_a.num]].cov_radius + e[cell_a.type[j%cell_a.num]].cov_radius) / 100.0;
				if (IF_COUNT_RIDUS == true) {
					if (i / cell_a.num == (yanshen - 1) / 2) {
						cell::inrich_connection_num(cell_a, temp, rule,start_times,end_times,step_times);
					}
				}

				if (temp<(e[cell_a.type[i%cell_a.num]].cov_radius+ e[cell_a.type[j%cell_a.num]].cov_radius)/100.0*val_radius_factor)
				{
					//cout << e[cell_a.type[i%cell_a.num]].cov_radius + e[cell_a.type[j%cell_a.num]].cov_radius << endl;
					expand_matrix[i][j] = 1;
					expand_matrix[j][i] = 1;
				}
				else
				{
					expand_matrix[i][j] = 0;
					expand_matrix[j][i] = 0;
				}
			}
		}
	}

	//然后使用离子半径连接正负基团
	for (i = 0; i < num; i++)
	{
		if (cell_a.if_positive[i%cell_a.num] == 1)
		{
			for (j = 0; j < num; j++)
			{
				if (cell_a.if_positive[j%cell_a.num] == -1)
				{
					temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
					if (abs(temp) < 1e-3)
						continue;
					//检查没有离子键的情况

					if (max_ionic[cell_a.type[i%cell_a.num]][0]<-100|| max_ionic[cell_a.type[j%cell_a.num]][1] < -100)
					{
						//cout << "unknown ionic!:atomic :" << atom_name[cell_a.type[i%cell_a.num]] << "," << atom_name[cell_a.type[j%cell_a.num]] << endl;
						//cin.get();
						continue;
					}
					double rule= (max_ionic[cell_a.type[i%cell_a.num]][0] + max_ionic[cell_a.type[j%cell_a.num]][1])*ridus_plus_factor;
					if (rule > dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]])
					{
						//后来我们只更新距离大的style
						dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = rule;
						dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = rule;

					}
					
					if (IF_COUNT_RIDUS == true) {
						if (i / cell_a.num == (yanshen - 1) / 2) {
							cell::inrich_connection_num(cell_a, temp, rule/ridus_plus_factor, start_times, end_times, step_times);
						}
					}
					if (temp < rule)
					{
						//cout << (max_ionic[cell_a.type[i%cell_a.num]][0] + max_ionic[cell_a.type[j%cell_a.num]][1])*ridus_plus_factor << endl;
						//cout << atom_name[cell_a.type[i%cell_a.num]] << "," << atom_name[cell_a.type[j%cell_a.num]] << endl;
						expand_matrix[i][j] = 1;
						expand_matrix[j][i] = 1;
					}	
					else {
						expand_matrix[i][j] = 0;
						expand_matrix[j][i] = 0;
					}
				}
			}
		}
	}


	//然后检查金属的情况，如果金属周围是孤立的，则尝试用金属半径连接金属与金属
	//这里需要作出修改，如果这个结构完全是合金的话，就不用管孤立金属了所有就是
	int pure_metal_flag = 1;
	for (int i = 0; i < cell_a.num; i++)
	{
		if (cell_a.my_classify[i] != 1)
		{
			pure_metal_flag = 0;
			break;
		}
			
	}
	if (pure_metal_flag == 1)
	{
		for (i = 0; i < num; i++)
		{
			for (j = 0; j < num; j++)
			{
				temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
				if (abs(temp) < 1e-3)
					continue;
				double rule = (e[cell_a.type[i%cell_a.num]].metal_radius[0] + e[cell_a.type[j%cell_a.num]].metal_radius[0]) / 100.0*metal_ridus_factor;

				if (rule > dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]])
				{
					dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = rule;
					dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = rule;		
					//dist[94][94] = 2;
					//cout << cell_a.type[i%cell_a.num] << "," << cell_a.type[j%cell_a.num] << ":" << rule << endl;
				}
				if (IF_COUNT_RIDUS == true) {
					if (i / cell_a.num == (yanshen - 1) / 2) {
						cell::inrich_connection_num(cell_a, temp, rule / metal_ridus_factor, start_times, end_times, step_times);
					}
				}
				if (temp < dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]])
				{
					/*cout << rule << temp<<endl;
					cin.get();*/
					expand_matrix[i][j] = 1;
					expand_matrix[j][i] = 1;
				}
				else {
					expand_matrix[i][j] = 0;
					expand_matrix[j][i] = 0;
				}

			}
		}


	}
	else
	{
		for (i = 0; i < num; i++)
		{
			int iso_flag = 1;
			if (cell_a.my_classify[i%cell_a.num] == 1)
			{
				for (j = 0; j < num; j++)
				{
					if (expand_matrix[i][j] == 1)
					{
						iso_flag = 0;
						break;
					}
				}
				if (iso_flag == 1)//说明这个是孤立的金属原子
				{
					for (j = 0; j < num; j++)
					{
						if (cell_a.my_classify[j%cell_a.num] == 1)
						{
							temp = dis(cell_a.real_position[i / cell_a.num][i % cell_a.num], cell_a.real_position[j / cell_a.num][j % cell_a.num]);
							if (abs(temp) < 1e-3)
								continue;
							double rule = (e[cell_a.type[i%cell_a.num]].metal_radius[0] + e[cell_a.type[j%cell_a.num]].metal_radius[0]) / 100.0*metal_ridus_factor;

							if (rule > dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]])
							{
								dist[cell_a.type[i%cell_a.num]][cell_a.type[j%cell_a.num]] = rule;
								dist[cell_a.type[j%cell_a.num]][cell_a.type[i%cell_a.num]] = rule;
								//cout << cell_a.type[i%cell_a.num] << "," << cell_a.type[j%cell_a.num] << ":" << rule << endl;
							}
							if (IF_COUNT_RIDUS == true) {
								if (i / cell_a.num == (yanshen - 1) / 2) {
									cell::inrich_connection_num(cell_a, temp, rule / metal_ridus_factor, start_times, end_times, step_times);
								}
							}
							if (temp < rule)
							{
								/*cout << rule << temp<<endl;
								cin.get();*/
								expand_matrix[i][j] = 1;
								expand_matrix[j][i] = 1;
							}
							else {
								expand_matrix[i][j] = 0;
								expand_matrix[j][i] = 0;
							}
						}
					}
				}
				else {
					//不孤立情况下，默认就是不会连接的
					//在不孤立情况下，他和所有金属都是不连接的
					for (int j = 0; j < num; j++)
					{
						if ((cell_a.my_classify[j%cell_a.num] == 1 && j != i)||
							(cell_a.if_positive[j%cell_a.num]==1 && j!=i))
						{
							expand_matrix[i][j] = 0;
							expand_matrix[j][i] = 0;
						}
					}
				}

			}
		}
		//然后我们检查为什么会有-1的情况
	/*	for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				if (expand_matrix[i][j] == -1 && i != j) {
					cout << i << ":" << atom_name[cell_a.type[i%cell_a.num]] << " " << j << ":" << atom_name[cell_a.type[j%cell_a.num]]<<",judge:"<<cell_a.if_positive[j%cell_a.num] << endl;
					cin.get();
				}
			}
		}*/
		//cout << "for matrix, inrich number finished!" << endl;
		return;
	}
}

void ex_find_connect(int edge_num, cell cell_a, int *visited, int **matrix, save *save_a)
{
	int i = 0, ii = 0, j = 0, k = 0, jj = 0;
	int save_flag = 1;
	int subnets = 0;
	for (k = 0; k < edge_num; k++)
	{
		if (!visited[k])
		{
			subnets++;
			ex_DFS(k, visited, cell_a, matrix, edge_num, subnets);

			//cout << "the subnets is:" << subnets << endl;
			save_a->sunbets = subnets;
			for (i = 0; i < edge_num; i++)
			{
				if (visited[i] == subnets)
				{
					save_a->save_list[subnets - 1][ii] = i;
					ii++;
				}
			}
		}
		ii = 0;
	}
	//cout << "the hanshu finished before,the subnets is:" << save_a->sunbets << endl;
	//cin.get();
	return;
}

void generate_testfile(save *save_a, cell cell_a, int edge_num) //������save*ָ�룬����ǽ����txt
{
	int i = 0, j = 0;
	FILE *out;
	out = fopen("test_output.txt", "wt");
	char a[50] = "the lian tong num is:";
	char b[20];
	sprintf(b, "%d\t", save_a->sunbets);
	strcat(a, b);
	fprintf(out, "%s\t", a);
	cout << "i got the liantong geshu is:" << save_a->sunbets << endl;
	for (i = 0; i < save_a->sunbets; i++)
	{
		for (j = 0; j < edge_num; j++)
		{
			if (save_a->save_list[i][j] != -1)
			{
				fprintf(out, "%d\t", save_a->save_list[i][j]);
			}
		}
		fprintf(out, "\n");
	}
	fclose(out);
	return;
}

void generate_outfile(cell cell_a, save *ex_pt, int edge_num, int *judge) ////��������������ǻ������ͨ����֮�󣬸����жϵĽ������txt�ļ����ڹ۲�
{
	int i, j;
	FILE *out;
	out = fopen("out_judge.txt", "w");
	char a[50] = "the sunbets num is:";
	char b[20];
	sprintf(b, "%d\t", ex_pt->sunbets);
	strcat(a, b);
	fprintf(out, "%s\n", a);
	cout << "i got the liantong geshu is:" << ex_pt->sunbets << endl;
	char c[60] = "for subnets,judge out:";
	fprintf(out, "%s\n", c);
	for (i = 0; i < ex_pt->sunbets; i++)
	{
		fprintf(out, "%d\t", judge[i]);
	}
	fprintf(out, "\n");
	for (i = 0; i < ex_pt->sunbets; i++)
	{
		for (j = 0; j < edge_num; j++)
		{
			if (ex_pt->save_list[ex_pt->sunbets - 1][j] != -1)
			{
				fprintf(out, "%d\t", ex_pt->save_list[i][j]);
			}
		}
		fprintf(out, "\n");
	}

	fclose(out);
	return;
}

void ex_DFS(int a, int *ex_visited, cell cell_a, int **expand_matrix, int num_edge, int subnets) //�������������ʵ�ִӶ���a����������������δ���ʹ����ڽӽڵ�
{
	ex_visited[a] = subnets;
	//printf("now i start the dot %d\n", a);
	int i = 0, j = 0, k = 0;
	for (j = 0; j < num_edge; j++)
	{
		if (expand_matrix[a][j] == 1 && !ex_visited[j])
		{
			ex_DFS(j, ex_visited, cell_a, expand_matrix, num_edge, subnets);
			//����д���˺������,��ʵ�ܼ򵥣����Ǽ�¼���ok��
		}
	}
}



int *judge_the_1d(cell cell_a, int *save_list, int edge_num, int *judge) //���������������������õ���ÿ����ͨ�������ж����ǲ�������Һõ�2d��ͨ����
{
	int i = 0, j = 0, k = 0, m = 0;
	int temp_org = 0;
	int *pt = judge;	
	int exit_flag = 0; //�˳���־��1��ʾ1d��2��ʾ2d��3��ʾ3d,0��ʾ���ڵ�̫���ڷ�ɢ����޷���������жϵ�����
	int *copy = new int[yanshen];
	for (int i = 0; i < yanshen; i++)
	{
		copy[i] = -1;
	}
	//�������ʵ�copy
	int temp = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (save_list[i] != -1)
		{
			temp = save_list[i] % cell_a.num;
			temp_org = save_list[i];
			for (k = 0; k < edge_num; k++)
			{
				if (save_list[k] == -1)
					break;
				if ((save_list[k]) % cell_a.num == temp)
				{
					copy[j] = save_list[k];
					j++;
				}
			}
			//cout << j << endl;
			if (j >=3)
				break;
			else
				j = 0;
		}
		else
			break;
	}

	//cout << j << endl;

	if (j <3 || j>cengshu)
	{
		//cout << "i can not find the enough vertex to complete the judgement" << endl;

		*pt = -1;
		*(pt + 1) = -1;
		delete[] copy;
		return pt;
	}


	exit_flag = 1;
	double **a = new double *[3];
	for (i = 0; i < 3; i++)
	{
		a[i] = new double[3];
	}
	a[0][0] = a[0][1] = a[0][2] = 1;

	double x = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0];
	double y = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1];
	double z = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2];
	a[1][0] = x;
	a[1][1] = y;
	a[1][2] = z;

	double nx = 0;
	double ny = 0;
	double nz = 0;
	for (i = 2; i < j; i++)
	{
		if (save_list[i] != -1)
		{
			nx = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0];
			ny = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1];
			nz = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2];

			a[2][0] = nx;
			a[2][1] = ny;
			a[2][2] = nz;
			if (abs(three_jie_chaji(a)) > 1e-3) //��һ�������ߵ�����Ϳ���
			{
				exit_flag = 0;
				break;
			}
		}
	}

	for (i = 0; i < 3; i++)
	{
		delete[] a[i];
	}
	delete[] a;
	if (exit_flag == 1)
	{
		//cout << "the 1d situation" << endl;
		*pt = 1;
		*(pt + 1) = temp_org;
		delete[] copy;
		return pt;
	}
	else
	{
		*pt = -1; //我们这里只需要查找到1D的情况
		*(pt + 1) = temp;
		delete[] copy;
		return pt;
	}
	return pt;
}
int *judge_the_0d(cell cell_a, int *save_list, int edge_num, int *judge, int** expand_graph) //���������������������õ���ÿ����ͨ�������ж����ǲ�������Һõ�2d��ͨ����
{
	int i = 0, j = 0, k = 0, m = 0;
	int *pt = judge;
	int exit_flag = 0; //�˳���־��1��ʾ1d��2��ʾ2d��3��ʾ3d,0��ʾ���ڵ�̫���ڷ�ɢ����޷���������жϵ�����
	int *copy = new int[yanshen];
	for (int i = 0; i < yanshen; i++)
	{
		copy[i] = -1;
	}	
	//�������ʵ�copy
	int temp = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (save_list[i] != -1)
		{
			temp = save_list[i] % cell_a.num;
			for (k = 0; k < edge_num; k++)
			{
				if (save_list[k] == -1)
					break;
				if ((save_list[k]) % cell_a.num == temp)
				{
					copy[j] = save_list[k];
					j++;
				}
			}
			//cout << j << endl;
			break;

		}
		else
			break;
	}
	//0d的条件，复制体只有1个，不和其他接触，同时还要求不是散点
	//要求组成这个连通分量的原子数目应该较多，大于rule
	int connect_num = 0;
	int group_num = 0;
	if (j == 1)
	{
		for (i = 0; i < edge_num; i++)
		{
			if (i != copy[0] && expand_graph[copy[0]][i] == 1)
			{
				connect_num++;
			}
			if (save_list[i] != -1)
				group_num++;
		}
		if (connect_num > zeroD_atom_num_rule && group_num > zeroD_group_num)
		{
			*pt = 0;
			//cout << "good!" << endl;
			*(pt + 1) = copy[0] % cell_a.num;
			delete[] copy;
			return pt;
		}
		else
		{
			//cout << "too  samll!" << endl;
			*pt = 4;
			*(pt + 1) = copy[0] % cell_a.num;
			delete[] copy;
			return pt;
		}

	}
	else
	{
		*pt = -1;
		*(pt + 1) = -1;
		delete[] copy;
		return pt;
		//cout << "too many!" << endl;
	}

}
int *judge_the_2d(cell cell_a, int *save_list, int edge_num, int *judge) //���������������������õ���ÿ����ͨ�������ж����ǲ�������Һõ�2d��ͨ����
{
	int i = 0, j = 0, k = 0, m = 0;
	int *pt = judge;
	int exit_flag = 0; //�˳���־��1��ʾ1d��2��ʾ2d��3��ʾ3d,0��ʾ���ڵ�̫���ڷ�ɢ����޷���������жϵ�����
	int *copy = new int[yanshen];
	for (int i = 0; i < yanshen; i++)
	{
		copy[i] = -1;
	}
	//cout << cell_a.num << endl;

	//�������ʵ�copy
	int temp = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (save_list[i] != -1)
		{
			temp = save_list[i] % cell_a.num;
			for (k = 0; k < edge_num; k++)
			{
				if (save_list[k] == -1)
					break;
				if ((save_list[k]) % cell_a.num == temp)
				{
					copy[j] = save_list[k];
					j++;
				}
			}
			if (j > 3)
				break;
			else
				j = 0;
		}
		else
			break;
	}

	if (j < 4 || j > cengshu * cengshu)
	{
		//cout << "i can not find the enough vertex to complete the judgement" << endl;
		*pt = -1;
		*(pt + 1) = -1;
		delete[] copy;
		return pt;
	}

	if (j == cengshu * cengshu)
	{
		*pt = 2;
		*(pt + 1) = temp;
		delete[] copy;
		return pt;
	}

	//cout << "i need to judge the ppints_num is:" << j << endl;
	//cin.get();
	//debug
	/*for (i = 0; i < j; i++)
	{
		cout << copy[i] << "," << endl;
	}*/
	
	exit_flag = 2;
	double **b = new double *[4];
	for (i = 0; i < 4; i++)
	{
		b[i] = new double[3];
	}
	int number = j; //�ȰѸ���������
	int *flag = new int[number];
	for (i = 0; i < number; i++)
	{
		flag[i] = 0;
	}

	int jj = 0;
	for (i = 0; i < number; i++)
	{
		flag[i] = 1;
		for (j = i + 1; j < number; j++)
		{
			flag[j] = 1;
			for (k = j + 1; k < number; k++)
			{
				flag[k] = 1;
				for (m = k + 1; m < number; m++)
				{
					flag[m] = 1;

					for (int mm = 0; mm < number; mm++) //�����ͻ�õ�4��������
					{
						if (flag[mm] == 1)
						{
							for (int ii = 0; ii < 3; ii++)
							{
								b[jj][ii] = cell_a.real_position[copy[mm] / cell_a.num][copy[mm] % cell_a.num][ii];
							}
							jj++;
							if (jj == 4)
								break;
						}
					}
					jj = 0;
					//������д�жϹ���,���淵��1
					//ֻҪ���ַǹ������������3d���
					if (inone_face(b) == 0)
					{
						exit_flag = 3;
						for (int i = 0; i < 4; i++)
						{
							delete[] b[i];
						}
						delete[] b;
						delete[] copy;
						delete[] flag;
						*pt = 4;
						*(pt + 1) = temp;
						return pt;
					}

					flag[m] = 0;
				}
				flag[k] = 0;
			}

			flag[j] = 0;
		}
		flag[i] = 0;
	}

	*pt = 2;
	//���������Ǿ͵õ��������2d�����;
	*(pt + 1) = temp;
	delete[] copy;
	delete[] flag;
	return pt;
}

int *judge_dimentional(cell &cell_a, int *save_list, int edge_num, int *judge, int** expand_graph)
{
	/*
	@paramters:sturucture num matrix
	@return :judge the dimensionl
	           -2 表示没有在相应晶胞
	

	*/
	int*res = judge;
	res[0] = -2;
	res[1] = -2;
	if (stu_check(cell_a, save_list, edge_num) == 0)
	{		
		return res;		
	}
	if (AIM == 0) {
		res = judge_the_0d(cell_a, save_list, edge_num, judge, expand_graph);
		return res;
	}
	else if (AIM == 1) {
		//这里需要补充寻找1D的时候，不仅需要check是不是1D，同时要给出是不是0D,和2D的判据
		res = judge_the_1d(cell_a, save_list, edge_num, judge);
		if (res[0] == -1) {
			res = judge_the_0d(cell_a, save_list, edge_num, judge, expand_graph);
			if (res[0] ==-1) {
				res = judge_the_2d(cell_a, save_list, edge_num, judge);
			}
		}
		return res;
	}
	else if (AIM == 2) {
		res = judge_the_2d(cell_a, save_list, edge_num, judge);
		return res;
	}
	else
	{
		cout << "unknown the AIM number!please check!" << endl;
		cin.get();
	}
	return res;

}









int generate_atom(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list)
{

	//这个的作用是根据得到的连通信息生成atom文件,输入是存储的连通分量，输出是atom文件

	int i = 0, j = 0;
	int k = 0;
	int exit_flag = 0;
	int orig = judge[1];
	int metal_count = 0;
	int metal_same_falg = 0;
	//cout << "这里的对应原始点是：" << orig << endl;
	string file[2];
	int num = cell_a.num;
	file[0] = "Lattice vector";
	file[1] = "Position";
	//首先开始生成abc的三个基矢分量
	//从原型序号中找到这样的点以及这样的距离

	double **letice = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice[i] = new double[3]; //建立letice数组用来储存
	}
	double **letice_r = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice_r[i] = new double[3];
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (list[i] % cell_a.num == orig)
		{
			j++;
		}
	}
	int copy_num = j;
	int *copy = new int[j];
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (list[i] % cell_a.num == orig)
		{
			copy[k++] = list[i];
		}
	}

	//有j个这样的复制体,并且都找到了，并且也判断了哪些原子需要放进去
	int *atom_flag_a = new int[cell_a.num];
	//这里的atom_falg_a针对的是第一种情况的二维材料
	for (i = 0; i < cell_a.num; i++)
	{
		atom_flag_a[i] = 0;
	}
	k = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (j = 0; j < cell_a.num; j++)
		{
			if (list[i] % cell_a.num == j && atom_flag_a[j] == 0)
			{
				atom_flag_a[j] = 1;
				k++;
				break;
			}
		}
	}
	int atom_num_write = k;
	double **fenshu = new double *[atom_num_write];
	for (i = 0; i < k; i++)
	{
		fenshu[i] = new double[3];
	}
	double **fenshu_plus = new double *[edge_num];
	for (i = 0; i < edge_num; i++)
	{
		fenshu_plus[i] = new double[3];
	}
	int atom_plus = 0;

	// 然后根据两种情况进行分类
	if (copy_num == cengshu * cengshu)
	{

		int m = 0;
		for (i = 0; i < 3; i++)
		{
			letice[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
			letice[1][i] = cell_a.real_position[copy[copy_num / cengshu] / cell_a.num][copy[copy_num / cengshu] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}
		letice[2][0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
		letice[2][1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
		letice[2][2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];
		double plus = get_vacumlayer_thick(list, edge_num, orig, cell_a, letice, copy);

		//然后将c的模长度拉长，保证真空层是严格的25的距离
		double pingfanghe = pow(letice[2][0], 2) + pow(letice[2][1], 2) + pow(letice[2][2], 2);
		double a = pow((pow(20 + 2 * plus, 2) / pingfanghe), 0.5);
		for (i = 0; i < 3; i++)
		{
			letice[2][i] = a * letice[2][i];
		}
		/*double yuxian = (letice[0][0] * letice[1][0] + letice[0][1] * letice[1][1] + letice[0][2] * letice[1][2]) / (pow(letice[0][0] * letice[0][0] + letice[0][1] * letice[0][1] + letice[0][2] * letice[0][2], 0.5)*pow(letice[1][0] * letice[1][0] + letice[1][1] * letice[1][1] + letice[1][2] * letice[1][2], 0.5));
		double theta = acos(yuxian);
		double mo[3];
		for (i = 0; i < 3; i++)
		{
			mo[i] = pow(letice[i][0] * letice[i][0] + letice[i][1] * letice[i][1] + letice[i][2] * letice[i][2], 0.5);
		}

		for (i = 0; i < 3; i++)
		{
			if (i == 0)
			{
				letice[i][0] = mo[0];
				letice[i][1] = 0;
				letice[i][2] = 0;
			}
			if (i == 1)
			{
				letice[i][0] = mo[1] * cos(theta);
				letice[i][1] = mo[1] * sin(theta);
				letice[i][2] = 0;
			}
			if (i == 2)
			{
				letice[i][0] = letice[i][1] = 0;
				letice[i][2] = mo[2];
			}
		}*/
		if (reverse_matrix(letice, 3, letice_r) == 0)
			return 0; //建立了逆矩阵
		//这里我们需要做一步，就是将产生的矩阵做成是尽量0多的

		//到这里完成了基矢的获取，下面是确定需要放哪个原子，以及新原子的位置是哪里
		//这里需要注意，我们首先需要知道偏移了多少，然后才能根据偏移方向针对性的写出原子真实坐标

		//首先进行原子平移,选出c最小的原子进行平移
		j = 0;

		//改变之前的做法，这里让即使横平竖直的情况，坐标变换也是基于筛选
		int metal_allflag = 99;
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			for (m = 0; m < 3; m++)
			{
				fenshu_plus[i][m] = (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0]) * letice_r[0][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1]) * letice_r[1][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2]) * letice_r[2][m];
			}
			if (fenshu_plus[i][0] >= 1 - 1e-3 || fenshu_plus[i][0] < -1e-3 || fenshu_plus[i][1] >= 1 - 1e-3 || fenshu_plus[i][1] < -1e-3)
			{
				fenshu_plus[i][0] = -100;
			}
			if (fenshu_plus[i][0] != -100)
			{
			/*	if (metal_allflag == 99)
				{
					for (metal_count = 0; metal_count < 88; metal_count++)
					{
						if (cell_a.type[list[i] % cell_a.num] < meatal_xuhao[metal_count])
						{
							metal_same_falg = 1;
							metal_allflag = 0;
							break;
						}
						if (meatal_xuhao[metal_count] == cell_a.type[list[i] % cell_a.num])
							break;
					}
				}*/
				atom_plus++;
			}
		}
		//if (metal_allflag == 99)
		//{
		//	//cout<<"2d metal situation"<<endl;
		//	//cin.get();
		//	return -5;
		//}
		//反变换之后开始进行c方向偏移
		double c_pingyi = 100;
		i = 0, j = 0;
		while (i < edge_num)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				if (fenshu_plus[i][2] < c_pingyi)
				{
					c_pingyi = fenshu_plus[i][2];
				}
			}
			i++;
		}

		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				fenshu_plus[i][2] = fenshu_plus[i][2] - c_pingyi + 0.02;
			}
		}
	}

	else
	{
		//cout << "开始进入偏移的状态了哦，哈哈哈哈哈" << endl;
		//cout << "the file is:" << wenjian << endl;
		//   cin.get();

		//从这里要确认怎么生成基矢,这里生成基矢的原则是面积产生最小且最接近90°
		double **chaji_save = new double *[3];
		for (i = 0; i < 3; i++)
		{
			chaji_save[i] = new double[3];
		}
		for (i = 0; i < 3; i++)
		{
			chaji_save[1][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}

		double jvli = 100;
		double temp_jvli;
		int *mianji_small = new int[cengshu * cengshu];
		for (i = 0; i < cengshu * cengshu; i++)
		{
			mianji_small[i] = -1;
		}

		for (i = 2; i < copy_num; i++)
		{
			for (j = 0; j < 3; j++)
			{
				chaji_save[2][j] = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
			}
			temp_jvli = three_jie_chaji(chaji_save);
			if (abs(temp_jvli) > 1e-6 && temp_jvli < jvli)
			{
				jvli = temp_jvli;
			}
		}

		for (i = 2, k = 0; i < copy_num; i++)
		{
			for (j = 0; j < 3; j++)
			{
				chaji_save[2][j] = cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
			}
			temp_jvli = three_jie_chaji(chaji_save);
			if (abs(temp_jvli) > 1e-6 && abs(temp_jvli - jvli) < 1e-1)
			{
				mianji_small[k] = copy[i];
				k++;
			}
		}
		//先找到了最小的面积是多少
		//然后看最接近90°的情况
		double angle_cha = 100;
		double angle_cha_temp = 0;
		double **vector = new double *[2];
		for (i = 0; i < 2; i++)
		{
			vector[i] = new double[3];
		}
		for (i = 0; i < 3; i++)
		{
			vector[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}

		int m = 0;
		for (i = 0; i < k; i++)
		{
			if (mianji_small[i] == -1)
				break;
			for (j = 0; j < 3; j++)
			{
				vector[1][j] = cell_a.real_position[mianji_small[i] / cell_a.num][mianji_small[i] % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
			}
			angle_cha_temp = abs(vector_angle(vector) - 90);
			if (angle_cha_temp < angle_cha)
			{
				angle_cha = angle_cha_temp;
				m = mianji_small[i];
			}
		}
		for (i = 0; i < 3; i++)
		{
			letice[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
			letice[1][i] = cell_a.real_position[m / cell_a.num][m % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		}
		letice[2][0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
		letice[2][1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
		letice[2][2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];
		double plus = get_vacumlayer_thick(list, edge_num, orig, cell_a, letice, copy);

		//然后将c的模长度拉长，保证真空层是严格的25的距离
		double pingfanghe = pow(letice[2][0], 2) + pow(letice[2][1], 2) + pow(letice[2][2], 2);
		double a = pow((pow(20 + 2 * plus, 2) / pingfanghe), 0.5);
		for (i = 0; i < 3; i++)
		{
			letice[2][i] = a * letice[2][i];
		}
		/*for (i = 0; i < 3; i++)
		{
			cout<<letice[2][i]<<endl;
		}
*/
/*double yuxian = (letice[0][0] * letice[1][0] + letice[0][1] * letice[1][1] + letice[0][2] * letice[1][2]) / (pow(letice[0][0] * letice[0][0] + letice[0][1] * letice[0][1] + letice[0][2] * letice[0][2], 0.5)*pow(letice[1][0] * letice[1][0] + letice[1][1] * letice[1][1] + letice[1][2] * letice[1][2], 0.5));
double theta = acos(yuxian);
double mo[3];
for (i = 0; i < 3; i++)
{
	mo[i] = pow(letice[i][0] * letice[i][0] + letice[i][1] * letice[i][1] + letice[i][2] * letice[i][2], 0.5);
}

for (i = 0; i < 3; i++)
{
	if (i == 0)
	{
		letice[i][0] = mo[0];
		letice[i][1] = 0;
		letice[i][2] = 0;
	}
	if (i == 1)
	{
		letice[i][0] = mo[1] * cos(theta);
		letice[i][1] = mo[1] * sin(theta);
		letice[i][2] = 0;
	}
	if (i == 2)
	{
		letice[i][0] = letice[i][1] = 0;
		letice[i][2] = mo[2];
	}
}*/
		if (reverse_matrix(letice, 3, letice_r) == 0)
			return 0; //建立了逆矩阵

		//现在遇到了问题，就是实际上放进去的原子个数应该是多余一个晶胞的，或者是不止一个晶胞个数，而应该要把包括的都放进去
		//这里我们这么做，先全部放进去。然后筛选
		//这里进一步筛选出纯金属的2D材料
		int metal_allflag = 99;
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			for (m = 0; m < 3; m++)
			{
				fenshu_plus[i][m] = (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0]) * letice_r[0][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1]) * letice_r[1][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2]) * letice_r[2][m];
			}
			if (fenshu_plus[i][0] >= 1 - 1e-3 || fenshu_plus[i][0] < -1e-3 || fenshu_plus[i][1] >= 1 - 1e-3 || fenshu_plus[i][1] < -1e-3)
			{
				fenshu_plus[i][0] = -100;
			}
			if (fenshu_plus[i][0] != -100)
			{
				//说明i被选中了，看看它
				/*if (metal_allflag == 99)
				{
					for (metal_count = 0; metal_count < 88; metal_count++)
					{
						if (cell_a.type[list[i] % cell_a.num] < meatal_xuhao[metal_count])
						{
							metal_same_falg = 1;
							metal_allflag = 0;
							break;
						}
						if (meatal_xuhao[metal_count] == cell_a.type[list[i] % cell_a.num])
							break;
					}
				}*/
				atom_plus++;
			}
		}
		//if (metal_allflag == 99)
		//{
		//	//cout << "2d metal situation" << endl;
		//	//cin.get();
		//	return -5;
		//}

		//反变换之后开始进行c方向偏移
		double c_pingyi = 100;
		i = 0, j = 0;
		while (i < edge_num)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				if (fenshu_plus[i][2] < c_pingyi)
				{
					c_pingyi = fenshu_plus[i][2];
				}
			}
			i++;
		}

		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			if (fenshu_plus[i][0] != -100)
			{
				fenshu_plus[i][2] = fenshu_plus[i][2] - c_pingyi + 0.02;
			}
		}

		for (i = 0; i < 3; i++)
		{
			delete[] chaji_save[i];
		}
		delete[] chaji_save;
		delete[] mianji_small;
		for (i = 0; i < 2; i++)
		{
			delete[] vector[i];
		}
		delete[] vector;
		//cin.get();
	}

	//然后我生成了新的坐标，开始写文件吧
	FILE *out = fopen(name.c_str(), "wt");
	char atom_head[20] = "atoms";

	if (copy_num == cengshu * cengshu)
	{
		fprintf(out, "%d\n", atom_plus);
	}
	else
	{
		fprintf(out, "%d\n", atom_plus);
	}

	//fprintf(out, "%s\n", atom_head);
	fprintf(out, "%s\n", file[0].c_str());
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fprintf(out, "\t%.9lf\t", letice[i][j]);
		}
		fprintf(out, "\n");
	}
	fprintf(out, "%s\n", file[1].c_str());
	j = 0;

	if (copy_num == cengshu * cengshu)
	{
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			int mm = 1;
			if (fenshu_plus[i][0] != -100)
			{
				fprintf(out, "  %d\t", cell_a.type[list[i] % cell_a.num]);
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%.9lf\t", fenshu_plus[i][m]);
				}
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%d\t", mm);
				}
				fprintf(out, "\n");
			}
		}
		fclose(out);
	}
	else
	{
		for (i = 0; i < edge_num; i++)
		{
			if (list[i] == -1)
				break;
			int mm = 1;
			if (fenshu_plus[i][0] != -100)
			{
				fprintf(out, "  %d\t", cell_a.type[list[i] % cell_a.num]);
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%.9lf\t", fenshu_plus[i][m]);
				}
				for (int m = 0; m < 3; m++)
				{
					fprintf(out, "%d\t", mm);
				}
				fprintf(out, "\n");
			}
		}
		fclose(out);
	}

	for (i = 0; i < atom_num_write; i++)
	{
		delete[] fenshu[i];
	}
	delete[] fenshu;
	for (i = 0; i < 3; i++)
	{
		delete[] letice_r[i];
	}
	delete[] letice_r;
	delete[] copy;
	for (i = 0; i < 3; i++)
	{
		delete[] letice[i];
	}
	delete[] letice;
	delete[] atom_flag_a;
	for (i = 0; i < edge_num; i++)
	{
		delete[] fenshu_plus[i];
	}
	delete[] fenshu_plus;
	return 1;
}

double get_vacumlayer_thick(int *list, int edge_num, int orig, cell cell_a, double **letice, int *copy)
{
	int i = 0, j = 0;
	int *dis_flag = new int[edge_num];
	for (int i = 0; i < edge_num; i++)
	{
		dis_flag[i] = 0;
	}
	//这个用来标记这个连通分量中哪些需要进行求距离的运算

	int *atom_flag = new int[cell_a.num];
	for (int i = 0; i < cell_a.num; i++)
	{
		atom_flag[i] = 0;
	} //标记哪些原子用到了
	for (int i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (int j = 0; j < cell_a.num; j++)
		{
			if ((atom_flag[j] != 1) && (list[i] % cell_a.num == j))
			{
				atom_flag[j] = 1;
				break;
			}
		}
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		else
		{
			if (list[i] % cell_a.num != orig && atom_flag[list[i] % cell_a.num] == 1)
			{
				dis_flag[i] = 1;
				atom_flag[list[i] % cell_a.num]++;
			}
		}
	}
	//到这里完成了该计算哪些原子的点面距离了

	double plus = 0;
	double temp_dis = 0;
	//其实现在a和b就是两个向量，c就是法向量
	double xiangliang[2][3];
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			xiangliang[i][j] = letice[i][j];
		}
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] != -1 && dis_flag[i] == 1)
		{
			//从连通分量中找出点到面的距离
			temp_dis = face_point_dis(xiangliang, cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num], cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num]);
			if (temp_dis > plus)
			{
				plus = temp_dis;
			}
		}
	}
	delete[] dis_flag;
	delete[] atom_flag;
	return plus;
}
inline double three_jie_chaji(double **a)
{
	int i = 0, j = 0;
	double temp = 0;
	temp = pow(pow((a[1][1] * a[2][2] - a[1][2] * a[2][1]), 2) + pow((a[1][2] * a[2][0] - a[1][0] * a[2][2]), 2) + pow((a[1][0] * a[2][1] - a[1][1] * a[2][0]), 2), 0.5);
	return temp;
}

inline int inone_face(double **a) //���淵��1�������淵��0
{
	int i = 0, j = 0;
	double x1, x2, x3, y1, y2, y3, z1, z2, z3;
	x1 = a[0][0] - a[3][0];
	x2 = a[1][0] - a[3][0];
	x3 = a[2][0] - a[3][0];

	y1 = a[0][1] - a[3][1];
	y2 = a[1][1] - a[3][1];
	y3 = a[2][1] - a[3][1];

	z1 = a[0][2] - a[3][2];
	z2 = a[1][2] - a[3][2];
	z3 = a[2][2] - a[3][2];
	double k = 0;
	k = (x1 * y2 * z3) + (x2 * y3 * z1) + (x3 * y1 * z2) - (x3 * y2 * z1) - (y3 * z2 * x1) - (z3 * x2 * y1);

	if (-1e-1 < k && k < 1e-1)
		return 1;
	else
		return 0;
}

double det(double **a, int num)
{
	int i = 0, j = 0, k = 0;
	if (num == 1)
	{
		return a[0][0];
	}
	double ans = 0;
	double **temp_det = new double *[num];
	for (i = 0; i < num; i++)
	{
		temp_det[i] = new double[num];
	}

	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num - 1; j++)
		{
			for (k = 0; k < num - 1; k++)
			{
				temp_det[j][k] = a[j + 1][(k >= i) ? k + 1 : k];
			}
		}
		double t = det(temp_det, num - 1);

		if (i % 2 == 0)
		{
			ans = ans + a[0][i] * t;
		}
		else
		{
			ans = ans - a[0][i] * t;
		}
	}
	return ans;
}

//����һ����������ĺ�����������ԭ��������Ƕ�Ӧλ�ñ�����ʽ���İ������
void getastar(double **a, int num, double **b)
{
	int i = 0, j = 0, k = 0, m = 0;
	if (num == 1)
	{
		b[0][0] = 1;
		return;
	}
	double **temp = new double *[num];
	for (i = 0; i < num; i++)
	{
		temp[i] = new double[num];
	}
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			for (k = 0; k < num - 1; k++)
			{
				for (m = 0; m < num - 1; m++)
				{
					temp[k][m] = a[(k >= i) ? k + 1 : k][(m >= j) ? m + 1 : m];
				}
			}

			b[j][i] = det(temp, num - 1);
			if ((i + j) % 2 != 0)
			{
				b[j][i] = -b[j][i];
			}
		}
	}
	for (i = 0; i < num; i++)
	{
		delete[] temp[i];
	}
	delete[] temp;
	return;
}

int reverse_matrix(double **a, int num, double **b)
{
	int i = 0, j = 0;
	double **temp = new double *[num];
	for (i = 0; i < num; i++)
	{
		temp[i] = new double[num];
	}
	getastar(a, num, temp);
	double value = det(a, num);
	if (abs(value) < 1e-4)
	{
		cout << "can not reverse!please check!" << endl;
		return 0;
	}
	for (i = 0; i < num; i++)
	{
		for (j = 0; j < num; j++)
		{
			b[i][j] = temp[i][j] / value;
		}
	}
	return 1;
}

string int2string(int i)
{

	stringstream ss;
	ss << i;

	return ss.str();
}

inline double face_point_dis(double letice[2][3], double in_face_point[3], double point[3])
{
	int i = 0, j = 0;
	double fa[3] = { 0 };
	fa[0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
	fa[1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
	fa[2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];
	double qp[3] = { 0 };
	for (i = 0; i < 3; i++)
	{
		qp[i] = in_face_point[i] - point[i];
	}
	double fenzi = qp[0] * fa[0] + qp[1] * fa[1] + qp[2] * fa[2];
	double dis = 0;
	dis = abs(fenzi / pow(pow(fa[0], 2) + pow(fa[1], 2) + pow(fa[2], 2), 0.5));
	return dis;
}

inline double vector_angle(double **a)
{
	//用来求两个向量的夹角，结果以角度制返回

	double a_mu = pow(pow(a[0][0], 2) + pow(a[0][1], 2) + pow(a[0][2], 2), 0.5);
	double b_mu = pow(pow(a[1][0], 2) + pow(a[1][1], 2) + pow(a[1][2], 2), 0.5);
	double diancheng = a[0][0] * a[1][0] + a[0][1] * a[1][1] + a[0][2] * a[1][2];
	double jungel_orig = acos(diancheng / (a_mu * b_mu)) * 180 / pi;
	//cout << "the judge angle is:" << jungel_orig << endl;
	return jungel_orig;
}

void buble_plus(double *a, int *b, int num)
{
	int i = 0, j = 0;
	int flag = 0;
	for (i = 0; i < num; i++)
	{
		b[i] = i;
	}
	double temp;
	int temp_xuhao;
	for (i = num - 1; i >= 0; i--)
	{
		flag = 0;
		for (j = 0; j < i; j++)
		{
			if (a[j] > a[j + 1]) //如果是这样的话需要交换
			{
				flag = 1;
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;

				temp_xuhao = b[j];
				b[j] = b[j + 1];
				b[j + 1] = temp_xuhao;
			}
		}
		if (flag == 0)
			break;
	}
	return;
}


bool if_valence_conncet(cell cell_a, element* e)
{
	//输入一个结构判断结构是否存在共价连接的情况
	int i = 0, j = 0;
	double dise = 0;
	for (i = 0; i < cell_a.num; i++)
	{
		if (cell_a.my_classify[i] == 2)
		{
			for (j = 0; j < yanshen*cell_a.num; j++)
			{
				if (cell_a.my_classify[j%cell_a.num] == 2)
				{
					//此时i是主族元素，j是周边的主族元素
					dise = dis(cell_a.real_position[13][i], cell_a.real_position[j / cell_a.num][j%cell_a.num]);
					if (abs(dise) > 1e-3 && dise<(e[cell_a.type[i]].cov_radius+e[cell_a.type[j%cell_a.num]].cov_radius)/100.0*val_radius_factor)
					{
						//说明存在共价元素
						//标记退出
						return true;
					}
				}
			}
		}
	}
	return false;
}


int generate_atom_0d(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list, int** expand_graph)
{
	int i = 0, j = 0, k = 0;
	int atom_num_write = 0;
	int orig = judge[1] % cell_a.num;
	for (int i = 0; i < edge_num; i++) {
		if (list[i] == -1)
			break;
		atom_num_write++;
	}
	double **fenshu_plus = new double *[edge_num];
	for (i = 0; i < edge_num; i++)
	{
		fenshu_plus[i] = new double[3];
	}
	double **letice = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice[i] = new double[3]; //建立letice数组用来储存
	}
	double **letice_r = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice_r[i] = new double[3];
	}
	
	int basic_length = 15;
	//然后是创造一个10,10,10的单元
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			letice[i][j] = 0;
	}
	letice[0][0] = letice[1][1] = letice[2][2] = basic_length;
	if (reverse_matrix(letice, 3, letice_r) == 0)
		return 0; //建立了逆矩阵
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (int m = 0; m < 3; m++)
		{
			fenshu_plus[i][m] = (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][0] - cell_a.real_position[list[0] / cell_a.num][list[0] % cell_a.num][0]) * letice_r[0][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][1] - cell_a.real_position[list[0] / cell_a.num][list[0] % cell_a.num][1]) * letice_r[1][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][2] - cell_a.real_position[list[0] / cell_a.num][list[0] % cell_a.num][2]) * letice_r[2][m];
			//cout << fenshu_plus[i][m] << ",";
		}		
	}
	//找到最小的
	double c_pingyi = 100, b_pingyi = 100, a_pingyi = 100;;
	i = 0, j = 0;
	while (i < edge_num)
	{
		if (list[i] == -1)
			break;
		if (fenshu_plus[i][0] != -100)
		{
			if (fenshu_plus[i][0] < a_pingyi)
			{
				a_pingyi = fenshu_plus[i][0];
			}
			if (fenshu_plus[i][2] < c_pingyi)
			{
				c_pingyi = fenshu_plus[i][2];
			}
			if (fenshu_plus[i][1] < b_pingyi)
			{
				b_pingyi = fenshu_plus[i][1];
			}
		}
		i++;
	}
	vector<int > save_type;
	vector<vector<double>> save_loc;
	vector<vector<double>> new_fenshu;
	double a_max=-100,b_max = -100, c_max = -100;
	double a_min=100,b_min = 100, c_min = 100;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (fenshu_plus[i][0] != -100)
		{
			fenshu_plus[i][0] = fenshu_plus[i][0] - a_pingyi + default_org_loc;
			fenshu_plus[i][2] = fenshu_plus[i][2] - c_pingyi + default_org_loc;
			fenshu_plus[i][1] = fenshu_plus[i][1] - b_pingyi + default_org_loc;
			if (fenshu_plus[i][0] > 1)
				fenshu_plus[i][0] = fenshu_plus[i][0] - floor(fenshu_plus[i][0]);
			if (fenshu_plus[i][1] > 1)
				fenshu_plus[i][1] = fenshu_plus[i][1] - floor(fenshu_plus[i][1]);
			if (fenshu_plus[i][2] > 1)
				fenshu_plus[i][2] = fenshu_plus[i][2] - floor(fenshu_plus[i][2]);
			//储存求得的真实坐标
			vector<double> temp_loc(3);
			for (int j = 0; j < 3; j++) {
				temp_loc[j] = fenshu_plus[i][j] * letice[j][j];
				//cout << temp_loc[j] << endl;
			}
			save_loc.push_back(temp_loc);
			save_type.push_back(cell_a.type[list[i] % cell_a.num]);
			//求得最大值
			a_max = max(a_max, fenshu_plus[i][0]);
			b_max = max(b_max, fenshu_plus[i][1]);
			c_max = max(c_max, fenshu_plus[i][2]);
			a_min = min(a_min, fenshu_plus[i][0]);
			b_min = min(b_min, fenshu_plus[i][1]);
			c_min = min(c_min, fenshu_plus[i][2]);
		}

	}

	//补充上真空层的部分判定
	//cout << "bmax is:" << b_max << "," << "cmax is:" << c_max << endl;
	//cout << "bmin is :" << b_min << "," << "c_min is:" << c_min << endl;
	double a_vacum = abs(letice[0][0] * ((1 + a_min) - a_max));
	double b_vacum = abs(letice[1][1] * ((1 + b_min) - b_max));
	double c_vacum = abs(letice[2][2] * ((1 + c_min) - c_max));
	double a_factor = letice[0][0] - a_vacum + oned_b_length;
	double b_factor = letice[1][1] - b_vacum + oned_b_length;
	double c_factor = letice[2][2] - c_vacum + oned_c_length;
	letice[0][0] = a_factor;
	letice[1][1] = b_factor;
	letice[2][2] = c_factor;
	//cout << "last one:" << endl;
	//shwo_letice(letice);
	//cin.get();
	//最后重新反变换基矢坐标
	if (letice[0][0]> TO_LARGE_CELL_length||letice[1][1] > TO_LARGE_CELL_length || letice[2][2] > TO_LARGE_CELL_length) {
		cout << name << ": to large!" << endl;
		letice[0][0]=letice[1][1] = letice[2][2] = TO_LARGE_CELL_length;
		if (reverse_matrix(letice, 3, letice_r) == 0)
			return 0; //建立了逆矩阵
		//shwo_letice(letice_r);
		for (int i = 0; i < atom_num_write; i++) {
			//cout << save_loc[i][0] << "," << letice_r[0][0] << endl;
			vector<double>temp_fenshu(3);
			for (int j = 0; j < 3; j++) {
				temp_fenshu[j] = save_loc[i][0] * letice_r[0][j] + save_loc[i][1] * letice_r[1][j] + save_loc[i][2] * letice_r[2][j];
			}
			new_fenshu.push_back(temp_fenshu);
		}
	}
	else
	{
		//正常情况下，先变换分数
		for (int i = 0; i < atom_num_write; i++) {
			//cout << save_loc[i][0] << "," << letice_r[0][0] << endl;
			vector<double>temp_fenshu(3);
			for (int j = 0; j < 3; j++) {
				temp_fenshu[j] = save_loc[i][j] / letice[j][j];
			}
			new_fenshu.push_back(temp_fenshu);
		}

	}
	a_min=100,b_min = 100, c_min = 100;
	for (int i = 0; i < new_fenshu.size(); i++) {
		a_min = min(a_min, new_fenshu[i][0]);
		b_min = min(b_min, new_fenshu[i][1]);
		c_min = min(c_min, new_fenshu[i][2]);
	}
	for (int i = 0; i < new_fenshu.size(); i++) {
		new_fenshu[i][0] -= (a_min - default_org_loc);
		new_fenshu[i][1] -= (b_min - default_org_loc);
		new_fenshu[i][2] -= (c_min - default_org_loc);
	}
	//建立了储存的原子信息
	my_atom* my_save = new my_atom;
	my_save->num = atom_num_write;
	my_save->type = save_type;
	my_save->fenshu = new_fenshu;
	my_save->real_loc = save_loc;
	my_save->letice = letice;
	my_save->save_information(name);

	
	
	for (i = 0; i < 3; i++)
	{
		delete[] letice_r[i];
	}
	delete[] letice_r;	
	for (i = 0; i < 3; i++)
	{
		delete[] letice[i];
	}
	delete[] letice;	
	for (i = 0; i < edge_num; i++)
	{
		delete[] fenshu_plus[i];
	}
	delete[] fenshu_plus;
	return 1;
}

int generate_atom_1d(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list,int** expand_graph)
{

	//这个的作用是根据得到的连通信息生成atom文件,输入是存储的连通分量，输出是atom文件

	int i = 0, j = 0;
	int k = 0;
	int exit_flag = 0;
	int orig = judge[1]%cell_a.num;
	//cout << "这里的对应原始点是：" << orig << endl;
	int num = cell_a.num;
	
	//首先开始生成abc的三个基矢分量
	//从原型序号中找到这样的点以及这样的距离

	double **letice = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice[i] = new double[3]; //建立letice数组用来储存
	}
	double **letice_r = new double *[3];
	for (i = 0; i < 3; i++)
	{
		letice_r[i] = new double[3];
	}
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		//cout << list[i] << endl;
		if (list[i] % cell_a.num == orig)
		{
			//cout << list[i] << endl;
			j++;
		}
	}
	int copy_num = j;
	int *copy = new int[j];
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (list[i] % cell_a.num == orig)
		{
			copy[k++] = list[i];
		}
	}

	//有j个这样的复制体,并且都找到了，并且也判断了哪些原子需要放进去
	int *atom_flag_a = new int[edge_num];
	//这里的atom_falg_a针对的是第一种情况的二维材料
	for (i = 0; i < edge_num; i++)
	{
		atom_flag_a[i] = 0;
	}
	k = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (j = 0; j < cell_a.num; j++)
		{
			if (list[i] % cell_a.num == j && atom_flag_a[i] == 0)
			{
				atom_flag_a[i] = 1;
				k++;				
			}
		}
	}
	int atom_num_write = k;
	double **fenshu = new double *[atom_num_write];
	for (i = 0; i < k; i++)
	{
		fenshu[i] = new double[3];
	}
	double **fenshu_plus = new double *[edge_num];
	for (i = 0; i < edge_num; i++)
	{
		fenshu_plus[i] = new double[3];
	}
	int atom_plus = 0;

	// 然后根据两种情况进行分类

	//相对于二维情况，一维情况我们只需要关注a方向的基矢，b可以自己创造，c也是基于此创造出来
	//明确原子放进来的机制
	int m = 0;
	double aa = 1e-3;
	for (i = 0; i < 3; i++)
	{
		letice[0][i] = cell_a.real_position[copy[1] / cell_a.num][copy[1] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
		if (abs(letice[0][i]) > aa)
		{
			aa = abs(letice[0][i]);
		}			
		//letice[1][i] = cell_a.real_position[copy[copy_num / cengshu] / cell_a.num][copy[copy_num / cengshu] % cell_a.num][i] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][i];
	}
	j = 0;
	//然后创造出b的基矢
	//这里b的基矢是指与第一个点相连之中，最垂直，距离最远的那个,从距离最远的依次看角度
	//修改，这里我们随便选定一个，并且放到垂直的地方
	//letice[0][0] = pow(pow(letice[0][0],2)+pow(letice[0][1],2)+pow(letice[0][2],2),0.5);
	//letice[0][1] = letice[0][2] = 0;
	//double bb = sunbets_get_distance(judge[1],edge_num, cell_a, list,expand_graph);
	//cout << "benshen shi " << bb << endl;
	//cin.get();
	/*letice[1][0] = 0;
	letice[1][1] = bb;
	letice[1][2] = 0;*/
	letice[1][0] =letice[1][2]=0;
	letice[1][1] = 5;
	if (abs(letice[0][1]) > 1e-4 &&abs(letice[0][2]) > 1e-4)//b和c都不为0
	{
		letice[1][2] = -letice[0][1] * letice[1][1] / letice[0][2];
	}
	else if(abs(letice[0][1]) < 1e-4 && abs(letice[0][2]) > 1e-4)  //b为0
	{
		letice[1][2] = 0;
	}
	else if (abs(letice[0][1]) > 1e-4 && abs(letice[0][2]) < 1e-4)//c为0
	{
		letice[1][2] = 5;
		letice[1][1] = letice[1][0] = 0;
	}
	else //
	{

	}
	//cout << "original:" << endl;
	//shwo_letice(letice);
	//int connect_num = 0;
	//for (i = 0; i < edge_num; i++)
	//{
	//	if (list[i] == -1)
	//		break;
	//	if (list[i] != copy[1]&&list[i] != copy[0] && expand_graph[copy[0]][list[i]] == 1)
	//	{
	//		connect_num++;
	//	}
	//}
	//struct three_double* my_double = new struct three_double[connect_num];


	//for (i = 0; i < edge_num; i++)
	//{
	//	if (list[i] == -1)
	//		break;
	//	if (list[i] != copy[1] && list[i] != copy[0] && expand_graph[copy[0]][list[i]] == 1)
	//	{
	//		my_double[j].distnace = dis(cell_a.real_position[copy[0]/cell_a.num][copy[0]%cell_a.num], cell_a.real_position[list[i]/cell_a.num][list[i]%cell_a.num]);
	//		my_double[j].xuhao = list[i];
	//		j++;
	//	}
	//}
	///*for (i = 0; i < connect_num; i++)
	//{
	//	cout << my_double[i].distnace << "," << my_double[i].xuhao << endl;
	//}*/
	//qsort((void*)&my_double[0],connect_num,sizeof(struct three_double),Comp);
	///*for (i = 0; i < connect_num; i++)
	//{
	//	cout << my_double[i].distnace << "," << my_double[i].xuhao << endl;
	//}*/
	////按照距离从大到小排序完了
	////下面开始查看他们的角度
	//double distance_cha = 0;
	//double **vector = new double*[2];
	//for (i = 0; i < 2; i++)
	//{
	//	vector[i] = new double[3];
	//}
	//for (i = 0; i < 3; i++)
	//{
	//	vector[0][i] = letice[0][i];
	//}
	////这里预防一个情况，如果找不到角度合适的怎么办？
	//bool angel_find = false;
	//for (i = 0; i < connect_num; i++)
	//{
	//	for (j = 0; j < 3; j++)
	//	{
	//		vector[1][j] = cell_a.real_position[my_double[i].xuhao / cell_a.num][my_double[i].xuhao % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j];
	//		//letice[1][j] = 3 * (cell_a.real_position[my_double[i].xuhao / cell_a.num][my_double[i].xuhao % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j]);
	//		
	//	}
	//	double angle = abs(vector_angle(vector));
	//	if (angle > double(90-oneD_angel_rule) &&angle<double(90+oneD_angel_rule) )
	//	{
	//		distance_cha = abs(my_double[i].distnace - my_double[0].distnace);
	//		for (j = 0; j < 3; j++)
	//		{
	//			letice[1][j] = 5*(cell_a.real_position[my_double[i].xuhao / cell_a.num][my_double[i].xuhao % cell_a.num][j] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][j]);
	//		}
	//		angel_find = true;
	//		break;
	//	}

	//}
	//
	//delete[]my_double;
	//for (i = 0; i < 2; i++)
	//{
	//	delete[]vector[i];
	//}
	//delete[]vector;
	//if (angel_find == false)
	//	return 0;
	////控制b和c的长度	
	//对于b的控制先扩充几倍如果不够，再扩充到固定长度
	double pingfanghe = pow(letice[1][0], 2) + pow(letice[1][1], 2) + pow(letice[1][2], 2);
	double a = pow((pow(default_length, 2) / pingfanghe), 0.5);
	for (i = 0; i < 3; i++)
	{
		letice[1][i] = a * letice[1][i];
	}
	letice[2][0] = letice[0][1] * letice[1][2] - letice[0][2] * letice[1][1];
	letice[2][1] = letice[0][2] * letice[1][0] - letice[0][0] * letice[1][2];
	letice[2][2] = letice[0][0] * letice[1][1] - letice[0][1] * letice[1][0];


	//pingfanghe = 0, a = 0;

	
	pingfanghe = pow(letice[2][0], 2) + pow(letice[2][1], 2) + pow(letice[2][2], 2);
	a = pow((pow(default_length, 2) / pingfanghe), 0.5);
	for (i = 0; i < 3; i++)
	{
		letice[2][i] = a * letice[2][i];
	}
	//cout << "after expand 1:" << endl;
	//shwo_letice(letice);
	if (reverse_matrix(letice, 3, letice_r) == 0)
		return 0; //建立了逆矩阵

	//到这里完成了基矢的获取，下面是确定需要放哪个原子，以及新原子的位置是哪里
	//这里需要注意，我们首先需要知道偏移了多少，然后才能根据偏移方向针对性的写出原子真实坐标

	//现在遇到了问题，就是实际上放进去的原子个数应该是多余一个晶胞的，或者是不止一个晶胞个数，而应该要把包括的都放进去
	//这里我们这么做，先全部放进去。然后筛选
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		for (m = 0; m < 3; m++)
		{
			fenshu_plus[i][m] = (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][0] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][0]) * letice_r[0][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][1] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][1]) * letice_r[1][m] + (cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num][2] - cell_a.real_position[copy[0] / cell_a.num][copy[0] % cell_a.num][2]) * letice_r[2][m];
			//cout << fenshu_plus[i][m] << ",";
		}	
		atom_plus++;
	}

	//在检视一下有没有重复的原子的坐标
	struct Atom_check* ond_check = new struct Atom_check[atom_plus];
	j = 0;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;		
		if (fenshu_plus[i][0] != -100)
		{
			ond_check[j++].element = cell_a.type[list[i] % cell_a.num];
			ond_check[j - 1].xuhao = i;
			ond_check[j - 1].same_flag = false;
			for (int k = 0; k < 3; k++)
			{
				ond_check[j - 1].loc[k] = fenshu_plus[i][k];
			}
		}
	}
	//cout << atom_plus << endl;
	int atom_real_num = atom_plus;
	for (int i = 0; i < atom_plus; i++)
	{
		if (ond_check[i].same_flag == false)
		{
			for (int j = i+1; j < atom_plus; j++)
			{				
				double a = fmod(abs(ond_check[j].loc[0] - ond_check[i].loc[0]),1.0);
				double b = fmod(abs(ond_check[j].loc[1] - ond_check[i].loc[1]), 1.0);
				double c = fmod(abs(ond_check[j].loc[2] - ond_check[i].loc[2]), 1.0);
				//debug:这里有可能出现为1的情况
				if (abs(a - 1.0) < same_atom_rule)
					a = 0;
				if (abs(b - 1.0) < same_atom_rule)
					b = 0;
				if (abs(c - 1.0) < same_atom_rule)
					c = 0;
				//cout << a << "," << b << "," << c << endl;
				if (ond_check[i].element == ond_check[j].element && a<same_atom_rule && b<same_atom_rule&& c<same_atom_rule)
				{
					ond_check[j].same_flag = true;
					fenshu_plus[ond_check[j].xuhao][0] = -100;
					atom_real_num--;
					//cout << "encounter same" << ond_check[j].loc[0] << "," << ond_check[j].loc[1] << "," << ond_check[j].loc[2] << endl;
					//cout << ond_check[i].loc[0] << "," << ond_check[i].loc[1] << "," << ond_check[i].loc[2] << endl;

				}

			}
		}
	}
	//坐标变化归一化部分
	letice[0][0] = pow((pow(letice[0][0], 2) + pow(letice[0][1], 2) + pow(letice[0][2], 2)), 0.5);
	letice[1][1] = pow((pow(letice[1][0], 2) + pow(letice[1][1], 2) + pow(letice[1][2], 2)), 0.5);
	letice[2][2] = pow((pow(letice[2][0], 2) + pow(letice[2][1], 2) + pow(letice[2][2], 2)), 0.5);
	letice[0][1] = letice[0][2] = letice[1][0] = letice[1][2] = letice[2][0] = letice[2][1] = 0;

	//cout << "guiyihua :" << endl;
	//shwo_letice(letice);
	//cin.get();
	double c_pingyi = 100, b_pingyi = 100, a_pingyi = 100;;
	i = 0, j = 0;
	while (i < edge_num)
	{
		if (list[i] == -1)
			break;
		if (fenshu_plus[i][0] != -100)
		{
			if (fenshu_plus[i][0] < a_pingyi)
			{
				a_pingyi = fenshu_plus[i][0];
			}
			if (fenshu_plus[i][2] < c_pingyi)
			{
				c_pingyi = fenshu_plus[i][2];
			}
			if (fenshu_plus[i][1] < b_pingyi)
			{
				b_pingyi = fenshu_plus[i][1];
			}
		}
		i++;
	}
	vector<int > save_type;
	vector<vector<double>> save_loc;
	vector<vector<double>> new_fenshu;
	double b_max = -100, c_max = -100;
	double b_min = 100, c_min = 100;
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		if (fenshu_plus[i][0] != -100)
		{
			fenshu_plus[i][0] = fenshu_plus[i][0] - a_pingyi + default_org_loc;
			fenshu_plus[i][2] = fenshu_plus[i][2] - c_pingyi + default_org_loc;
			fenshu_plus[i][1] = fenshu_plus[i][1] - b_pingyi + default_org_loc;
			if (fenshu_plus[i][0] > 1)
				fenshu_plus[i][0] = fenshu_plus[i][0] - floor(fenshu_plus[i][0]);
			if (fenshu_plus[i][1] > 1)
				fenshu_plus[i][1] = fenshu_plus[i][1] - floor(fenshu_plus[i][1]);
			if (fenshu_plus[i][2] > 1)
				fenshu_plus[i][2] = fenshu_plus[i][2] - floor(fenshu_plus[i][2]);
			//储存求得的真实坐标
			vector<double> temp_loc(3);
			for (int j = 0; j < 3; j++) {
				temp_loc[j] = fenshu_plus[i][j] * letice[j][j];
				//cout << temp_loc[j] << endl;
			}
			save_loc.push_back(temp_loc);
			save_type.push_back(cell_a.type[list[i] % cell_a.num]);
			//求得最大值
			b_max = max(b_max, fenshu_plus[i][1]);
			c_max = max(c_max, fenshu_plus[i][2]);
			b_min = min(b_min, fenshu_plus[i][1]);
			c_min = min(c_min, fenshu_plus[i][2]);
		}	

	}

	//cin.get();
	
	//补充上真空层的部分判定
	//cout << "bmax is:" << b_max << "," << "cmax is:" << c_max << endl;
	//cout << "bmin is :" << b_min << "," << "c_min is:" << c_min << endl;
	double b_vacum = abs(letice[1][1] * ((1 + b_min) - b_max));
	double c_vacum=abs(letice[2][2] * ((1+c_min)-c_max));
	double b_factor = letice[1][1] - b_vacum+oned_b_length;
	double c_factor = letice[2][2] - c_vacum+oned_c_length;
	letice[1][1] = b_factor;
	letice[2][2] = c_factor;
	//cout << "last one:" << endl;
	//shwo_letice(letice);
	//cin.get();
	//最后重新反变换基矢坐标
	if (letice[1][1] > TO_LARGE_CELL_length || letice[2][2] > TO_LARGE_CELL_length) {
		cout << name << ": to large!" << endl;
		letice[1][1] = letice[2][2] = TO_LARGE_CELL_length;
		if (reverse_matrix(letice, 3, letice_r) == 0)
			return 0; //建立了逆矩阵
		//shwo_letice(letice_r);
		for (int i = 0; i < atom_real_num; i++) {
			//cout << save_loc[i][0] << "," << letice_r[0][0] << endl;
			vector<double>temp_fenshu(3);
			for (int j = 0; j < 3; j++) {
				temp_fenshu[j] = save_loc[i][0] * letice_r[0][j] + save_loc[i][1] * letice_r[1][j] + save_loc[i][2] * letice_r[2][j];
			}
			new_fenshu.push_back(temp_fenshu);
		}
	}
	else
	{
		//正常情况下，先变换分数
		for (int i = 0; i < atom_real_num; i++) {
			//cout << save_loc[i][0] << "," << letice_r[0][0] << endl;
			vector<double>temp_fenshu(3);	
			for (int j = 0; j < 3; j++) {
				temp_fenshu[j] = save_loc[i][j] / letice[j][j];
			}			
			new_fenshu.push_back(temp_fenshu);
		}
		
	}
	b_min = 100, c_min = 100;
	for (int i = 0; i < new_fenshu.size(); i++) {
		b_min = min(b_min, new_fenshu[i][1]);
		c_min = min(c_min, new_fenshu[i][2]);
	}
	for (int i = 0; i < new_fenshu.size(); i++) {
		new_fenshu[i][1] -= (b_min - default_org_loc);
		new_fenshu[i][2] -= (c_min - default_org_loc);
	}
	//建立了储存的原子信息
	my_atom* my_save = new my_atom;
	my_save->num = atom_real_num;
	my_save->type = save_type;
	my_save->fenshu = new_fenshu;
	my_save->real_loc = save_loc;
	my_save->letice = letice;
	my_save->save_information(name);
	
	
	for (i = 0; i < atom_num_write; i++)
	{
		delete[] fenshu[i];
	}
	delete[] fenshu;
	for (i = 0; i < 3; i++)
	{
		delete[] letice_r[i];
	}
	delete[] letice_r;
	delete[] copy;
	for (i = 0; i < 3; i++)
	{
		delete[] letice[i];
	}
	delete[] letice;
	delete[] atom_flag_a;
	for (i = 0; i < edge_num; i++)
	{
		delete[] fenshu_plus[i];
	}	
	delete[] fenshu_plus;
	return 1;
}

double get_line_dis(int *list, int edge_num, int orig, cell cell_a, int *copy)
{
	int i = 0, j = 0, k = 0;
	//这里的程序实现这样的功能，找到这个链上最长的距离
	for (i = 0; i < edge_num; i++)
	{
		if (list[i] == -1)
			break;
		else
		{
			if (list[i] % cell_a.num == orig)
			{
				j++;
			}
		}
	}
	double *distance = new double[j];
	double temp = 0;
	double max_dis = 0;
	for (i = 0; i < j; i++)
	{
		distance[i] = 200;
	}
	for (i = 0; i < j; i++)
	{
		for (k = 0; k < edge_num; k++)
		{
			if (list[k] == -1)
				break;
			temp = dis(cell_a.real_position[copy[i] / cell_a.num][copy[i] % cell_a.num], cell_a.real_position[list[k] / cell_a.num][list[k] % cell_a.num]);
			if (temp > 1e-3 && list[k] % cell_a.num != orig && temp < distance[i])
			{
				distance[i] = temp;
			}
		}
	}
	//到这里找出了每个最小的distance
	for (i = 0; i < j; i++)
	{
		if (distance[i] > max_dis)
			max_dis = distance[i];
	}

	delete[] distance;
	return max_dis;
}


int Comp(const void *p1, const void *p2)
{
	//自定义一个排序函数
	if ( ((struct three_double*)p1)->distnace < ((struct three_double*)p2)->distnace)
	{
		return 1;
	}	
	return 0;
}




int get_tablefor_1dchemistry(string&path, string& file_name,int num,element*e)
{
	
	int flag[120] = { 0 };
	qsort((void *)&e[1], 103, sizeof(class element), comp_nega);
	int yueshu = 1;
	int i = 0, j = 0;
	ifstream fin;
	fin.open(file_name, ios::in);
	if (!fin.is_open())
	{
		cout << "i can not find the file:name" << endl;
		cin.get();
	}
	string name;
	ofstream fout;
	fout.open("1d_analyse", ios::out);
	fout << "name" << "\t" << "all_name" << "\t" << "simplest_name" << endl;
	string result;
	for (i = 0; i < num; i++)
	{
		bool exit = false;
		fin >> name;
		fout << name << "\t";
		cell cell_a(const_cast<char*>((path + name).c_str()));
		fout << get_formula_name(cell_a) << "\t";
		//下面开始真正的输出名字
		
		for (j = 0; j < cell_a.num; j++)
		{
			flag[cell_a.type[j]]++;
		}
		while (exit==false)
		{
			for (j = 0; j < 120; j++)
			{
				/*if (flag[j] != 0)
				{
					cout << flag[j] << endl;
				}*/
				
				if (flag[j] != 0 && flag[j] % yueshu != 0)
				{
					exit = true;
				}
			}
			yueshu++;
			//cout << yueshu << endl;
		}
		yueshu = yueshu - 2;
		//cout << yueshu << endl;
		for (j = 0; j < 120; j++)
		{
			if (flag[j] != 0)
			{
				flag[j] = flag[j] / yueshu;
				//cout <<flag[j] << endl;
			}
		}
		//然后按照电负性顺序拼接起来
		/*for (i = 1; i < 103; i++)
		{
			cout << e[i].electron_negativity << "\t" << e[i].name << endl;
		}*/
		
		//先检查一下是不是排好序了
		/*for (i = 1; i < 103; i++)
		{
			cout << e[i].electron_negativity << "\t" << e[i].atomic_num<< endl;
		}
		cin.get();*/
		
		for (j = 1; j < 104; j++)
		{
			if (flag[e[j].atomic_num] != 0)
			{
				result += atom_name[e[j].atomic_num];
				result += to_string(flag[e[j].atomic_num]);
			}
		}
		//cout << result << endl;
		fout << result << endl;
		result.clear();
		
		for (j = 0; j < 120; j++)
		{
			flag[j] = 0;
		}
		yueshu = 1;
		cout << "has generate :" << name << endl;
	}



	fout.close();
	fin.close();
	return 1;
}
string  get_formula_name(cell& cell_a)
{
	int i = 0;
	int flag[120] = { 0 };
	for (i = 0; i < cell_a.num; i++)
	{
		flag[cell_a.type[i]]++;
	}
	string name;
	for (i = 0; i < 120; i++)
	{
		if (flag[i] != 0)
		{
			name += atom_name[i];
			name += to_string(flag[i]);
		}

	}
	//cout << name << endl;
	//cin.get();
	return name;
}

int comp_nega(const void *p1, const void *p2)
{
	//注意这个是从大到小
	if (((element *)p1)->electron_negativity < ((element *)p2)->electron_negativity)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int stu_check(cell &cell_a, int* list, int num)
{
	//输入是点的序号，输出是这个结构合不合适，需要穿过中间格子
	int flag = 0;
	for (int i = 0; i < num; i++)
	{
		if (list[i] == -1)
			break;
		else
		{
			if (list[i] / cell_a.num == center_judge)
			{
				flag = 1;
				break;
			}
		}
	}
	return flag;
}

void judge_uniqe(cell &cell_a, int*save_list, int edge_num, int**expand_graph,vector<int>&save,int use_flag)
{
	//@paramters：structure cell, connect group ,edge num and the connection matrix
	//@output: 0D atom num and element sum
	if (use_flag == 0) {
		vector<int >flag(cell_a.num, 0);
		for (int i = 0; i < edge_num; i++)
		{
			if (save_list[i] == -1)
				break;
			flag[save_list[i] % cell_a.num] = 1;
		}
		for (int i = 0; i < cell_a.num; i++)
		{
			if (flag[i] == 1)
			{
				save[0] += flag[i];
				save[1] += cell_a.type[i];
			}

		}
		//cout << save[0] << "," << save[1] << endl;
		return;
	}
	else if (use_flag == 1) {
		//默认是普通版本就是计算原子数和元素和数
		for (int i = 0; i < edge_num; i++) {
			if (save_list[i] == -1)
				break;
			save[0] += 1;
			save[1] += cell_a.type[save_list[i] % cell_a.num];
		}
		return;
	}
	else
	{
		cout << "unkonwn flag!plesae check!" << endl;
		cin.get();
	}
}

double sunbets_get_distance(int orig, int edge_num, cell& cell_a, int *list,int **expand_graph)
{
	//这个是为了保证真空层的
	double res = 0;
	for (int i = 0; i < edge_num; i++) {
		if (list[i] == -1)
			return res;
		if (expand_graph[orig][list[i]] == 1)
		{
			//cout << cell_a.type[orig%cell_a.num] << "," << cell_a.type[list[i] % cell_a.num] << endl;
			double temp = dis(cell_a.real_position[list[i] / cell_a.num][list[i] % cell_a.num], cell_a.real_position[orig / cell_a.num][orig % cell_a.num]);
			if (temp > res)
			{
				res = temp;
			}
		}
		
	}
	return res;
}
void shwo_letice(double**letice)
{
	//展示晶格矩阵
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++) {
			cout << letice[i][j] << ",";
		}
		cout << endl;
	}
}
//辅助统计不同倍数下的连接的条数
void cell::inrich_connection_num(cell&cell_a, double real, double org_rule, double start, double end, double step) {
	if (IF_COUNT_RIDUS == true) {
		int count_number = (end_times - start_times) / step_times;
		//下面开始正式统计
		double time = 0;
		for (int i = 0; i < count_number+1; i++) {
			time = start + i * step;
			if (real < org_rule*time)
				cell_a.count[i] += 1;
		}
		return;
		
	}
}
//输出统计的连键信息相关信息
void cell::output_connection_result(string outname) {
	if (IF_COUNT_RIDUS == true) {
		ofstream fout;
		fout.open(outname+"_con", ios::out);
		for (int i = 0; i < this->count.size(); i++)
			fout << this->count[i] << "\t";
		fout.close();
		return;
	}
	

}
bool cell::getBondsDistribution(string path,double**dist,element* e, vector<vector<double>>&max_ionic_riuds,vector<string>&spec) {
	//基于cohp进行相关统计
	//输出两个信息，两个文件
	//1个是nochp的输入文件中，最后几行
	//2是cohp最后统计半径信息时候的输出（待定）
	ofstream fout;
	fout.open(path + "_cohpin", ios::out);
	//注意这里的写法，一定是阴离子在前，阳离子在后
	map<string, int> bond_save;
	vector<int> negative;
	vector<int>postive;//将结构包含的元素分成正负两种情况
	for (int i = 0; i < this->type_num; i++) {
		int judge = -1;
		judge = classify_metal_maingroup(this->type_save[i]);
		if (judge == 1) {
			//对应金属
			postive.push_back(this->type_save[i]);
		}
		else if (judge == 2) {
			//对应主族相关情况
			negative.push_back(this->type_save[i]);
		}
		else {
			cout << "for aelement:" << atom_name[this->type_save[i]] << ", no judge result!" << endl;
			cin.get();
		}
	}
	bool normal = true;
	//检查得到的正负情况
	if (negative.size() == 0 || postive.size() == 0) {
		cout << "no element for positive or negative!" << endl;	
		cout << "generate all the element situations!" <<endl;
		//cin.get();
		normal = false;
	}
	//下面开始正式写入了
	//注意金属在前，非金属在后
	double time_dis = 1.2;
	if (normal == true) {
		for (auto pos : postive) {
			for (auto neg : negative) {
				fout << "cohpGenerator from " << dist[pos][neg] * 0.2 << " to " << dist[pos][neg] * time_dis << " type " << atom_name[pos] << " type " << atom_name[neg] << endl;
			}
		}
	}
	else {
		//如果没有特定的金属和非金属，就全打印出来吧
		for (int i = 0; i < this->type_num; i++) {
			for (int j = i + 1; j < this->type_num; j++) {
				fout << "cohpGenerator from " << dist[this->type_save[i]][this->type_save[j]] * 0.2 << " to " << dist[this->type_save[i]][this->type_save[j]] * time_dis << " type " << atom_name[this->type_save[i]] << " type " << atom_name[this->type_save[j]] << endl;
			}
		}
	}
	
	fout.close();

	fout.open(path + "_cohp_band", ios::out);
	bool five_flag = false;
	if (find(spec.begin(), spec.end(), path) != spec.end()) {
		//找到的情况下，就是特殊的结构
		five_flag = true;
	}
	//方法改进，这里我们只输出阳离子中心的半径
	//然后对于Ge进行归一化，如果中心不止一个元素，那么那么就输出多个元素
	if (cell::LobisterFlag == "Aver") {
		//2:第二步，打印键长的分配比例
		int bond_num = 0;//打印的半径的种类数目
		if (normal == true) {
			//对应正常情况的数值
			//要求第一个一定是正的，第二个一定是负的
			//结果还是归一化的数值，但是单独归一化
			for (int i = 0; i < this->num; i++) {
				if (this->if_positive[i] == 1) {
					for (int j = 0; j < yanshen*this->num; j++) {
						if (this->if_positive[j%this->num] == -1) {
							if (this->big_matrix[(center_judge - 1)*this->num + i][j] == 1) {
								//如果找到了连接的情况，那么这里就进行插入
								string key = string(atom_name[this->type[i]]) + "-" + string(atom_name[this->type[j%this->num]]);
								if (bond_save.find(key) == bond_save.end()) {
									bond_save[key] = 1;
									bond_num++;
								}
								else {
									int temp = bond_save[key];
									bond_save[key] = temp + 1;
								}
							}
						}
					}
				}

			}
		}
		else {
			//在这种情况下，我们相当于任意取出两种元素的组合
			for (int i = 0; i < this->num; i++) {
				for (int j = 0; j < yanshen*this->num; j++) {
					int samll = min(this->type[i], this->type[j%this->num]);
					int big = max(this->type[i], this->type[j%this->num]);
					if (samll != big && this->big_matrix[(center_judge - 1)*this->num + i][j] == 1) {
						//如果找到了连接的情况，那么这里就进行插入
						string key = string(atom_name[samll]) + "-" + string(atom_name[big]);
						if (bond_save.find(key) == bond_save.end()) {
							bond_save[key] = 1;
							bond_num++;
						}
						else {
							int temp = bond_save[key];
							bond_save[key] = temp + 1;
						}
					}
				}
			}
		}
		//最后我们根据统计得到的结果，输出键长阴阳离子的均分平均值
		
		int total_sum = 0;
		double res = 0;
		map<string, int>::iterator it = bond_save.begin();
		while (it != bond_save.end()) {
			total_sum += it->second;
			it++;
		}
		//在这里我们需要对应的半径相关信息
		it = bond_save.begin();
		fout << bond_num << endl;
		while (it != bond_save.end()) {
			vector<int> two = elementsIndex(it->first);
			fout << it->first << ":";
			if (normal == true&&five_flag==false) {
				fout << max_ionic_riuds[two[0]][0] / max_ionic_riuds[two[1]][1] << endl;
			}
			else {
				//共价情况，或者是第五类的，统一用共价半径
				if (e[two[0]].electron_negativity > e[two[1]].electron_negativity)
					fout << e[two[1]].cov_radius / e[two[0]].cov_radius << endl;
				else
					fout << e[two[0]].cov_radius / e[two[1]].cov_radius << endl;
			}



			//这里我们不再使用均分平均值
			//double type1 = max_ionic_riuds[two[0]][0] / max_ionic_riuds[two[1]][1];
			////让电负性大的作为分子
			//double type2 = 0;
			//if (e[two[0]].electron_negativity > e[two[1]].electron_negativity)
			//	type2 = e[two[0]].cov_radius / e[two[1]].cov_radius;
			//else
			//	type2 = e[two[1]].cov_radius / e[two[0]].cov_radius;
			//if (normal == true) {
			//	//对应正常的阴阳离子的情况
			//	//cout << it->first << "," << it->second << endl;
			//	//cout << double(it->second) / double(total_sum) << endl;
			//	res += double(it->second) / double(total_sum) *type1;
			//}
			//else {
			//	//对应全是主族元素的情况
			//	res += double(it->second) / double(total_sum)*type2;
			//}

			it++;
		}
		//cout << res << endl;
		//fout << res << endl;
		
	}
	else if (cell::LobisterFlag == "Ridus") {
		int out_num = 0;
		double normailize = 0.87;
		double covNormalize = 1.22;
		if (normal == true) {
			fout << postive.size() << endl;//首先打印有多少个半径
			for (auto index : postive)
				fout << max_ionic_riuds[index][0] / normailize << endl;
		}
		else {
			//这个时候我们需要手动修改每个
			//对于这里我们全是通过共价半径进行连接的
			//通过我们手动看完结构，我们发现是一些特定的元素
			vector<int>okay_element = {5,14,15,33};
			vector<int>target;			
			for (int i = 0; i < this->type_num; i++) {
				if (find(okay_element.begin(), okay_element.end(), this->type_save[i]) != okay_element.end()) {
					target.push_back(this->type_save[i]);
				}
			}
			fout << target.size() << endl;
			for (int i = 0; i < target.size(); i++) {
				fout << e[target[i]].cov_radius / 100.0 / covNormalize << endl;
			}
				
		}
	}
	else {
		cout << "unknown the lobsiter flag!" << endl;
		cin.get();
		return false;
	}
	fout.close();
	return true;

}
void cell::generateAllCohpIn(string file_name, string path,element*e, vector<vector<double>>&max_ionic_riuds){
	//输入是calculation路径，以及对应的计算文件夹名字
	double**temp_dis = new double*[120];
	for (int i = 0; i < 120; i++) {
		temp_dis[i] = new double[120];
	}
	string res_path = "/share2/wangz/1007_formation_energy/1d_new_0422/cohp_config/cohp_in/";
	vector<string>all_name = get_file_name(file_name);
	//增加一个功能，有一些结构需要进行特殊化标记
	string special_name = "/share2/wangz/1007_formation_energy/1d_new_0422/0602_special";
	vector<string> spec = get_file_name(special_name);
	for (int i = 0; i < spec.size(); i++)
		spec[i] = res_path + spec[i];

	//正式开始每一个产生相应的输入文件
	for (auto singel : all_name) {
		cout << "start to generate the file:" << singel << endl;
		cell cell_a(const_cast<char*>((path + "/" + singel).c_str()),e,max_ionic_riuds,temp_dis,2);
		cell_a.getBondsDistribution(res_path  + singel, temp_dis,e,max_ionic_riuds,spec);
	}
	for (int i = 0; i < 120; i++) {
		delete[]temp_dis[i];
	}
	delete[]temp_dis;
	cout << "all total work done!" << endl;
	return;
}
vector<string> get_file_name(string file_name)
{
	ifstream fin;
	fin.open(file_name, ios::in);
	vector<string> name;
	if (!fin.is_open())
	{
		cout << "i can not find the file:" << file_name << endl;
		cin.get();
	}
	char buff[100];
	while (fin.good() && fin.peek() != EOF)
	{
		fin.getline(buff, 100);
		name.push_back(string(buff));
		memset(buff, 0, 100);
	}
	fin.close();
	//system(("rm " + file_name).c_str());
	return name;

}
vector<int>cell::elementsIndex(string key) {
	//根据字符串返回这两个元素类型
	int index = key.find("-");
	string first = key.substr(0, index);
	string sec = key.substr(index + 1, key.size());
	vector<int>res(2, -1);
	bool first_flag = false, sec_falg = false;
	for (int i = 0; i < 120; i++) {
		if (first_flag&&sec_falg)
			break;
		if (first == atom_name[i]) {
			first_flag = true;
			res[0] = i;
		}			
		if (sec == atom_name[i]) {
			sec_falg = true;
			res[1] = i;
		}
			
	}
	return res;
}
void test(vector<vector<double>>&max_ioncic, element*e) {
	//执行2D到1D的寻找代码测试
	//执行测试的函数
	








	ofstream fout;
	//fout.open("max_ionic", ios::out);
	//for (i = 0; i < 120; i++)
	//{
	//	fout << max_ionic_riuds[i][0] << "\t" << max_ionic_riuds[i][1] << endl;
	//}
	//fout.close();

	////然后输出一个用于展示的半径表，
	/*fout.open("max_show", ios::out);
	fout << "element  " << "number  " << "coval_ridus  " << "positive_ridus  " << "negative_r  " <<"metal_ridus" <<endl;
	for (i = 1; i < 104; i++)
	{
		cout << i << endl;
		fout << a[i] << "  " << i << "  " << e[i].cov_radius << "  " << max_ionic_riuds[i][0] << "  " << max_ionic_riuds[i][1] << "  " << e[i].metal_radius[0] <<  endl;
	}
	fout.close();*/
	//输出主族元素之间的离子半径和共价半径加和的比较
	/*fout.open("check", ios::out);
	for (i = 0; i < main_groupnum; i++)
	{
		for (j = i+1; j < main_groupnum; j++)
		{
			if (i == j)
				continue;
			if (e[main_group_element[i]].electron_negativity > e[main_group_element[j]].electron_negativity)
			{
				double puls = max_ionic_riuds[main_group_element[i]][1] + max_ionic_riuds[main_group_element[j]][0];
				double cov = (e[main_group_element[i]].cov_radius + e[main_group_element[j]].cov_radius) / 100.0;
				fout << atom_name[main_group_element[i]] << "  " << atom_name[main_group_element[j]] << " ";
				fout << puls << "  " << cov << endl;
			}
			else
			{
				double puls = max_ionic_riuds[main_group_element[i]][0] + max_ionic_riuds[main_group_element[j]][1];
				double cov = (e[main_group_element[i]].cov_radius + e[main_group_element[j]].cov_radius) / 100.0;
				fout << atom_name[main_group_element[i]] << "  " << atom_name[main_group_element[j]] << " ";
				fout << puls << "  " << cov << endl;
			}
		}
	}
	fout.close();
	cout << "done!" << endl;
	cin.get();
	return 0;*/




	//一个小功能，输入是一个路径和文件名字，输出是化学表达式
	/*string oned_path = "/share/home/wangz/2d_search/ridus_cut/result/result_ionicandval/0103/my_1d/";
	string oned_name = "/share/home/wangz/2d_search/ridus_cut/result/result_ionicandval/0103/my_1d/name";
	get_tablefor_1dchemistry(oned_path, oned_name, 1569, e);
	cout << "analyse 1d stu complete!" << endl;
	cin.get();*/


	////判断这8千个结构是不是存在共价键
	//ofstream fout;
	//fout.open("new_null_bulk", ios::out);
	//ifstream fin;
	//fin.open("null_bulk", ios::in);
	//if (!fin.is_open())
	//{
	//	cout << "i can not find the file!  null_bulk" << endl;
	//	cin.get();
	//}
	//string name_temp;
	//i = 0;
	//while (fin.good() && fin.peek() != EOF)
	//{
	//	fin >> name_temp;
	//	cout << "has gone" << i ++<< "file!" << endl;
	//	cell cell_a(const_cast<char*>((data_path + name_temp+"_1/atom.config").c_str()));
	//	if(if_valence_conncet(cell_a, e) == true)
	//	{
	//		fout << name_temp << endl;
	//	}
	//}
	//fin.close();
	//fout.close();
	//cout << "new null file done!" << endl;
	//cin.get();
	//return 1;
}