#include <stdio.h>
#include "Mat.hpp"
#include "common.hpp"
#include <omp.h>

#define STRASSEN
#define FOLDING_SIZE 16
#define PE C[C_idx] += tmp * B[B_idx]; \
		   B_idx ++; \
		   C_idx ++;


namespace Arsenal{
template <typename Dtype>
void Mat<Dtype>::MM_multiply(const Mat &A, const Mat &B, Mat &C, const int m, const int n, const int k){

	const Dtype *A_data = new Dtype(m * k); A_data = A.MAT_DATA;
	const Dtype *B_data = new Dtype(k * n); B_data = B.MAT_DATA;
	Dtype *C_data = new Dtype(m * n); C_data = C.MAT_DATA;
#ifdef NAIVE
	MM_multiply_naive(A_data, B_data, C_data, m ,n, k);
#endif

#ifdef ROW_DOMIN
	MM_multiply_common(A_data. B_data, C_data, m, n, k);
#endif

#ifdef STRASSEN
	MM_multiply_STRASSEN(A_data, B_data, C_data, m, n, k);
#endif
}


template <typename Dtype>
void Mat<Dtype>::MM_multiply_STRASSEN(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k){
	if ((m % 2 == 0) & (n % 2 == 0) & (k % 2 == 0)){ 
		int tmp_m = m / 2;
		int tmp_n = n / 2;
		int tmp_k = k / 2; 		
		Dtype *A11 = new Dtype(tmp_m * tmp_k);
		Dtype *A12 = new Dtype(tmp_m * tmp_k);
		Dtype *A21 = new Dtype(tmp_m * tmp_k);
		Dtype *A22 = new Dtype(tmp_m * tmp_k);
		Dtype *B11 = new Dtype(tmp_k * tmp_n);
		Dtype *B12 = new Dtype(tmp_k * tmp_n);
		Dtype *B21 = new Dtype(tmp_k * tmp_n);
		Dtype *B22 = new Dtype(tmp_k * tmp_n);
		Dtype *C11 = new Dtype(tmp_m * tmp_n);
		Dtype *C12 = new Dtype(tmp_m * tmp_n);
		Dtype *C21 = new Dtype(tmp_m * tmp_n);
		Dtype *C22 = new Dtype(tmp_m * tmp_n);

		Dtype *P1 = new Dtype(tmp_m * tmp_n);
		Dtype *P2 = new Dtype(tmp_m * tmp_n);
		Dtype *P3 = new Dtype(tmp_m * tmp_n);
		Dtype *P4 = new Dtype(tmp_m * tmp_n);
		Dtype *P5 = new Dtype(tmp_m * tmp_n);
		Dtype *P6 = new Dtype(tmp_m * tmp_n);
		Dtype *P7 = new Dtype(tmp_m * tmp_n);

		Dtype *P11 = new Dtype(tmp_k * tmp_n);
		Dtype *P20 = new Dtype(tmp_m * tmp_k);
		Dtype *P30 = new Dtype(tmp_m * tmp_k);
		Dtype *P41 = new Dtype(tmp_k * tmp_n);
		Dtype *P50 = new Dtype(tmp_m * tmp_k);
		Dtype *P51 = new Dtype(tmp_k * tmp_n);
		Dtype *P60 = new Dtype(tmp_m * tmp_k);
		Dtype *P61 = new Dtype(tmp_k * tmp_n);
		Dtype *P70 = new Dtype(tmp_m * tmp_k);
		Dtype *P71 = new Dtype(tmp_k * tmp_n);

		for (int i_tmp_m = 0; i_tmp_m < tmp_m; i_tmp_m ++){				
				int IDX = tmp_k * i_tmp_m;
				int A11_idx = k * i_tmp_m;
				int A12_idx = k * i_tmp_m + tmp_k;
				int A21_idx = k * (i_tmp_m + tmp_m);
				int A22_idx = k * (i_tmp_m + tmp_m) + tmp_k;
			for (int i_tmp_k = 0; i_tmp_k < tmp_k; i_tmp_k ++){
				A11[IDX] = A[A11_idx];  A12[IDX] = A[A12_idx];  A21[IDX] = A[A21_idx];  A22[IDX] = A[A22_idx];
				IDX ++;  A11_idx ++;  A12_idx ++;  A21_idx ++;  A22_idx ++;
			}
		}

		for (int i_tmp_k = 0; i_tmp_k < tmp_k; i_tmp_k ++){
				int IDX = tmp_n * i_tmp_k;
				int B11_idx = n * i_tmp_k;
				int B12_idx = n * i_tmp_k + tmp_n;
				int B21_idx = n * (i_tmp_k + tmp_k);
				int B22_idx = n * (i_tmp_k + tmp_k) + tmp_n;
			for (int i_tmp_n = 0; i_tmp_n < tmp_n; i_tmp_n ++){
				B11[IDX] = B[B11_idx]; B12[IDX] = B[B12_idx]; B21[IDX] = B[B21_idx]; B22[IDX] = B[B22_idx];
				IDX ++;  B11_idx ++;  B12_idx ++;  B21_idx ++;  B22_idx ++;
			}
		}

		MM_minus(B12, B22, P11, tmp_k, tmp_n);
		MM_multiply_STRASSEN(A11, P11, P1, tmp_m, tmp_n, tmp_k);

		MM_add(A11, A12, P20, tmp_m, tmp_k);
		MM_multiply_STRASSEN(P20, B22, P2, tmp_m, tmp_n, tmp_k);

		MM_add(A21, A22, P30, tmp_m, tmp_k);
		MM_multiply_STRASSEN(P30, B11, P3, tmp_m, tmp_n, tmp_k);

		MM_minus(B21, B11, P41, tmp_k, tmp_n);
		MM_multiply_STRASSEN(A22, P41, P4, tmp_m, tmp_n, tmp_k);

		MM_add(A11, A22, P50, tmp_m, tmp_k);
		MM_add(B11, B22, P51, tmp_k, tmp_n);
		MM_multiply_STRASSEN(P50, P51, P5, tmp_m, tmp_n, tmp_k);

		MM_minus(A12, A22, P60, tmp_m, tmp_k);
		MM_add(B21, B22, P61, tmp_k, tmp_n);
		MM_multiply_STRASSEN(P60, P61, P6, tmp_m, tmp_n, tmp_k);

		MM_minus(A11, A21, P70, tmp_m, tmp_k);
		MM_add(B11, B12, P71, tmp_k, tmp_n);
		MM_multiply_STRASSEN(P70, P71, P7, tmp_m, tmp_n, tmp_k);

		MM_add(P5, P4, C11, tmp_m, tmp_n);
		MM_minus(C11, P2, C11, tmp_m, tmp_n);
		MM_add(C11, P6, C11, tmp_m, tmp_n);
		MM_add(P1, P2, C12, tmp_m, tmp_n);
		MM_add(P4, P3, C21, tmp_m, tmp_n);
		MM_add(P1, P5, C22, tmp_m, tmp_n);
		MM_minus(C22, P3, C22, tmp_m, tmp_n);
		MM_minus(C22, P7, C22, tmp_m, tmp_n);

		for (int i_tmp_m = 0; i_tmp_m < tmp_m; i_tmp_m ++){
				int IDX = tmp_n * i_tmp_m;
				int C11_idx = n * i_tmp_m;
				int C12_idx = n * i_tmp_m + tmp_n;
				int C21_idx = n * (i_tmp_m + tmp_m);
				int C22_idx = n * (i_tmp_m + tmp_m) + tmp_n;
			for (int i_tmp_n = 0; i_tmp_n < tmp_n; i_tmp_n ++){
				C[C11_idx] = C[IDX];  C[C12_idx] = C[IDX];  C[C21_idx] = C[IDX];  C[C22_idx] = C[IDX];
				IDX ++;  C11_idx ++;  C12_idx ++;  C21_idx ++;  C22_idx ++;
			}
		}

		delete [] A11;	delete [] A12;	delete [] A21;	delete [] A22;
		delete [] B11;	delete [] B12;	delete [] B21;	delete [] B22;
		delete [] C11; 	delete [] C12; 	delete [] C21; 	delete [] C22; 	
		delete [] P1;   delete [] P2;   delete [] P3;   delete [] P4;   delete [] P5;   delete [] P6;   delete [] P7; 
		delete [] P11;  delete [] P20;  delete [] P30;  delete [] P41;  delete [] P50;  delete [] P51;  delete [] P60;  delete [] P61;  delete [] P70;  delete [] P71;

	}
	else{
		MM_multiply_common(A, B, C, m, n, k);
	}
}

template <typename Dtype>
void Mat<Dtype>::MM_add(Dtype *A, Dtype *B, Dtype *C, const int m, const int n){	//C = A + B
	for (int i = 0; i < m * n; i ++){
		C[i] = A[i] + B[i];
	}
}

template <typename Dtype>
void Mat<Dtype>::MM_minus(Dtype *A, Dtype *B, Dtype *C, const int m, const int n){   //C = A - B
	for (int i = 0; i < m * n; i ++){
		C[i] = A[i] - B[i];
	}
}

template <typename Dtype>
void Mat<Dtype>::MM_multiply_common(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k){	//common multiply:C = A * B

#pragma omp parallel for num_threads(2)
	for (int i = 0; i < m * n; i ++){
		C[i] = 0;
	}
	
	for (int i_m = 0; i_m < m; i_m ++){   //from naive to row-domin
		for (int i_k = 0; i_k < k; i_k ++){
			int A_idx = i_m * k + i_k;
			Dtype tmp = A[A_idx];
			int B_idx = i_k * n; int C_idx = i_m * n;	
			if (tmp){	
				for (int i_n = 0; i_n <= (n / FOLDING_SIZE); i_n ++){
					if (i_n   < (n / FOLDING_SIZE)){						
						PE; PE; PE; PE;
						PE; PE; PE; PE;
						PE; PE; PE; PE;
						PE; PE; PE; PE;
					}
					else{
						for (int i_para = 0; i_para < n % FOLDING_SIZE; i_para ++){
							C[C_idx] += tmp * B[B_idx];						
							B_idx ++;
							C_idx ++;
						}
					}				
				}
			}
		}
	}
}

template <typename Dtype>
void Mat<Dtype>::MM_multiply_naive(const Dtype *A, const Dtype *B, Dtype *C, const int m, const int n, const int k){
	for (int i_m = 0; i_m < m; i_m ++){
		for (int i_n = 0; i_n < n; i_n ++){
			int C_idx = i_m * n + i_n;
			Dtype val = 0;
			for (int i_k = 0; i_k < k; i_k ++){
				int A_idx = i_m * k + i_k;
				int B_idx = i_k * n + i_n;
				val += A[A_idx] * B[B_idx];
			}
			C[C_idx] = val;
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
