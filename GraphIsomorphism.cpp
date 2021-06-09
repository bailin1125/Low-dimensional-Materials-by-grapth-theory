#include "GraphIsomorphism.h"
#include <fstream>
#include <iostream>

using namespace std;
void Cell_Matrix::generate_matrix() {
	//通过指定的维度参数构造对应的图的表示
	this->part_num = this->dimension[0] * this->dimension[1] * this->dimension[2] * this->num;
	//获得对应维度的原始的序号
	
	for (int i = 0; i < yanshen*this->num; i++) {
		//注意需要检查格子的数字来确定位置
		int loc = i / this->num;
		int z = loc / (cengshu*cengshu) + 1;
		int y = (loc % (cengshu*cengshu)) / cengshu + 1;
		int x = (loc % (cengshu*cengshu)) % cengshu + 1;
		//cout << x << "," << y << "," << z << endl;
		if (x <= dimension[0] && y <= dimension[1] && z <= dimension[2]) {
			this->choose_id.push_back(i);
			this->chhose_ele.push_back(this->type[i%this->num]);
			//cout << "choose:" << i << "," << atom_name[this->type[i%this->num]] << endl;
		}
			
	}
	//正式基于原始的大矩阵构建部分矩阵
	//cout << "expand num:" << part_num << ", choose id num:" << choose_id.size() << endl;
	if (part_num != choose_id.size()) {
		cout << this->name<<":generate matrix base dimension ecnounter error!" << endl;
		this->part_num = -1;
		this->valid = false;
		//cin.get();
		return;
	}
	this->matrix = vector<vector<int>>(choose_id.size(), vector<int>(choose_id.size(), 0));
	for (int i = 0; i < choose_id.size();i++) {
		for (int j = i + 1; j < choose_id.size(); j++) {
			//cout << this->big_matrix[choose_id[i]][choose_id[j]] << ",";
			this->matrix[i][j] = this->matrix[j][i] = this->big_matrix[choose_id[i]][choose_id[j]];
		}	
		//cout << endl;
	}
	//我们打印一下选定的结构的图论矩阵
	/*for (int i = 0; i < choose_id.size(); i++) {
		for (int j = 0; j < choose_id.size(); j++) {
			cout << this->matrix[i][j] << ",";
		}
		cout << endl;
	}*/
	return;	
}


bool Cell_Matrix::comareGrapthMatrix(Cell_Matrix& ca, Cell_Matrix& cb) {
	//输入第一个是2D对应的表示，第二个是1D结构的表示
	//输出是否可以从前者中“切割”出后者
	if (ca.part_num <= cb.part_num||ca.valid==false||cb.valid==false) {
		cout << "cell atom nums do not compare!or str is not valid!failed!" << endl;
		return false;
	}

	vector<bool> use_flag(ca.part_num, false);
	vector<int> preIndex;//从Ca映射到Cb的初始序号对应关系
	/*for (int i = 0; i < cb.part_num; i++)
		preIndex.push_back(i);*/

	bool result=Cell_Matrix::dfsSearch(use_flag, preIndex, ca, cb);

	return result;
}
bool Cell_Matrix::dfsSearch(vector<bool>&use_flag, vector<int>&preIndex,Cell_Matrix&ca, Cell_Matrix&cb) {
	
	//发起深度优先搜索
	bool result = false;
	int target = cb.part_num;
	//初始化cb的联通序列
	vector<int>searchIndex = cb.getSequenceIndex();
	//从每一个起点当做第一个对应点开始深度搜索
	for (int i = 0; i < ca.part_num; i++) {
		//考虑元素情况下
		if (Cell_Matrix::IF_ELEMENT == true) {
			if (ca.chhose_ele[i] == cb.chhose_ele[0]) {
				preIndex.push_back(i);
				use_flag[i] = true;	
				result = dfsInner(use_flag, preIndex, searchIndex,target, ca, cb);
				if (result == true)
					break;
				use_flag[i] = false;
				preIndex.pop_back();
			}
		}
		else {
			preIndex.push_back(i);
			use_flag[i] = true;			
			result = dfsInner(use_flag, preIndex, searchIndex, target, ca, cb);
			if (result == true)
				break;
			use_flag[i] = false;
			preIndex.pop_back();
		}
		
	}
	return result;
}
bool Cell_Matrix::dfsInner(vector<bool>&use_flag, vector<int>&preIndex, vector<int>&indexSequence, int target_length,Cell_Matrix&ca, Cell_Matrix&cb) {
	//递归的深度优先搜索

	////单独进行测试
	//preIndex.push_back(1);
	//preIndex.push_back(3);
	//preIndex.push_back(4);
	//preIndex.push_back(6);
	//preIndex.push_back(7);
	
	bool result = false;
	//在这里进行最大匹配次数的限制
	if (ca.compareNum > Cell_Matrix::Max_compare_num) {
		cout << ca.name << ": max compare times!" << endl;
		return false;
	}
	if (preIndex.size() == target_length) {
		//检查当前的匹配同构性
		return Cell_Matrix::compareMatrix(preIndex, indexSequence,target_length, ca, cb);
	}
	for (int i = 0; i < ca.part_num; i++) {

		//在不考虑元素情况下，剪枝条件是，扩展的标准是没用过的点，并且和之前有连接关系
		//在考虑元素情况下，剪枝的条件就是元素对应
		
		if (Cell_Matrix::IF_ELEMENT == true) {
			if (use_flag[i] == false && ca.chhose_ele[i] == cb.chhose_ele[indexSequence[preIndex.size()]] && ca.ifConnect(use_flag, i)) {
				preIndex.push_back(i);
				use_flag[i] = true;
				result = dfsInner(use_flag, preIndex, indexSequence,target_length, ca, cb);
				if (result == true)
					break;
				use_flag[i] = false;
				preIndex.pop_back();
			}
		}
		else {
			if (use_flag[i] == false && ca.ifConnect(use_flag, i)) {
				preIndex.push_back(i);
				use_flag[i] = true;
				result = dfsInner(use_flag, preIndex, indexSequence,target_length, ca, cb);
				if (result == true)
					break;
				use_flag[i] = false;
				preIndex.pop_back();
			}
		}
		
		
	}
	return result;
}
bool Cell_Matrix::ifConnect(vector<bool>&use_flag, int index) {
	//判断某个点和之前选中的点是不是有连接关系的
	for (int i = 0; i < use_flag.size(); i++) {
		if (use_flag[i] == true && this->matrix[i][index] == 1)
			return true;
	}
	return false;
}

bool Cell_Matrix::compareMatrix(vector<int>&preIndex, vector<int>&indexSequence, int length, Cell_Matrix&ca, Cell_Matrix&cb) {
	//根据当前的映射关系，查看两个图是否同构
	//所谓同构必须映射后的关系完全相同

	//打印一下当前的关系
	/*for (int i = 0; i < cb.part_num; i++) {
		cout << "now parraled situations:" << i << ":"<< atom_name[cb.chhose_ele[i] ]<<" "<<preIndex[i] << ":" << atom_name[ca.chhose_ele[preIndex[i]]] << endl;
	}
	cin.get();	
	cout << "ca:" << endl;
	for (int i = 0; i < cb.part_num; i++) {
		for (int j = 0; j < cb.part_num; j++) {
			cout << ca.matrix[preIndex[i]][preIndex[j]] << " ";
		}
		cout << endl;
	}
	cout << "cb:" << endl;
	for (int i = 0; i < cb.part_num; i++) {
		for (int j = 0; j < cb.part_num; j++) {
			cout << cb.matrix[i][j] << " ";
		}
		cout << endl;
	}*/
	ca.compareNum++;	
	//cout << "compare_num:" << ca.compareNum + 1 << endl;
	//完全匹配模式
	for (int i = 0; i < preIndex.size(); i++) {
		for (int j = i+1; j < cb.part_num; j++) {
			if (cb.matrix[indexSequence[i]][indexSequence[j]] != ca.matrix[preIndex[i]][preIndex[j]])
				return false;
		}
	}
	////只计算连接个数的模式
	//int suma = 0,sumb=0;
	//for (int i = 0; i < preIndex.size(); i++) {
	//	suma = Cell_Matrix::getMatrixRowSum(ca, preIndex[i],preIndex,"ca");
	//	sumb = Cell_Matrix::getMatrixRowSum(cb, i,preIndex,"cb");
	//	if (suma != sumb)
	//		return false;
	//}

	return true;
}
int Cell_Matrix::getMatrixRowSum(Cell_Matrix&ca,int index_line, vector<int>&preIndex,string flag)//计算构造的矩阵的某一行的和
{
	int sum = 0;
	if (flag == "cb") {		
		for (int i = 0; i < ca.part_num; i++) {
			sum += ca.matrix[index_line][i];
		}		
	}
	else if (flag == "ca") {
		//注意此时，只能记录映射后的点的连接关系的总和
		for (int i = 0; i < preIndex.size(); i++) {
			sum += ca.matrix[index_line][preIndex[i]];
		}		
	}
	else {
		cout << "unkonwon flag name!" << endl;
		cin.get();
		sum=-1;
	}
	return sum;
	
}
int Cell_Matrix::test(string file_name1, string file_name2, vector<vector<double>>&max_ioncic, element*e) {
	//控制扩胞的逻辑
	int enlarge2d = 0;
	int enlarge1d = 0;//对于1D的扩胞情况
	cell cell_a(const_cast<char*>(file_name1.c_str()));
	cell cell_b(const_cast<char*>(file_name2.c_str()));
	if (cell_b.num <= 4) {
		enlarge1d = 3;
	}
	else
		enlarge1d = 2;
	/*if (enlarge1d*cell_b.num > 15)
		enlarge1d = 1;*/
	enlarge2d = cell_b.num / cell_a.num + 1;
	enlarge1d = 1;
	//加一个原子数目的剪枝
	/*if (cell_a.num >= Cell_Matrix::Max_num || cell_b.num >= 7)
		return false;*/

	//元素覆盖情况检查
	if (Cell_Matrix::IF_ELEMENT == true) {		
		if (Cell_Matrix::IF_ALL_ELEMNET == false) {
			bool setin = true;
			int j = 0;
			for (int i = 0; i < cell_b.num; i++) {
				for (j = 0; j < cell_a.num; j++) {
					if (cell_b.type[i] == cell_a.type[j])
						break;
				}
				if (j == cell_a.num) {
					setin = false;
					break;
				}

			}
			if (setin == false) {
				//cout << "element can not compare!break!" << endl;
				//cin.get();
				return false;
			}
		}
		else
		{
			//此时要求的是严格的元素一一对应
			if (cell_a.type_num != cell_b.type_num)
				return false;
			else {
				for (int i = 0; i < cell_a.type_num; i++) {
					if (cell_a.type_save[i] != cell_b.type_save[i])
						return false;
				}
			}
		}
		
	}
	
	//执行单元测试
	int dimen_2d[3] = { enlarge2d,enlarge2d,1 };
	int dimen_1d[3] = { enlarge1d,1,1 };
	string name_2d = "2d";
	string name_1d = "1d";
	double **dist = new double*[120];
	for (int i = 0; i < 120; i++)
	{
		dist[i] = new double[120];
		for (int j = 0; j < 120; j++)
		{
			dist[i][j] = -100;
		}
	}
	//建立原始的大矩阵
	Cell_Matrix ca(file_name1, 2, dimen_2d, const_cast<char*>(file_name1.c_str()), max_ioncic,e,dist);
	Cell_Matrix cb(file_name2, 1, dimen_1d, const_cast<char*>(file_name2.c_str()), max_ioncic, e,dist);
	/*ca.flushDataByFile("/share/home/wangz/2d_search/ridus_cut/code/from_2d_1d/config/testC.txt");
	cb.flushDataByFile("/share/home/wangz/2d_search/ridus_cut/code/from_2d_1d/config/testLC.txt");*/
	//ca.showInformation();
	//cb.showInformation();
	bool result = Cell_Matrix::comareGrapthMatrix(ca, cb);
	for (int i = 0; i < 120; i++) {
		delete[]dist[i];
	}
	delete[]dist;
	/*cout << "the test compare result is:" << result << endl;
	cin.get();*/
	return result;;

}

void Cell_Matrix::flushDataByFile(string file_name) {
	//从自定义的文件中读取相关信息，重新刷新数据
	//首先重新刷新数据
	this->part_num = 0;
	this->matrix.shrink_to_fit();
	this->chhose_ele.shrink_to_fit();
	this->choose_id.shrink_to_fit();
	char buffer[100];
	ifstream fin;
	fin.open(file_name, ios::in);
	fin >> this->part_num;
	fin.getline(buffer, 99);
	this->chhose_ele.resize(this->part_num);
	this->choose_id.resize(this->part_num);
	for (int i = 0; i < part_num; i++) {
		choose_id[i] = i;
		chhose_ele[i] = 6;
	}
	fin.getline(buffer, 99);
	this->matrix = vector<vector<int>>(part_num, vector<int>(part_num,0));
	for (int i = 0; i < part_num; i++) {
		for (int j = 0; j < part_num; j++) {
			fin >> this->matrix[i][j];
		}
		fin.getline(buffer, 99);
	}
	fin.close();
	return;

}

void Cell_Matrix::showInformation() {
	//展示一下相关信息
	cout << "part_num:" << this->part_num;
	cout << "id and ele:" << endl;
	for (int i = 0; i < this->part_num; i++) {
		cout << "id:" << this->choose_id[i] << ":" << this->chhose_ele[i] << endl;
	}
	cout << "the matrix is:" << endl;
	for (int i = 0; i < this->part_num; i++) {
		for (int j = 0; j < this->part_num; j++) {
			cout << this->matrix[i][j] << " ";
		}
		cout << endl;
	}
	return;
}
vector<int> Cell_Matrix::getSequenceIndex() {
	//产生特定的初始搜索序列，为了加速剪枝
	vector<int>resultIndex;
	resultIndex.push_back(0);
	//采用并查集减少操作，初始化时每个的父节点都是-1
	my_union temp(this->part_num);
	while (resultIndex.size() < this->part_num) {
		for (int i = 0; i < this->part_num; i++) {
			for (int j = 0; j < resultIndex.size(); j++) {
				if (i != resultIndex[j] && this->matrix[i][resultIndex[j]] == 1&&!temp.is_connected(i,resultIndex[j])) {
					resultIndex.push_back(i);
					//cout << "push:" << i << endl;
					temp.union2ele(i,resultIndex[j]);
				}
			}
		}
	}	
	return resultIndex;
}
