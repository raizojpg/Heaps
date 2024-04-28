
// Soare Alex Antonio 151
// Grigorascu Andrei Antonio 151
// Serbanescu George Florin 151

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm> // for default sort
#include <random>	// for random
#include <chrono>  // for time
#include<math.h>  // log
#include<limits>

std::ifstream fin("nr.txt");
std::ifstream in("tests.txt");
std::ofstream out("results.txt");
std::ofstream heapout("heapout.txt");



// vector with pairs ( N, MAX ) 
std::vector<std::pair<int, long long int>> tests;
long long int nr_of_tests, n;
long long int maxi;

class HeapBase {
public:
	virtual void test() = 0;
	virtual int getMin() = 0;
	virtual void insert(int value) = 0;
	virtual void pop() = 0;
	virtual bool empty() = 0;
};

namespace Heap {


	class Heap : public HeapBase {
	private:
		std::vector<int> heap;		// this is a min heap data structure
	public:
		Heap() : heap{ 0 } {}		// the indexing will start from 1 

		void swap(int& x, int& y) {
			int z = x;
			x = y;
			y = z;
		}

		void test() { std::cout << "Heap\n"; }

		int getMin() override {return heap[1];}

		void pop() override {				// returning and deleting the min element
			if (heap.size() != 1) {

				swap(heap[1], heap[heap.size() - 1]);		 // moving the first element to the last position
				heap.erase(heap.begin() + heap.size() - 1);  // deleting it

				long long ind = 1;
				long long son = 2;
				if (son < heap.size()) {
					if (son + 1 < heap.size() && heap[son] > heap[son + 1]) {
						son++;  // if there exist a second son that has a lower value we take him
					}
					while (heap[son] < heap[ind]) {
						swap(heap[son], heap[ind]);
						ind = son;
						son = ind * 2;
						if (son >= heap.size()) { break; }  // we reached the end
						if (son + 1 < heap.size() && heap[son] > heap[son + 1]) {
							son++;	// if there exist a second son that has a lower value we take him
						}
					}
				}
			}
		}

		void insert(int x) override  {
			long long ind = heap.size();
			heap.push_back(x);
			while (heap[ind] < heap[ind / 2] && ind > 1) {
				swap(heap[ind], heap[ind / 2]);
				ind /= 2;
			}
		}

		bool empty() override {
			if (heap.size() == 1) { return true; }
			else { return false; }
		}

	};

}

namespace Fibonacci {

	class List;

	class FibNode {
	public:
		int value;
		FibNode* parent;
		FibNode* left;
		FibNode* right;
		List* child;
		int degree; // size of child list
		bool marked;

		FibNode() {
			value = 0;
			parent = nullptr;
			left = nullptr;
			right = nullptr;
			child = nullptr;
			degree = 0;
			marked = false;
		}

		FibNode(int val) {
			FibNode();
			value = val;
		}

		//~FibNode() {delete child;}
	};

	class List {
	public:
		int size;
		FibNode* head;
		FibNode* tail;
		int max_degree;

		List() {
			size = 0;
			head = nullptr;
			tail = nullptr;
			max_degree = 0;
		}

		void setMaxDegree() {
			FibNode* ptr = head;
			while (ptr != nullptr) {
				if (ptr->degree > max_degree) { max_degree = ptr->degree; }
				ptr = ptr->right;
			}

		}

		void push_back(FibNode* node) {
			size++;
			if (tail == nullptr) {
				head = node;
				tail = node;
			}
			else {
				tail->right = node;
				node->left = tail;
				tail = node;
			}
			if (node->degree > max_degree) { max_degree = node->degree; }
		}

		void erase(FibNode* node) {
			if (node != nullptr) {
				size--;
				FibNode* prev = node->left;
				FibNode* next = node->right;
				if (prev != nullptr) { prev->right = next; }
				else { head = next; }
				if (next != nullptr) { next->left = prev; }
				else { tail = prev; }
				node->parent = nullptr;
				node->left = nullptr;
				node->right = nullptr;
				if (node->degree == max_degree) { setMaxDegree(); }
			}
		}

		void merge(List* other) {
			if (other != nullptr && other->head != nullptr) {
				size += other->size;
				if (tail != nullptr) { tail->right = other->head; }
				else { head = other->head; }
				other->head->left = tail;
				tail = other->tail;
				if (other->max_degree > max_degree) { max_degree = other->max_degree; }
			}
		}

		void clear() {
			while (head->right != nullptr) {
				size--;
				head = head->right;
				delete head->left;
			}
			size--;
			delete head;
			head = nullptr;
			tail = nullptr;
			max_degree = 0;
		}

		bool empty() {
			if (head == nullptr) { return true; }
			//if (size == 0) { return true; }
			return false;
		}

		void print() {
			if (head != nullptr) {
				FibNode* ptr = head;
				while (ptr != nullptr) {
					std::cout << ptr->value << " ";
					//ptr->child->print();
					ptr = ptr->right;
				}
				std::cout << std::endl;
			}
			else {
				//std::cout<<"Heap is empty\n";
			}
		}



	};

	class FibHeap : public HeapBase {
	public:
		List* roots;
		FibNode* min;

		FibHeap() { 
			roots = new List();
			min = nullptr; 
		}

		void test() { std::cout << "FIB\n"; }

		int getMin() override { return min->value; }

		void findMin() {
			min = roots->head;
			if (min != nullptr) {
				FibNode* ptr = roots->head->right;
				while (ptr != nullptr) {
					if (ptr->value < min->value) {
						min = ptr;
					}
					ptr = ptr->right;
				}
			}
		}

		void insert(int value) override {
			FibNode* ptr = new FibNode(value);
			roots->push_back(ptr);
			if (min == nullptr || value < min->value) { min = ptr; }
		}

		void merge(FibHeap* other)  {
			if (other != nullptr) {
				roots->merge(other->roots);
				if (other->min->value < min->value) { min = other->min; }
			}
		}

		void consolidate() {
			int size = std::max(roots->max_degree, roots->size);
			std::vector<FibNode*> freq(size + 1, 0);
			FibNode* node = roots->head;
			min = node;
			while (node != nullptr) {
				//std::cout << node->value << " ";
				if (freq[node->degree] == nullptr || freq[node->degree] == node) {
					if (node->value <= min->value) { min = node; }
					freq[node->degree] = node;
					node->parent = nullptr;
					node = node->right;
				}
				else {
					FibNode* other = freq[node->degree];
					if (node->value > other->value) {
						FibNode* aux = other;
						other = node;
						node = aux;
					}
					roots->erase(other);
					if (node->child == nullptr) { node->child = new List(); }
					node->child->push_back(other);
					other->parent = node;
					freq[node->degree] = nullptr;
					node->degree++;
					if (node->degree > roots->max_degree) { roots->max_degree = node->degree; }
				}
			}
			/*
			for (int i = 0; i < freq.size(); i++) {
				std::cout << freq[i] << " ";
			}
			std::cout << std::endl;
			*/
		}

		void pop() override  {
			if (min != nullptr) {
				roots->erase(min);
				roots->merge(min->child);
				min->child = nullptr;
				delete min;
				consolidate();
			}
		}

		/*
		void decreaseKey(FibNode* node, int value) {
			node->value = value;
			if (min == nullptr || value < min->value) { min = node; }
			FibNode* parent = node->parent;
			if (parent != nullptr && parent->value > node->value) {
				node->parent = nullptr;
				roots.push_back(node);
				//parent->child.remove(node);
				while (parent->marked && parent != nullptr) {
					roots.push_back(parent);
					FibNode* aux = parent;
					parent = parent->parent;
					aux->parent = nullptr;
				}
				if (parent != nullptr) {
					parent->marked = true;
				}
			}
		}
		*/

		bool empty() override { return roots->empty(); }

		void print() { roots->print(); }

		~FibHeap() {
			delete roots;
		}
	};


}

namespace old {
	class FibNode {
	public:
		int value;
		FibNode* parent;
		FibNode* left;
		FibNode* right;
		std::vector<FibNode*> child;
		int degree; // size of child list
		bool marked;

		FibNode() {
			value = 0;
			parent = nullptr;
			left = nullptr;
			right = nullptr;
			degree = 0;
			marked = false;
		}

		FibNode(int val) {
			FibNode();
			value = val;
		}
	};

	class List {
	public:
		int max_degree;
		int size;
		FibNode* head;
		FibNode* tail;

		List() {
			max_degree = 0;
			size = 0;
			head = nullptr;
			tail = nullptr;
		}

		void setMaxDegree() {
			FibNode* ptr = head;
			while (ptr != nullptr) {
				if (ptr->degree > max_degree) { max_degree = ptr->degree; }
				ptr = ptr->right;
			}

		}

		void push_back(FibNode* node) {
			size++;
			if (tail == nullptr) {
				head = node;
				tail = node;
			}
			else {
				tail->right = node;
				node->left = tail;
				tail = node;
			}
			if (node->degree > max_degree) { max_degree = node->degree; }
		}

		void erase(FibNode* node) {
			if (node != nullptr) {
				size--;
				FibNode* prev = node->left;
				FibNode* next = node->right;
				if (prev != nullptr) { prev->right = next; }
				else { head = next; }
				if (next != nullptr) { next->left = prev; }
				else { tail = prev; }
				node->parent = nullptr;
				node->left = nullptr;
				node->right = nullptr;
				if (node->degree == max_degree) { setMaxDegree(); }
			}
		}

		bool empty() {
			if (head == nullptr) { return true; }
			//if (size == 0) { return true; }
			return false;
		}

		void print() {
			if (head != nullptr) {
				FibNode* ptr = head;
				while (ptr != nullptr) {
					std::cout << ptr->value << " ";
					//ptr->child->print();
					ptr = ptr->right;
				}
				std::cout << std::endl;
			}
			else {
				//std::cout<<"Heap is empty\n";
			}
		}
	};

	class FibHeap {
	public:
		List roots;
		FibNode* min;

		FibHeap() { min = nullptr; }

		int getMin() { return min->value; }

		void findMin() {
			min = roots.head;
			if (min != nullptr) {
				FibNode* ptr = roots.head->right;
				while (ptr != nullptr) {
					if (ptr->value < min->value) {
						min = ptr;
					}
					ptr = ptr->right;
				}
			}
		}

		void insert(int value) {
			FibNode* ptr = new FibNode(value);
			roots.push_back(ptr);
			if (min == nullptr || value < min->value) { min = ptr; }
		}

		void merge(FibHeap* other) {
			if (other != nullptr) {
				roots.size += other->roots.size;
				roots.tail->right = other->roots.head;
				other->roots.head->left = roots.tail;
				roots.tail = other->roots.tail;
				if (other->min->value < min->value) { min = other->min; }
			}
		}

		void consolidate() {
			int size = std::max(roots.max_degree, roots.size);
			std::vector<FibNode*> freq(size + 1, 0);
			FibNode* node = roots.head;
			min = node;
			while (node != nullptr) {
				//std::cout << node->value << " ";
				if (freq[node->degree] == nullptr || freq[node->degree] == node) {
					if (node->value < min->value) { min = node; }
					freq[node->degree] = node;
					node = node->right;
				}
				else {
					FibNode* other = freq[node->degree];
					if (node->value < other->value) {
						roots.erase(other);
						node->child.push_back(other);
						other->parent = node;
					}
					else {
						roots.erase(node);
						other->child.push_back(node);
						node->parent = other;
						node = other;
					}
					freq[node->degree] = nullptr;
					node->degree++;
					if (node->degree > roots.max_degree) { roots.max_degree = node->degree; }
				}
			}
			/*
			for (int i = 0; i < freq.size(); i++) {
				std::cout << freq[i] << " ";
			}
			std::cout << std::endl;
			*/
		}

		void pop() {
			if (min != nullptr) {
				for (FibNode* child : min->child) {
					roots.push_back(child);
				}
				roots.erase(min);
				delete min;
				//findMin();
				consolidate();
				//findMin(); //cred ca asta e irelevant
			}
		}

		void decreaseKey(FibNode* node, int value) {
			node->value = value;
			if (min == nullptr || value < min->value) { min = node; }
			FibNode* parent = node->parent;
			if (parent != nullptr && parent->value > node->value) {
				node->parent = nullptr;
				roots.push_back(node);
				//parent->child.remove(node);
				while (parent->marked && parent != nullptr) {
					roots.push_back(parent);
					FibNode* aux = parent;
					parent = parent->parent;
					aux->parent = nullptr;
				}
				if (parent != nullptr) {
					parent->marked = true;
				}
			}
		}

		bool empty() { return roots.empty(); }

		void print() {
			roots.print();
		}

		void clear() {
			FibNode* ptr = roots.head->right;
			while (ptr != nullptr) {
				std::cout << "Deleting " << ptr->left->value << "\n";
				delete ptr->left;
				ptr = ptr->right;
			}
			std::cout << "Deleting " << roots.tail->value << "\n";
			delete roots.tail;
			roots.head = nullptr;
			roots.tail = nullptr;
		}

	};


}

namespace Heap23{
	
	class Node {
		friend class TwoThreeHeap;
		friend int mergeNodes(Node** a, Node** b);

	public:
		Node(int key, int value) :
			child(NULL),
			left(NULL),
			right(NULL),
			parent(NULL),
			partner(NULL),
			extra(NULL),
			dim(0), // nr of children
			key_(key),
			value_(value) {}

		int priority() { return key_; }
		int value() { return value_; }

		void addChild(Node* node) {
			if (child) {
				left = child;
				right = left->right; // right sibling of the left node
				node->left = left;
				node->right = right;
				right->left = node;
				left->right = node;
			}
			else {
				node->left = node;
				node->right = node;
			}
			child = node;
			node->parent = this;
		}

		void removeChild(Node* node) {
			if (node->dim) { // we have at least one child
				left = node->left;
				right = node->right;
				left->right = right;
				right->left = left;
				child = left;
			}
			else {
				child = NULL;
			}
		}

	private:
		Node* parent;
		Node* child;
		Node* left;
		Node* right;
		Node* partner;
		bool extra;
		int dim;
		int key_;
		int value_;
	};

	int mergeNodes(Node** a, Node** b) {
		Node* tree;
		Node* nextTree;
		Node* other;
		Node* nextOther;
		int c = 0;

		if ((*a)->priority() <= (*b)->priority()) {
			tree = (*a);
			other = (*b);
		}
		else {
			tree = (*b);
			other = (*a);
		}
		c++;

		nextTree = tree->partner;
		nextOther = other->partner;

		if (!nextTree) {
			if (nextOther) {
				tree->addChild(other);
				tree->dim++;
				(*a) = NULL;
				(*b) = tree;
			}
			else {
				tree->partner = other;
				other->partner = tree;
				other->extra = true;

				(*a) = tree;
				(*b) = NULL;
			}
		}
		else if (!nextOther) {
			tree->partner = NULL;
			other->partner = nextTree;
			nextTree->partner = other;

			if (other->priority() < nextTree->priority()) {
				tree->addChild(other);
			}
			else {
				nextTree->extra = false;
				other->extra = true;
				tree->addChild(nextTree);
			}

			(tree->dim)++;
			c++;

			(*a) = NULL;
			(*b) = tree;
		}
		else {
			tree->partner = NULL;
			nextTree->partner = NULL;
			nextTree->extra = false;
			nextTree->left = nextTree;
			nextTree->right = nextTree;
			nextTree->parent = NULL;

			tree->addChild(other);
			(tree->dim)++;

			(*a) = nextTree;
			(*b) = tree;
		}
		return c;
	}


	class TwoThreeHeap : public HeapBase {
	public:
		TwoThreeHeap(int max_nodes = 100) : nrNodes(0), value(0) {

			max_trees_ = (int)(0.5 + std::log(max_nodes + 1) / std::log(2.0));
			trees = new Node * [max_trees_];

			for (int i = 0; i < max_trees_; ++i) {
				trees[i] = NULL;
			}

		}

		~TwoThreeHeap() {
			delete[] trees;
		}

		void test() { std::cout << "23\n"; }

		void insert(int value) override {
			int key = value;
			Node* node = new Node(key, value);
			meld(node); // Meld the new node into the heap
			nrNodes++; // Increment the number of nodes in the heap
			//return node; // Return a pointer to the inserted node

		}

		Node* mini() {
			Node* minNode;
			Node* next;
			long key1, key2;
			int rank, v;

			v = value;
			rank = -1;

			while (v) {
				v = v >> 1;
				rank++;
			}

			minNode = trees[rank];
			key1 = minNode->priority();
			while (rank > 0) {
				rank--;
				next = trees[rank];
				if (next) {
					key2 = next->priority();
					if (key2 < key1) {
						key1 = key2;
						minNode = next;
					}
				}
			}
			return minNode;
		}

		int getMin() override  {
			return this->mini()->value();
		}

		bool empty() override {
			return nrNodes == 0;
		}

		void pop() override  {
			Node* child;
			Node* next;
			Node* partner;
			Node* minNode;
			minNode = mini();

			int rank = minNode->dim; // nr of children of minNode

			partner = minNode->partner;
			if (partner) {
				partner->partner = NULL;
				partner->extra = false;
				partner->parent = NULL;
				partner->left = partner;
				partner->right = partner;
				trees[rank] = partner;
			}
			else {
				trees[rank] = NULL;
				value -= (1 << rank);
			}
			nrNodes--;

			child = minNode->child;
			if (child) {
				next = child->right;
				next->left = NULL;
				child->right = NULL;
				meld(next);
			}
			//return minNode;
		}

		bool merge(TwoThreeHeap& heap) {
			for (int i = 0; i < heap.max_trees_; ++i) {
				if (heap.trees[i] != NULL) {
					heap.trees[i]->right = NULL; // disconnect the three from the heap
					heap.trees[i]->left = NULL;
					meld(heap.trees[i]);
					heap.trees[i] = NULL;
				}
			}
			nrNodes += heap.nrNodes;
			heap.nrNodes = 0;
			return true; // the merge was successful
		}

	protected:
		void meld(Node* list) {
			Node* next;
			Node* add_tree;
			Node* carry_tree;
			int d;

			next = NULL;
			add_tree = list;
			carry_tree = NULL;

			do {
				if (add_tree) {
					next = add_tree->right;
					add_tree->right = add_tree->left = add_tree;
					add_tree->parent = NULL;
				}
				else {
					add_tree = carry_tree;
					carry_tree = NULL;
				}

				if (carry_tree) {
					mergeNodes(&add_tree, &carry_tree);
				}

				if (add_tree) {
					d = add_tree->dim;
					if (trees[d]) {
						mergeNodes(&trees[d], &add_tree);
						if (!trees[d]) value -= (1 << d);
						carry_tree = add_tree;
					}
					else {
						trees[d] = add_tree;
						value += (1 << d);
					}
				}

				add_tree = next;
			} while (add_tree || carry_tree);
		}

	private:
		Node** trees;
		int max_trees_;
		int nrNodes;
		int value;


	};




}


template <typename Type>
void print(std::vector<Type>& vec) {
	for (int j = 0; j < vec.size(); j++) {
		heapout << vec[j] << " ";
	}
	heapout << std::endl;
}

std::vector<int> generateRandomI(unsigned long long n, int maxi) {
	int x;
	std::vector<int> numbers;
	std::random_device rd;  // Seed for the random number generator
	std::mt19937 rng(rd()); // Mersenne Twister engine
	std::uniform_int_distribution<int> distribution((-1) * maxi - 1, maxi + 1); // Generator for numbers between -maxi and max
	for (unsigned long long j = 0; j < n; j++) {
		x = distribution(rng);
		numbers.push_back(x);
	}
	return numbers;
}
std::vector<long long> generateRandomLL(unsigned long long n, long long maxi) {
	long long x;
	std::vector<long long> numbers;
	std::random_device rd;  // Seed for the random number generator
	std::mt19937 rng(rd()); // Mersenne Twister engine
	std::uniform_int_distribution<long long> distribution((-1) * maxi - 1, maxi + 1); // Generator for numbers between -maxi and max
	for (unsigned long long j = 0; j < n; j++) {
		x = distribution(rng);
		numbers.push_back(x);
	}
	return numbers;
}

bool testing2(std::vector<int>& heap, std::vector<int>& other, std::vector<int>& nr) {
	//heapout << "\n\n\n\n\n";
	if (heap.size() != other.size()) {
		heapout << "Size-ul trebuia sa fie " << heap.size() << " dar este " << other.size() << std::endl;
		print(nr);
		print(heap);
		print(other);
		return false;
	}
	for (unsigned long long int i = 0; i < heap.size(); i++) {
		if (heap[i] != other[i]) {
			heapout << "Trebuia sa fie " << heap[i] << " dar este " << other[i];
			return false;
		}
		//heapout << other[i] << " ";
	}
	return true;
}

bool testingAll(std::vector<int>& heap, std::vector<int>& other1, std::vector<int>& other2, std::vector<int>& other3) {
	if (heap.size() != other1.size() or heap.size() != other2.size()) {
		heapout << "Size-ul trebuia sa fie " << heap.size() << std::endl;
		return false;
	}
	for (unsigned long long int i = 0; i < heap.size(); i++) {
		if (heap[i] != other1[i] or heap[i] != other2[i]) {
			return false;
		}
	}
	return true;
}


void probe_1(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		for (auto& nr : numbers) {
			heap.insert(nr);
		}
		while (!heap.empty()) {
			output_heap.push_back(heap.getMin());
			heap.pop();
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out << name <<" time: " << duration.count() << " miliseconds\n";

	}
	catch (const std::bad_alloc& e) {
		out << "\tyou don't have enough memory" << std::endl;
	}
	catch (...) {
		out << "\tnope" << std::endl;
	}
}

void probe_2(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		for (auto& nr : numbers) {
			heap.insert(nr);
			output_heap.push_back(heap.getMin());
			heap.pop();
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out << name << " time: " << duration.count() << " miliseconds\n";

	}
	catch (const std::bad_alloc& e) {
		out << "\tyou don't have enough memory" << std::endl;
	}
	catch (...) {
		out << "\tnope" << std::endl;
	}
}

void probe_3(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		for (unsigned long long int i = 0; i < numbers.size(); i++) {
			heap.insert(numbers[i]);
			output_heap.push_back(heap.getMin());
			if (i % 10 == 1) {
				heap.pop();
			}
		}
		while (!heap.empty()) {
			output_heap.push_back(heap.getMin());
			heap.pop();
		}

		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out << name << " time: " << duration.count() << " miliseconds\n";

	}
	catch (const std::bad_alloc& e) {
		out << "\tyou don't have enough memory" << std::endl;
	}
	catch (...) {
		out << "\tnope" << std::endl;
	}
}



// we take a function pointer to a probe as an argument
void init_probe(void (*probe)(std::string, std::vector<int>&, std::vector<int>&, HeapBase&), std::vector<int>& numbers) {
	std::vector<int> output_heap, output_fibheap, output_heap23, output_heap233;
	Heap::Heap heap;
	Fibonacci::FibHeap fibheap;
	Heap23::TwoThreeHeap heap23;

	probe("Heap   ", numbers, output_heap, heap);
	probe("FibHeap", numbers, output_fibheap, fibheap);
	//robe("23Heap ", numbers, output_heap23, heap23);

	//if (!testingAll(output_heap, output_fibheap, output_heap23, output_heap233)) { out << "WRONG\n"; }
	if (!testing2(output_heap, output_fibheap, numbers)) { out << "WRONG\n"; }
	else { out << "Successful\n"; }
}



void compareHeaps(unsigned long long int n, int maxi) {
	// generate random numbers
	std::vector<int> numbers = generateRandomI(n, maxi); // will be random numbers
	
	out << "\nProbe 1\n";
	init_probe(probe_1, numbers); 
	out << "\nProbe 2\n";
	init_probe(probe_2, numbers);
	out << "\nProbe 3\n";
	init_probe(probe_3, numbers);
}


int main() {

	/*
	Heap23::TwoThreeHeap heap;
	std::vector<int> numbers = generateRandomI(100000, 10000);
	for (auto& nr : numbers) {
		heap.insert(nr);
	}
	while (!heap.empty()) {
		heap.pop();
	}

	return 0;
	*/

	//intra in roots noduri fara vecini si cu parinti nush dc
	
	/*
	Fibonacci::FibHeap fib;
	int x;
	while (fin >> x) {
		fib.insert(x);
	}
	int ct = 0;
	while (!fib.empty()) {
		ct++;
		std::cout << fib.getMin()<<" => "<<ct << "\n\n";
		fib.pop();
		fib.print();

	}

	return 0;
	*/

	unsigned long long int n;
	int maxi;

	srand(time(nullptr)); // seed for random number generator
	//try opening the file
	if (in.is_open()) {
		in >> nr_of_tests;
		for (int i = 0; i < nr_of_tests; i++) {
			in >> n >> maxi;
			out << std::endl << " --- TEST NR: " << i << " \tN = " << n << "\tMAX = " << maxi << std::endl;
			try {
				if (0 < n && n < INT64_MAX - 2) {
					if (0 < maxi && maxi < INT32_MAX - 2) {

						out << "\n ---  Int Values --- \n";
						compareHeaps(n, maxi);
						out << "\n ------------------------------\n";

					}
					else if (0 < maxi && maxi < INT64_MAX - 2) {

						out << "\n --- Long Long Values --- \n";
						
						out << "\n ------------------------------\n";

					}
					else {
						throw 1; // we throw an exception 
					}
				}
				else {
					throw 1; // we throw an exception
				}
			}
			catch (...) {
				out << "We could not process this input :( \n";
			}
		}
		in.close();
	}
	else {
		out << "Could not open the file\n";
	}
	return 0;

}