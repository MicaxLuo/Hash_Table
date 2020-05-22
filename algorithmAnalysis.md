
let n represent the amount of data (number of key/value pairs) currently in the table

## 1. numRecords()

let A(n) represent the number of operations needed

``` C++
template <class TYPE>
int SimpleTable<TYPE>::numRecords() const{
	int rc=0; // 1
	for(int i=0;records_[i]!=nullptr;i++){ // 1 + n + n - 1
		rc++; // n
	}
	return rc; // 1
}
```
Therefore

A(n) = 1 + 1 + 2n - 1 + n + 1

A(n) = 2 + 3n

Thus,

A(n) is O(n)


## 2. update() - if item does not exists so you need to add it as a new record and you don't need to grow the array()

let B(n) represent the number of operations needed

``` C++
template <class TYPE>
void SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	// is passed a key-value pair.
  // if item does not exists so you need to add it as a new record and you don't need
	// to grow the array()
	int idx=-1; // 1
	int size=numRecords(); // 1 + A(n), which is 3n + 2
	for(int i=0;i<size;i++){ // 1 + n + n
		if(records_[i]->key_ == key){ // n + n
			idx=i;
		}
	}
	if(idx==-1){ // 1
		if(size == capacity_){ // 1
			grow();
		}
		records_[size++]=new Record(key,value); // 1 + 1 + 1 + T(n) for Record(), which is 2
		for(int i=0;i<size-1;i++){ // 1 + n - 1 + n - 1
			for(int j=0;j<size-1-i;j++){ // (n- 1)1 + 3(n(n+1)/2)+ (n-1)1
				if(records_[j]->key_ > records_[j+1]->key_){ // n(n+1)/2
					Record* tmp=records_[j]; // n(n+1)/2
					records_[j]=records_[j+1]; // n(n+1)/2
					records_[j+1]=tmp; // n(n+1)/2
				}
			}
		}
	}
	else{
		records_[idx]->data_=value;
	}
}
```
Therefore

B(n) = 1 + 1 + 3n + 2 + 1 + 4n + 2 + 3 + 2 + 1 + n - 1 + n - 1 +
			(n- 1)1 + 3(n(n+1)/2)+ (n-1)1 + (n(n+1)/2)4

B(n) = 5+ 7n + 6 + 2n + (n -1) + 3(n(n+1)/2)+ (n-1)1 + (n(n+1)/2)4

B(n) = 10 + 10n + (3/2)(n^2 + n) + n - 1 + (4/2)(n^2 + n)

B(n) = 9 + 11n + (7/2)(n^2 + n)

B(n) = 9 + 11n + 3.5n^2 + 3.5n

B(n) = 3.5n^2 + 14.5n + 9

Thus,

B(n) is O(n^2)


## 3. update() - if item does not exists so you need to add it as a new record and you need to grow the array()

let C(n) represent the number of operations needed

``` C++
template <class TYPE>
void SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	// is passed a key-value pair.
  // if item does not exists so you need to add it as a new record and you need to
	// grow the array()
	int idx=-1; // 1
	int size=numRecords(); // 1 + A(n), which is 3n + 2
	for(int i=0;i<size;i++){ // 1 + n + n
		if(records_[i]->key_ == key){ // n + n
			idx=i;
		}
	}
	if(idx==-1){ // 1
		if(size == capacity_){ // 1
			grow(); // T(n) for grow(), which is 37 + 4n
		}
		records_[size++]=new Record(key,value); // 1 + 1 + 1 + T(n) for Record(), which is 2
		for(int i=0;i<size-1;i++){ // 1 + n - 1 + n - 1
			for(int j=0;j<size-1-i;j++){ // (n- 1)1 + 3(n(n+1)/2)+ (n-1)1
				if(records_[j]->key_ > records_[j+1]->key_){ // n(n+1)/2
					Record* tmp=records_[j]; // n(n+1)/2
					records_[j]=records_[j+1]; // n(n+1)/2
					records_[j+1]=tmp; // n(n+1)/2
				}
			}
		}
	}
	else{
		records_[idx]->data_=value;
	}
}
```
Therefore

C(n) = 1 + 1 + 3n + 2 + 1 + 4n + 2 + 37 + 4n + 3 + 2 + 1 + n - 1 + n - 1 +
        (n- 1)1 + 3(n(n+1)/2)+ (n-1)1 + (n(n+1)/2)4

C(n) = 5+ 7n + 6 + 37 + 4n + 2n + (n -1) + 3(n(n+1)/2)+ (n-1)1 + (n(n+1)/2)4

C(n) = 10 + 10n + 37 + 4n + (3/2)(n^2 + n) + n - 1 + (4/2)(n^2 + n)

C(n) = 46 + 15n + (7/2)(n^2 + n)

C(n) = 46 + 15n + 3.5n^2 + 3.5n

C(n) = 3.5n^2 + 18.5n + 46

Thus,

C(n) is O(n^2)

## 4. update() - if item does exists and you are just modifying what is there

let D(n) represent the number of operations needed

``` C++
template <class TYPE>
void SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	// is passed a key-value pair.
  // if item does exists and you are just modifying what is there
	int idx=-1; // 1
	int size=numRecords(); // 1 + A(n)
	for(int i=0;i<size;i++){ // 1 + n + n
		if(records_[i]->key_ == key){ // n + n
			idx=i; // 1
		}
	}
	if(idx==-1){ // 1
		if(size == capacity_){
			grow();
		}
		records_[size++]=new Record(key,value);
		for(int i=0;i<size-1;i++){
			for(int j=0;j<size-1-i;j++){
				if(records_[j]->key_ > records_[j+1]->key_){
					Record* tmp=records_[j];
					records_[j]=records_[j+1];
					records_[j+1]=tmp;
				}
			}
		}
	}
	else{
		records_[idx]->data_=value; // 1 + 1
	}
}
```
Therefore

D(n) = 1 + 1 + 3n + 2 + 1 + 4n + 2 + 2

D(n) = 9 + 7n

Thus,

D(n) is O(n)

## 5. find() - if item is there

let E(n) represent the number of operations needed

``` C++
template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
	// If your table contains a record with a matching key, the function returns true,
	// and passes back the value from the record.
	int idx=-1; // 1
	for(int i=0;i<numRecords();i++){ // 1 + n(1 + A(n)) + n
		if(records_[i]->key_ == key){ // n + n
			idx=i; // 1
		}
	}
	if(idx==-1) // 1
		return false;
	else{
		value=records_[idx]->data_; // 1 + 1
		return true; // 1
	}
}
```
Therefore

E(n) = 1 + 1 + n(1 + (2 + 3n)) + n + n + n + 1 + 1 + 1 + 1

E(n) = 2 + n(3 + 3n) + 3n + 4

E(n) = 6n + 6 + 3n^2

Thus,

E(n) is O(n^2)

## 6. find() - if item is not there

let F(n) represent the number of operations needed

``` C++
template <class TYPE>
bool SimpleTable<TYPE>::find(const string& key, TYPE& value){
	// If your table contains a record with a matching key, the function returns true,
	// and passes back the value from the record. If it does not find a record with a matching key, function returns false.
	int idx=-1; // 1
	for(int i=0;i<numRecords();i++){ // 1 + n(1 + A(n)) + n
		if(records_[i]->key_ == key){ // n + n
			idx=i;
		}
	}
	if(idx==-1) // 1
		return false; // 1
	else{
		value=records_[idx]->data_;
		return true;
	}
}
```
Therefore

F(n) = 1 + 1 + n(1 + (2 + 3n)) + n + n + 1 + 1

F(n) = 2 + n(3 + 3n) + 2n + 2

F(n) = 5n + 4 + 3n^2

Thus,

F(n) is O(n^2)

## 7. remove() - if item is there

let G(n) represent the number of operations needed

``` C++
template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
	// If your table contains a record with a matching key, the record (both the key and the value) is removed from the table
	int idx=-1; // 1
	for(int i=0;i<numRecords();i++){ // 1 + n（1 + A(n)) + n
		if(records_[i]->key_ == key){ // n
			idx=i; // 1
		}
	}
	if(idx!=-1){ // 1
		delete records_[idx]; // 1
		int size=numRecords(); // 1 + A(n)
		for(int i=idx;i<size-1;i++){ // 1 + n - 1 + n - 1
			records_[i]=records_[i+1]; // (1 + 1 + 1 + 1)(n-1)
		}
		records_[size-1]=nullptr; // 1 + 1 + 1
		return true; // 1
	}
	else{
		return false;
	}
}
```
Therefore

G(n) = 1 + 1 + n(1 +(2 + 3n)) + n + n + 1 + 1 + 2 + （2 + 3n) +
			1 + n - 1 + n - 1 + (1 + 1 + 1 + 1)(n-1) + 4

G(n) = 2 + n(3 + 3n) + 2n + 4 + 2 + 3n + 2n - 1 + 4n

G(n) = 2 + 3n + 3n^2 + 5 + 9n

G(n) = 3n^2 + 12n + 7

Thus,

G(n) is O(n^2)

## 8. remove() - if item is not there

let H(n) represent the number of operations needed

``` C++
template <class TYPE>
bool SimpleTable<TYPE>::remove(const string& key){
	// If your table contains a record with a matching key, the record (both the key and the value) is removed from the table
	int idx=-1; // 1
	for(int i=0;i<numRecords();i++){ // 1 + n（1 + A(n) + n
		if(records_[i]->key_ == key){ // n
			idx=i; // 1
		}
	}
	if(idx!=-1){ // 1
		delete records_[idx];
		int size=numRecords();
		for(int i=idx;i<size-1;i++){
			records_[i]=records_[i+1];
		}
		records_[size-1]=nullptr;
		return true;
	}
	else{
		return false; // 1
	}
}
```
Therefore

H(n) = 1 + 1 + n(1 + (2 + 3n)) + n + n + 3

H(n) = 2 + 3n + 3n^2 + 2n + 3

H(n) = 3n^2 + 5n + 5

Thus,

H(n) is O(n^2)

## 9. isEmpty()

let I(n) represent the number of operations needed

``` C++
virtual bool isEmpty() const{
  return numRecords()==0; // 1 + A(n)
}
```
Therefore

I(n) = 3 + 3n

Thus,

I(n) is O(n)


## 10. copy constructor

let J(n) represent the number of operations needed

``` C++
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){
	records_=new Record*[other.capacity_]; // 1+ 1
	capacity_=other.capacity_; // 1
	for(int i=0;i<other.numRecords();i++){ // 1 + n(1 + A(n)) + n
		update(other.records_[i]->key_,other.records_[i]->data_); // n(B(n))
	}
}
```
Therefore

J(n) =1 + 1 + 1 + 1 + n(3 + 3n) + n + n(3.5n^2 + 14.5n + 9)

J(n) = 4 + 3n + 3.5n^3 + 17.5n^2 + 9n

J(n) = 3.5n^3 + 17.5n^2 + 12n + 4

Thus,

J(n) = O(n^3)

## 11. copy assignment operator

let K(n) represent the number of operations needed

``` C++
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& other){
	if(this!=&other){ // 1
		if(records_){ // 1
			int sz=numRecords(); // 1 + A(n)
			for(int i=0;i<sz;i++){ // 1 + n + n
				remove(records_[0]->key_); // n(G(n))
			}
			delete [] records_; // 1
		}
		records_=new Record*[other.capacity_]; // 1 + 1
		capacity_=other.capacity_; // 1
		int size=other.numRecords(); // 1 + A(n)
		for(int i=0;i<size;i++){ // 1 + n + n
			update(other.records_[i]->key_,other.records_[i]->data_); // n(B(n))
		}

	}
	return *this; // 1
}
```
Therefore

K(n) = 1 + 1 + 1 + (2 + 3n) + 1 + n + n + n(3n^2 + 12n + 7) + 1 + 1 + 1 + 1 +
			1 + (2 + 3n) + 1 + n + n + n(3.5n^2 + 14.5n + 9) + 1

K(n) = 3 + 2 + 3n + 1 + 2n + 3n^3 + 12n^2 + 7n + 4 + 3 + 3n + 1 + 2n + 3.5n^3 + 14.5n^2 + 9n + 1

K(n) = 6 + 5n + 3n^3 + 12n^2 + 10n + 7 + 1 + 2n + 3.5n^3 + 14.5n^2 + 9n + 1

K(n) = 15n + 12n^2 + 14 + 11n + 1 + 6.5n^3 + 14.5n^2

K(n) = 26n + 15 + 16.5n^2 + 6.5n^3

Thus,

K(n) is O(n^3)

## 12. move assignment operator

let L(n) represent the number of operations needed

``` C++
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& other){
	swap(records_,other.records_); // 1
	swap(capacity_,other.capacity_); // 1
	return *this; // 1
}
```
Therefore

L(n) = 1 + 1 + 1

L(n) = 3

Thus,

L(n) is O(1)

## 13. move constructor

let M(n) represent the number of operations needed

``` C++
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& other){
	capacity_=other.capacity_; // 1
	records_=other.records_; //1
	other.records_=nullptr; // 1
	other.capacity_=0; // 1
}
```
Therefore

M(n) = 1 + 1 + 1 + 1

M(n) = 4

Thus,

M(n) is O(1)

## 14. destructor

let P(n) represent the number of operations needed

``` C++
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
	if(records_){ // 1
		int sz=numRecords(); // 1 + A(n)
		for(int i=0;i<sz;i++){ // 1 + n + n
			remove(records_[0]->key_); // n(1 + G(n))
		}
		delete [] records_; // 1
	}
}
```
Therefore

P(n) = 1 + 1 + (2 + 3n) + 1 + n + n + n(1 + (15n + 9)) + 1

P(n) = 4 + 3n + 1 + 2n + n(15n + 10) + 1

P(n) = 5n + 5 + 1 + 15n^2 + 10n

P(n) = 15n^2 + 15n + 6

Thus,

P(n) is O(n^2)
