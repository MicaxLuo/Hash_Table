3 improvements to the code that will improve its efficiency.

## 1. Record Number

we can save the number of records into an integer variable instead of using numRecords(). numRecords()'s A(n) is O(n), but if we just update or get the integer variable, we only need one operation. This lowers I(n) from O(n) to O(1) in the following example:

``` C++
virtual bool isEmpty() const{
  // return numRecords()==0; // 1 + A(n) , which is 2 + 3n
  return numRecords_ == 0; // 1
}
```

## 2. Copy Constructor

In the copy constructor, we do not need to call update(), because update() has extra condition checks than we need here. As a copy constructor, it directly copies
from another object that already has a sorted array, so we can directly call the 2 argument constructor. After this change among with rumRecords() change, J(n) is O(n) rather than O(n^3).

``` C++
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& other){ // copy constructor
  // Car a;
  // Car b(a)
	records_=new Record*[other.capacity_]; // 1 + 1
	capacity_=other.capacity_; // 1
	for(int i=0;i<other.numRecords();i++){ // 1 + n(1 + A(n)) + n
		// update(other.records_[i]->key_,other.records_[i]->data_); // n(3.5n^2 + 14.5n + 9)
    records_[i] = new Record(other.records_[i]->key_,other.records_[i]->data_); // n(4 + T(n) for Record(), which is 2)
	}
}
```

## 3. update()

update() used bubble sort to sort the array of elements. It is unnecessary here because we are just inserting one element from a sorted array, not an unsorted one. With bubble sort, C(n) is O(n^2), with the new implementation, C(n) is O(n).

``` C++
template <class TYPE>
void SimpleTable<TYPE>::update(const string& key, const TYPE& value){
	int idx=-1; // 1
	int size=numRecords(); // 1 + A(n)
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
    /*
    // bubble sort ////////////////////////////
		for(int i=0;i<size-1;i++){ // 1 + n - 1 + n - 1
			for(int j=0;j<size-1-i;j++){ // (n- 1)1 + 3(n(n+1)/2)+ (n-1)1
				if(records_[j]->key_ > records_[j+1]->key_){ // n(n+1)/2
					Record* tmp=records_[j]; // n(n+1)/2
					records_[j]=records_[j+1]; // n(n+1)/2
					records_[j+1]=tmp; // n(n+1)/2
				}
			}
		} */

    // new implementation /////////////////////////
    for (i = size - 1; (i >= 0 && records_[i] > key); i--){ // 1 + 1 + 4n + n
      records_[i + 1] = records_[i]; // 4(n)
    }
    records_[i + 1]->key_ = key; // 3
    records_[i + 1]->data_ = value; // 3
	}
	else{
		records_[idx]->data_=value;
	}
}
```
