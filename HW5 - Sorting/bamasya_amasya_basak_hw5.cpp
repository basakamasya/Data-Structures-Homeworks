//Basak Amasya

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>

using namespace std;

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode //templated node class to be used in binary search tree
{
public:
	Comparable element;
	BinaryNode *left;
	BinaryNode *right;
	BinaryNode(const Comparable & theElement, BinaryNode *lt, BinaryNode *rt) : element(theElement), left(lt), right(rt) { }
};

template <class Comparable>
class BinarySearchTree //templated Binary Search Tree class
{
public:
	explicit BinarySearchTree(const Comparable & notFound);
	void find(const Comparable & x);
	void insert(const Comparable & x);
	Comparable ITEM_NOT_FOUND; //moved from private to public
	bool found; //added to keep track of not found searches, deletions or additions
private:
	BinaryNode<Comparable> *root;
	const Comparable & elementAt(BinaryNode<Comparable> *t) const;
	void insert(const Comparable & x, BinaryNode<Comparable> * & t);
	vector<BinaryNode<Comparable>*> find(const Comparable & x, BinaryNode<Comparable> *t, vector<BinaryNode<Comparable>*> &) const;
};

template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound) : ITEM_NOT_FOUND(notFound), root(NULL) { } //constructor

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::elementAt(BinaryNode<Comparable> *t) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
void BinarySearchTree<Comparable>::find(const Comparable & x) //modified for partial searches
{
	vector<BinaryNode<Comparable>*> founditems; //keeping a vector for found contacts
	find(x, root, founditems);
	/*cout << "binary search tree results" << endl;
	for (int i = 0; i < founditems.size(); i++)
	{
		cout << elementAt(founditems[i]) << endl;
	}*/
	//cout << endl;
	//cout << founditems.size() << endl;
	/*if (founditems.size() == 0)
	{
		found = false;
	}
	else found = true;*/
}

template <class Comparable>
vector<BinaryNode<Comparable>*> BinarySearchTree<Comparable>::find(const Comparable & x, BinaryNode<Comparable> * t, vector<BinaryNode<Comparable>*> & found) const
{
	string name, surname, elem;
	if (t)
	{
		elem = t->element;
		Parse(elem);
		elem = elem.substr(0, x.length()); //getting the substring for partial searches
	}
	if (t == NULL)
		return found;
	else if (x < elem)
	{
		find(x, t->left, found);
		return found;
	}
	else if (elem < x)
	{
		find(x, t->right, found);
		return found;
	}
	else
	{
		found.push_back(t);
		if (t)
		{
			find(x, t->left, found);
			find(x, t->right, found);
		}
		return found;
	}
}

template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x)
{
	insert(x, root);
}
template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x, BinaryNode<Comparable> * & t)
{
	static int count = 0;
	string elem, key;
	if (t)
	{
		elem = t->element;
		Parse(elem);
		key = x;
		Parse(key);
	}
	if (t == NULL) // create a new node at the right place
	{
		t = new BinaryNode<Comparable>(x, NULL, NULL);
	}
	else if (key < elem)
		insert(x, t->left); // insert at the left or
	else if (elem < key)
		insert(x, t->right); // right subtree
	else if (key == elem)
	{
		BST.found = true;
	}
}
template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode //templated node to be used in AVL Tree class
{
public:
	Comparable 	element;
	AvlNode *left;
	AvlNode *right;
	int height;
	AvlNode(const Comparable & theElement, AvlNode *lt, AvlNode *rt, int h = 0)
		: element(theElement), left(lt), right(rt), height(h) { }
};

template <class Comparable>
class AvlTree //templated AVL Tree class
{
public:
	explicit AvlTree(const Comparable & notFound);
	void find(const Comparable & x) const;
	void insert(const Comparable & x);
	Comparable ITEM_NOT_FOUND; //moved from private to public
private:
	AvlNode<Comparable> *root;
	const Comparable & elementAt(AvlNode<Comparable> *t) const;
	void insert(const Comparable & x, AvlNode<Comparable> * & t) const;
	vector<AvlNode<Comparable>*> find(const Comparable & x, AvlNode<Comparable> *t, vector<AvlNode<Comparable>*> &) const;
	int height(AvlNode<Comparable> *t) const; //AVL manipulations
	int max(int lhs, int rhs) const;
	void rotateWithLeftChild(AvlNode<Comparable> * & k2) const;
	void rotateWithRightChild(AvlNode<Comparable> * & k1) const;
	void doubleWithLeftChild(AvlNode<Comparable> * & k3) const;
	void doubleWithRightChild(AvlNode<Comparable> * & k1) const;
};

template <class Comparable>
AvlTree<Comparable>::AvlTree(const Comparable & notFound) : ITEM_NOT_FOUND(notFound), root(NULL) { } //constructor

template <class Comparable>
void AvlTree<Comparable>::find(const Comparable & x) const
{
	vector<AvlNode<Comparable>*> found;
	find(x, root, found);
	/*cout << "avl results" << endl;
	for (int i = 0; i < found.size(); i++)
	{
		cout << elementAt(found[i]) << endl;
	}
	cout << endl;*/
}

template <class Comparable>
vector<AvlNode<Comparable>*> AvlTree<Comparable>::find(const Comparable & x, AvlNode<Comparable> *t, vector<AvlNode<Comparable>*> & found) const
{
	string name, surname, elem;
	if (t)
	{
		elem = t->element;
		Parse(elem);
		elem = elem.substr(0, x.length());
	}
	if (t == NULL)
		return found;
	else if (x < elem)
	{
		find(x, t->left, found);
		return found;
	}
	else if (elem < x)
	{
		find(x, t->right, found);
		return found;
	}
	else
	{
		found.push_back(t);
		if (t)
		{
			find(x, t->left, found);
			find(x, t->right, found);
		}
		return found;
	}
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::elementAt(AvlNode<Comparable> *t) const
{
	return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
int AvlTree<Comparable>::height(AvlNode<Comparable> *t) const
{
	if (t == NULL)
		return -1;
	return t->height;
}

template <class Comparable>
int AvlTree<Comparable>::max(int lhs, int rhs) const
{
	if (lhs > rhs)
		return lhs;
	return rhs;
}

template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable & x, AvlNode<Comparable> * & t) const
{
	if (t == NULL)
		t = new AvlNode<Comparable>(x, NULL, NULL);
	else if (x < t->element)
	{
		// X should be inserted to the left tree!
		insert(x, t->left);
		// Check if the left tree is out of balance (left subtree grew in height!)
		if (height(t->left) - height(t->right) == 2)
			if (x < t->left->element)  // X was inserted to the left-left subtree!
				rotateWithLeftChild(t);
			else			     // X was inserted to the left-right subtree!
				doubleWithLeftChild(t);
	}
	else if (t->element < x)
	{    // Otherwise X is inserted to the right subtree
		insert(x, t->right);
		if (height(t->right) - height(t->left) == 2)
			// height of the right subtree increased
			if (t->right->element < x)
				// X was inserted to right-right subtree
				rotateWithRightChild(t);
			else // X was inserted to right-left subtree
				doubleWithRightChild(t);
	}
	else
		//cout << "duplicate AVL" << endl;
	;  // Duplicate; do nothing
	t->height = max(height(t->left), height(t->right)) + 1; // update the height the node
}

template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable & x)
{
	insert(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(AvlNode<Comparable> * & k2) const
{
	AvlNode<Comparable> *k1 = k2->left;
	k2->left = k1->right;
	k1->right = k2;
	k2->height = max(height(k2->left), height(k2->right)) + 1;
	k1->height = max(height(k1->left), k2->height) + 1;
	k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild(AvlNode<Comparable> * & k1) const
{
	AvlNode<Comparable> *k2 = k1->right;
	k1->right = k2->left;
	k2->left = k1;
	k1->height = max(height(k1->left), height(k1->right)) + 1;
	k2->height = max(height(k2->right), k1->height) + 1;
	k1 = k2;
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild(AvlNode<Comparable> * & k3) const
{
	rotateWithRightChild(k3->left);
	rotateWithLeftChild(k3);
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild(AvlNode<Comparable> * & k1) const
{
	rotateWithLeftChild(k1->right);
	rotateWithRightChild(k1);
}

//function to parse the contact into key (name and surname)
void Parse(string & elem)
{
	istringstream iss(elem);
	string name, surname;
	iss >> name >> surname;
	elem = name + " " + surname;
}

//function that makes the string uppercase
void UpperCase(string & str)
{
	for (unsigned int i = 0; i < str.length(); i++) // checking every character of the string
	{
		if ('a' <= str.at(i) && str.at(i) <= 'z') // if the letter is lowercase
		{
			str.at(i) = str.at(i) + ('A' - 'a'); // make the character at that index uppercase
		}
	}
}

BinarySearchTree<string> BST(" "); //BST to be used as phonebook, declared as global variable
AvlTree<string> AVL(" "); //AVL Tree to be used as phonebook, declared as global variable
vector<string> insertionsorted; //vectors for sorting algorithms
vector<string> quicksorted;
vector<string> mergesorted;
vector<string> heapsorted;
string tobesearched; //key that will be searched


//void BinarySearch(vector<string> & array, int left, int right, string key)
//{
//	if (right >= left) 
//	{
//		int mid = left + (right - left) / 2;
//		string elemmain = array[mid];
//		elemmain = elemmain.substr(0, key.length());
//		if (elemmain == key)
//		{
//			//cout << array[mid] << endl;
//
//			int goleft = mid;
//			int goright = mid;
//			while (goleft > left)
//			{
//				goleft--;
//				string elem = array[goleft];
//				elem = elem.substr(0, key.length());
//				if (elem == key)
//				{
//					//cout << array[goleft] << endl;
//				}
//				else
//				{
//					break;
//				}
//			}
//			while (goright < right)
//			{
//				goright++;
//				string elem = array[goright];
//				elem = elem.substr(0, key.length());
//				if (elem == key)
//				{
//					//cout << array[goright] << endl;
//				}
//				else
//				{
//					break;
//				}
//			}
//		}
//		if (elemmain > key)
//		{
//			BinarySearch(array, left, mid - 1, key);
//		}
//		else if (elemmain < key)
//		{
//			BinarySearch(array, mid + 1, right, key);
//		}
//	}
//}

void BinarySearch(vector<string> & array, int left, int right, string key)
{
	if (right >= left)
	{
		int mid = left + (right - left) / 2;
		string elemmain = array[mid];
		elemmain = elemmain.substr(0, key.length());
		if (elemmain == key)
		{
			//cout << array[mid] << endl;
			BinarySearch(array, left, mid - 1, key);
			BinarySearch(array, mid + 1, right, key);
			
		}
		if (elemmain > key)
		{
			BinarySearch(array, left, mid - 1, key);
		}
		else if (elemmain < key)
		{
			BinarySearch(array, mid + 1, right, key);
		}
	}
}

//void BinarySearch(vector<string> & array, int left, int right, string key)
//{
//	while (left <= right) 
//	{
//		int mid = left + (right - left) / 2;
//
//		// Check if x is present at mid 
//		string elemmain = array[mid];
//		elemmain = elemmain.substr(0, key.length());
//		if (key == elemmain)
//		{
//			//cout << array[mid] << endl;
//			BinarySearch(array, left, mid - 1, key);
//			BinarySearch(array, mid + 1, right, key);
//			return;
//		}
//
//		// If x greater, ignore left half 
//		else if (elemmain < key)
//			left = mid + 1;
//
//		// If x is smaller, ignore right half 
//		else if (elemmain > key)
//			right = mid - 1;
//	}
//}

//void BinarySearch(vector<string> & array, int left, int right, string key)
//{
//	while (left <= right) 
//	{
//		int mid = left + (right - left) / 2; 
//		string elemmain = array[mid];
//		elemmain = elemmain.substr(0, key.length());
//		if (key == elemmain)
//		{
//			//cout << array[mid] << endl;
//
//			int goleft = mid;
//			int goright = mid;
//			while (goleft > left)
//			{
//				goleft--;
//				string elem = array[goleft];
//				elem = elem.substr(0, key.length());
//				if (elem == key)
//				{
//					//cout << array[goleft] << endl;
//				}
//				else
//				{
//					break;
//				}
//			}
//			while (goright < right)
//			{
//				goright++;
//				string elem = array[goright];
//				elem = elem.substr(0, key.length());
//				if (elem == key)
//				{
//					//cout << array[goright] << endl;
//				}
//				else
//				{
//					break;
//				}
//			}
//			break;
//		}
//
//		else if (elemmain < key)
//			left = mid + 1;
//
//		else if (elemmain > key)
//			right = mid - 1;
//	}
// 
//}

//sorting algorithms except mergesort taken from lecture slides
//insertion sort algorithm
template <class Comparable>
void insertionsort(vector <Comparable> & a)
{
	int j;
	for (int p = 1; p < a.size(); p++)
	{
		Comparable tmp = a[p];
		for (j = p; j > 0 && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
 	}
}
//standard swap operation for heap sort
template <class Comparable>
inline void swap(Comparable & obj1, Comparable & obj2)
{
	Comparable tmp = obj1;
	obj1 = obj2;
	obj2 = tmp;
}

//approximating the median for quicksort
template <class Comparable>
const Comparable & median3(vector<Comparable> & a,int left, int right)
{
	int center = (left + right) / 2;
	if (a[center] < a[left])
		swap(a[left], a[center]);
	if (a[right] < a[left])
		swap(a[left], a[right]);
	if (a[right] < a[center])
		swap(a[center], a[right]);

	// Place pivot at position right - 1
	swap(a[center], a[right - 1]);
	return a[right - 1];
}

//quicksort algorithm with cutoff for small arrays
template <class Comparable>
void quicksort(vector<Comparable> & a, int left, int right)
{
	if (left + 10 <= right)
	{
		Comparable pivot = median3(a, left, right);
		// Begin partitioning
		int i = left, j = right - 1;
		for (; ; )
		{
			while (a[++i] < pivot) {}

			while (pivot < a[--j]) {}

			if (i < j)
				swap(a[i], a[j]);
			else
				break;
		}
		swap(a[i], a[right - 1]);   // Restore pivot

		quicksort(a, left, i - 1);       // Sort small elements
		quicksort(a, i + 1, right);    // Sort large elements
	}
	else  // Do an insertion sort on the subarray
		insertionSortforQuickSort(a, left, right);
}

//insertion sort for quick sort for small arrays
template <class Comparable>
void insertionSortforQuickSort(vector<Comparable> & a, int left, int right)
{
	for (int p = left + 1; p <= right; p++)
	{
		Comparable tmp = a[p];
		int j;
		for (j = p; j > left && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}

template <class Comparable>
void quicksort(vector<Comparable> & a)
{
	quicksort(a, 0, a.size() - 1);
}

//merging operation for in place merge for mergesort
template <class Comparable>
void merge(vector<Comparable> & array, int start, int mid, int end)
{
	int start2 = mid + 1; //keeping the second starting point
	if (array[mid] <= array[start2]) //if it's already sorted
	{
		return;
	}
	while (start <= mid && start2 <= end) 
	{
		if (array[start] <= array[start2]) 	// If element 1 is in right place 
		{
			start++; //increase the starting point
		}
		else 
		{
			Comparable value = array[start2];
			int index = start2; 
			while (index != start) //Shift all the elements between element 1 and element 2, right by 1
			{
				array[index] = array[index - 1];
				index--;
			}
			array[start] = value;
			start++; // Update all the pointers 
			mid++;
			start2++;
		}
	}
}

template <class Comparable>
void mergeSort(vector<Comparable> & a)
{
	mergeSort(a, 0, a.size() - 1);
}

//recursive mergesort algorithm
template <class Comparable>
void mergeSort(vector<Comparable> & array, int left, int right)
{
	if (left < right) 
	{
		int middle = left + (right - left) / 2;
		mergeSort(array, left, middle); // Sort the first half
		mergeSort(array, middle + 1, right); // Sort the second half
		merge(array, left, middle, right); //merge the 2 parts
	}
}

//heapsort algorithm
template <class Comparable>
void heapsort(vector<Comparable> & a)
{
	for (int i = a.size() / 2; i >= 0; i--) //buildheap
		percDown(a,i,a.size());
	for (int j = a.size() - 1; j > 0; j--) //sort
	{
		swap(a[0], a[j]);    // swap max to the last pos.
		percDown(a, 0, j); // re-form the heap
	}
}
//returning the leftchild for heap
inline int leftChild(int i)
{
	return 2 * i + 1;
}

//percolate down operation for heapsort
template <class Comparable>  // for deleteMax, a is the array, i is the position to percolate down from, n is the logical size of the array
void percDown(vector<Comparable> & a, int i, int n)
{
	int child;
	Comparable tmp;
	for (tmp = a[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if (child != n - 1 && a[child] < a[child + 1])
			child++;
		if (a[child] > tmp)
			a[i] = a[child];
		else
			break;
	}
	a[i] = tmp;
}
//function that compares the speedups in search operation
void SearchSpeedUp(long long int BSTtime, long long int AVLtime, long long int binarysearchtime)
{
	cout << "SpeedUps in search" << endl;
	cout << "======================================" << endl;
	if (BSTtime > AVLtime) //comparing search in BST with AVL
	{
		cout << "(BST / AVL) SpeedUp = " << float(BSTtime) / AVLtime << endl;
	}
	else
	{
		cout << "(AVL / BST) SpeedUp = " << float(AVLtime) / BSTtime << endl;
	}
	if (binarysearchtime > AVLtime) //comparing search in sorted vector with AVL
	{
		cout << "(Binary Search / AVL) SpeedUp = " << float(binarysearchtime) / AVLtime << endl;
	}
	else
	{
		cout << "(AVL / Binary Search) SpeedUp = " << float(AVLtime) / binarysearchtime << endl;
	}
	if (BSTtime > binarysearchtime) //comparing search in BST with sorted vector
	{
		cout << "(BST / Binary Search) SpeedUp = " << float(BSTtime) / binarysearchtime << endl;
	}
	else
	{
		cout << "(Binary Search / BST) SpeedUp = " << float(binarysearchtime) / BSTtime << endl;
	}
	cout << endl;
}
//function that searches the key in the phonebook copies
void Search(string key)
{
	cout << "Searching for " << key << endl;
	cout << "======================================" << endl;
	UpperCase(key);

	auto BSTstarted = std::chrono::high_resolution_clock::now(); //start keeping the time for BST
	BST.find(key);
	auto BSTfinished = std::chrono::high_resolution_clock::now(); //searching finished
	auto BSTtime = chrono::duration_cast<std::chrono::nanoseconds>(BSTfinished - BSTstarted).count();

	auto AVLstarted = std::chrono::high_resolution_clock::now(); //start keeping the time for AVL
	AVL.find(key);
	auto AVLfinished = std::chrono::high_resolution_clock::now(); //searching finished
	auto AVLtime = chrono::duration_cast<std::chrono::nanoseconds>(AVLfinished - AVLstarted).count();

	/*for (int i = 0; i < insertionsorted.size(); i++)
	{
		cout << insertionsorted[i] << endl;
	}*/

	auto binarysearchstarted = std::chrono::high_resolution_clock::now(); //start keeping the time for Binary Search
	BinarySearch(insertionsorted, 0, insertionsorted.size()-1,key);
	auto binarysearchfinished = std::chrono::high_resolution_clock::now(); //searching finished
	auto insertionsorttime = chrono::duration_cast<std::chrono::nanoseconds>(binarysearchfinished - binarysearchstarted).count();

	cout << "The search in BST took "; //display all the times in terms of nanoseconds
	cout << BSTtime << " Nanoseconds" << endl;
	cout << "The search in AVL took ";
	cout << AVLtime << " Nanoseconds" << endl;
	cout << "Binary Search Time: ";
	cout << insertionsorttime << " Nanoseconds" << endl;
	cout << endl;
	SearchSpeedUp(BSTtime, AVLtime, insertionsorttime); //compare the speedups
}
//function that compares the speedups for different sorting algorithms
void SortSpeedUp(long long int quicktime, long long int insertiontime, long long int mergetime, long long int heaptime)
{
	Search(tobesearched); //searching
	cout << "SpeedUps between Sorting Algorithms" << endl;
	cout << "======================================" << endl; //comparing only with quicksort
	if (insertiontime > quicktime) //compare quicksort with insertionsort
	{
		cout << "(Insertion Sort / Quick Sort) SpeedUp = " << float(insertiontime) / quicktime << endl;
	}
	else
	{
		cout << "(Quick Sort / Insertion Sort) SpeedUp = " << float(quicktime) / insertiontime << endl;
	}
	if (mergetime > quicktime) //compare quicksort with mergesort
	{
		cout << "(Merge Sort / Quick Sort) SpeedUp = " << float(mergetime) / quicktime << endl;
	}
	else
	{
		cout << "(Quick Sort / Merge Sort) SpeedUp = " << float(quicktime) / mergetime << endl;
	}
	if (heaptime > quicktime) //compare quicksort with heapsort
	{
		cout << "(Heap Sort / Quick Sort) SpeedUp = " << float(heaptime) / quicktime << endl;
	}
	else
	{
		cout << "(Quick Sort / Heap Sort) SpeedUp = " << float(quicktime) / heaptime << endl;
	}
	cout << endl;
}

//function that sorts the vectors with different algorithms
void SortVectors()
{
	cout << "Sorting the vector copies" << endl;
	cout << "======================================" << endl;
	auto quickstart = std::chrono::high_resolution_clock::now(); //start keeping the time
	quicksort(quicksorted);
	auto quickfinish = std::chrono::high_resolution_clock::now(); //creating has finished
	auto quicktime = chrono::duration_cast<std::chrono::nanoseconds>(quickfinish - quickstart).count();
	cout << "Quick Sort Time: " << quicktime << " Nanoseconds" << endl;

	auto insertionstart = std::chrono::high_resolution_clock::now(); //start keeping the time
	insertionsort(insertionsorted);
	auto insertionfinish = std::chrono::high_resolution_clock::now(); //creating has finished
	auto insertiontime = chrono::duration_cast<std::chrono::nanoseconds>(insertionfinish - insertionstart).count();
	cout << "Insertion Sort Time: " << insertiontime << " Nanoseconds" << endl;

	auto mergestart = std::chrono::high_resolution_clock::now(); //start keeping the time
	mergeSort(mergesorted);
	auto mergefinish = std::chrono::high_resolution_clock::now(); //creating has finished
	auto mergetime = chrono::duration_cast<std::chrono::nanoseconds>(mergefinish - mergestart).count();
	cout << "Merge Sort Time: " << mergetime << " Nanoseconds" << endl;

	auto heapstart = std::chrono::high_resolution_clock::now(); //start keeping the time
	heapsort(heapsorted);
	auto heapfinish = std::chrono::high_resolution_clock::now(); //creating has finished
	auto heaptime = chrono::duration_cast<std::chrono::nanoseconds>(heapfinish - heapstart).count();
	cout << "Heap Sort Time: "<< heaptime << " Nanoseconds" << endl;
	cout << endl;
	SortSpeedUp(quicktime, insertiontime, mergetime, heaptime);
}

//uploading all contacts the vectors
void CreateVectorsUnsorted(ifstream & file)
{
	string line, name, surname, tel, city, key;
	while (getline(file, line)) //read file line by line
	{
		istringstream iss(line);
		iss >> name >> surname >> tel >> city; //get each word
		UpperCase(name); // turn the name and surname into uppercase
		UpperCase(surname);
		key = name + " " + surname + " " + tel + " " + city; //make the key
		insertionsorted.push_back(key);
		quicksorted.push_back(key);
		mergesorted.push_back(key);
		heapsorted.push_back(key);
	}
}
//creating the Binary Search Tree
void CreateBST(ifstream & file)
{
	string line, name, surname, tel, city, key;
	while (getline(file, line)) //read file line by line
	{
		istringstream iss(line);
		iss >> name >> surname >> tel >> city; //get each word
		UpperCase(name); // turn the name and surname into uppercase
		UpperCase(surname);
		key = name + " " + surname + " " + tel + " " + city; //make the key
		BST.insert(key); //insert the key
	}
}
//creating the AVL tree
void CreateAVL(ifstream & file)
{
	string line, name, surname, tel, city, key; //similiar to creating the BST
	while (getline(file, line))
	{
		istringstream iss(line);
		iss >> name >> surname >> tel >> city;
		UpperCase(name);
		UpperCase(surname);
		key = name + " " + surname + " " + tel + " " + city; //make the key
		AVL.insert(key); //insert the key
	}
}
//reading the input file
void ReadFile()
{
	ifstream file;
	string filename;
	cout << "Please enter the contact file name: " << endl;
	cin >> filename;
	file.open(filename.c_str());
	while (file.fail()) //keep asking for the file until a valid one is entered
	{
		cout << "Couldn't open file. Please enter again: ";
		cin >> filename;
		file.open(filename.c_str());
	}
	CreateBST(file); //create the Binary Search Tree
	file.clear();
	file.seekg(0);
	CreateAVL(file); //create the AVL tree
	file.clear();
	file.seekg(0);
	CreateVectorsUnsorted(file); //create the 4 vector copies unsorted
	file.close();
}

int main()
{
	ReadFile(); //reading the file into the phonebook copies

	cout << "Please enter the word to be queried :" << endl;
	cin.ignore();
	getline(cin, tobesearched);
	//cin >> tobesearched;
	cout << endl;

	SortVectors(); //sorting the vectors

	system("pause");
	return 0;
}