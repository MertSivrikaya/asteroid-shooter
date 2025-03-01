#ifndef _LINKED_LIST_
#define _LINKED_LIST_

template <class T>
class LinkedList
{
    public:
        LinkedList() : head(nullptr) {};
        ~LinkedList();
        LinkedList(const LinkedList<T> & rhs);
        LinkedList<T> & operator=(const LinkedList<T> & rhs);

        void addNode(T * data);
        T * removeNode(T * data);
        void removeAll();

    protected:
        class Node
        {
            public:
                Node(T * data, Node * next) : data(data), next(next) {};
                Node(T * data) : data(data), next(nullptr) {};

                T * data;
                Node * next;
        };

        Node * head;
};

template <class T>
LinkedList<T>::~LinkedList()
{
    Node * curr = head;
    while (curr)
    {
        Node * temp = curr->next;

        if (curr->data)
            delete curr->data;

        delete curr;
        curr = temp;
    }

    head = nullptr;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T> & rhs)
{
    Node *currThis = nullptr, *currRHS = rhs.head;
    while (currRHS)
    {
        if (!currThis)
        {
            head = new Node(currRHS->data);
            currThis = head;
        }    
        else
        {
            currThis->next = new Node(currRHS->data);
            currThis = currThis->next;
        }
        
        currRHS = currRHS->next;
    }
}

template <class T>
LinkedList<T> & LinkedList<T>::operator=(const LinkedList<T> & rhs)
{
    if (this != &rhs)
    {
        Node * curr = head;
        while (curr)
        {
            Node * temp = curr->next;
            delete curr;
            curr = temp;
        }

        head = nullptr;

        Node *currThis = nullptr, *currRHS = rhs.head;
        while (currRHS)
        {
            if (!currThis)
            {
                head = new Node(currRHS->data);
                currThis = head;
            }    
            else
            {
                currThis->next = new Node(currRHS->data);
                currThis = currThis->next;
            }
            
            currRHS = currRHS->next;
        }
    }

    return *this;
}

template <class T>
void LinkedList<T>::addNode(T * data)
{
    head = new Node(data, head);
}

template <class T>
T * LinkedList<T>::removeNode(T * data)
{
    Node *curr = head, *prev = nullptr;
    T * removedData = nullptr;

    while (curr)
    {
        if (curr->data == data) 
        {
            removedData = curr->data;
            break;
        }

        prev = curr;
        curr = curr->next;
    }

    if (!curr) // if not in the list
        return nullptr;

    if (prev)
    {
        Node * temp = curr->next;
        delete curr;
        prev->next = temp;
    }
    else
    {
        Node * temp = curr->next;
        delete curr;
        head = temp;
    }

    return removedData;
}

template <class T>
void LinkedList<T>::removeAll()
{
    Node *curr = head;
    while (curr)
    {
        Node *temp = curr->next;

        if (curr->data)
            delete curr->data;

        delete curr;
        curr = temp;
    }

    head = nullptr;
}

#endif