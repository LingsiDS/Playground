#ifndef __mapreduce_h__
#define __mapreduce_h__

class MapCollector{
public:
    vector<Pair> pairList;
    int size() {
        return pairList.size();
    }
    void add(Pair p) {
        pairList.push_back(p);
    }
};

class Pair{
public:
    char* key;
    char* value;
    Pair(char* k = NULL, char* v = NULL):key(k), value(v){//构造函数
    }
};

// Different function pointer types used by MR
typedef char *(*Getter)(char *key, int partition_number);
typedef void (*Mapper)(char *file_name);
typedef void (*Reducer)(char *key, Getter get_func, int partition_number);
typedef unsigned long (*Partitioner)(char *key, int num_partitions);

// External functions: these are what you must define
Pair MR_Emit(char *key, char *value);

unsigned long MR_DefaultHashPartition(char *key, int num_partitions);

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition);

#endif // __mapreduce_h__
