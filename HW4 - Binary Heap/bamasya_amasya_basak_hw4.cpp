//Basak Amasya

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

//BinaryHeap class partially taken from lecture slides and modified
template <class Comparable>
class BinaryHeap
{
public:
	BinaryHeap() { array.resize(100);  currentSize = 0; }; //default constructor
	bool isEmpty() const; //checks if the heap is empty
	const Comparable & findTop() const; //returns the top element of the heap
	int findSize(); //returns the current size of the heap
	void insert(const Comparable & x); //inserts to heap according to min or max sorting
	void deleteTop(Comparable & Item); //deletes the top element of the heap
protected:
	int currentSize; //number of elements in heap
	vector<Comparable> array; //the heap array
	void percolateDown(int hole); //percolate down function for deletion
	bool isItMinHeap; //boolean flag to decide sorting order
};

template <class Comparable>
class MaxHeap : public BinaryHeap<Comparable> //inherited from BinaryHeap
{
public:
	MaxHeap() : BinaryHeap() //default constructor expilicitly calls parent constructor
	{
		isItMinHeap = false; //flag set to false, root will be max
	}
};

template <class Comparable>
class MinHeap : public BinaryHeap<Comparable> //inherited from BinaryHeap
{
public:
	MinHeap() : BinaryHeap() //default constructor expilicitly calls parent constructor
	{
		isItMinHeap = true; //flag set to true, root will be min
	}
};

template <class Comparable>
bool BinaryHeap<Comparable>::isEmpty() const
{
	return (currentSize == 0);
}

template <class Comparable>
const Comparable & BinaryHeap<Comparable>::findTop() const
{
	return array[1]; //returns the root
}

template <class Comparable>
int BinaryHeap<Comparable>::findSize()
{
	return currentSize;
}

template <class Comparable>
void BinaryHeap<Comparable>::deleteTop(Comparable & Item)
{
	if (isEmpty())
		return;
	Item = array[1];
	//array[1] = array[currentSize--];
	array[1] = array[currentSize]; //modified so that last element will be 0
	array[currentSize] = 0;
	currentSize--;
	percolateDown(1);
}

template <class Comparable>
void BinaryHeap<Comparable>::percolateDown(int hole)
{
	int child;
	Comparable tmp = array[hole];   // tmp is the item that will be percolated down
	for (; hole * 2 <= currentSize; hole = child)
	{
		child = hole * 2;
		if (isItMinHeap) //percolate down so that the root will be min
		{
			if (child != currentSize && array[child + 1] < array[child])
				child++;
			if (array[child] < tmp)
				array[hole] = array[child];
			else
				break;
		}
		else //added, percolate down so that the root will be max
		{
			if (child != currentSize && array[child + 1] > array[child]) //modified
				child++;
			if (array[child] > tmp) //modified
				array[hole] = array[child];
			else
				break;
		}
		
	}
	array[hole] = tmp;
}

template <class Comparable>
void BinaryHeap<Comparable>::insert(const Comparable & x)
{
	if (currentSize == array.size() - 1)
		array.resize(array.size() * 2);
	int hole = ++currentSize;
	if (isItMinHeap) //insert in increasing order
	{
		for (; hole > 1 && x < array[hole / 2]; hole /= 2)
			array[hole] = array[hole / 2];
	}
	else //added, insert in decreasing order
	{
		for (; hole > 1 && x > array[hole / 2]; hole /= 2) //modified
			array[hole] = array[hole / 2];
	}
	array[hole] = x;
}

//function that finds the median from the heaps
float FindMedian(MinHeap<int> minheap, MaxHeap<int> maxheap)
{
	float median;
	float maxheapsize = maxheap.findSize(); //getting the sizes of the heaps
	float minheapsize = minheap.findSize();
	if (minheapsize == maxheapsize) //if sizes are equal get the average of the top items
	{
		median = minheap.findTop() + maxheap.findTop();
		median = median / 2.0;
	}
	else if (minheapsize > maxheapsize) //if minheap has more elements
	{
		median = minheap.findTop(); //get the top element of minheap
	}
	else if (minheapsize < maxheapsize) //if maxheap has more elements
	{
		median = maxheap.findTop(); //get the top element of maxheap
	}
	return median;
}
//functions that inserts the number into the heap
void InserttoHeap(int num, MinHeap<int> & minheap, MaxHeap<int> & maxheap)
{
	float median;
	int sizedifference, maxheapsize, minheapsize;
	maxheapsize = maxheap.findSize();
	minheapsize = minheap.findSize();
	if (minheapsize == 0) //first element goes to the minheap to start
	{
		minheap.insert(num);
		minheapsize++;
	}
	else
	{
		median = FindMedian(minheap, maxheap); //get the current median
		if (num < median) //if the number is less than the median, goes to the maxheap
		{
			maxheap.insert(num);
			maxheapsize++;
		}
		else //if the number is greater or equal to the median, goes to the minheap
		{
			minheap.insert(num);
			minheapsize++;
		}
	}
	sizedifference = maxheapsize - minheapsize; //the size difference should be 0 or 1 in absolute value
	int topnum;
	if (sizedifference > 1) //if maxheap has more than 1 element extra
	{
		maxheap.deleteTop(topnum); //put the top element into the other heap
		minheap.insert(topnum);
	}
	if (sizedifference < -1) //same check for the minheap
	{
		minheap.deleteTop(topnum);
		maxheap.insert(topnum);
	}
}
//function that reads the numbers from the input file
void ReadFile(string filename, MinHeap<int> & minheap, MaxHeap<int> & maxheap)
{
	int num;
	string line;
	ifstream file;
	file.open(filename.c_str());
	istringstream iss;
	if (!file.fail()) //if the file is opened successfully
	{
		while (getline(file, line)) //read line by line
		{
			istringstream iss(line);
			while (iss >> num) //read each number
			{
				InserttoHeap(num,minheap,maxheap); //insert each number to one of the heaps
			}
		}
	}
}

//main part of the program
int main()
{
	MaxHeap<int> maxheap;
	MinHeap<int> minheap;
	string filename;
	cout << "Please enter the next filename that contains integer stream:";
	while (getline(cin, filename))
	{
		ReadFile(filename, minheap, maxheap); //processing the file
		cout << "current median: " << FindMedian(minheap, maxheap) << endl; //displaying the median
		cout << "Please enter the next filename that contains integer stream:";
	}
	system("pause");
	return 0;
}