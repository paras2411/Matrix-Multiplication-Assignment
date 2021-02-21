
// Create other necessary functions here
#include<iostream>
using namespace std;

#pragma unroll
#define blocksize 16


__global__ void DMMul(int N, int *matA, int *matB, int *output) {

    int i = blocksize * (blockIdx.x / (N / blocksize));
    int j = blocksize * (blockIdx.x % (N / blocksize));
    int ind = blockIdx.x * (2 * N - 1);

    for(int k = i; k < i + blocksize; k++) {
        #pragma unroll(4)
        for (int l = j; l < j + blocksize; l++) {
            output[ind + (k + l)] += matA[k * N + l] * matB[l * N + N - 1 - k];
        }
    }
    __syncthreads();
}

// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{
    int *devA, *devB, *dev_output;
    cudaMalloc((void **)&devA, (N * N) * sizeof(int));
    cudaMalloc((void **)&devB, (N * N) * sizeof(int));

    cudaMemcpy(devA, matA, (N * N) * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(devB, matB, (N * N) * sizeof(int), cudaMemcpyHostToDevice);

    int blocks = (N * N) / (blocksize * blocksize);
    int *temp = new int[blocks * (2 * N - 1)];
    cudaMalloc((void **)&dev_output, blocks * (2 * N - 1) * sizeof(int));

    auto begin = std::chrono::high_resolution_clock::now();
    DMMul<<<(N * N) / (blocksize * blocksize), 1>>>(N, devA, devB, dev_output);
    auto end = std::chrono::high_resolution_clock::now();
    cout << "Device function execution time: " << ((double)std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())/1000.0 << "ms\n";

    cudaMemcpy(temp, dev_output, blocks * (2 * N - 1) * sizeof(int), cudaMemcpyDeviceToHost);

    for(int i = 0; i < blocks * (2 * N - 1); i++) {
        output[i % (2 * N - 1)] += temp[i];
    }

    cudaFree(devA); cudaFree(devB); cudaFree(dev_output);
}

