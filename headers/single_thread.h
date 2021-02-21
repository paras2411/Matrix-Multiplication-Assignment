#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <fstream>
#include<immintrin.h>
using namespace std;

#define blocksize 16

void singleThread(int N, int *matA, int *matB, int *output){

    for(int i = 0; i < N; i += blocksize){
        for(int j = 0; j < N ; j += blocksize){
            for(int k = i ; k < i + blocksize; k++){
                for(int l = j; l < j + blocksize; l+=4){

                    // loop unrolling
                    output[k+l] += matA[k*N + l] * matB[l*N + N-1-k];
                    output[k+l+1] += matA[k*N + (l+1)] * matB[(l+1)*N + N-1-k];
                    output[k+l+2] += matA[k*N + (l+2)] * matB[(l+2)*N + N-1-k];
                    output[k+l+3] += matA[k*N + (l+3)] * matB[(l+3)*N + N-1-k];
                }
            }
        }
    }
}
