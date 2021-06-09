#pragma once
#pragma once

#ifndef GRAPH

#include "CONNECTION.h"
#include <iostream>
using namespace std;
/**
	���ļ���Ҫ������2D��1D�ṹѰ�ҵĴ���
	�������ԣ�����Ҫ����Ԫ����Ϣ������֦����
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
	int part_num = 0;//��Ӧͼ�ĵ�ĸ���
	int dimension[3] = { 0 };//��Ӧ�������������������
	string name = "";
	int material_dimen = 0;
	vector<vector<int>> matrix;//��Ӧ��ͼ�ı�ʾ
	vector<int>choose_id;//��ѡ�ĵ�����
	vector<int>chhose_ele;//��ѡ��Ԫ����Ϣ
	
	static bool dfsSearch(vector<bool>&use_flag, vector<int>&preIndex, Cell_Matrix&ca, Cell_Matrix&cb);//���������������
	static bool dfsInner(vector<bool>&use_flag, vector<int>&preIndex, vector<int>&indexSquence,int length, Cell_Matrix&ca, Cell_Matrix&cb);//������������ľ���ʵ��
	static bool comareGrapthMatrix(Cell_Matrix &ca, Cell_Matrix &cb);
	static bool compareMatrix(vector<int>&preIndex, vector<int>&indexSequence,int length, Cell_Matrix&ca, Cell_Matrix&cb);//���ݵ�ǰ��ƥ���ϵ���鿴�Ƿ�ͬ��
	static int test(string file_name1, string file_name2, vector<vector<double>>&max_ioncic, element*e);
	static int getMatrixRowSum(Cell_Matrix&ca,int index_line, vector<int>&preIndex, string flag);//���㹹��ľ����ĳһ�еĺ�
	int compareNum = 0;//ƥ��ȽϵĴ���


	static bool IF_ELEMENT;//�ڱȽϵ�ʱ���Ƿ�����Ԫ��
	static bool IF_ALL_ELEMNET;//�ڱȽϵ�ʱ���Ƿ���Ԫ��һһ��Ӧ
	static int Max_num;//����ƥ���ԭ�ӵ���Ŀ
	static int Max_compare_num;//����ƥ���������Ŀ
private:
	bool valid = true;
	void generate_matrix();//ͨ��ָ����ά�Ȳ�������	
	bool ifConnect(vector<bool>&use_flag, int index);//�ж�ĳ�����Ǻ�֮ǰѡ�еĵ�������ӹ�ϵ��
	void flushDataByFile(string file_name);//���Զ�����ļ��ж�ȡ�����Ϣ������ˢ������
	void showInformation();
	vector<int>getSequenceIndex();//�����ض��ĳ�ʼ�������У�Ϊ�˼��ټ�֦
	
};


class my_union {
	//�Լ������Ĳ��鼯�ṹ
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
	//��ѯĳ���ڵ�ĸ��ڵ���˭
	//һֱ�ݹ����ֱ�����ҵ�ԭʼ�ڵ�
	int find(int index) {
		if (parent[index] != index) {
			return find(parent[index]);
		}
		else
			return index;
	}
	//����Ԫ����ͬһ������
	bool is_connected(int a, int b) {
		return find(a) == find(b);
	}
	//������Ԫ�غϲ�
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