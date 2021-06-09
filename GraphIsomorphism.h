#pragma once
#pragma once

#ifndef GRAPH

#include "CONNECTION.h"
#include <iostream>
using namespace std;
/**
	本文件主要包含从2D到1D结构寻找的代码
	经过测试，必须要包含元素信息辅助剪枝才行
**/

class Cell_Matrix :public cell {
public:
	Cell_Matrix(string name, int materia_dimension,int* dimen,char*jiegou):cell(jiegou){
		this->name = name;
		this->material_dimen = materia_dimension;
		for (int i = 0; i < 3; i++) {
			this->dimension[i] = dimen[i];
		}
		generate_matrix();
	}
	Cell_Matrix(string name, int materia, int*dimen, char*jiegou, vector<vector<double>>&max_ioncic, element*e,double**dist) :cell(jiegou, e,max_ioncic,dist, 2) {
		this->name = name;
		this->material_dimen = materia;
		for (int i = 0; i < 3; i++) {
			this->dimension[i] = dimen[i];
		}
		generate_matrix();
	}
	~Cell_Matrix() {
		this->matrix.shrink_to_fit();
		this->chhose_ele.shrink_to_fit();
		this->choose_id.shrink_to_fit();
	}
	int part_num = 0;//对应图的点的个数
	int dimension[3] = { 0 };//对应这个矩阵是如何扩胞获得
	string name = "";
	int material_dimen = 0;
	vector<vector<int>> matrix;//对应的图的表示
	vector<int>choose_id;//挑选的点的序号
	vector<int>chhose_ele;//挑选的元素信息
	
	static bool dfsSearch(vector<bool>&use_flag, vector<int>&preIndex, Cell_Matrix&ca, Cell_Matrix&cb);//发起深度优先搜索
	static bool dfsInner(vector<bool>&use_flag, vector<int>&preIndex, vector<int>&indexSquence,int length, Cell_Matrix&ca, Cell_Matrix&cb);//深度优先搜索的具体实现
	static bool comareGrapthMatrix(Cell_Matrix &ca, Cell_Matrix &cb);
	static bool compareMatrix(vector<int>&preIndex, vector<int>&indexSequence,int length, Cell_Matrix&ca, Cell_Matrix&cb);//根据当前的匹配关系，查看是否同构
	static int test(string file_name1, string file_name2, vector<vector<double>>&max_ioncic, element*e);
	static int getMatrixRowSum(Cell_Matrix&ca,int index_line, vector<int>&preIndex, string flag);//计算构造的矩阵的某一行的和
	int compareNum = 0;//匹配比较的次数


	static bool IF_ELEMENT;//在比较的时候是否区分元素
	static bool IF_ALL_ELEMNET;//在比较的时候是否考虑元素一一对应
	static int Max_num;//最多的匹配的原子的数目
	static int Max_compare_num;//最多的匹配情况的数目
private:
	bool valid = true;
	void generate_matrix();//通过指定的维度参数构造	
	bool ifConnect(vector<bool>&use_flag, int index);//判断某个点是和之前选中的点存在连接关系的
	void flushDataByFile(string file_name);//从自定义的文件中读取相关信息，重新刷新数据
	void showInformation();
	vector<int>getSequenceIndex();//产生特定的初始搜索序列，为了加速剪枝
	
};


class my_union {
	//自己独立的并查集结构
public:
	my_union(int size) {
		parent = new int[size];
		for (int i = 0; i < size; i++)
			parent[i] = i;
	}
	~my_union() {
		delete[]parent;
		//cout << "stru destroied!" << endl;
	}
	//查询某个节点的根节点是谁
	//一直递归查找直到查找到原始节点
	int find(int index) {
		if (parent[index] != index) {
			return find(parent[index]);
		}
		else
			return index;
	}
	//两个元素是同一个集合
	bool is_connected(int a, int b) {
		return find(a) == find(b);
	}
	//将两个元素合并
	void union2ele(int a, int b) {
		int aroot = find(a);
		int broot = find(b);
		if (aroot != broot) {
			parent[aroot] = broot;
		}
		else
		{
			return;
		}
	}
private:
	int*parent;
};












#define GRAPH
#endif