#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <sys/time.h>
#include "cusparse.h"

#define NET_SIZE 256
#define NZ_NUM 11877


#define CLEANUP(s)                                     \
do {                                                   \
    printf ("%s\n", s);                                \
    if (cooRowIndexHostPtr) free(cooRowIndexHostPtr);  \
    if (cooColIndexHostPtr) free(cooColIndexHostPtr);  \
    if (cooValHostPtr)      free(cooValHostPtr);       \
    if (csrRowPtr)          cudaFree(csrRowPtr);       \
    if (cooRowIndex)        cudaFree(cooRowIndex);     \
    if (cooColIndex)        cudaFree(cooColIndex);     \
    if (cooVal)             cudaFree(cooVal);          \
    if (descr)              cusparseDestroyMatDescr(descr); \
    if (handle)             cusparseDestroy(handle);   \
    cudaDeviceReset();                                 \
    fflush (stdout);                                   \
} while (0)

int main(){
    cudaError_t cudaStat1, cudaStat2, cudaStat3, cudaStat4;
    cusparseStatus_t status;
    cusparseHandle_t handle = 0;
    cusparseMatDescr_t descr = 0;
    int *    cooRowIndexHostPtr = 0;
    int *    cooColIndexHostPtr = 0;
    float *  cooValHostPtr = 0;
    int *    cooRowIndex = 0;
    int *    cooColIndex = 0;
    float *  cooVal = 0;
    int *    csrRowPtr = 0;
    float *  yHostPtr = 0;
    float *  y = 0;
    int      n, nnz;
    float    fzero = 0.0;
    float    fone = 1.0;

    int i, j, ptr;
    FILE *ifp;
    struct timeval t1, t2, tr;
    double inWMd[NET_SIZE];
    double ofbWMd[2][NET_SIZE];
    double intWMd[NET_SIZE][NET_SIZE];
    double outWMd[NET_SIZE+1][2];

    float inWM[NET_SIZE];
    float ofbWM[2][NET_SIZE];
    float intWM[NET_SIZE][NET_SIZE];
    float outWM[NET_SIZE+1][2];

    int samplelength = 10000;
    double sampleinputd[samplelength];
    float sampleinput[samplelength];
    
    float totalstate[NET_SIZE+3];
    float internalState[NET_SIZE];
    float netOut[2];

    // Read in inWM from "inWM.bin" file
    if (!(ifp = fopen("./weights/inWM.bin", "rb"))) {
        printf("File inWM.bin cannot be opened for read.\n");
        return -1;
    }
    fread(inWMd, 8, NET_SIZE, ifp);
    fclose(ifp);
    for (i = 0; i < NET_SIZE; ++i)
        inWM[i] = (float) inWMd[i];

    // Read in ofbWM from "ofbWM.bin" file
    if (!(ifp = fopen("./weights/ofbWM.bin", "rb"))) {
        printf("File ofbWM.bin cannot be opened for read.\n");
        return -1;
    }
    fread(ofbWMd[0], 8, NET_SIZE, ifp);
    fread(ofbWMd[1], 8, NET_SIZE, ifp);
    fclose(ifp);
    for (i = 0; i < NET_SIZE; ++i) {
        ofbWM[0][i] = (float) ofbWMd[0][i];
        ofbWM[1][i] = (float) ofbWMd[1][i];
    }

    // Read in intWM from "intWM.bin" file
    if (!(ifp = fopen("./weights/intWM.bin", "rb"))) {
        printf("File intWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < NET_SIZE; ++i)
        fread(intWMd[i], 8, NET_SIZE, ifp);
    fclose(ifp);
    for (i = 0; i < NET_SIZE; ++i)
        for (j = 0; j < NET_SIZE; ++j)
            intWM[i][j] = (float) intWMd[i][j];

    // Read in outWM from "outWM.bin" file
    if (!(ifp = fopen("./weights/outWM.bin", "rb"))) {
        printf("File outWM.bin cannot be opened for read.\n");
        return -1;
    }
    for (i = 0; i < NET_SIZE+1; ++i)
        fread(outWMd[i], 8, 2, ifp);
    fclose(ifp);
    for (i = 0; i < NET_SIZE+1; ++i) {
        outWM[i][0] = (float) outWMd[i][0];
        outWM[i][1] = (float) outWMd[i][1];
    }

    // Read in sample input from "sampleinput.bin" file
    if (!(ifp = fopen("./weights/sampleinput.bin", "rb"))) {
        printf("File sampleinput.bin cannot be opened for read.\n");
        return -1;
    }
    fseek(ifp, 8, SEEK_SET);
    fread(sampleinputd, 8, samplelength, ifp);
    fclose(ifp);
    for (i = 0; i < samplelength; ++i)
        sampleinput[i] = (float) sampleinputd[i];

    for (i = 0; i < (NET_SIZE + 3); ++i)
        totalstate[i] = 0;

    printf("testing example\n");

    n = NET_SIZE; nnz = NZ_NUM;

    cooRowIndexHostPtr = (int *)   malloc(nnz * sizeof(cooRowIndexHostPtr[0]));
    cooColIndexHostPtr = (int *)   malloc(nnz * sizeof(cooColIndexHostPtr[0]));
    cooValHostPtr      = (float *) malloc(nnz * sizeof(cooValHostPtr[0]));
    if ((!cooRowIndexHostPtr) || (!cooColIndexHostPtr) || (!cooValHostPtr)) {
        CLEANUP("Host malloc failed (matrix)");
        return 1;
    }

    ptr = 0;
    for (i = 0; i < NET_SIZE; ++i) {
        for (j = 0; j < NET_SIZE; ++j) {
            if (intWM[i][j] != 0) {
                cooRowIndexHostPtr[ptr] = i;
                cooColIndexHostPtr[ptr] = j;
                cooValHostPtr[ptr] = intWM[i][j];
                ptr++;
            }
        }
    }
    // cooRowIndexHostPtr[0] = 0; cooColIndexHostPtr[0] = 0; cooValHostPtr[0] = 1.0;

    yHostPtr   = (float *) malloc(2 * n * sizeof(yHostPtr[0]));
    if ((!yHostPtr)) {
        CLEANUP("Host malloc failed (vectors)");
        return 1;
    }
    // yHostPtr[0] = 10.0;
    for (i = 0; i < NET_SIZE; ++i) {
        yHostPtr[i] = totalstate[i];
    }

    cudaStat1 = cudaMalloc((void**)&cooRowIndex, nnz * sizeof(cooRowIndex[0]));
    cudaStat2 = cudaMalloc((void**)&cooColIndex, nnz * sizeof(cooColIndex[0]));
    cudaStat3 = cudaMalloc((void**)&cooVal,      nnz * sizeof(cooVal[0]));
    cudaStat4 = cudaMalloc((void**)&y,           2 * n * sizeof(y[0]));
    if ((cudaStat1 != cudaSuccess) ||
        (cudaStat2 != cudaSuccess) ||
        (cudaStat3 != cudaSuccess) ||
        (cudaStat4 != cudaSuccess)) {
        CLEANUP("Device malloc failed");
        return 1;
    }
    cudaStat1 = cudaMemcpy(cooRowIndex, cooRowIndexHostPtr,
                           (size_t)(nnz * sizeof(cooRowIndex[0])),
                           cudaMemcpyHostToDevice);
    cudaStat2 = cudaMemcpy(cooColIndex, cooColIndexHostPtr,
                           (size_t)(nnz * sizeof(cooColIndex[0])),
                           cudaMemcpyHostToDevice);
    cudaStat3 = cudaMemcpy(cooVal,      cooValHostPtr,
                           (size_t)(nnz * sizeof(cooVal[0])),
                           cudaMemcpyHostToDevice);
    cudaStat4 = cudaMemcpy(y,           yHostPtr,
                           (size_t)(2 * n * sizeof(y[0])),
                           cudaMemcpyHostToDevice);
    if ((cudaStat1 != cudaSuccess) ||
        (cudaStat2 != cudaSuccess) ||
        (cudaStat3 != cudaSuccess) ||
        (cudaStat4 != cudaSuccess)) {
        CLEANUP("Memory from Host to Device failed");
        return 1;
    }

    /* initialize cusparse library */
    status = cusparseCreate(&handle);
    if (status != CUSPARSE_STATUS_SUCCESS) {
        CLEANUP("CUSPARSE Library initialization failed");
        return 1;
    }

    /* create and setup matrix descriptor */
    status = cusparseCreateMatDescr(&descr);
    if (status != CUSPARSE_STATUS_SUCCESS) {
        CLEANUP("Matrix descriptor initialization failed");
        return 1;
    }
    cusparseSetMatType(descr, CUSPARSE_MATRIX_TYPE_GENERAL);
    cusparseSetMatIndexBase(descr, CUSPARSE_INDEX_BASE_ZERO);

    /* exercise conversion routines (convert matrix from COO to CSR format) */
    cudaStat1 = cudaMalloc((void**)&csrRowPtr, (n + 1) * sizeof(csrRowPtr[0]));
    if (cudaStat1 != cudaSuccess) {
        CLEANUP("Device malloc failed (csrRowPtr)");
        return 1;
    }
    status = cusparseXcoo2csr(handle, cooRowIndex, nnz, n,
                              csrRowPtr, CUSPARSE_INDEX_BASE_ZERO);
    if (status != CUSPARSE_STATUS_SUCCESS) {
        CLEANUP("Conversion from COO to CSR format failed");
        return 1;
    }

    // The following test only works for compute capability 1.3 and above
    // because it needs double precision.
    int devId;
    cudaDeviceProp prop;
    cudaError_t cudaStat;
    cudaStat = cudaGetDevice(&devId);
    if (cudaSuccess != cudaStat) {
        CLEANUP("cudaGetDevice failed");
        printf("Error: cudaStat %d, %s\n", cudaStat, cudaGetErrorString(cudaStat));
        return 1;
    }
    cudaStat = cudaGetDeviceProperties( &prop, devId) ;
    if (cudaSuccess != cudaStat){
        CLEANUP("cudaGetDeviceProperties failed");
        printf("Error: cudaStat %d, %s\n", cudaStat, cudaGetErrorString(cudaStat));
        return 1;
    }
    int cc = 100 * prop.major + 10 * prop.minor;
    if (cc < 130) {
        CLEANUP("waive the test because only sm13 and above are supported\n");
        printf("the device has compute capability %d\n", cc);
        printf("example test WAIVED");
        return 2;
    }

    /* exercise Level 2 routine (csrmv) */
    gettimeofday(&t1, NULL);
    for (i = 0; i < 100000; ++i) {
        //cudaMemcpy(y, yHostPtr, (size_t)(2 * n * sizeof(y[0])), cudaMemcpyHostToDevice);

        status = cusparseScsrmv(handle, CUSPARSE_OPERATION_NON_TRANSPOSE, n, n, nnz,
                            &fone, descr, cooVal, csrRowPtr, cooColIndex,
                            &y[0], &fzero, &y[n]);
    
        if (status != CUSPARSE_STATUS_SUCCESS) {
            CLEANUP("Matrix-vector multiplication failed");
            return 1;
        }

        //cudaMemcpy(yHostPtr, y, (size_t)(n * sizeof(y[0])), cudaMemcpyDeviceToHost);
    }
    gettimeofday(&t2, NULL);
    timersub(&t1, &t2, &tr);
    printf("Execute time: %.5f sec\n", -tr.tv_sec-(double)tr.tv_usec/1000000.0);

    /* destroy matrix descriptor */
    status = cusparseDestroyMatDescr(descr);
    descr = 0;
    if (status != CUSPARSE_STATUS_SUCCESS) {
        CLEANUP("Matrix descriptor destruction failed");
        return 1;
    }

    /* destroy handle */
    status = cusparseDestroy(handle);
    handle = 0;
    if (status != CUSPARSE_STATUS_SUCCESS) {
        CLEANUP("CUSPARSE Library release of resources failed");
        return 1;
    }

    //printf("ptr = %d\n", ptr);
}


