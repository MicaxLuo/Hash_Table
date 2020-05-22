
/*************************************************************/
/*                                                           */
/*    starter file                                           */
/*    													                             */
/*                                                           */
/*************************************************************/

#include <string>
#include <utility>
#include <functional>
#include "mylist.h"
using namespace std;

////////////////////////////////// TABLE //////////////////////////////////////////
template <class TYPE>

class Table{ // templated abstract base class
public:
	Table(){} // initialize to empty table
	virtual void update(const string& key, const TYPE& value)=0;
	virtual bool remove(const string& key)=0;
	virtual bool find(const string& key, TYPE& value)=0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
	virtual ~Table() {}
};

////////////////////////////////// SIMPLE TABLE //////////////////////////////////////////
// uses a sorted array, all key/value pairs are stored in sorted order
template <class TYPE>

class SimpleTable:public Table<TYPE>{
	// The way you store record (whether you make a template struct called Record with a data member for key and value,
	// or you store them separately in parallel structures is entirely up to you.
	struct Record{ // consists of key-value pairs
		TYPE data_; // any data supports =, instantiation without arguments
		string key_;
		Record(const string& key, const TYPE& data) {
			key_ = key;
			data_ = data;
		}

	};

	Record** records_;   //the table
	int capacity_;       //capacity of the array

	void grow() {
		Record** tmp = new Record*[capacity_ + 10];		//4
		for (int i = 0; i < capacity_; i++) {				//3+2n
			tmp[i] = records_[i];						//3n
		}
		for (int i = capacity_; i < capacity_ + 10; i++) {	//4+30
			tmp[i] = nullptr;							//20
		}
		delete[] records_;							//n?
		records_ = tmp;								//1
		capacity_ += 10;								//1
	}
public:
	SimpleTable(int capacity);
	SimpleTable(const SimpleTable& other); // copy constructor
	SimpleTable(SimpleTable&& other); // move constructor
	virtual void update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& other); //copy assign
	virtual const SimpleTable& operator=(SimpleTable&& other); // move assign
	virtual ~SimpleTable();

	virtual bool isEmpty() const{return numRecords()==0;} // returns true if the table is empty, false otherwise
	virtual int numRecords() const;
};

template <class TYPE>

int SimpleTable<TYPE>::numRecords() const{ // counts the number of records
	int rc=0;
	for(int i=0;records_[i]!=nullptr;i++){
		rc++;
	}
	return rc;									//1
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int capacity) : Table<TYPE>() {
	records_ = new Record*[capacity];
	capacity_ = capacity;
	for (int i = 0; i < capacity_; i++) {
		records_[i] = nullptr;
	}
}

template <class TYPE>

SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other) {// copy constructor
	records_ = new Record*[other.capacity_];
	capacity_ = other.capacity_;
	for (int i = 0; i < other.numRecords(); i++) {
		update(other.records_[i]->key_, other.records_[i]->data_);
	}
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other) {// move constructor
	capacity_ = other.capacity_;
	records_ = other.records_;
	other.records_ = nullptr;
	other.capacity_ = 0;
}

template <class TYPE>
void SimpleTable<TYPE>::update(const string& key, const TYPE& value) {
// is passed a key-value pair. If your table already has a record with a matching key, the record's value
	// is replaced by the value passed to this function. If no record exists, a record with key-value pair is added to the table.
	int idx = -1;							
	int size = numRecords();				
	for (int i = 0; i < size; i++) {			
		if (records_[i]->key_ == key) {	
			idx = i;						

		}
	}
	if (idx == -1) {						//1
		if (size == capacity_) {			//1
			grow();						//4+3+2n+3n+4+30+20+n+1+1
		}
		records_[size++] = new Record(key, value);					//3+
		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < size - 1 - i; j++) {
				if (records_[j]->key_ > records_[j + 1]->key_) {
					Record* tmp = records_[j];
					records_[j] = records_[j + 1];
					records_[j + 1] = tmp;
				}
			}
		}
	}
	else {
		records_[idx]->data_ = value;
	}

}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key) {
// If your table contains a record with a matching key, the record (both the key and the value) is removed from the table
	int idx = -1;
	for (int i = 0; i < numRecords(); i++) {
		if (records_[i]->key_ == key) {
			idx = i;
		}
	}
	if (idx != -1) {
		delete records_[idx];
		int size = numRecords();
		for (int i = idx; i < size - 1; i++) {
			records_[i] = records_[i + 1];
		}
		records_[size - 1] = nullptr;
		return true;
	}
	else {
		return false;
	}
}

template <class TYPE>

bool SimpleTable<TYPE>::find(const string& key, TYPE& value) {
// If your table contains a record with a matching key, the function returns true,
	// and passes back the value from the record. If it does not find a record with a matching key, function returns false.
	int idx = -1;
	for (int i = 0; i < numRecords(); i++) {
		if (records_[i]->key_ == key) {
			idx = i;

		}
	}
	if (idx == -1)
		return false;
	else {
		value = records_[idx]->data_;
		return true;
	}
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other) {
	if (this != &other) {
		if (records_) {
			int sz = numRecords();
			for (int i = 0; i < sz; i++) {

				remove(records_[0]->key_);
			}
			delete[] records_;
		}
		records_ = new Record*[other.capacity_];
		capacity_ = other.capacity_;
		int size = other.numRecords();
		for (int i = 0; i < size; i++) {
			update(other.records_[i]->key_, other.records_[i]->data_);
		}

	}
	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other) {
	swap(records_, other.records_);
	swap(capacity_, other.capacity_);

	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable() {
	if (records_) {
		int sz = numRecords();
		for (int i = 0; i < sz; i++) {
			remove(records_[0]->key_);
		}
		delete[] records_;
	}
}

////////////////////////////////// CHAINING TABLE //////////////////////////////////////////
//Part 3: Hash Table
template <class TYPE>
class ChainingTable :public Table<TYPE> {

	DList<TYPE>** records_;   //the table
	int capacity_;       //capacity of the array
	int numHold_;
	double loadFactor_;
	std::hash<string> hash_;
	int numGrow_;

	void grow() {
		DList<TYPE>** tmp = new DList<TYPE>*[capacity_ * 2];
		for (int i = 0; i < capacity_; i++) {
			tmp[i] = records_[i];
		}
		for (int i = capacity_; i < capacity_ * 2; i++) {
			tmp[i] = nullptr;
		}
		delete[] records_;
		records_ = tmp;
		capacity_ *= 2;
		numGrow_++;
	}
public:
	ChainingTable(int maxExpected, double maxLoadFactor);
	int capacity() const;
	ChainingTable(ChainingTable&& other);
	ChainingTable(const ChainingTable& other);
	virtual void update(const string& key, const TYPE& value);
	virtual bool remove(const string& key);
	virtual bool find(const string& key, TYPE& value);
	virtual bool isEmpty() const { return numRecords() == 0; }
	virtual int numRecords() const;
	virtual const ChainingTable& operator=(const ChainingTable& other);
	virtual const ChainingTable& operator=(ChainingTable&& other);

	virtual ~ChainingTable();
};

template <class TYPE>
int ChainingTable<TYPE>::numRecords() const {
	return numHold_;
}
template <class TYPE>
int ChainingTable<TYPE>::capacity() const {
	return capacity_;
}

template <class TYPE>
ChainingTable<TYPE>::ChainingTable(int capacity, double maxLoadFactor) : Table<TYPE>() {
	loadFactor_ = 1;
	capacity_ = 1;
	numHold_ = 0;
	numGrow_ = 0;
	if (maxLoadFactor > 0) {
		loadFactor_ = maxLoadFactor;
	}
	if (capacity > 0) {
		records_ = new DList<TYPE>*[capacity];
		capacity_ = capacity;
		for (int i = 0; i < capacity_; i++) {
			records_[i] = nullptr;
		}
	}
}
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other) {
	records_ = nullptr;
	*this = other;
}
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other) {
	records_ = nullptr;
	capacity_ = 0;
	*this = move(other);
}

template <class TYPE>
void ChainingTable<TYPE>::update(const string& key, const TYPE& value) {
	size_t hashKey = hash_(key);
	bool find = false;
	int capacity = capacity_;
	TYPE temp;
	size_t idx;
	for (int i = numGrow_; i >= 0 && !find; i--) {
		size_t tidx = hashKey % capacity;
		if (records_[tidx] != nullptr&&records_[tidx]->find(key, temp)) {
			find = true;
			idx = tidx;
		}
		else {
			capacity /= 2;
		}
	}

	if (find) {
		records_[idx]->update(key, value);
	}
	else {
		if ((double )numHold_ / capacity_== loadFactor_) {
			grow();
			//capacity = capacity_;
			//numGrow = numGrow_;
		}
		idx = hashKey % capacity_;
		if (records_[idx] == nullptr) {
			records_[idx] = new DList<TYPE>();
		}
		records_[idx]->push_back(value, key);
		numHold_++;
	}

}

template <class TYPE>
bool ChainingTable<TYPE>::remove(const string& key) {
	size_t hashKey = hash_(key);
	bool find = false;
	int capacity = capacity_;
	TYPE temp;
	size_t idx;
	for (int i = numGrow_; i >= 0 && !find; i--) {
		size_t tidx = hashKey % capacity;
		if (records_[tidx] != nullptr&&records_[tidx]->find(key, temp)) {
			find = true;
			idx = tidx;
		}
		else {
			capacity /= 2;
		}
	}

	if (find) {
		records_[idx]->drop(key);
	}
	return find;
}

template <class TYPE>
bool ChainingTable<TYPE>::find(const string& key, TYPE& value) {
	size_t hashKey = hash_(key);
	bool find = false;
	int capacity = capacity_;
	for (int i = numGrow_; i >= 0 && !find; i--) {
		size_t tidx = hashKey % capacity;
		if (records_[tidx] != nullptr&&records_[tidx]->find(key, value)) {
			find = true;
		}
		else {
			capacity /= 2;
		}
	}
	return find;
}
//not done
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other) {
	if (this != &other) {
		if (records_) {
			for (int i = 0; i < capacity_; i++) {
				if (records_[i] != nullptr) {
					delete records_[i];
				}
			}
			delete[] records_;
		}
		capacity_ = other.capacity_;
		numHold_ = other.numHold_;
		loadFactor_ = other.loadFactor_;
		numGrow_ = other.numGrow_;
		records_ = new DList<TYPE>*[capacity_];
		for (int i = 0; i < capacity_; i++) {
			if (other.records_[i] != nullptr) {
				records_[i] = new DList<TYPE>(*(other.records_[i]));
			}
			else {
				records_[i] = nullptr;
			}
		}
	}
	return *this;
}
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other) {
	if (this != &other) {
		swap(records_, other.records_);
		swap(capacity_, other.capacity_);
		numHold_ = other.numHold_;
		loadFactor_ = other.loadFactor_;
		numGrow_ = other.numGrow_;
	}
	return *this;
}

template <class TYPE>
ChainingTable<TYPE>::~ChainingTable() {
	for (int i = 0; i < capacity_; i++) {
		if (records_[i] != nullptr) {
			delete records_[i];
		}
	}
	delete[] records_;
}

//template <class TYPE>
//class ChainingTable:public Table<TYPE>{
//public:
//	ChainingTable(int maxExpected, double maxLoadFactor);
//	ChainingTable(const ChainingTable& other);
//	ChainingTable(ChainingTable&& other);
//	virtual void update(const string& key, const TYPE& value);
//	virtual bool remove(const string& key);
//	virtual bool find(const string& key, TYPE& value);
//	virtual const ChainingTable& operator=(const ChainingTable& other);
//	virtual const ChainingTable& operator=(ChainingTable&& other);
//	virtual ~ChainingTable();
//	virtual bool isEmpty() const {}
//	virtual int numRecords() const {}
//
//};
//template <class TYPE>
//ChainingTable<TYPE>::ChainingTable(int capacity,double maxLoadFactor): Table<TYPE>(){
//
//}
//
//template <class TYPE>
//ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other){
//
//}
//template <class TYPE>
//ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other){
//
//}
//template <class TYPE>
//void ChainingTable<TYPE>::update(const string& key, const TYPE& value){
//
//}
//
//template <class TYPE>
//bool ChainingTable<TYPE>::remove(const string& key){
//	return true;
//}
//
//template <class TYPE>
//bool ChainingTable<TYPE>::find(const string& key, TYPE& value){
//	return true;
//}
//
//template <class TYPE>
//const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other){
//	return *this;
//
//}
//template <class TYPE>
//const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other){
//	return *this;
//
//}
//template <class TYPE>
//ChainingTable<TYPE>::~ChainingTable(){
//
//}
//
//
