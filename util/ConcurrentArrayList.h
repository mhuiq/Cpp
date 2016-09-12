/*
 * ConcurrentArrayList.h
 *	线程安全的arrayList实现
 *  Created on: 2016/09/09
 *      Author: maihuiqiang
 */

#ifndef CONCURRENTARRAYLIST_H_
#define CONCURRENTARRAYLIST_H_

#include <pthread.h>

template <class T>
class ConcurrentArrayList {
private :
	const static int DEFAULT_CAPACITY = 16;	// 默认初始长度
	const static int DEFAULT_STEP = 16;		// 默认步进长度
	int capacity;	// list容量
	int length;		// list元素个数
	T *data;
	pthread_rwlock_t rwLock;
public :
	ConcurrentArrayList() {
		pthread_rwlock_init(&rwLock, NULL);
		capacity = 16;
		data = new T[capacity];
		length = 0;
	}
	ConcurrentArrayList(int size) {
		pthread_rwlock_init(&rwLock, NULL);
		length = 0;
		this->capacity = size;
		this->data = new T[capacity];
	}
	~ConcurrentArrayList() {
		delete []data;
	}

	/**
	 * 通过下标获取对应的元素
	 */
	T get(int i) {
		pthread_rwlock_rdlock(&rwLock);
		if (i >= this->length) {
			pthread_rwlock_unlock(&rwLock);
			throw "array index out of bound!";
		}
		T tmp = data[i];
		pthread_rwlock_unlock(&rwLock);
		return tmp;
	}

	/**
	 * 在list末尾添加元素
	 */
	void add(T t) {
		pthread_rwlock_wrlock(&rwLock);
		T buf = t;
		// 若长度不够，数组长度每次步进DEFAULT_STEP
		if (length + 1 > capacity) {
			capacity = capacity + DEFAULT_STEP;
			T *p = data;
			data = new T[capacity];
			for (int i=0; i<length; ++i) {
				data[i] = p[i];
			}
			delete []p;
		}
		data[length] = buf;
		length++;
		pthread_rwlock_unlock(&rwLock);
	}

	/**
	 * 返回list元素个数
	 */
	int size() {
		pthread_rwlock_rdlock(&rwLock);
		int tmp = length;
		pthread_rwlock_unlock(&rwLock);
		return tmp;
	}

	/**
	 * 根据下标删除元素
	 */
	void remove(int index) {
		pthread_rwlock_wrlock(&rwLock);
		if (index >= this->length) {
			pthread_rwlock_unlock(&rwLock);
			throw "array index out of bound!";
		}
		for (int i=index; i<length-1; ++i) {
			data[i] = data[i+1];
		}
		--length;
		pthread_rwlock_unlock(&rwLock);
	}
};



#endif /* CONCURRENTARRAYLIST_H_ */
