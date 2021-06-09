#pragma once

#ifndef CONNCETION

#include <math.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include<map>
using namespace std;
const char atom_name[120][3] = { " ", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg" };

//金属元素的规则：没有负价半径，元素周期表显示为金属，不含分界线上的元素
//0113：更新，更新元素的金属非金属
const int meatal_xuhao[89] = { 3, 4, 11, 12, 13, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 37, 38, 39, 40, 41, 42, 43, 44, 45,46, 47, 48, 49, 50,  51,55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 85,87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111 };
const int main_group_element[16] = { 1, 5,6,7,8,9,14,15,16,17,33,34,35,52,53,84 };//at为金属元素
const int rare_gas[6] = { 2,10,18,36,54,86 };


const int other_elements = {};
//const double ridus_plus_factor = 1.25;
//const double val_radius_factor = 1.25;//根据统计的结果
//const double metal_ridus_factor = 1.25;
const double ridus_plus_factor = 1.15;
const double val_radius_factor = 1.2;//根据统计的结果
const double metal_ridus_factor = 1.1;
const int metal_num = 89;
const int main_groupnum = 16;
const int rare_gasnum = 6;

const bool IF_COUNT_RIDUS = false;//是否开启记录键长信息
const double start_times = 0.8;//开始记录的倍数起始值
const double end_times = 1.3;//记录连接数的倍数的最终值
const double step_times = 0.02;//记录倍数的步长

const int AIM = 1;//标志寻找几维材料，1是1d，2是2d，0是0d
const int cengshu = 3;								 //��������������������ھ�����չʱ�Ĳ���
const int yanshen = cengshu * cengshu * cengshu;
const bool IF_SAME_MZ =false; //是否在连键中采用mz的方法
const int center_judge = (yanshen-1)/2;
const double H_MATAL_RULE = 2;
const double OF_RULE = 2;
const double pi = 3.1415926; //˵�����˶��پ���
const int TO_MANY_ATOM = 150;
//const int oneD_angel_rule = 10;该选项现在废弃
//关于0D结构的判据，满足该原子连接数目较多，且整个集团的原子数较多
const int zeroD_atom_num_rule = 2;
const int zeroD_group_num = 4;
const int oned_b_length = 15;
const int oned_c_length = 15;

const int default_length = 35;//初始矢量的长度
const double TO_LARGE_CELL_length = 35;
const double default_org_loc = 0.02;//产生的结构的初始矢量位置
const double same_atom_rule = 1e-3;//判断是不是重复的原子的规则


class element {
public:
	//element();
	int atomic_num;//atomic number
	char name[3];   // element name
	double vdw_radius_min;
	double vdw_radius_max;
	//van der waals radius,min and max
	double cov_radius;//covalence radius
	int num_metal_radius;  // number of metallic radius
	double * metal_radius; // the radius of metallic radius
	int num_common_val;  // number of common valence
	int * common_val;  // common valence
	int num_unusual_val; // number of unusual valence
	int * unusual_val;  // unusual valence
	double electron_negativity;   //electronic negativity
	double first_ionization_energy;
};

class ionic_radius {
public:
	int atomic_num;    // ionic atomic number
	int coor_num;     // ionic coordination number
	int val_state;    // ionic valence state

	double radius;     // ionic radius
	int structure_type;    // ionic structure type, 0=common, 1=square, 2=pyrometric cone
	int spin_stat;     // ionic spin state, 0=common, 1=high spin, 2=low spin

};
class cell
{
public:
	cell(char *jiegou_name, element* e,vector<vector<double>> &max,double**dist,int flag = 0);//0是默认值，1表示读取带有positive和半径信息的情况，2表示根据元素的主族金属标定
	cell(char*jiegou_name);
	int num = 0;
	double **letice;
	double **p;
	double ***p_real;
	double ***real_position;
	double **ridus;
	int *type;
	int type_num;
	int *type_save;
	int *positive;

	int* if_positive;//是否是正价，1是正价，-1是负价，0表示稀有气体
	int* my_classify;
	void generateGrapth(int num, element*e, vector<vector<double>>&max_ionic_riuds, double**dist);
	bool complexMalloc = false;
	//扩胞之后的大矩阵的描述
	int**big_matrix;

	void output_connection_result(string outname);
	static void inrich_connection_num(cell &cell_a,double real, double org_rule, double start = 0.8, double end = 1.3, double step = 0.02);
	vector<int>count;

	//有关成键信息的统计，和cohp相关
	bool getBondsDistribution(string path,double**dist,element*e, vector<vector<double>>&max_ionic_riuds, vector<string>&spec);//输入是当前的计算路径,以及被填充好的dist阈值判断矩阵
	static void generateAllCohpIn(string file_name, string path, element*e, vector<vector<double>>&max_ionic_riuds);//输入是calculation路径，以及对应的计算文件夹名字
	static string LobisterFlag;//生成bond信息的flag，Aver代表键长信息的均分;Ridus代表仅输出阳离子的中心半径
	//~cell() {
	//	//删除大矩阵，释放内存
	//	if (complexMalloc == true) {
	//		if (this->big_matrix != nullptr) {				
	//			/*for (int i = 0; i < yanshen*num; i++)
	//			{
	//				delete[] big_matrix[i];
	//			}
	//			delete[]this->big_matrix;*/
	//		}
	//		delete[]if_positive;
	//		delete[]my_classify;
	//	}		
	//	delete[]type;
	//	delete[]type_save;
	//}

private:
	vector<int>elementsIndex(string key);//根据字符串返回这两个元素类型
	
};


class save
{

public:
	int sunbets; //����Ƿֳɵ���ͨ��������
	int **save_list;
	save(int num);
	~save(void);
};
struct three_double
{
	double distnace = 0;
	double angle = 0;
	int xuhao = 0;
};

struct Atom_check
{
	int element = 0;
	double loc[3] = { 0 };
	int xuhao = 0;
	bool same_flag = false;
};


class my_atom {
public:	
	int num;
	vector<int> type;
	double**letice;
	vector<vector<double>> real_loc;
	vector<vector<double >>fenshu;
	void save_information(string name) {
		string file[2];
		file[0] = "Lattice vector";
		file[1] = "Position";
		//然后我生成了新的坐标，开始写文件吧
		FILE *out = fopen(name.c_str(), "wt");
		char atom_head[20] = "atoms";
		fprintf(out, "%d\n", this->num);
		fprintf(out, "%s\n", file[0].c_str());
		int mm = 1;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				fprintf(out, "\t%.9lf\t", letice[i][j]);
			}
			fprintf(out, "\n");
		}
		fprintf(out, "%s\n", file[1].c_str());		
		for (int i = 0; i < num; i++) {
			fprintf(out, "  %d\t", type[i]);
			for (int j = 0; j < 3; j++) {
				fprintf(out, "%.9lf\t", fenshu[i][j]);
			}
			for (int m = 0; m < 3; m++)
			{
				fprintf(out, "%d\t", mm);
			}
			fprintf(out, "\n");
		}		
		fclose(out);
	};
	
};


bool if_valence_conncet(cell cell_a, element* e);
int classify_metal_maingroup(int &atomic_number);
bool CheckAllmetal(int*list, cell& cell_a,int edge_num);

double dis(double *p1, double *p2);


double three_jie_chaji(double **a);
double vector_angle(double **a);
int inone_face(double **a);							 //���淵��1�������淵��0
double det(double **a, int num);					 //����������ʽ��ֵ
void getastar(double **a, int num, double **b);		 //��������������ʽ����䵽b����
int reverse_matrix(double **a, int num, double **b); //�����������󣬳ɹ����1 ���������0
void full_expand_generate_graph(cell &cell_a, int **expand_matrix, int num, element *e,vector<vector<double>>& max_ion,double**dist);
void ex_find_connect(int edge_num, cell cell_a, int *visited, int **matrix, save *save_a);
void ex_DFS(int a, int *ex_visited, cell cell_a, int **expand_matrix, int num_edge, int subets);
void generate_testfile(save *save_a, cell cell_a, int edge_num);



int *judge_the_2d(cell cell_a, int *save_list, int edge_num, int *judge);
int *judge_the_1d(cell cell_a, int *save_list, int edge_num, int *judge);
int *judge_the_0d(cell cell_a, int *save_list, int edge_num, int *judge, int** expand_graph);
int *judge_dimentional(cell &cell_a, int *save_list, int edge_num, int *judge, int** expand_graph);
//补充上关于寻找1d结构的代码

int generate_atom_1d(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list,int** expand_graph);
int generate_atom_0d(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list, int** expand_graph);
double sunbets_get_distance(int orig, int edge_num, cell& cell_a, int *list,int **expand_graph);
double get_line_dis(int *list, int edge_num, int orig, cell cell_a, int *copy);

void generate_outfile(cell cell_a, save *ex_pt, int edge_num, int *judge);
int generate_atom(save *ex_pt, int *judge, int edge_num, cell cell_a, string name, int *list);
string int2string(int i);
double face_point_dis(double letice[2][3], double in_face_point[3], double point[3]);
double get_vacumlayer_thick(int *list, int edge_num, int orig, cell cell_a, double **letice, int *copy);
int judge_falg_file(char *path);
void random_choose_two(int *org, int num, int **in);
void buble_plus(double *a, int *b, int num);
void read_element(element *e, string &file_element_r);
void read_radius(ionic_radius *****ir, string& file_name1, string& file_name2);
void read_element(element *e, string& file_element_r, string& file_colvance, string& file_electronic_negativity, string& file_first_ionization_energy);

int Comp(const void *p1, const void *p2);

int comp_nega(const void *p1, const void *p2);

int get_tablefor_1dchemistry(string&path, string& file_name,int num,element* e);

string  get_formula_name(cell& cell_a);
int stu_check(cell &cell_a, int* list, int num);
void judge_uniqe(cell &cell_a,int*save_list, int edge_num, int**expand_graph,vector<int>& save,int flag=1);
void shwo_letice(double**letice);
vector<string> get_file_name(string file_name);//读取文件返回string列表
void test();//一些相关的测试
#define CONNCETION
#endif // !CONNCETION


