#include <iostream>
#include <cstring>

#define ENDCSS '?'
#define BEGCSS '*'
//#define EOF '~'
#define TT 8

using namespace std;



class MyString {
public:
    // Constructors
    MyString();                         // Default constructor
    MyString(const char* str);          // Constructor with C-string input
    MyString(const MyString& other);    // Copy constructor
    MyString(MyString&& other);         // Move constructor

    // Destructor
    ~MyString();

    // Assignment operators
    MyString& operator=(const MyString& other);   // Copy assignment
    MyString& operator=(MyString&& other);        // Move assignment

    // Member functions
    int length() const;                // Returns the length of the string
    const char* c_str() const;         // Returns the C-string representation
    void append(const char* str);     // Appends a C-string to the string
    void clear();                     // Clears the string
    friend std::istream& operator>>(std::istream& is, MyString& myString);
    
    char& operator[](int index);
    const char& operator[](int index) const;

private:
    char* data_;   // Pointer to the dynamically allocated character array
    int length_;   // Length of the string
};

// Default constructor
MyString::MyString() : data_(nullptr), length_(0) {}

// Constructor with C-string input
MyString::MyString(const char* str) {
    length_ = strlen(str);
    data_ = new char[length_ + 1];
    strcpy(data_, str);
}

// Copy constructor
MyString::MyString(const MyString& other) {
    length_ = other.length_;
    data_ = new char[length_ + 1];
    strcpy(data_, other.data_);
}

// Move constructor
MyString::MyString(MyString&& other) {
    length_ = other.length_;
    data_ = other.data_;
    other.length_ = 0;
    other.data_ = nullptr;
}

// Destructor
MyString::~MyString() {
    delete[] data_;
}

// Copy assignment operator
MyString& MyString::operator=(const MyString& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data_;
    length_ = other.length_;
    data_ = new char[length_ + 1];
    strcpy(data_, other.data_);

    return *this;
}

// Move assignment operator
MyString& MyString::operator=(MyString&& other) {
    if (this == &other) {
        return *this;
    }

    delete[] data_;
    length_ = other.length_;
    data_ = other.data_;
    other.length_ = 0;
    other.data_ = nullptr;

    return *this;
}

// Returns the length of the string
int MyString::length() const {
    return length_;
}

// Returns the C-string representation
const char* MyString::c_str() const {
    return data_;
}

// Appends a C-string to the string
void MyString::append(const char* str) {
    int newLength = length_ + strlen(str);
    char* newData = new char[newLength + 1];
    strcpy(newData, data_);
    strcat(newData, str);
    delete[] data_;
    data_ = newData;
    length_ = newLength;
}

// Clears the string
void MyString::clear() {
    delete[] data_;
    data_ = nullptr;
    length_ = 0;
}
std::istream& operator>>(std::istream& is, MyString& myString) {
    char buffer[1024];
    is >> buffer;
    myString.clear();

    // Allocate new memory for the string data
    myString.length_ = strlen(buffer);
    myString.data_ = new char[myString.length_ + 1];

    // Copy the input buffer to the string data
    strcpy(myString.data_, buffer);

    return is;
}
char& MyString::operator[](int index) {
    if (index >= 0 && index < length_) {
        return data_[index];
    }
    else {
        // Throw an exception for out-of-bounds access
        throw std::out_of_range("Index out of range");
    }
}

// Overload [] operator to provide const access to individual characters
const char& MyString::operator[](int index) const {
    if (index >= 0 && index < length_) {
        return data_[index];
    }
    else {
        // Throw an exception for out-of-bounds access
        throw std::out_of_range("Index out of range");
    }
}


template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(T data) : data(data), prev(nullptr), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int size;
public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    void add(T data) {
        Node* node = new Node(data);
        if (head == nullptr) {
            head = node;
            tail = node;
        }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        size++;
    }

    void add(const LinkedList<T>& list) {
        Node* current = list.head;
        while (current) {
            add(current->data);
            current = current->next;
        }
    }

    void add(LinkedList<T>* list) {
        add(*list);
    }

    template <typename U>
    void add(LinkedList<U>* list) {
        Node* current = list->head;
        LinkedList<T>* newList = new LinkedList<T>();
        while (current) {
            newList->add(current->data);
            current = current->next;
        }
        add(newList);
    }

    template <typename U, size_t N>
    void add(U(&arr)[N]) {
        /*for (size_t i = 0; i < N; i++) {
            add(arr[i]);
        }*/
        T* newArray = new T[N]; // Allocate memory for new array
        std::copy(std::begin(arr), std::end(arr), newArray); // Copy array elements to new memory location
        Node* node = new Node(newArray); // Create a new node with the array as data
        if (head == nullptr) {
            head = node;
            tail = node;
        }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        size++;
    }
    

    T& operator[](int index) {
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    template <typename U = T>
    typename std::enable_if<!std::is_array<U>::value, U>::type get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head;
        int i = 0;
        while (i < index) {
            current = current->next;
            i++;
        }

        if (std::is_same<T, LinkedList<T>*>::value) {
            return static_cast<U>(current->data);
        }
        else {
            return current->data;
        }
    }

    template <typename U = T>
    typename std::enable_if<std::is_array<U>::value, U>::type get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head;
        int i = 0;
        while (i < index) {
            current = current->next;
            i++;
        }

        return current->data;
    }


    int getSize() const {
        return size;
    }

    void clear() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    template<typename U>
    LinkedList& operator=(const LinkedList<U>& other) {
        if constexpr (std::is_same_v<T, U>) {
            if (this != &other) {
                clear();
                Node* current = other.head;
                while (current) {
                    if constexpr (std::is_same_v<T, LinkedList<T>*> && std::is_same_v<U, LinkedList<U>*>) {
                        this->add((T)(new LinkedList<T>(*((LinkedList<T>*) current->data))));
                    }
                    else {
                        this->add(current->data);
                    }
                    current = current->next;
                }
            }
        }
        return *this;
    }

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const LinkedList<U>& list);
};


struct Section {
	int commandQue;
	LinkedList<char> section;
};

class CSS {
	LinkedList<LinkedList<char>> fullCSS;
	LinkedList<LinkedList<char>> fullCom;
	LinkedList<Section*> sectionsList;
    

	void populateLists(MyString inputArray,int inputSize) {
		bool inCSS = true; // track if we're currently in CSS or command section
		LinkedList<char> firstListNode;
		LinkedList<char> firstListComNode;
		fullCSS.add(firstListNode);
		int nodeCount = 0;
		int nodeComCount = -1;
		for (int i = 0; i < inputSize; i++) {
			if (inputArray[i] == ENDCSS && inputArray[i + 1] == ENDCSS && inputArray[i + 2] == ENDCSS && inputArray[i + 3] == ENDCSS) {
				inCSS = false;
				nodeComCount++;
				LinkedList <char> newComLinkedList;
				fullCom.add(newComLinkedList);
				i += 4;
			}
			
			if (inputArray[i] == BEGCSS && inputArray[i + 1] == BEGCSS && inputArray[i + 2] == BEGCSS && inputArray[i + 3] == BEGCSS) {
				inCSS = true;
				nodeCount++;
				LinkedList <char> newLinkedList;
				fullCSS.add(newLinkedList);
				i += 4;

			}
			if (inCSS && inputArray[i]!=EOF) {
				fullCSS[nodeCount].add(inputArray[i]);
			}
			else if(inputArray[i] != EOF){
				fullCom[nodeComCount].add(inputArray[i]);
			}
		}
	}
    


	

public:
	CSS(MyString inputArray, int inputSize) {
		populateLists(inputArray, inputSize);
	}
    void cssSpliter() {
        int k = 0;
        int cssPos = 0;
        while (k < fullCSS.getSize()) {
            Section section[TT];
            for (int i = 0; i < TT; i++) {
                do {
                    section[i].section.add(fullCSS[k][cssPos]);
                    section[i].commandQue = k;
                    cssPos++;
                } while (fullCSS[k][cssPos] != '}');
                if (cssPos == fullCSS[k].getSize() - 1) {
                    k++;
                    cssPos = 0;
                    if (k == fullCSS.getSize())
                        i = TT;
                }

            }
            sectionsList.add(section);
        }

    }

	
    void outputSection() {
        
        cout << sectionsList[0][0].section;
    }
};


MyString readInput() {
    MyString c;
	while (!cin.eof()) {
		cin >> c;
	}
	return c;
}

void inputTransfer(LinkedList<char> input, char* inputArray) {

	for (int i = 0; i < input.getSize(); i++) {
		inputArray[i] = input[i];
	}
}
int main() {
    MyString input = " ";
    input = readInput();
    CSS css(input, input.length());
    css.cssSpliter();
    css.outputSection();
    cout << "\n";
    return 0;
}


template<typename U>
std::ostream& operator<<(std::ostream& os, const LinkedList<U>& list) {
	if (list.getSize() != 0) {
		os << "{ ";
		for (int i = 0; i < list.getSize(); i++) {
			os << list.get(i);
		}
		os << " }";
	}
	else
		os << "{}";
	return os;
}