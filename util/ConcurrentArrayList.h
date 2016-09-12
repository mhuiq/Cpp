/*
 * ConcurrentArrayList.h
 *
 *  Created on: 2016年9月11日
 *      Author: 80374361
 */

#ifndef CONCURRENTARRAYLIST_H_
#define CONCURRENTARRAYLIST_H_

#include <pthread.h>

template <class T>
class ConcurrentArrayList {
private :
	int capacity;
	int length;
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
	~ConcurrentArrayList() {}

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

	void put(T t) {
		pthread_rwlock_wrlock(&rwLock);
		T buf = t;
		// 若数组长度不够，数组长度将线性增长，长度每次步进16
		if (length + 1 > capacity) {
			T *p = data;
			data = new T[capacity + 16];
			for (int i=0; i<length; ++i) {
				data[i] = p[i];
			}
		}
		data[length] = buf;
		length++;
		pthread_rwlock_unlock(&rwLock);
	}

	int size() {
		pthread_rwlock_rdlock(&rwLock);
		int tmp = length;
		pthread_rwlock_unlock(&rwLock);
		return tmp;
	}

	// TODO 待优化，若大量的remove会导致内存泄露
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
