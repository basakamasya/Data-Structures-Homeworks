//Basak Amasya
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

//all templated classes and necessary functions are taken from lecture slides and modified if necessary
template <class Object>
class List;     // Incomplete declaration.

template <class Object>
class ListItr;     // Incomplete declaration.

template <class Object> 
class ListNode //ListNode for List Class
{
	ListNode(const Object & theElement = Object(), ListNode * n = NULL)
		: element(theElement), next(n) { }  // Constructor 

	Object   element;
	ListNode *next;
	friend class List<Object>;
	friend class ListItr<Object>;
};

template <class Object>
class ListItr //List Iterator for List class
{
public:
	ListItr() : current(NULL) { }
	bool isPastEnd() const
	{
		return current == NULL;
	}
	void advance()
	{
		if (!isPastEnd())   current = current->next;
	}
	Object & retrieve()
	{
		if (isPastEnd())    return Object(); //modified
		return current->element;
	}
	ListNode<Object> *current;    // Current position //moved to public from private
	ListItr(ListNode<Object> *theNode) : current(theNode) { } //moved to public from private
private:
	friend class List<Object>;    // Grant access to constructor
};

template <class Object>
class List //List Class to be used for separate chaining method to resolve collisions in the hash table
{
public:
	List();
	List(const List & rhs);
	~List();
	bool isEmpty() const;
	void makeEmpty();
	ListItr<Object> zeroth() const;
	ListItr<Object> first() const;
	void insert(const Object & x, const ListItr<Object> & p);
	ListItr<Object> find(const Object & x) const;
	ListItr<Object> findPrevious(const Object & x) const;
	void remove(const Object & x);
	const List & operator=(const List & rhs);
private:
	ListNode<Object> *header;
};

template <class Object>
bool List<Object>::isEmpty() const
{
	return header->next == nullptr;
}

template <class Object>
List<Object>::List()
{
	header = new ListNode<Object>;
}

template <class Object>
List<Object>::~List()
{
	makeEmpty();
}

template <class Object>
List<Object>::List(const List<Object> & rhs)
{
	header = new ListNode<Object>;
	*this = rhs;  // Deep Copy
}

template <class Object>
void List<Object>::insert(const Object & x, const ListItr<Object> & p)
{
	if (p.current != NULL)
		p.current->next = new ListNode<Object>(x, p.current->next);
}

template <class Object>
ListItr<Object> List<Object>::find(const Object & x) const
{
	ListNode<Object> *itr = header->next; // Initialize
	while (itr != NULL && itr->element != x)
		itr = itr->next;

	return ListItr<Object>(itr);
}

template <class Object>
ListItr<Object> List<Object>::findPrevious(const Object & x) const
{
	ListNode<Object> *itr = header;
	while (itr->next != NULL && itr->next->element != x)
		itr = itr->next;

	return ListItr<Object>(itr);
}

template <class Object>
void List<Object>::remove(const Object & x)
{
	ListItr<Object> p = findPrevious(x); // Locate previous of x, if any
	if (p.current->next != NULL)
	{
		ListNode<Object> *oldNode = p.current->next;
		p.current->next = p.current->next->next;  // Bypass deleted node
		delete oldNode;
	}
}

template <class Object>
ListItr<Object> List<Object>::zeroth() const
{
	return ListItr<Object>(header);
}

template <class Object>
ListItr<Object> List<Object>::first() const
{
	return ListItr<Object>(header->next);
}

template <class Object>
void List<Object>::makeEmpty()
{
	while (!isEmpty())
		remove(first().retrieve());
}

template <class Object>
const List<Object> & List<Object>::operator=(const List<Object> & rhs) //overloading = operator for deep copy, added to the class
{
	if (this != &rhs)
	{
		makeEmpty();
		ListItr<Object> ritr = rhs.first();
		ListItr<Object> itr = zeroth();
		for (; !ritr.isPastEnd(); ritr.advance(), itr.advance())
			insert(ritr.retrieve(), itr);
	}
	return *this;
}

template <class HashedObj>
class HashTable //HashTable class
{
public:
	HashTable(const HashedObj & notFound, int size = 101);
	HashTable(const HashTable & rhs) : ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), theLists(rhs.theLists) { itemnumber = rhs.itemnumber; }
	const HashedObj & find(const string name, const HashedObj & x) const;
	void makeEmpty();
	void insert(const HashedObj & x);
	void remove(const HashedObj & x);
	const HashTable & operator=(const HashTable & rhs);
	void rehash();
	float itemnumber;
	vector<List<HashedObj>> theLists; // The array of Lists //moved to the public
private:
	const HashedObj ITEM_NOT_FOUND;
};

//taken from lecture slides, hash function uses Horner's method
int hashfunction(const string & key, int tableSize)
{
	int hashVal = 0;
	for (int i = 0; i < key.length(); i++)
		hashVal = 37 * hashVal + key[i];
	hashVal = hashVal % tableSize;
	if (hashVal < 0)
		hashVal = hashVal + tableSize;
	return(hashVal);
}

template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size)
	: ITEM_NOT_FOUND(notFound), theLists(nextPrime(size)) 
{
	itemnumber = 0;
}

template <class HashedObj>
void HashTable<HashedObj>::rehash() //rehash function modified for separate chaining method
{
	itemnumber = 0;
	vector<List<HashedObj>> oldArray = theLists;
	theLists.clear(); //clear the table
	theLists.resize(nextPrime(2 * oldArray.size())); 	// Create new double-sized, empty table
	for (int i = 0; i < oldArray.size(); i++) 	// Copy table over
	{
		ListItr<HashedObj> itr = oldArray[i].first();
		for (; !itr.isPastEnd(); itr.advance())
		{
			insert(itr.retrieve()); //insert each item
		}
	}
}

//function to check if the number is prime
bool isPrime(int n)
{
	if (n == 2 || n == 3)
		return true;
	if (n == 1 || n % 2 == 0)
		return false;
	for (int i = 3; i * i <= n; i += 2)
		if (n % i == 0)
			return false;
	return true;
}

//function to find the next prime given the number
int nextPrime(int n)
{
	if (n % 2 == 0)
		n++;
	for (; !isPrime(n); n += 2)
		;
	return n;
}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
	for (int i = 0; i < theLists.size(); i++)
		theLists[i].makeEmpty(); 	// destroy the lists but not the vector!
}

template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x)
{
	itemnumber++;
	string name = x.itemname; //get the string key for the hashfunction
	List<HashedObj> & whichList = theLists[hashfunction(name, theLists.size())]; // hash the given object and locate the list it should be on 
	// locate the object in the list (using List’s find)
	ListItr<HashedObj> itr = whichList.find(x);
	if (itr.isPastEnd())
	{
		whichList.insert(x, whichList.zeroth()); 	// insert the new item at the head of the list if not found!
	}
	else //modified
	{
		itr.retrieve().count++; //increase the count of the item if already existing
	}
	//rehash is not here since the hashtable is being traversed in other parts of the program
	//when traversing is done load factor is checked and rehash is called if necessary
}

template <class HashedObj>
void HashTable<HashedObj>::remove(const HashedObj & x)
{
	theLists[hashfunction(x, theLists.size())].remove(x); // remove from the appropriate list
}

template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find(const string name, const HashedObj & x) const
{
	ListItr<HashedObj> itr;
	itr = theLists[hashfunction(name, theLists.size())].find(x); // locate the approriate list and search there
	// retrieve from the located position
	if (itr.isPastEnd())
		return ITEM_NOT_FOUND;
	return itr.retrieve();
}

//added, overloading = operator so that it does deep copy
template <class HashedObj>
const HashTable<HashedObj> & HashTable<HashedObj>::operator=(const HashTable<HashedObj> & rhs)
{
	if (this != &rhs)
	{
		makeEmpty();
		ITEM_NOT_FOUND = rhs.ITEM_NOT_FOUND;
		theLists = rhs.theLists;
		itemnumber = rhs.itemnumber;
	}
	return *this;
}

//struct to keep Item properties together in the hash table
struct Item
{
	Item() {};
	Item(string name, int count, float supportvalue) //constructor
		: itemname(name), count(count), supportvalue(supportvalue) {}
	string itemname;
	int count; //count of the item
	float supportvalue; //suppotvalue of the item
	bool operator==(const Item &rhs) const //implementing == operator for the struct, needed for the hastable implementation
	{  
		return (itemname == rhs.itemname);  
	}
	bool operator!=(const Item &rhs) const //implementing != operator for the struct, needed for the hastable implementation
	{
		return ! (itemname == rhs.itemname);
	}
	const Item operator=(const Item &rhs) const //overloading =
	{
		Item newitem(rhs.itemname, rhs.count, rhs.supportvalue);
		return newitem;
	}
};

// struct to keep Rule properties together in the hash table
struct Rule
{
	Rule() {}
	Rule(string i1, string i2, int count)
		: item1(i1), item2(i2), count(count) 
	{
		itemname = item1 + " " + item2;
	}
	string item1; //left hand side of the implication
	string item2; //right hand side of the implication
	string itemname; //key for the hashfunction
	int count; //count of the pair
	bool operator==(const Rule &rhs) const //implementing == operator for the struct, needed for the hastable implementation
	{
		string name = rhs.item1 + rhs.item2;
		return ((item1 + item2) == name);
	}
	bool operator!=(const Rule &rhs) const //implementing != operator for the struct, needed for the hastable implementation
	{
		string name = rhs.item1 + rhs.item2;
		return !((item1 + item2) == name);
	}
};
//function that calculates support value
Item CalculateSupportValue(HashTable<Item> items, string item, int transno)
{
	double itemno, supportvalue;
	Item itemstruct(item, 0, 0);
	itemno = items.find(item, itemstruct).count; //get the count from the hashtable
	supportvalue = itemno / transno; //divide the count to the number of transactions
	Item newitem(item, itemno, supportvalue);
	return newitem;
}
//function that calculates confidence value
double CalculateConfidenceValue(HashTable<Item> lookup, HashTable<Rule> & rules, Rule rule, float transno)
{
	string itemname = rule.item1;
	Item item1(itemname, 0, 0);
	string pairname = rule.item1 + " " + rule.item2; //prepare the key
	double item1support = lookup.find(itemname, item1).supportvalue; //get the supportvalue of the LHS of the rule from the lookup table
	double pairsupport = rules.find(pairname, rule).count / transno; //get the count of the rule from the rules table and divide it to transactions number
	double confidencevalue = pairsupport / item1support; //calculate the confidence value
	return confidencevalue;
}
//function that checks the confidence threshold and writes the resulting rules to the file
void WriteResults(HashTable<Item> lookup, HashTable<Rule> & rules, double confidencethreshold, int transno)
{
	string p2item1, p2item2, p1item1, p1item2;
	ofstream file;
	int count = 0; //counter for number of rules
	double confidence;
	string filename = "results.txt"; 
	file.open(filename); //open the results.txt file
	for (int i = 0; i < rules.theLists.size(); i++) //start traversing the hashtable
	{
		ListItr<Rule> itr = rules.theLists[i].first();
		for (; !itr.isPastEnd(); itr.advance())
		{
			Rule rule = itr.retrieve(); //get the rule
			confidence = CalculateConfidenceValue(lookup, rules, rule, transno); //calculate confidence value
			//stringstream s;
			//s << setprecision(2) << fixed << confidence; //set the precision after . to 2
			//confidence = s;
			if (confidence >= confidencethreshold) //if value is greater or equal to the threshold
			{
				string item1 = rule.item1; //LHS of rule
				istringstream issp1(rule.item1);
				issp1 >> p1item1 >> p1item2;
				string item2 = rule.item2; //RHS of rule
				istringstream issp2(item2);
				issp2 >> p2item1 >> p2item2;
				if (p2item2 == "" && p1item2 == "") //if there are 2 items
				{
					file << item1 << ";" << item2 << ";";
				}
				else if (p2item2 == "" && p1item2 != "") //if there are 3 items
				{
					file << p1item1 << "," << p1item2 << ";" << item2 << ";";
				}
				else if (p2item2 != "" && p1item2 == "") //if there are 3 items
				{
					file << item1 << ";" << p2item1 << "," << p2item2 << ";";
				}
				else //if there are 4 items
				{
					file << p1item1 << "," << p1item2 << ";" << p2item1 << "," << p2item2 << ";";
				}
				stringstream s;
				s << setprecision(2) << fixed << confidence; //set the precision after . to 2
				file << s.str() << endl;
				count++; //increase the count
				p1item2 = p2item2 = ""; //empty the strings
			}
		}
	}
	file.close();

	if (count == 0) cout << "There is no rule for the given support and confidence values." << endl;
	else cout << count << " rules are written to results.txt" << endl;
}
//function that goes over the hashtable, calculates corresponding support values and checks the threshold
void CalculateAllSupportValuesCheckThreshold(double threshold, HashTable<Item> hashtable, int transno, HashTable<Item> & result)
{
	double supportvalue;
	for (int i = 0; i < hashtable.theLists.size(); i++) //traverse the hashtable
	{
		ListItr<Item> itr = hashtable.theLists[i].first();
		for (; !itr.isPastEnd(); itr.advance())
		{
			Item newitem = CalculateSupportValue(hashtable, itr.retrieve().itemname, transno); //calculate the supportvalue
			supportvalue = newitem.supportvalue;
			if (supportvalue >= threshold) //if greater or equal to threshold
			{
				result.insert(newitem); //put it into the resulting hashtable
			}
		}
	}
	float loadfactor = result.itemnumber / result.theLists.size(); //calculate the loadfactor
	if (loadfactor > 0.7)
	{
		result.rehash(); //rehash the hashtable since the loadfactor is above the threshold
	}
}
//function that finds the pair combinations of the items
void FindPairs(HashTable<Item> & pairs, HashTable<Item> & lookuponlyitems, int transno, double threshold)
{
	vector<string> history; //keeping a vector to check if permutation of that combination exist
	bool found = false;
	for (int i = 0; i < lookuponlyitems.theLists.size(); i++) //traverse the table for the first item
	{
		ListItr<Item> itrp1 = lookuponlyitems.theLists[i].first();
		for (; !itrp1.isPastEnd(); itrp1.advance())
		{
			for (int j = 0; j < lookuponlyitems.theLists.size(); j++) //traverse the table for the second item
			{
				ListItr<Item> itrp2 = lookuponlyitems.theLists[j].first();
				for (; !itrp2.isPastEnd(); itrp2.advance())
				{
					if (itrp1.retrieve().itemname != itrp2.retrieve().itemname)
					{
						for (int k = 0; k < history.size() && !found; k++) //for each item in the vector
						{
							istringstream iss(history[k]);
							string p1, p2;
							iss >> p1 >> p2; //check if the permutation of this combination exist
							if ((p2 == itrp1.retrieve().itemname && p1 == itrp2.retrieve().itemname)) //check if the permutation of this combination exist
							{
								found = true;
							}
						}
						if (!found) //if not already found
						{
							string pair = itrp1.retrieve().itemname + " " + itrp2.retrieve().itemname;
							Item pairstruct(pair, 0, 0);
							pairs.insert(pairstruct); //insert the pair to the hastable
							history.push_back(pair);
						}
					}
					found = false;
				}
			}
		}
	}
	float loadfactor = pairs.itemnumber / pairs.theLists.size(); //calculate the loadfactor
	if (loadfactor > 0.7)
	{
		pairs.rehash(); //rehash the hashtable since the loadfactor is above the threshold
	}
}
//function that adds the pairs to the lookup table
void CreateLookUpTableWithPairs(HashTable<Item> & lookuponlyitems, int transno, double threshold, vector<vector<string>> & transactions)
{
	Item notfound("", -1, -1);
	HashTable<Item> pairs(notfound, 101);
	FindPairs(pairs,lookuponlyitems,transno,threshold); //find all pairs
	string line, item, pair;
	for (int t = 0; t < transactions.size(); t++) //instead of reading the file again, using a vector for transactions
	{
		for (int i = 0; i < transactions[t].size(); i++) //for each transaction
		{
			for (int j = 0; j < transactions[t].size(); j++)
			{
				if (i != j)
				{
					pair = transactions[t][i] + " " + transactions[t][j];
					Item tobesearched(pair, -1, -1);
					Item found = pairs.find(pair, tobesearched); //search for the pair in the hashtable
					if (found.itemname != "") //if the pair is in the hastable
					{
						pairs.insert(tobesearched); //insert will increase the count by 1
						float loadfactor = pairs.itemnumber / pairs.theLists.size();
						if (loadfactor > 0.7) //check the loadfactor
						{
							pairs.rehash();
						}
					}
				}
			}
		}
	}
	CalculateAllSupportValuesCheckThreshold(threshold, pairs, transno, lookuponlyitems); //calculate the tables supportvalues
}
//function that reads the file and creates the hastable for items
HashTable<Item> CreateItemsTable(ifstream & file, int & transno, double supportthreshold, vector<vector<string>> & transactions)
{
	transno = 0;
	Item notfound("",-1,-1);
	HashTable<Item> items(notfound, 101);
	string line, item;
	float loadfactor;
	while (getline(file, line)) //read the file line by line
	{
		vector<string> transaction; //for each line create a vector
		istringstream iss(line);
		while (iss >> item) //read the line item by item
		{
			transaction.push_back(item); //put the item to the vector
			Item structitem(item,1,0); //create the item struct with count number 1
			items.insert(structitem); //insert the item to the table
			loadfactor = items.itemnumber / items.theLists.size();
			if (loadfactor > 0.7) //check the load factor
			{
				items.rehash();
			}
		}
		transactions.push_back(transaction); //put the transaction into transactions
		transno++;
	}
	HashTable<Item> lookuponlyitems(notfound, 101); //create the lookup table
	CalculateAllSupportValuesCheckThreshold(supportthreshold, items, transno, lookuponlyitems); //lookuptable with only items
	CreateLookUpTableWithPairs(lookuponlyitems, transno, supportthreshold, transactions); //add the pairs

	return lookuponlyitems;
}

//function that gets the input file name from the user and opens the file
void GetFile(ifstream & file)
{
	string filename, line;
	cout << "Please enter the transaction file name: ";
	cin >> filename;
	file.open(filename.c_str());
	while (file.fail()) //keep asking for the file until a valid one is entered
	{
		cout << "Couldn't open file. Please enter again: ";
		cin >> filename;
		file.open(filename.c_str()); //open the file
	}
}
//function that calculates how many times that rule is seen in the transactions
void CalculateCountOfRules(HashTable<Item> lookup, vector<vector<string>> transactions, HashTable<Rule> & rules)
{
	string p2item1, p2item2, p1item1, p1item2;
	for (int t = 0; t < transactions.size(); t++) //for each transaction
	{
		for (int h = 0; h < rules.theLists.size(); h++) //for each rule
		{
			ListItr<Rule> itrp1 = rules.theLists[h].first();
			for (; !itrp1.isPastEnd(); itrp1.advance())
			{
				Rule rule = itrp1.retrieve(); //get the rule
				string item1 = rule.item1; //get item1
				istringstream issp1(item1);
				issp1 >> p1item1 >> p1item2; //parse the items in case they are a pair
				string item2 = rule.item2; //get item2
				istringstream issp2(item2);
				issp2 >> p2item1 >> p2item2; //parse the items in case they are a pair
				for (int i = 0; i < transactions[t].size(); i++) //for each item in the transaction
				{
					for (int j = 0; j < transactions[t].size(); j++) //for each item in the transaction
					{
						if (transactions[t][i] == item1 && transactions[t][j] == item2)
						{//if there are 2 items
							rules.insert(rule); //insert function will increase the count of the rule
						}
						else if (p1item2 == "" && p2item2 != "" && transactions[t][i] == item1 && transactions[t][j] == p2item1)
						{//if there are 3 items
							for (int k = 0; k < transactions[t].size(); k++)
							{
								if (transactions[t][k] == p2item2)
								{
									rules.insert(rule);
								}
							}
						}
						else if (p1item2 != "" && p2item2 == "" && transactions[t][i] == item2 && transactions[t][j] == p1item1)
						{//if there are 3 items
							for (int k = 0; k < transactions[t].size(); k++)
							{
								if (transactions[t][k] == p1item2)
								{
									rules.insert(rule);
								}
							}
						}
						else if (p1item2 != "" && p2item2 != "" && transactions[t][i] == p1item1 && transactions[t][j] == p1item2)
						{//if there are 4 items
							for (int k = 0; k < transactions[t].size(); k++)
							{
								if (transactions[t][k] == p2item1)
								{
									for (int l = 0; l < transactions[t].size(); l++)
									{
										if (transactions[t][l] == p2item2)
										{
											rules.insert(rule);
										}
									}
								}
							}

						}
					}
				}
				p2item2 = p1item2 = ""; //empty the strings
			}
		}
		float loadfactor = rules.itemnumber / rules.theLists.size();
		if (loadfactor > 0.7) //check the loadfactor
		{
			rules.rehash();
		}
	}
}
//function that finds all of the implication rules from the lookuptable
void FindImplications(HashTable<Item> & lookuptable, HashTable<Rule> & rules)
{
	string p1, p2, p1item1, p1item2, p2item1, p2item2;
	for (int i = 0; i < lookuptable.theLists.size(); i++) //go over the lookup hashtable
	{
		ListItr<Item> itrp1 = lookuptable.theLists[i].first();
		for (; !itrp1.isPastEnd(); itrp1.advance())
		{
			p1 = itrp1.retrieve().itemname; //get the first item
			istringstream issp1(p1);
			issp1 >> p1item1 >> p1item2;
			for (int j = 0; j < lookuptable.theLists.size(); j++)
			{
				ListItr<Item> itrp2 = lookuptable.theLists[j].first();
				for (; !itrp2.isPastEnd(); itrp2.advance())
				{
						p2 = itrp2.retrieve().itemname; //get the second item
						istringstream issp2(p2);
						issp2 >> p2item1 >> p2item2;
						if ((p1item2 == "" && p1item1 != p2item1 && p1item1 != p2item2) 
							|| (p2item2 == "" && p1item1 != p2item1 && p1item2 != p2item1) 
							|| (p1item2 != "" && p2item2 != "" && p1item1 != p2item1 && p1item1 != p2item2 && p1item2 != p2item1 && p1item2 != p2item2))
						{ //if both sides of the implication is unique
							Rule rule(p1, p2, 0); //create the rule
							rules.insert(rule); //insert the rule
						}
						p2item2 = ""; //empty the string
				}
			}
			p1item2 = ""; //empty the string
		}
	}
}

//main part of the program
int main()
{
	int transno;
	ifstream file;
	double supthreshold, confhreshold;
	vector<vector<string>> transactions; //vector to store transactions
	Rule notfound("", "", -1);
	HashTable<Rule> rules(notfound, 101); //hashtable for the rules

	GetFile(file); //getting the file name and opening it
	cout << "Please enter support and confidence values between 0 and 1:";
	cin >> supthreshold >> confhreshold; //getting the threshold values

	HashTable<Item> lookuptable = CreateItemsTable(file, transno,supthreshold, transactions); //creating lookup hashtable
	FindImplications(lookuptable, rules); //finding all implications
	CalculateCountOfRules(lookuptable, transactions, rules); //finding confidence values
	WriteResults(lookuptable,rules,confhreshold,transno); //writing the resulting rules to the file

	system("pause");
	return 0;
}