#include "CONNECTION.h"
#include "GraphIsomorphism.h"
#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#pragma warning(disable : 4996)
using namespace std;

static char wenjian[100] = "15173";
static char result_path[40] = "427402";
static char style[100] = "rule_noorg_for_pc.txt";
static char wenjian_real[100];
bool Cell_Matrix::IF_ELEMENT = false;
bool Cell_Matrix::IF_ALL_ELEMNET = false;
int Cell_Matrix::Max_num = 10;
int Cell_Matrix::Max_compare_num = 2e7;
string cell::LobisterFlag = "Aver";
const char a[120][3] = { " ", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };


int main(int argc, const char *argv[])
{
	//先读取各种信息
	int i = 0, j = 0, k = 0, l = 0, m = 0;
	//string supporting_data_path="D:\\my_code\\new_connection1231\\data\\";
	//string supporting_data_path ;
	string supporting_data_path = "/share/home/wangz/2d_search/ridus_cut/code/from_2d_1d/si_data/";
	string file_name1 = supporting_data_path + "input_ionic";
	string file_name2 = supporting_data_path + "input_ionic_plus";
	string file_element_r = supporting_data_path + "ridus";
	string file_colvance = supporting_data_path + "colvance";
	string file_file_nagetivity = supporting_data_path + "negativity.txt";
	string file_first_ionization_energy = supporting_data_path + "first_ionazation_energy.txt";
	string max_ionic = supporting_data_path + "max_ionic";
	string data_path = "/share/home/wangz/icsd_2019/icsd_2019/";
	//string data_path = "C:\\Users\\王志\\Desktop\\graph_series\\new_connection1231\\test\\102628.config";
	string zerod_result_path = "/share/home/wangz/2d_search/ridus_cut/result/result_ionicandval/0103/my_0d/";
	double **dist = new double*[120];
	for (i = 0; i < 120; i++)
	{
		dist[i] = new double[120];
		for (j = 0; j < 120; j++)
		{
			dist[i][j] = -100;
		}
	}
	//建立元素的	
	element *e;
	e = new element[120];
	//开始依次读取相关信息
	//read_radius(ir, file_name1, file_name2);
	read_element(e, file_element_r, file_colvance, file_file_nagetivity, file_first_ionization_energy);  

	//根据元素找出对应元素正价和负价的最大值半径储存下来
	ifstream fin;
	fin.open(max_ionic, ios::in);
	vector<vector<double>> max_ionic_riuds(120);
	for (i = 0; i < 120; i++)
	{
		max_ionic_riuds[i].resize(2);
		max_ionic_riuds[i][0] = -1;
		max_ionic_riuds[i][1] = -1;
		fin >> max_ionic_riuds[i][0];
		fin >> max_ionic_riuds[i][1];

	}
	fin.close();
	//测试有关从2D到1D寻找的code
	for (i = 0; i < argc; i++)
	{

		if (i == 1)
		{
			for (j = 0; j < strlen(argv[i]); j++)
			{
				wenjian[j] = argv[i][j];
				//cout<<argv[i][j]<<endl;
				//cout<<wenjian[j]<<endl;
			}
			wenjian[strlen(argv[i])] = '\0';
		}
		if (i == 2)
		{
			for (j = 0; j < strlen(argv[i]); j++)
			{
				result_path[j] = argv[i][j];
			}
			result_path[strlen(argv[i])] = '\0';
		}

		/*if (i == 4)
		{
			for (j = 0; j < strlen(argv[i]); j++)
			{
				wenjian_real[j] = argv[i][j];
			}
			wenjian_real[strlen(argv[i])] = '\0';
		}*/
	}
	/*Cell_Matrix::test("/share/home/wangz/2d_search/ridus_cut/code/from_2d_1d/config/100610-0_2d.config",
		"/share/home/wangz/2d_search/ridus_cut/code/from_2d_1d/config/411177-0_1d.config", max_ionic_riuds, e);*/
	/*return Cell_Matrix::test(wenjian, result_path, max_ionic_riuds, e);
	double temp = 0.0;*/
	//有关批量产生cohp关键输入的
	cell::generateAllCohpIn("/share2/wangz/1007_formation_energy/1d_new_0422/cohp_config/0522_cohp_name","/share2/wangz/1007_formation_energy/1d_new_0422/cohp_config/config/",
		e,max_ionic_riuds);

	char path_flag[200];
	//cout << wenjian << endl;
	//cin.get();
	string input = data_path + string(wenjian)+".cif.config";
	
	cell cell_a(const_cast<char*>(input.c_str()), e, max_ionic_riuds, dist,2);
	if (cell_a.num > TO_MANY_ATOM)
		return 0;	
	cell_a.output_connection_result(result_path);
	//return 0;
	
	//建立有关成键可能性的记录
	//第一个数是
	double ***possiblty = new double **[cell_a.type_num];
	for (i = 0; i < cell_a.type_num; i++)
	{
		possiblty[i] = new double *[cell_a.type_num];
		for (j = 0; j < cell_a.type_num; j++)
		{
			possiblty[i][j] = new double[3];
			possiblty[i][j][0] = -999;
			possiblty[i][j][1] = 100;
		}
	}

	int num = cell_a.num * yanshen;
	save save_a(num);
	save *pt = &save_a; //ָ��ָ��save_a	
	int *ex_visited; //�����Ƿ�������������
	ex_visited = new int[num];
	
	for (i = 0; i < num; i++)
	{
		ex_visited[i] = 0;
	}	
	
	
	//补充完最后一项,最后记录
	int poss_save[2];
	double min_temp = 200;
	for (i = 0; i < cell_a.type_num; i++)
	{
		for (j = 0; j < cell_a.type_num; j++)
		{
			if (i != j)
			{
				possiblty[i][j][2] = abs(possiblty[i][j][0] - possiblty[i][j][1]);
				if (possiblty[i][j][2] < min_temp)
				{
					min_temp = possiblty[i][j][2];
					poss_save[0] = i;
					poss_save[1] = j;
				}
			}
		}
	}

	ex_find_connect(num, cell_a, ex_visited, cell_a.big_matrix, pt);
	//cout << "the subnets is :" << pt->sunbets << endl;
	
	int success_ful = 0;
	if (save_a.sunbets == 1)
	{
		//cout << "this atom do not have the graph_connection!";
		//generate_testfile(pt, cell_a, num);
		delete[] ex_visited;
		for (i = 0; i < num; i++)
		{
			delete[] cell_a.big_matrix[i];
		}
		delete[] cell_a.big_matrix;
		//cout << "all work done" << endl;
		//cin.get();
		return success_ful;
	}
	//�õ�����ͨ����Ϊ1˵���϶��ǲ������������ˣ�ֱ��������
	//�������ÿ�ֵõ�����ͨ���������ж�

	else
	{
		//cout << "this atom has :" << pt->sunbets << " graph_connection!,please check!" << endl;
		int **judge = new int *[pt->sunbets];
		for (int i = 0; i < pt->sunbets; i++)
		{
			judge[i] = new int[2];
		}


		if (AIM == 2)
		{
			//现在建立一个标志位用来标志二维的连通分量是不是两头的，0表示起始，1表示中间，2表示末尾，-1表示不是2d
		//我们现在做这样的事情，建立二维连通分量只针对中间的来建立
		//同时要求只能切出一种2D结构，保证化学计量比不变
		//同时，应该说，经过中间的联通分量，只能是2D，而且只能是一种唯一的2D结构

			//测试下如果通过，穿过中间格子的方法来改进行不行
			int *middle_flag = new int[pt->sunbets];
			for (i = 0; i < pt->sunbets; i++)
			{
				middle_flag[i] = 0;
			}
			//���������ͨ�����������жϽ��

			for (j = 0; j < pt->sunbets; j++)
			{

				//cout << "now it's judge the :" << j << endl;
				judge[j][0] = 0;
				judge[j] = judge_dimentional(cell_a, pt->save_list[j], num, judge[j],cell_a.big_matrix);
				//cout << judge[j][0] << endl;
				if (judge[j][0] == 2) {
					middle_flag[j] = 1;
					success_ful = 1;
				}
				else if (judge[j][0] == -1 || judge[j][0] == 4) {
					//虽然在中心但是不是2D,这种就GG
					success_ful = 0;
					return success_ful;
				}
					
				
			}		


			
			if (success_ful == 0)
				return 0;

			//这里我们重写一下关于两边的判断
			int twod_num = 0; //先看一下有多少个2d的连通分量
			j = 0;

			for (i = 0; i < pt->sunbets; i++)
			{
				if (judge[i][0] == 2)
				{
					twod_num++;
				}
			}
			//然后根据元素和原子数筛选结构
			vector<vector<int>> twod_unqie_check(twod_num);
			for (int i = 0; i < twod_num; i++) {
				twod_unqie_check[i].resize(3);
			}
			for (int i = 0; i < pt->sunbets; i++) {
				if (judge[i][0] == 2) {
					judge_uniqe(cell_a, pt->save_list[i], num, cell_a.big_matrix, twod_unqie_check[j++]);
					twod_unqie_check[j - 1][2] = i;
				}
			}
			if (twod_num > 1) {
				for (int i = 1; i <twod_num; i++) {
					//如果相同，后面的就设置-1，意思是重复的
					if (twod_unqie_check[i][0] == twod_unqie_check[0][0] && twod_unqie_check[i][1] == twod_unqie_check[0][1]) {
						judge[twod_unqie_check[i][2]][0] = -1;
					}
					//如果不相同，这个体材料不能切下来，直接不要了
					if (twod_unqie_check[i][0] != twod_unqie_check[0][0] || twod_unqie_check[i][1] != twod_unqie_check[0][1]) {
						return 0;
					}
				}
			}





			

			////然后需要输出层之间的最短距离。
			double temp_layer_dis = 0;
			double *layer_dis = new double[pt->sunbets];
			string **show_ele = new string *[pt->sunbets];
			double *duiying_style = new double[pt->sunbets];

			for (i = 0; i < pt->sunbets; i++)
			{
				show_ele[i] = new string[2];
			}

			for (int i = 0; i < pt->sunbets; i++)
			{
				layer_dis[i] = 100.0;
			}


			//这里输出每个2d结构距离周围其他原子rule和最长距离
			for (j = 0; j < pt->sunbets; j++)
			{
				if (judge[j][0] == 2 && middle_flag[j] == 1)
				{
					for (i = 0; i < num && pt->save_list[j][i] != -1; i++)
					{
						//找到了每个2d连通分量的序号save_list[j][i]
						//对应找到的是其他的联通分量
						for (int n = 0; n < pt->sunbets; n++)
						{
							if (n != j)
							{
								for (int m = 0; m < num && pt->save_list[n][m] != -1; m++)
								{
									if (cell_a.big_matrix[pt->save_list[n][m]][pt->save_list[j][i]] != 1)
									{
										temp_layer_dis = dis(cell_a.real_position[(yanshen-1)/2][pt->save_list[j][i] % cell_a.num], cell_a.real_position[(yanshen - 1) / 2][pt->save_list[n][m] % cell_a.num]);
										if (temp_layer_dis < layer_dis[j] && temp_layer_dis > 1e-3)
										{
											//cout << a[cell_a.type[pt->save_list[j][i] % cell_a.num]] << "," << a[cell_a.type[pt->save_list[n][m] % cell_a.num]] << ",";
											//cout << pt->save_list[j][i] % cell_a.num << "," << pt->save_list[n][m] % cell_a.num << endl;
											layer_dis[j] = temp_layer_dis;
											show_ele[j][0] = a[cell_a.type[pt->save_list[j][i] % cell_a.num]];
											show_ele[j][1] = a[cell_a.type[pt->save_list[n][m] % cell_a.num]];
											duiying_style[j] = dist[cell_a.type[pt->save_list[j][i] % cell_a.num]][cell_a.type[pt->save_list[n][m] % cell_a.num]];
											
											//cout << "now the lauer:" << j << "is:" << temp_layer_dis;
										}
									}
								}
							}
						}
						
												
					}
					
				}

				//只需要找到一层就行
			}
			////到这里我们找到了最近距离
			ofstream fout;
			char dis_name[100] = ".txt";
			char dis_name_real[300];
			strcpy(dis_name_real, result_path);
			fout.open(strcat(dis_name_real, dis_name), ios::out);
			/*if (!fout.is_open())
			{
				cout << "filed to open the file!" << endl;
				cout << "now the file path is :" << dis_name_real << endl;
				cin.get();
			}*/

			//然后开始根据我们的判断结果进行生成新的atom.config文件了
			//如果根据连通性获得了多个，我们不是记录了最多的，同时分别生成对应的2d文件

			int *generate_flag = new int[pt->sunbets]; //标记是不是需要写进文件0的话不写
			for (i = 0; i < pt->sunbets; i++)
			{
				generate_flag[i] = 1;
			}
			success_ful = 0;
			//fout << wenjian_real << "\n";
			char two_filename[100];
			int towd_num = 0;
			char temp_num[20];
			for (j = 0; j < pt->sunbets; j++)
			{
				if (judge[j][0] == 2 && generate_flag[j] == 1 && middle_flag[j] == 1)
				{
					for (int i = 0; i < pt->sunbets; i++)
					{
						if (judge[j][1] == judge[i][1])
							generate_flag[i] = 0;
					}
					//two_filename = wenjian + '-' + int2string(towd_num) + "_2d.config";
					sprintf(temp_num, "%d", towd_num);
					strcpy(two_filename, result_path);
					//strcat(two_filename, wenjian);
					strcat(two_filename, "-");
					strcat(two_filename, temp_num);
					strcat(two_filename, "_2d.config");

					if (generate_atom(pt, judge[j], num, cell_a, two_filename, pt->save_list[j]) == 1)
					{
						//cout << two_filename << endl;
						success_ful = 1;
						fout << show_ele[j][0] << "\t" << show_ele[j][1] << "\t" << layer_dis[j] << "\t" << duiying_style[j];
						fout << "\t" << double(layer_dis[j] / duiying_style[j]) << endl;
						/*fout << rule_max << "\t" << no_connect << "\t" << abs(rule_max - no_connect) << endl;
						fout << possiblty[poss_save[0]][poss_save[1]][0] << "\t" << possiblty[poss_save[0]][poss_save[1]][1] << "\t" << possiblty[poss_save[0]][poss_save[1]][2] << endl;
						*/
						//break;
						towd_num++;

					}
					
				}
			}

			//generate_outfile(cell_a, pt, num, judge);
			//fout << endl;
			fout.close();
			delete[] generate_flag;
			//delete[] layer_dis;
			//delete[] duiying_style;
			delete[] middle_flag;
			//cin.get();
		}
		else if (AIM == 1)
		{
			//cout << "show the connections xuhao!" << endl;
			/*for (i = 0; i < pt->sunbets; i++)
			{
				int num_point = 0;
				for (j = 0; j < num; j++)
				{
					if (pt->save_list[i][j] != -1)
					{
						num_point++;
						cout << pt->save_list[i][j]<<",";
					}
				}
				cout << num_point << endl;
				cout << endl;
			}
			cin.get();*/
			for (j = 0; j < pt->sunbets; j++)
			{

				//cout << "now it's judge the :" << j << endl;
				judge[j][0] = 0;
				judge[j] = judge_dimentional(cell_a, pt->save_list[j], num, judge[j],cell_a.big_matrix);				
				//cout << "the judge result is :" << judge[j][0] << judge[j][1] << endl;
			}
			//cin.get();
			//判断是不是这个体材料是不是ok
			int ond_num = 0;
			for (int i = 0; i < pt->sunbets; i++) {
				
				if (judge[i][0] == 1)
				{
					if (CheckAllmetal(pt->save_list[i], cell_a, num) == false)
						ond_num++;
					else
						judge[i][0] = 0;
				}					
				if (judge[i][0] == 0 || judge[i][0] == 4 || judge[i][0] == 2 || judge[i][0] == -1)
				{
					success_ful = 0;
					return success_ful;
				}
			}
			j = 0;
			if (ond_num == 0)
				return 0;
			vector<vector<int>> ond_unqie_check(ond_num);
			for (int i = 0; i < ond_num; i++) {
				ond_unqie_check[i].resize(3);
			}
			for (int i = 0; i < pt->sunbets; i++) {
				if (judge[i][0] == 1) {
					judge_uniqe(cell_a, pt->save_list[i], num, cell_a.big_matrix, ond_unqie_check[j++]);
					ond_unqie_check[j - 1][2] = i;
				}
			}
			if (ond_num > 1) {
				for (int i = 1; i < ond_num; i++) {
					//如果相同，后面的就设置-1，意思是重复的
					if (ond_unqie_check[i][0] == ond_unqie_check[0][0] && ond_unqie_check[i][1] == ond_unqie_check[0][1]) {
						judge[ond_unqie_check[i][2]][0] = -1;
					}
					//如果不相同，这个体材料不能切下来，直接不要了
					if (ond_unqie_check[i][0] != ond_unqie_check[0][0] || ond_unqie_check[i][1] != ond_unqie_check[0][1]) {
						return 0;
					}
				}
			}
			//然后需要输出层之间的最短距离。
			double temp_layer_dis = 0;
			double *layer_dis = new double[pt->sunbets];
			string **show_ele = new string *[pt->sunbets];
			double *duiying_style = new double[pt->sunbets];

			for (i = 0; i < pt->sunbets; i++)
			{
				show_ele[i] = new string[2];
			}

			for (int i = 0; i < pt->sunbets; i++)
			{
				layer_dis[i] = 100.0;
			}

			for (j = 0; j < pt->sunbets; j++)
			{
				if (judge[j][0] == 1)
				{
					for (i = 0; i < num && pt->save_list[j][i] != -1; i++)
					{
						//找到了每个2d连通分量的序号save_list[j][i]
						for (int m = 0; m < pt->sunbets; m++)
						{
							if (m != j && judge[m][0] == 1)
							{
								for (int k = 0; k < num && pt->save_list[m][k] != -1; k++)
								{
									temp_layer_dis = dis(cell_a.real_position[pt->save_list[j][i] / cell_a.num][pt->save_list[j][i] % cell_a.num], cell_a.real_position[pt->save_list[m][k] / cell_a.num][pt->save_list[m][k] % cell_a.num]);
									if (temp_layer_dis < layer_dis[j] && temp_layer_dis > 1e-3)
									{
										layer_dis[j] = temp_layer_dis;
										show_ele[j][0] = a[cell_a.type[pt->save_list[j][i] % cell_a.num]];
										show_ele[j][1] = a[cell_a.type[pt->save_list[m][k] % cell_a.num]];
										duiying_style[j] = dist[cell_a.type[pt->save_list[j][i] % cell_a.num]][cell_a.type[pt->save_list[m][k] % cell_a.num]];
										//cout << "now the lauer:" << j << "is:" << temp_layer_dis;
									}
								}
							}
						}
					}
				}
			}
			//到这里我们找到了最近距离
			ofstream fout;
			char dis_name[100] = ".txt";
			char dis_name_real[200];
			strcpy(dis_name_real, result_path);
			fout.open(strcat(dis_name_real, dis_name), ios::out | ios::app);
			if (!fout.is_open())
			{
				cout << "filed to open the file!" << endl;
				cout << "now the file path is :" << dis_name << endl;
				cin.get();
			}

			//然后开始根据我们的判断结果进行生成新的atom.config文件了
			//如果根据连通性获得了多个，我们不是记录了最多的，同时分别生成对应的2d文件
			int *generate_flag = new int[pt->sunbets]; //标记是不是需要写进文件0的话不写
			for (i = 0; i < pt->sunbets; i++)
			{
				generate_flag[i] = 1;
			}
			success_ful = 0;
			//fout << wenjian_real << "\n";
			char two_filename[100];
			int towd_num = 0;
			char temp_num[20];
			for (j = 0; j < pt->sunbets; j++)
			{
				//这里我们需要注意，产生的1d结构就只能在中间部分，别跑到外面产生				
				if (judge[j][0] == 1 && generate_flag[j] == 1)
				{
					for (int i = 0; i < pt->sunbets; i++)
					{
						if (judge[j][1] == judge[i][1])
							generate_flag[i] = 0;
					}
					//two_filename = wenjian + '-' + int2string(towd_num) + "_2d.config";
					sprintf(temp_num, "%d", towd_num);
					strcpy(two_filename, result_path);
					//strcat(two_filename, wenjian);
					strcat(two_filename, "-");
					strcat(two_filename, temp_num);
					strcat(two_filename, "_1d.config");
					/*cout << "1d succeed!" << endl;
					cin.get();*/
					if (generate_atom_1d(pt, judge[j], num, cell_a, two_filename, pt->save_list[j], cell_a.big_matrix) != 0)
					{

						//cout << "look i got the 1d situation!" << endl;
						//cout << "the file is: " << wenjian << endl;
						//cin.get();						
						success_ful = 1;

						fout << show_ele[j][0] << "\t" << show_ele[j][1] << "\t" << layer_dis[j] << "\t" << duiying_style[j];
						fout << "\t" << double(layer_dis[j] / duiying_style[j]) << endl;
						//break;
						towd_num++;
					}
					
				}
			}

			//generate_outfile(cell_a, pt, num, judge);
			//fout << endl;
			fout.close();

			for (int i = 0; i < pt->sunbets; i++)
			{				
				delete[] show_ele[i];
			}
			delete[] show_ele;			
			//delete[]subnets_num;
			delete[] generate_flag;
			delete[] layer_dis;
			delete[] duiying_style;
		}
		else if (AIM == 0)
		{
			//这里注意出现寻找1d的情况的时候，不产生结构文件，只是将结构复制到指定目录下
			//同时需要注意为了保证是不带电的0D材料，需要确保每个结构只能切出一个0D结构
			string com;
			int zeod_num = 0;
			vector<int> judge_traget;
			for (j = 0; j < pt->sunbets; j++)
			{

				//cout << "now it's judge the :" << j << endl;
				judge[j][0] = 0;
				judge[j] = judge_dimentional(cell_a, pt->save_list[j], num, judge[j],cell_a.big_matrix);
				if (judge[j][0] == 0 )
				{
					success_ful = 1;
					judge_traget.push_back(j);
					zeod_num++;
				}
				else if(judge[j][0]==-1 ||judge[j][0] == 4){
					//如果出现其他情况，全部认为错误
					//cout << "other situation!" << endl;
					//cin.get();
					success_ful = 0;
					return 0;
				}
				else if (judge[j][0] == -2) {
					//表示没有在中心，不进行判断
				}
				else {
					cout << "unkonwn the flag!plesase check!" << endl;
					cin.get();
				}
				//cout << "the judge result is :" << judge[j][0] << judge[j][1] << endl;
			}
			//cin.get();
			if (success_ful == 0)
				return 0;
			vector<vector<int>> zeod_save(zeod_num);
			for (int i = 0; i < zeod_num; i++)
			{
				zeod_save[i].resize(2);
			}
			
			for (int i = 0; i < zeod_num; i++)
			{
				judge_uniqe(cell_a, pt->save_list[judge_traget[i]], num, cell_a.big_matrix,zeod_save[i],0);
			}

			bool repertion = false;//标记是不是有重复的0D结构
			for (int i = 1; i < zeod_num; i++)
			{
				if (zeod_save[i][0] != zeod_save[0][0] || zeod_save[i][1] != zeod_save[0][1])
				{
					repertion = true;
					return 0;
				}
					
			}
			if (repertion == false)
			{
				//下面开始获得0D的结构，需要将该结构拿出来
				//这样0D的联通群序号是judge_traget【0】
				string zerod_file_name = string(result_path) + "-0_0d.config";	
				int j = judge_traget[0];
				if (generate_atom_0d(pt, judge[j], num, cell_a, zerod_file_name, pt->save_list[j], cell_a.big_matrix) == 0)
				{
					cout << "generate the zerod file faliled!please check!" << zerod_file_name << endl;
					return 1;
				}
				com = "cp " + string(wenjian) + " " + zerod_result_path+"_bulk";
				system(com.c_str());
				return 1;
			}
			

		}
		else
		{
			cout << "wrong AIM number!please check!" << endl;
			cin.get();
		}
		//下面这些是真的需要删掉的
		for (int i = 0; i < pt->sunbets; i++)
		{
			delete[] judge[i];
			//delete[] show_ele[i];
		}
		//delete[] show_ele;
		delete[] judge;
		delete[] ex_visited;
		
		//delete[]subnets_num;
		//cout << "all total work done" << success_ful << endl;
		//cin.get();
		return success_ful;
	}
	//cout << "all total work done" << success_ful << endl;
	//cin.get();
}