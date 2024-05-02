
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
#include <math.h>  // log
#include <limits>

std::ifstream fin("nr.txt");
std::ifstream in("tests.txt");
std::ofstream out("results5.txt");
std::ofstream heapout("heapout.txt");


// vector with pairs ( N, MAX ) 
std::vector<std::pair<int, long long int>> tests;
long long int nr_of_tests;

class HeapBase {
public:
	virtual void test() = 0;
	virtual int getMin() = 0;
	virtual void insert(int value) = 0;
	virtual void pop() = 0;
	virtual bool empty() = 0;
	virtual ~HeapBase() = default;
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

		void merge(Heap& other) {
			while (!other.empty()) {
				this->insert(other.getMin());
				other.pop();
			}
		}

		~Heap() override = default;

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
				if (node->degree == max_degree) { 
					//setMaxDegree(); 
					max_degree--;
				}
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
					ptr = ptr->right;
				}
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
				if (this->min == nullptr || other->min->value < min->value) { min = other->min; }
			}
		}

		void consolidate() {
			int size = std::max(roots->max_degree + 1, roots->size);
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

		void decreaseKey(FibNode* node, int value) {
			node->value = value;
			if (min == nullptr || value < min->value) { min = node; }
			FibNode* parent = node->parent;
			if (parent != nullptr && parent->value > node->value) {
				node->parent = nullptr;
				parent->child->erase(node);
				roots->push_back(node);
				while (parent != nullptr && parent->marked) {
					node = parent;
					parent = node->parent;
					node->parent = nullptr;
					parent->child->erase(node);
					roots->push_back(node);
				}
				if (parent != nullptr && parent->parent != nullptr) {
					parent->marked = true;
				}
			}
		}
		
		bool empty() override { return roots->empty(); }

		void print() {
			FibNode* ptr = roots->head;
			while (ptr != nullptr) {
				std::cout << "\nRoot\t\t   " << ptr->value << "\n";
				if (ptr->child != nullptr) {
					std::cout << "\t\t";
					ptr->child->print();
					std::cout << "\n\t    ";
					FibNode* child_ptr = ptr->child->head;
					while (child_ptr != nullptr) {
						if (child_ptr->child != nullptr) {
							child_ptr->child->print();
						}
						std::cout << "  \t  ";
						child_ptr = child_ptr->right;
					}
					std::cout << "\n";
				}
				ptr = ptr->right;
			}
		}

		void print_bfs() {
			int lvl;
			std::queue<std::pair<FibNode*, int>> q;
			FibNode* ptr = roots->head;
			std::cout << "\nRoots : ";
			while (ptr != nullptr) {
				std::cout << ptr->value << " ";
				q.push({ ptr, 1 });
				ptr = ptr->right;
			}
			while (!q.empty()) {
				ptr = q.front().first;
				lvl = q.front().second;
				q.pop();
				if (ptr->child != nullptr) {
					std::cout << "\nLvl " << lvl << " parent " << ptr->value << " : ";
					FibNode* ptr_child = ptr->child->head;
					while (ptr_child != nullptr) {
						std::cout << ptr_child->value << " ";
						q.push({ ptr_child,lvl + 1 });
						ptr_child = ptr_child->right;
					}
				}
			}
			std::cout << "\n";
		}

		~FibHeap() override {delete roots;}
	};

	void tests() {
		FibHeap fib;
		int x;
		while (fin >> x) {
			fib.insert(x);
		}
		int ct = 0;
		while (!fib.empty()) {
			ct++;
			std::cout << fib.getMin()<<" => "<<ct << "\n\n";
			fib.pop();
			fib.print_bfs();

			if (ct == 2) {
				fib.decreaseKey(fib.roots->head->child->head->right->right->child->head, -30);
				fib.decreaseKey(fib.roots->head->child->head->right->right->child->head, -20);
				fib.print_bfs();
				while (true) {
					
					
				}
			}
		}
	}

}

namespace Heap23{

	class Node {
		friend class TwoThreeHeap;

	public:
		Node(int key, int value) :
			child(nullptr),
			left(nullptr),
			right(nullptr),
			parent(nullptr),
			partner(nullptr),
			extra(nullptr),
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

		static int mergeNodes(Node** a, Node** b) {
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
					(*a) = nullptr;
					(*b) = tree;
				}
				else {
					tree->partner = other;
					other->partner = tree;
					other->extra = true;

					(*a) = tree;
					(*b) = nullptr;
				}
			}
			else if (!nextOther) {
				tree->partner = nullptr;
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

				(*a) = nullptr;
				(*b) = tree;
			}
			else {
				tree->partner = nullptr;
				nextTree->partner = nullptr;
				nextTree->extra = false;
				nextTree->left = nextTree;
				nextTree->right = nextTree;
				nextTree->parent = nullptr;

				tree->addChild(other);
				(tree->dim)++;

				(*a) = nextTree;
				(*b) = tree;
			}
			return c;
		}

		void removeChild(Node* node) {
			Node* left;
			Node* right;

			if (node->dim) {
				left = node->left;
				right = node->right;
				left->right = right;
				right->left = left;
				child = left;
			}
			else {
				child = nullptr;
			}
		}

		void replaceChild(Node* newNode) {
			Node* l;
			Node* r = right;

			if (r == this) {
				newNode->right = newNode;
				newNode->left = newNode;
			}
			else {
				l = left;
				l->right = newNode;
				r->left = newNode;
				newNode->left = l;
				newNode->right = r;
			}
			newNode->parent = parent;
			if (parent->child == this) {
				parent->child = newNode;
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


	class TwoThreeHeap : public HeapBase {
	public:
		TwoThreeHeap(int max_nodes = 1000000000) : nrNodes(0), value(0) {

			max_trees_ = (int)(0.5 + std::log(max_nodes + 1) / std::log(2.0));
			trees = new Node * [max_trees_];

			for (int i = 0; i < max_trees_; ++i) {
				trees[i] = nullptr;
			}

		}

		~TwoThreeHeap() override {delete[] trees;}

		void insert(int value) override {
			int key = value;
			Node* node = new Node(key, value);
			meld(node); // Meld the new node into the heap
			nrNodes++; // Increment the number of nodes in the heap
			//return node;
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
			return minNode; // returns a pointer to the minimum value
		}

		int getMin() override {return this->mini()->value();}

		bool empty() override {return nrNodes == 0;}

		void pop() override  {
			if (nrNodes != 0) {
				Node* child;
				Node* next;
				Node* partner;
				Node* minNode;
				minNode = mini();

				int rank = minNode->dim; // nr of children of minNode

				partner = minNode->partner;
				if (partner) {
					partner->partner = nullptr;
					partner->extra = false;
					partner->parent = nullptr;
					partner->left = partner;
					partner->right = partner;
					trees[rank] = partner;
				}
				else {
					trees[rank] = nullptr;
					value -= (1 << rank);
				}
				nrNodes--;

				child = minNode->child;
				if (child) {
					next = child->right;
					next->left = nullptr;
					child->right = nullptr;
					meld(next);
				}
			}
		}

		void merge(TwoThreeHeap& heap) {
			for (int i = 0; i < heap.max_trees_; ++i) {
				if (heap.trees[i] != nullptr) {
					heap.trees[i]->right = nullptr; // disconnect the three from the heap
					heap.trees[i]->left = nullptr;
					meld(heap.trees[i]);
					heap.trees[i] = nullptr;
				}
			}
			nrNodes += heap.nrNodes;
			heap.nrNodes = 0;
			//return true; // the merge was successful
		}

		void remove(Node* node) {
			Node* p, * middle, * extraNodeLeft, * extraNodeRight, * extraNodeParentLeft,
				* extraNodeParentRight, * extraNodeFirstChild,
				* parentOfExtraNodeLeft, * parentOfExtraNodeRight,
				* leftSibling, * rightSibling;

			Node* nodePartner = node->partner;
			int d;

			if (node->extra) { // if the node is extra we just disconnect it 
				node->partner->partner = nullptr;
				node->partner = nullptr;
				node->extra = false;
			}
			else if (nodePartner) {
				nodePartner->partner = nullptr;
				node->partner = nullptr;
				nodePartner->extra = false;

				node->replaceChild(nodePartner);
			}
			else {
				middle = node->right;
				if (middle->dim) {
					extraNodeLeft = nullptr;
					extraNodeParentLeft = nullptr;
					extraNodeRight = middle->child->partner;
					if (extraNodeRight) {
						extraNodeParentRight = nullptr;
					}
					else {
						extraNodeParentRight = middle->child;
					}
				}
				else {
					middle = node->parent;
					if (middle->extra) {
						extraNodeRight = nullptr;
						extraNodeParentRight = nullptr;
						middle = middle->partner;
						extraNodeLeft = middle->child->partner;
						if (extraNodeLeft) {
							extraNodeParentLeft = nullptr;
						}
						else {
							extraNodeParentLeft = middle->child;
						}
					}
					else {
						if (middle->parent) {
							extraNodeLeft = middle->left->partner;
							if (extraNodeLeft) {
								extraNodeParentLeft = nullptr;
							}
							else {
								extraNodeParentLeft = middle->left;
							}
						}
						else {
							extraNodeLeft = nullptr;
							extraNodeParentLeft = nullptr;
						}
						middle = middle->partner;
						if (middle) {
							extraNodeRight = middle->child->partner;
							if (extraNodeRight) {
								extraNodeParentRight = nullptr;
							}
							else {
								extraNodeParentRight = middle->child;
							}
						}
						else {
							extraNodeRight = nullptr;
							extraNodeParentRight = nullptr;
						}

					}
				}

				if (extraNodeRight) {
					extraNodeRight->partner->partner = nullptr;
					extraNodeRight->partner = nullptr;
					extraNodeRight->extra = false;

					node->replaceChild(extraNodeRight);
				}
				else if (extraNodeParentRight) {
					extraNodeFirstChild = extraNodeParentRight->parent;

					remove(extraNodeFirstChild);
					extraNodeFirstChild->removeChild(extraNodeParentRight);
					extraNodeParentRight->partner = extraNodeFirstChild;
					extraNodeFirstChild->partner = extraNodeParentRight;
					extraNodeParentRight->extra = true;
					extraNodeFirstChild->dim = node->dim;

					node->replaceChild(extraNodeFirstChild);
				}
				else if (extraNodeLeft) {
					p = node->parent;
					parentOfExtraNodeRight = extraNodeLeft->partner;
					parentOfExtraNodeLeft = parentOfExtraNodeRight->parent;

					p->removeChild(node);

					extraNodeLeft->partner = nullptr;
					extraNodeLeft->extra = false;

					d = parentOfExtraNodeRight->dim;
					parentOfExtraNodeRight->dim = p->dim;
					p->dim = d;

					parentOfExtraNodeRight->addChild(extraNodeLeft);

					if (p->extra) {
						p->partner = nullptr;
						p->extra = false;

						parentOfExtraNodeRight->partner = parentOfExtraNodeLeft;
						parentOfExtraNodeLeft->partner = parentOfExtraNodeRight;
						parentOfExtraNodeRight->extra = true;
						parentOfExtraNodeRight->replaceChild(p);
					}
					else {
						parentOfExtraNodeRight->partner = nullptr;
						leftSibling = parentOfExtraNodeRight->left;
						if (leftSibling != p) {
							rightSibling = p->right;
							p->left = leftSibling;
							parentOfExtraNodeRight->right = rightSibling;
							p->right = parentOfExtraNodeRight;
							parentOfExtraNodeRight->left = p;
							leftSibling->right = p;
							rightSibling->left = parentOfExtraNodeRight;

							if (parentOfExtraNodeLeft->child == p) {
								parentOfExtraNodeLeft->child = parentOfExtraNodeRight;
							}

						}
						else {
							parentOfExtraNodeLeft->child = parentOfExtraNodeRight;
						}
					}

				}
				else if (extraNodeParentLeft) {
					p = node->parent;
					p->removeChild(node);
					remove(p);
					p->dim = node->dim;
					p->partner = extraNodeParentLeft;
					extraNodeParentLeft->partner = p;

					if (p->priority() < extraNodeParentLeft->priority()) {
						p->extra = false;
						extraNodeParentLeft->replaceChild(p);
						extraNodeParentLeft->extra = true;
					}
					else {
						p->extra = true;
					}
				}
				else {
					d = node->dim;
					p = node->parent;

					p->removeChild(node);

					trees[d + 1] = nullptr;
					value -= (1 << (d + 1));
					p->dim = d;

					p->left = nullptr;
					p->right = nullptr;
					meld(p);
				}

			}

		}

		void decreaseKey(Node* node, int newKey) {
			node->key_ = newKey;
			node->value_ = newKey;

			if (!(node->parent || node->extra)) {
				return;
			}

			remove(node);
			node->right = nullptr;
			node->left = nullptr;

			meld(node);
		}

		void test() { std::cout << "23\n"; }

	protected:
		void meld(Node* list) {
			Node* next;
			Node* addTree;
			Node* carryTree;
			int d;

			next = nullptr;
			addTree = list;
			carryTree = nullptr;

			do {
				if (addTree) {
					next = addTree->right;
					addTree->right = addTree;
					addTree->left = addTree;
					addTree->parent = nullptr;
				}
				else {
					addTree = carryTree;
					carryTree = nullptr;
				}

				if (carryTree) {
					Node::mergeNodes(&addTree, &carryTree);
				}

				if (addTree) {
					d = addTree->dim;
					if (trees[d]) {
						Node::mergeNodes(&trees[d], &addTree);
						if (!trees[d]) value -= (1 << d);
						carryTree = addTree;
					}
					else {
						trees[d] = addTree;
						value += (1 << d);
					}
				}

				addTree = next;
			} while (addTree || carryTree);
		}

	private:
		Node** trees;
		int max_trees_;
		int nrNodes;
		int value;
	};

}

namespace Leftist {

	template <typename Type>
	class LeftistNode
	{
		LeftistNode* left, * right;
		Type value;
		int heap_null_path_length;

	public:
		// node constructor:
		LeftistNode(Type const val) : value(val), left(nullptr), right(nullptr), heap_null_path_length(0) {}
		// the data of the node:
		Type Value() const
		{
			return value;
		}
		// checks if the node is empty:
		bool empty() const
		{
			return this == nullptr;
		}

		LeftistNode<Type>* LeftChild()
		{
			return left;
		}
		LeftistNode<Type>* RightChild()
		{
			return right;
		}

		void push(LeftistNode<Type>* nodeToInsert, LeftistNode<Type>*& currentHeap)
		{
			if (nodeToInsert == nullptr)
			{
				// if the node is null, do nothing
				return;
			}
			else if (currentHeap == nullptr)
			{
				// if the heap is empty, the nodes becomes the root of the heap:
				currentHeap = nodeToInsert;
				return;
			}

			if (nodeToInsert->value < currentHeap->value)
			{
				std::swap(nodeToInsert, currentHeap);
			}

			if (currentHeap->right == nullptr)
			{
				// if the right subtree of the heap is empty, check for the left subtree:
				if (currentHeap->left == nullptr)
				{
					// if the left subtree of the heap is empty, add the node there.
					currentHeap->left = nodeToInsert;
				}
				else
				{ // else add the node to the right subtree
					currentHeap->right = nodeToInsert;
				}
			}
			else
			{
				push(nodeToInsert, currentHeap->right); // insert into the right subtree
			}

			// look at the null path length of the left subtree and right subtree
			// and move the one with the higher length to the left:
			if (currentHeap->left && currentHeap->right)
			{
				if (currentHeap->left->heap_null_path_length < currentHeap->right->heap_null_path_length)
				{
					std::swap(currentHeap->left, currentHeap->right);
				}
			}
			// update the heap null path length:
			if (currentHeap->right)
				currentHeap->heap_null_path_length = currentHeap->right->heap_null_path_length + 1;
		}

		void clear()
		{
			// clears the heap: firstly the subtrees, then the root itself.
			if (left != nullptr)
			{
				left->clear();
			}
			if (right != nullptr)
			{
				right->clear();
			}
			delete this;
		}
	};

	template <typename Type>
	class LeftistHeap : public HeapBase
	{
		LeftistNode<Type>* rootNode;
		int size;

	public:
		LeftistHeap() : rootNode(nullptr), size(0) {}
		LeftistHeap(LeftistHeap const& heap) : rootNode(nullptr), size(heap.size)
		{
			if (heap.size == 0) {
				size = 0;
				return;
			}

			else if (heap.size == 1)
				rootNode = heap.rootNode;
			else
			{
				// BFS in the heap, starting from the root node:
				std::queue<LeftistNode<Type>*> heapQueue;
				heapQueue.push_back(heap.rootNode);
				while (!heapQueue.empty())
				{
					LeftistNode<Type>* current = heapQueue.pop_front();
					rootNode->push(current, rootNode);
					if (current->left != nullptr) heapQueue.push_back(current->left);
					if (current->right != nullptr) heapQueue.push_back(current->right);
				}
			}
		}

		void test()  { std::cout << "Left\n"; }

		void insert (const Type val) override
		{
			LeftistNode<Type>* newNode = new LeftistNode<Type>(val);
			rootNode->push(newNode, rootNode);
			size++;
		}

		int getMin() override
		{
			return rootNode->Value();
		}

		bool empty() override  { return size == 0; }

		void pop() override  {
			if (size == 0) {
				//throw 1; // can't pop because the heap is empty
			}
			else {
				Type result = rootNode->Value(); /* save the root value so we can
												 delete the node and return it's value afterwards */

				LeftistNode<Type>* temp = rootNode;
				rootNode = temp->LeftChild();
				if (temp->RightChild()) {
					rootNode->push(temp->RightChild(), rootNode);
				}
				delete temp;
				size--;
				//return result;
			}
		}
		void swap(LeftistHeap& heap) {
			std::swap(rootNode, heap.rootNode);
			std::swap(size, heap.size);
		}

		~LeftistHeap() override = default;

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
	if (heap.size() != other1.size() or heap.size() != other2.size() or heap.size() != other3.size()) {
		heapout << "Size-ul trebuia sa fie " << heap.size() << std::endl;
		return false;
	}
	for (unsigned long long int i = 0; i < heap.size(); i++) {
		if (heap[i] != other1[i] or heap[i] != other2[i] or heap[i] != other3[i]) {
			return false;
		}
	}
	return true;
}

void probe_0(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		for (auto& nr : numbers) {
			heap.insert(nr);
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
		
		unsigned long long int n = numbers.size();

		for (unsigned long long int i = 1; i <= numbers.size(); i++) {
			heap.insert(numbers[i-1]);
			output_heap.push_back(heap.getMin());
			if (i % (n/10) == 0) {
				heap.pop();
			}
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

void probe_4(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		unsigned long long int n = numbers.size();

		for (unsigned long long int i = 1; i <= numbers.size(); i++) {
			heap.insert(numbers[i - 1]);
			output_heap.push_back(heap.getMin());
			if (i % (n / 100) == 0) {
				heap.pop();
			}
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

void probe_5(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		for (unsigned long long int i = 1; i <= numbers.size(); i++) {
			heap.insert(numbers[i-1]);
			output_heap.push_back(heap.getMin());
			if (i % 1000 == 0) {
				heap.pop();
			}
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

void probe_6(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		for (unsigned long long int i = 1; i <= numbers.size(); i++) {
			heap.insert(numbers[i-1]);
			output_heap.push_back(heap.getMin());
			if (i % 1000 == 0) {
				for (int j = 0; j < 100; j++) {
					heap.pop();
				}
			}
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

void probe_7(std::string name, std::vector<int>& numbers, std::vector<int>& output_heap, HeapBase& heap) {
	heap.test();
	try {
		auto start_time = std::chrono::high_resolution_clock::now();

		for (unsigned long long int i = 1; i <= numbers.size(); i++) {
			heap.insert(numbers[i-1]);
			output_heap.push_back(heap.getMin());
			if (i % 1000 == 0) {
				for (int j = 0; j < 1000; j++) {
					heap.pop();
				}
			}
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
	std::vector<int> output_heap, output_fibheap, output_heap23, output_leftist;
	Heap::Heap heap;
	Fibonacci::FibHeap fibheap;
	Heap23::TwoThreeHeap heap23;
	Leftist::LeftistHeap<int> leftist;
	/*
	std::vector<int> dump_cache;
	for (auto& nr : numbers) {
		dump_cache.push_back(nr);
	}
	*/
	probe("Heap   ", numbers, output_heap, heap);
	probe("FibHeap", numbers, output_fibheap, fibheap);
	probe("23Heap ", numbers, output_heap23, heap23);
	probe("Leftist", numbers, output_leftist, leftist);

	if (!testingAll(output_heap, output_fibheap, output_heap23, output_leftist)) { out << "WRONG\n"; }
	else { out << "Successful\n"; }
}


void mergeFib(std::vector<int>& numbers, std::vector<int>& output_fibheap) {
	Fibonacci::FibHeap fibheap,aux;
	try {
		auto start_time = std::chrono::high_resolution_clock::now();
		unsigned long long int i = 0;
		for (; i < numbers.size() / 2;i++) {
			fibheap.insert(numbers[i]);
		}
		for (; i < numbers.size(); i++) {
			aux.insert(numbers[i]);
		}
		fibheap.merge(&aux);
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out << "FibHeap" << " time: " << duration.count() << " miliseconds\n";

		while (!fibheap.empty()) {
			output_fibheap.push_back(fibheap.getMin());
			fibheap.pop();
		}
	}
	catch (const std::bad_alloc& e) {
		out << "\tyou don't have enough memory" << std::endl;
	}
	catch (...) {
		out << "\tnope" << std::endl;
	}
}

void mergeFibBatch(std::vector<int>& numbers, std::vector<int>& output_fibheap) {
	Fibonacci::FibHeap fibheap;
	try {
		auto start_time = std::chrono::high_resolution_clock::now();
		unsigned long long batch = 100;
		for (unsigned long long int i = 0; i < batch; i++) {
			Fibonacci::FibHeap aux;
			for (unsigned long long int j = 0; j < numbers.size() / batch; j++) {
				aux.insert(numbers[i*(numbers.size() / batch)+j]);
			}
			fibheap.merge(&aux);
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out << "FibHeap" << " time: " << duration.count() << " miliseconds\n";

		while (!fibheap.empty()) {
			output_fibheap.push_back(fibheap.getMin());
			fibheap.pop();
		}
	}
	catch (const std::bad_alloc& e) {
		out << "\tyou don't have enough memory" << std::endl;
	}
	catch (...) {
		out << "\tnope" << std::endl;
	}
}

void merge23(std::vector<int>& numbers, std::vector<int>& output_heap23) {
	Heap23::TwoThreeHeap heap23, aux;
	try {
		auto start_time = std::chrono::high_resolution_clock::now();
		unsigned long long int i = 0;
		for (; i < numbers.size() / 2; i++) {
			heap23.insert(numbers[i]);
		}
		for (; i < numbers.size(); i++) {
			aux.insert(numbers[i]);
		}
		heap23.merge(aux);
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out << "Heap23 " << " time: " << duration.count() << " miliseconds\n";

		while (!heap23.empty()) {
			output_heap23.push_back(heap23.getMin());
			heap23.pop();
		}
	}
	catch (const std::bad_alloc& e) {
		out << "\tyou don't have enough memory" << std::endl;
	}
	catch (...) {
		out << "\tnope" << std::endl;
	}
}

void merge23Batch(std::vector<int>& numbers, std::vector<int>& output_heap23) {
	Heap23::TwoThreeHeap heap23;
	try {
		auto start_time = std::chrono::high_resolution_clock::now();
		unsigned long long batch = 100;
		for (unsigned long long int i = 0; i < batch; i++) {
			Heap23::TwoThreeHeap aux;
			for (unsigned long long int j = 0; j < numbers.size() / batch; j++) {
				aux.insert(numbers[i * (numbers.size() / batch) + j]);
			}
			heap23.merge(aux);
		}
		auto end_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
		out << "Heap23 " << " time: " << duration.count() << " miliseconds\n";

		while (!heap23.empty()) {
			output_heap23.push_back(heap23.getMin());
			heap23.pop();
		}
	}
	catch (const std::bad_alloc& e) {
		out << "\tyou don't have enough memory" << std::endl;
	}
	catch (...) {
		out << "\tnope" << std::endl;
	}
}

void init_merge(std::vector<int>& numbers) {
	std::vector<int> output_heap, output_fibheap, output_heap23, output_leftist;
	
	mergeFib(numbers,output_fibheap);
	merge23(numbers, output_heap23);

	if (!testing2(output_fibheap, output_heap23, numbers)) { out << "WRONG\n"; }
	else { out << "Successful\n"; }
}

void init_merge_batch(std::vector<int>& numbers) {
	std::vector<int> output_heap, output_fibheap, output_heap23, output_leftist;

	mergeFibBatch(numbers, output_fibheap);
	merge23Batch(numbers, output_heap23);

	if (!testing2(output_fibheap, output_heap23, numbers)) { out << "WRONG\n"; }
	else { out << "Successful\n"; }
}


void compareHeaps(unsigned long long int n, int maxi) {
	std::vector<int> numbers = generateRandomI(n, maxi); // will be random numbers
	
	// 0: n insert
	// 1: n insert, n pop
	// 2: n ( 1 insert, 1 pop)
	// 3: n ( 1 insert, 1 in n/10 pop)     => n insert, 10  pop
	// 4: n ( 1 insert, 1 in n/100 pop)	   => n insert, 100 pop
	// 5: n/1000 ( 1000  insert, 1    pop)
	// 6: n/1000 ( 1000  insert, 100   pop)
	// 7: n/1000 ( 1000  insert, 1000  pop)

	
	out << "\nProbe 0\n";
	init_probe(probe_0, numbers);
	out << "\nProbe 1\n";
	init_probe(probe_1, numbers); 
	out << "\nProbe 2\n";
	init_probe(probe_2, numbers);
	out << "\nProbe 3\n";
	init_probe(probe_3, numbers);
	out << "\nProbe 4\n";
	init_probe(probe_4, numbers);
	out << "\nProbe 5\n";
	init_probe(probe_5, numbers);
	out << "\nProbe 6\n";
	init_probe(probe_6, numbers);
	out << "\nProbe 7\n";
	init_probe(probe_7, numbers);
	

	/*
	out << "\nMerge \n";
	init_merge(numbers);
	out << "\nMergeBatched \n";
	init_merge_batch(numbers);
	*/
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


	//Fibonacci::tests();
	//return 0;


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

						out << "\n ------------------------------\n";
						compareHeaps(n, maxi);
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
