#ifndef MAT_HPP_
#define MAT_HPP_

#include <string>
#include <fstream>

using std::string;
using std::ifstream;


namespace Arsenal{

template <typename Dtype>
class Mat{
public: 
Mat(){
}

Mat(int row, int column){	//构造函数
	_row = row;
	_column = column;
	MAT_DATA = new Dtype[_row * _column];
}

Mat(int row, int column, string file_name){	//文件输入构造函数
	_row = row;
	_column = column;
	MAT_DATA = new Dtype[_row * _column];
	const char* _file_name = file_name.data();
	ifstream in_file(_file_name);
	for (int i = 0; i < _row * _column; i ++){
		in_file >> MAT_DATA[i];
	}
	//MAT_DATA = 
}

Mat(const Mat &src){		//拷贝构造函数
	_row = src._row;
	_column = src._column;
	MAT_DATA = new Dtype[_row * _column];
	for (int i = 0; i < _column; i ++){
		for (int j = 0; j < _row; j ++){
			int MAT_DATA_idx = j * _row + i;
			MAT_DATA[MAT_DATA_idx] = src.MAT_DATA[MAT_DATA_idx];
		}
	}
} 

~Mat(){		//析构函数
	delete []MAT_DATA;
}
//++++++++++++++++++Mat_computation++++++++++++++++++++++++++++++++++++++++++
void MM_multiply(const Mat &A, const Mat &B, Mat &C, const int m, const int n, const int k);   //C(m*n) = A(m*k) * B(k*n)矩阵乘法	
void MV_multiply(bool TRANS, const Mat &A, const Mat &B, Mat &C, const int m, const int n);	
//矩阵向量乘法 if(TRANS = false) C(m*1) = A(m*n) * B(n*1)
//			   if(TRANS = true)  C(1*m) = B(1*m) * A(m*n)
void VV_multiply(bool TRANS, const Mat &A, const Mat &B, Mat &C, const int m, const int n);
//向量乘法 if(TRANS = false) C(m*n) = A(m*1) * B(1*n)
//		   if(TRANS = true)  C(1*1) = B(1*m) * A(m*1)
void Dot(const Mat &A, const Mat &B, Mat &C); //点乘 C = A .* B;
void Transpose();
void MM_add(const Mat&A, const Mat &B, Mat &C);
//++++++++++++++++++Mat_generate+++++++++++++++++++++++++++++++++++++++++++++
void Zeros();
void Ones();
void Eye();
void Random(const Dtype LowerBound, const Dtype UpperBound);	//产生均匀分布的随机阵
void Randn(Dtype Average, Dtype Varience);
void Randn_int(const int Average, const int Varience);   //产生int正态分布的随机阵
void Randn_float(const float Average, const float Varience);	//产生float正态分布随机阵
void Randn_double(const double Average, const double Varience);	  //产生double正态分布随机阵	

//++++++++++++++++++Mat_shape_display+++++++++++++++++++++++++++++++++++++++++
Dtype* GET_CPU_DATA();
void Reshape(int row, int column);	//change the shape of Mat to (row, column)
void Display(int row_begin, int row_end, int column_begin, int column_end);
void Display();
void Mutable();
void Constant();

private:
void MM_multiply_STRASSEN(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k);	//STRASSEN矩阵算法
void MM_add(Dtype *A, Dtype *B, Dtype *C, const int m, const int n);	//矩阵加法
void bias_add(Dtype *A, Dtype *B, Dtype *C, int m, int n);
void MM_minus(Dtype *A, Dtype *B, Dtype *C, const int m, const int n);	//矩阵减法
void MM_multiply_common(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k);	//普通矩阵乘法
void MM_multiply_naive(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k);	//朴素矩阵乘法
void MV_multiply_navie(bool TRANS, const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n);		//朴素矩阵向量乘法
void VV_multiply_navie(bool TRANS, const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n);		//朴素向量乘法
Dtype *MAT_DATA;
int _row, _column;

};

}


#endif
