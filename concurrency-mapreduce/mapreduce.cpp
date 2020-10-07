#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapreduce.h"
#include <pthread.h>
#include <vector>

using namespace std;
/*
// Different function pointer types used by MR
typedef char *(*Getter)(char *key, int partition_number);
typedef void (*Mapper)(char *file_name);
typedef void (*Reducer)(char *key, Getter get_func, int partition_number);
typedef unsigned long (*Partitioner)(char *key, int num_partitions);

// External functions: these are what you must define
void MR_Emit(char *key, char *value);

unsigned long MR_DefaultHashPartition(char *key, int num_partitions);

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition);
*/

typedef struct{
    char* fileName;
    void* mapperRet;
    int partition_number;
}mapArgs;

//Map()中调用，产生<key, value>键值对
Pair MR_Emit(char *key, char *value) {
    return Pair(key, value);
}

//哈希函数，将字符串转化为一个数字，相同的字符串会转化为相同的数字
unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}


void* mapperFunc(void* args) {
    //处理输入
    mapArgs* mArgs = (mapArgs *) args;
    char* fileName = mArgs->fileName;
    int partition_number = mArgs->partition_number;//partition_number get from args

    //执行Map()操作
    MapCollector mapCollector = Map(fileName);

    shuffling(MR_DefaultHashPartition, partition_number, mapCollector);
}

void* shuffling(Partitioner partition, int partition_number, MapCollector mapCollector) {
    for (int i = 0; i < mapCollector.size(); i++) {
        char *key = mapCollector.pairList[i].key;
        int pIdx = partition(key, partition_number);//将key转化为一个partition编号

    }
    
    //分区partition       相同的key在同一个部分 并且相同分区拷贝到同一个reduce
    //sort根据key排序
    //combine进行局部value的合并
}


void* reducerFunc(void* args) {
    
}



void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition) {
    
    pthread_t mappersId[num_mappers];
    mapArgs args[num_mappers];
    
    //为每个map创建一个线程
    for (int i = 0; i < num_mappers; i++) {//先假设num_mappers和文件数一样都为10(argc == 11)
        args[i].fileName = argv[i];
        args[i].partition_number = 10;//每个mapworker将最后的key-value分成partition_number个分区
        int err = pthread_create(&mappersId[i], NULL, mapperFunc, ((void*)&args[i]));
        if (!err) {//err == 0 is failed
            printf("create thread failed");
        }
    }
    
    //等待每个map，接受map的返回结果
    void* mapperRet[num_mappers] = {NULL};
    for (int i = 0; i < num_mappers; i++) {
        int err = pthread_join(mappersId[i], &mapperRet[i]);
        if (!err) printf("pthread_join failed");
    }

    pthread_t reducerId[num_reducers];
    for (int i = 0; i < num_reducers; i++) {
        int err = pthread_create(&reducerId[i], NULL, reducerFunc, NULL);
    }
    

}

MapCollector Map(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    assert(fp != NULL);

    MapCollector res;
    char *line = NULL;
    size_t size = 0;
    while (getline(&line, &size, fp) != -1) {
        char *token, *dummy = line;
        while ((token = strsep(&dummy, " \t\n\r")) != NULL) {
            res.add(MR_Emit(token, "1"));
        }
    }
    free(line);
    fclose(fp);
    return res;
}

void Reduce(char *key, Getter get_next, int partition_number) {
    int count = 0;
    char *value;
    while ((value = get_next(key, partition_number)) != NULL)
        count++;
    printf("%s %d\n", key, count);
}

int main(int argc, char *argv[]) {
    MR_Run(argc, argv, Map, 10, Reduce, 10, MR_DefaultHashPartition);
}