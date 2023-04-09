#include <iostream>

#define ENDCSS '?'
#define BEGCSS '*'
#define EOF '~'
#define TT 8

using namespace std;






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
    

	void populateLists(LinkedList<char> inputArray,int inputSize) {
		bool inCSS = true; // track if we're currently in CSS or command section
		LinkedList<char> firstListNode;
		LinkedList<char> firstListComNode;
		fullCSS.add(firstListNode);
		int nodeCount = 0;
		int nodeComCount = -1;
		for (int i = 0; i < inputSize; i++) {
            if (i + 4 < inputSize) {
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
	CSS(LinkedList<char> inputArray, int inputSize) {
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


void readInput(LinkedList<char> list) {
     
     char k=' ';
	while (k!=EOF) {
        cin >> k;
        list.add(k);
	}
}



int main() {
    LinkedList <char> input;
    readInput(input);
    CSS css(input, input.getSize());
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