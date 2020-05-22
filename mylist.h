/**************************************************/
/*  Put your linked list code into this file      */
/**************************************************/
//Codes from lab4
#include <iostream>
#include <string>
using namespace std;
template <typename T>
class DList {
	struct Node {
		T data_;
		string key_;
		Node* next_;
		Node* prev_;
		Node(const T& data = T{}, const string& key="",Node* next = nullptr, Node* prev = nullptr) {
			data_ = data;
			key_ = key;
			next_ = next;
			prev_ = prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	DList() {
		front_ = nullptr;
		back_ = nullptr;
	}
	DList(const DList& other) {
		front_ = nullptr;
		back_ = nullptr;
		*this = other;
	}
	DList& operator=(const DList& other) {
		if (this != &other) {
			if (front_ != nullptr) {
				while (front_ != nullptr) {
					Node* dd = front_->next_;
					delete front_;
					front_ = dd;
				}
			}
			if (other.front_ == nullptr) {
				front_ = nullptr;
				back_ = nullptr;
			}
			else {
				for (Node* i = other.front_; i != nullptr; i = i->next_) {
					push_back(i->data_, i->key_);
				}
			}
		}
		return *this;
	}

	void push_front(const T& data, const string& key) {
		Node* nn = new Node(data, key, front_);
		if (front_ == nullptr) {
			back_ = nn;
		}
		else {
			front_->prev_ = nn;
		}
		front_ = nn;

	}
	void push_back(const T& data, const string& key) {
		Node* nn = new Node(data, key, nullptr, back_);
		if (front_ == nullptr) {
			front_ = nn;
		}
		else {
			back_->next_ = nn;
		}
		back_ = nn;
	}
	bool find(const string& key, Node*& tag) {
		bool find = false;
		Node* temp = front_;
		while (temp != nullptr && !find) {
			if (key == temp->key_) {
				find = true;
				tag = temp;
			}
			else {
				temp = temp->next_;
			}
		}
		return find;
	}
	bool find(const string& key, T& value) {
		bool find = false;
		Node* temp = front_;
		while (temp != nullptr && !find) {
			if (key == temp->key_) {
				find = true;
				value = temp->data_;
			}
			else {
				temp = temp->next_;
			}
		}
		return find;
	}
	bool update(const string& key, const T& value) {
		bool f = false;
		Node* temp = nullptr;
		f = find(key, temp);
		if (f) {
			temp->data_ = value;
		}
		return f;
	}
	void drop(const string& key) {
		Node* temp = nullptr;
		bool f = find(key, temp);
		if (f) {
			if (front_ == back_) {
				front_ = nullptr;
				back_ = nullptr;
			}
			else if (temp->next_ == nullptr) {
				back_ = temp->prev_;
				back_->next_ = nullptr;
			}
			else if (temp->prev_ == nullptr) {
				front_ = temp->next_;
				front_->prev_ = nullptr;
			}
			else {
				temp->next_->prev_ = temp->prev_;
				temp->prev_->next_ = temp->next_;
			}
			delete temp;
		}
	}

	~DList() {
		while (front_ != nullptr) {
			Node* dd = front_->next_;
			delete front_;
			front_ = dd;
		}

	}

	//iterators are effectively pointers wrapped inside 
	//an object
	class const_iterator {
		friend class DList;
	protected:
		Node* curr_;    //this is the node being referred to
		const DList* theList_;
		const_iterator(const DList* theList, Node* n) {
			this->curr_ = n;
			this->theList_ = theList;
		}
	public:
		const_iterator() {
			this->curr_ = nullptr;
			this->theList_ = nullptr;
		}
		const_iterator operator++() {
			//++x
			this->curr_ = this->curr_->next_;
			return *this;
		}
		const_iterator operator++(int) {
			//x++
			const_iterator rc = *this;
			this->curr_ = this->curr_->next_;
			return rc;
		}
		const_iterator operator--() {
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator n = *this;
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return n;
		}
		bool operator==(const_iterator rhs) {
			return this->curr_ == rhs.curr_&&this->theList_ == rhs.theList_;
		}
		bool operator!=(const_iterator rhs) {
			return this->curr_ != rhs.curr_;
		}
		const T& operator*()const {
			return this->curr_->data_;
		}
	};
	class iterator :public const_iterator {
		friend class DList;
		iterator(DList* theList_, Node* n) :
			const_iterator(theList_, n) {}
	public:
		iterator() :const_iterator() {};
		iterator operator++() {
			this->curr_ = this->curr_->next_;
			return *this;
		}
		iterator operator++(int) {
			iterator rc = *this;
			this->curr_ = this->curr_->next_;
			return rc;
		}
		iterator operator--() {
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return *this;
		}
		iterator operator--(int) {
			iterator n = *this;
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return n;
		}
		T& operator*() {
			return this->curr_->data_;
		}
		const T& operator*()const {
			return this->curr_->data_;
		}
	};

	//these belong to DList
	const_iterator cbegin() const {
		return const_iterator(this, front_);
	}
	iterator begin() {
		return iterator(this, front_);
	}
	const_iterator cend() const {
		return const_iterator(this, nullptr);
	}
	iterator end() {
		return iterator(this, nullptr);
	}
};

/*
template <typename T>
void DList<T>::push_front(const T& data,const string& key) {
	Node* nn = new Node(data, key,front_);
	if (front_ == nullptr) {
		back_ = nn;
	}
	else {
		front_->prev_ = nn;
	}
	front_ = nn;
}
template <typename T>
void DList<T>::push_back(const T& data, const string& key) {
	Node* nn = new Node(data, key, nullptr,back_);
	if (front_ == nullptr) {
		front_ = nn;
	}
	else {
		back_->next_ = nn;
	}
	back_ = nn;
}
template <typename T>
bool DList<T>::find(const string& key,Node*& tag) {
	bool find = false;
	Node* temp = front_;
	while (temp != nullptr && !find) {
		if (key == temp->key_) {
			find = true;
			tag = temp;
		}
		else {
			temp = temp->next_;
		}
	}
	return find;
}
template <typename T>
bool DList<T>::find(const string& key, T& value) {
	bool find = false;
	Node* temp = front_;
	while (temp != nullptr && !find) {
		if (key == temp->key_) {
			find = true;
			value = temp;
		}
		else {
			temp = temp->next_;
		}
	}
	return find;
}
template <typename T>
bool DList<T>::update(const string& key, const T& value) {
	bool find = false;
	Node* temp = nullptr;
	find = find(key, temp);
	if (find) {
		temp->data_ = value;
	}
	return find;
}

template <typename T>
void DList<T>::drop(const string& key) {
	Node* temp = nullptr;
	bool find = find(key,temp);
	if (find) {
		if (front_ = back_) {
			front_ = nullptr;
			back_ = nullptr;
		}
		else if (temp->next_ = nullptr) {
			back_ = temp->prev_;
			back_->next_ = nullptr;
		}
		else if (temp->prev_ = nullptr) {
			front_ = temp->next_;
			front_->prev_ = nullptr;
		}
		else{
			temp->next_->prev_ = temp->prev_;
			temp->prev_->next_ = temp->next_;
		}
		delete temp;
	}
}

template <typename T>
DList<T>::~DList() {
	while (front_ != nullptr) {
		Node* dd = front_->next_;
		delete front_;
		front_ = dd;
	}
}
*/


/*
template <typename T>
class Sentinel {
	struct Node {
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data = T{}, Node* next = nullptr, Node* prev = nullptr) {
			data_ = data;
			next_ = next;
			prev_ = prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	Sentinel() {
		front_ = new Node();
		back_ = new Node();
		front_->next_ = back_;
		back_->prev_ = front_;
	}
	void push_front(const T& data);
	~Sentinel();

	class const_iterator {
		friend class Sentinel;
	protected:
		Node* curr_;
		const Sentinel* theList_;
		const_iterator(const Sentinel* theList, Node* n) {
			this->curr_ = n;
			this->theList_ = theList;
		}
	public:
		const_iterator() {
			this->curr_ = nullptr;
			this->theList_ = nullptr;
		}
		const_iterator operator++() {
			this->curr_ = this->curr_->next_;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator rc = *this;
			this->curr_ = this->curr_->next_;
			return rc;
		}
		const_iterator operator--() {
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator n = *this;
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return n;
		}
		bool operator==(const_iterator rhs) {
			return this->curr_ == rhs.curr_&&this->theList_ == rhs.theList_;
		}
		bool operator!=(const_iterator rhs) {
			return this->curr_ != rhs.curr_;
		}
		const T& operator*()const {
			return this->curr_->data_;
		}
	};
	class iterator :public const_iterator {
		friend class Sentinel;
		iterator(Sentinel* theList_, Node* n) :
			const_iterator(theList_, n) {}
	public:
		iterator() :const_iterator() {};
		iterator operator++() {
			this->curr_ = this->curr_->next_;
			return *this;
		}
		iterator operator++(int) {
			iterator rc = *this;
			this->curr_ = this->curr_->next_;
			return rc;
		}
		iterator operator--() {
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return *this;
		}
		iterator operator--(int) {
			iterator n = *this;
			if (this->curr_ != nullptr) {
				this->curr_ = this->curr_->prev_;
			}
			else {
				this->curr_ = this->theList_->back_;
			}
			return n;
		}
		T& operator*() {
			return this->curr_->data_;
		}
		const T& operator*()const {
			return this->curr_->data_;
		}
	};
	const_iterator cbegin() const {
		return const_iterator(this, front_->next_);
	}
	iterator begin() {
		return iterator(this, front_->next_);
	}
	const_iterator cend() const {
		return const_iterator(this, back_);
	}
	iterator end() {
		return iterator(this, back_);
	}
};


template <typename T>
void Sentinel<T>::push_front(const T& data) {
	Node* nn = new Node(data, front_->next_, front_);
	front_->next_->prev_ = nn;
	front_->next_ = nn;
}


template <typename T>
Sentinel<T>::~Sentinel() {
	Node* d = nullptr;
	while (front_ != nullptr) {
		d = front_->next_;
		delete front_;
		front_ = d;
	}
}*/