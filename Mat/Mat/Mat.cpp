
#include "stdafx.h"
#include "Mat.hpp"
#include "common.hpp"
namespace Arsenal{
template <typename Dtype>
void Mat<Dtype>::MM_multiply(const Mat &A, const Mat &B, Mat &C, const int m, const int n, const int k){
	for (int i_m = 0; i_m < m; i_m ++){
		for (int i_n = 0; i_n < n; i_n ++){
			int C_idx = i_n * m + i_m;
			C.MAT_DATA[C_idx] = 0;
			for (int i_k = 0; i_k < k; i_k ++){
				int A_idx = i_k * m + i_m;
				int B_idx = i_n * k + i_k;
				C.MAT_DATA[C_idx] += A.MAT_DATA[A_idx] * B.MAT_DATA[B_idx];
			}
		}
	}
}

template <typename Dtype>
Dtype* Mat<Dtype>::GET_CPU_DATA(){
	Dtype *CPU_DATA = new Dtype(_row * _column);
	CPU_DATA = MAT_DATA;
	return CPU_DATA;
}

INSTANCE_CLASS(Mat);
}
