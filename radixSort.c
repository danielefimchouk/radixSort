#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RADIX 16
#define KEY_SIZE 32

union FloatInt 
{
    float f;
    uint32_t u;
};

union FloatInt findMax(float* arr, int n) 
{
    union FloatInt max_fi;
    for (int i = 0; i < n; i++) 
    {
        union FloatInt fi;
        fi.f = arr[i];
        fi.u = fi.u ^ (-(fi.u >> (KEY_SIZE - 1)) | 0x80000000);
        if (i == 0 || fi.u > max_fi.u)
            max_fi = fi;
    }
    return max_fi;
}

int maxIterations(union FloatInt max_fi) 
{
    int max_iterations = 0;
    while (max_fi.u > 0) 
    {
        max_fi.u >>= 4;
        max_iterations++;
    }
    return max_iterations;
}

void radix_sort(float* arr, int n) 
{
    float* buffer = (float*)malloc(sizeof(float) * n);
    float** buckets = (float**)malloc(sizeof(float*) * RADIX);
    float** bucket_ptrs = (float**)malloc(sizeof(float*) * RADIX);

    for (int i = 0; i < RADIX; i++) 
    {
        buckets[i] = (float*)malloc(sizeof(float) * n);
        bucket_ptrs[i] = buckets[i];
    }

    union FloatInt max_fi = findMax(arr, n);
    int max_iterations = maxIterations(max_fi);

    for (int pos = 0; pos < max_iterations * 4; pos += 4) 
    {
        for (int i = 0; i < n; i++) 
        {
            union FloatInt fi;
            fi.f = arr[i];
            fi.u = fi.u ^ (-(fi.u >> (KEY_SIZE - 1)) | 0x80000000);
            int index = ((fi.u >> pos) & 0xF);
            *bucket_ptrs[index]++ = arr[i];
        }

        float* temp_ptr = buffer;
        for (int i = 0; i < RADIX; i++) 
        {
            float* bucket_start = buckets[i];
            float* bucket_end = bucket_ptrs[i];
            while (bucket_start != bucket_end)
                *temp_ptr++ = *bucket_start++;
            bucket_ptrs[i] = buckets[i];
        }

        float* temp = arr;
        arr = buffer;
        buffer = temp;
    }

    free(buffer);
    for (int i = 0; i < RADIX; i++)
    {
        free(buckets[i]);
    }
    free(buckets);
    free(bucket_ptrs);
}

int main() 
{
    int count;
    scanf("%d", &count);

    float* arr = (float*)malloc(sizeof(float) * count);
    for (int i = 0; i < count; i++){scanf("%f", &arr[i]);}

    radix_sort(arr, count);

    for (int i = 0; i < count; i++){printf("%.2f\n", arr[i]);}

    free(arr);
    return 0;
}
