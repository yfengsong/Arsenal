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

Mat(int row, int column){	
	_row = row;
	_column = column;
	MAT_DATA = new Dtype[_row * _column];
}

Mat(int row, int column, string file_name){	
	_row = row;
	_column = column;
	MAT_DATA = new Dtype[_row * _column];
	const char* _file_name = file_name.data();
	ifstream in_file(_file_name);
	for (int i = 0; i < _row * _column; i ++){
		in_file >> MAT_DATA[i];
	}
	in_file.close();
	//MAT_DATA = 
}

Mat(const Mat<Dtype> &src){		
	_row = src._row;
	_column = src._column;
	MAT_DATA = new Dtype[_row * _column];
	for (int i = 0; i < _column*_row; i ++){
		MAT_DATA[i] = src.MAT_DATA[i];
	}
} 

~Mat(){		
	delete []MAT_DATA;
}
//++++++++++++++++++Mat_computation++++++++++++++++++++++++++++++++++++++++++
void MM_multiply(const Mat &A, const Mat &B, Mat &C, const int m, const int n, const int k);   //C(m*n) = A(m*k) * B(k*n)
void MV_multiply(bool TRANS, const Mat &A, const Mat &B, Mat &C, const int m, const int n);	
//			   if(TRANS = false) C(m*1) = A(m*n) * B(n*1)
//			   if(TRANS = true)  C(1*m) = B(1*m) * A(m*n)
void VV_multiply(bool TRANS, const Mat &A, const Mat &B, Mat &C, const int m, const int n);
//		   if(TRANS = false) C(m*n) = A(m*1) * B(1*n)
//		   if(TRANS = true)  C(1*1) = B(1*m) * A(m*1)
void Dot(Mat &A, Mat &B, Mat &C); //C = A .* B;
void bias_multiply(Mat &A, Dtype bias, Mat &B);
void bias_division(Mat &A, Dtype bias, Mat &B);
void Transpose();
void MM_add(Mat &A, Mat &B, Mat &C);
void MM_sub(Mat &A, Mat &B, Mat &C);
Dtype Sum();
int* Accuracy(Mat &A, Mat &B);//A,B must be vector! the item return has 3 values, correct:item[0] error:item[1] total:item[2]
//++++++++++++++++++Mat_generate+++++++++++++++++++++++++++++++++++++++++++++
void Zeros();
void Ones();
void Eye();

//fuction for randn, un usable
/*
void Random(const Dtype LowerBound, const Dtype UpperBound);	//
void Randn(Dtype Average, Dtype Varience);
void Randn_int(const int Average, const int Varience);   //
void Randn_float(const float Average, const float Varience);	//
void Randn_double(const double Average, const double Varience);	  //
*/
//++++++++++++++++++Mat_shape_display+++++++++++++++++++++++++++++++++++++++++
Dtype* GET_CPU_DATA();
void Reshape(int row, int column);	//change the shape of Mat to (row, column)
void Display(int row_begin, int row_end, int column_begin, int column_end);
void Display();
void Mutable();
void Constant();
void Shape_Init(int row, int column);
void Shape_Init(int row, int column, string file_name);
int* size(); 
void Copy(Mat<Dtype> copies);
void Copy(int begin_bia, Mat<Dtype> copies);	//copy part of copies.
void Shape_Change(int row, int column);

private:
void MM_multiply_STRASSEN(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k);	//STRASSEN
void MM_add(Dtype *A, Dtype *B, Dtype *C, const int m, const int n);	//
void bias_add(Dtype *A, Dtype *B, Dtype *C, int m, int n);
void bias_sub(Dtype *A, Dtype *B, Dtype *C, int m, int n);
void MM_minus(Dtype *A, Dtype *B, Dtype *C, const int m, const int n);	//
void MM_multiply_common(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k);	//
void MM_multiply_naive(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k);	//
void MV_multiply_navie(bool TRANS, const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n);		//
void VV_multiply_navie(bool TRANS, const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n);		//
Dtype *MAT_DATA;
int _row, _column;

};

}


#endif
