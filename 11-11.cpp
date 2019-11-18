//============================================================================
// Name        : 11-11.cpp
// Author      : Michaela Byrd
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

class MyObject{
private:
	string _text;
public:
	MyObject(){
		_text = "";
	}
	MyObject(string text){
		_text = text;
	}
	void setText(string text){
		_text = text;
	}
	string getText(){
		return _text;
	}
};

class ObjNode{
private:
	MyObject _data;
	ObjNode* _next;
public:
	ObjNode(){
		_data = MyObject();
		_next = nullptr;
	}
	ObjNode(string text){
		_data = MyObject(text);
		_next = nullptr;
	}
	ObjNode(MyObject object, ObjNode* node): _data(object), _next(node){}
	ObjNode(string m, ObjNode* node): _data(MyObject(m)), _next(node){}
	~ObjNode(){
		delete(_next);
	}
	friend class ObjectRep;
	friend class ObjectCtr;
};

class ObjectRep{
private:
	ObjNode* _head;
	int** _count;
	int _ctr;
	int _rctr;
	int _size;
public:
	ObjectRep(): _head(nullptr), _count(nullptr), _ctr(0), _rctr(1), _size(0){}

	//pre: parameter must be greater than or equal to 1
	ObjectRep(int length){
		_head = new ObjNode("", _head);
		for (int i=1; i<length; i++){
			_head = new ObjNode("", _head);
		}
		_count = new int*[length];
		_rctr = 1;
		_ctr = 0;
		_size = length;
	}
	ObjectRep(const ObjectRep& copy){
		_rctr = 1;
		_ctr = copy._ctr;
		_size = copy._size;
		if (copy._head==nullptr){
			_head = nullptr;
			_count = nullptr;
			return;
		}

		_count = new int*[copy._size];
		if (copy._count[0]==nullptr){
			_head = new ObjNode();
		}
		else{
			_count[0] = new int (*(copy._count[0]));
			_head = new ObjNode(copy._head->_data.getText(), nullptr);
		}
		ObjNode* prev = _head;
		ObjNode* temp = _head->_next;
		for (int i=1; i<_size; i++){
			if (copy._count[i]!=nullptr){
				_count[i] = new int (*(copy._count[i]));
				prev->_next = new ObjNode (temp->_data.getText(), nullptr);
			}
			else{
				prev->_next = new ObjNode();
			}
			prev = prev->_next;
			temp = temp->_next;
		}
	}
	~ObjectRep(){
		free();
	}
	void free(){
		if (_head!=nullptr){
			delete (_head);
		}
		if (_count!=nullptr){
			for (int i=0; i<_size; i++){
				delete(_count[i]);
			}
			delete(_count);
		}
	}
	//post: adds a node to the end of the current representation
	void addNode(string newtext, int newcount){
		if (_ctr==_size)
			return;
		ObjNode* temp = _head;
		for (int i=0; i<_ctr && temp!=nullptr; i++){
			temp = temp->_next;
		}
		temp->_data.setText(newtext);
		_count[_ctr] = new int(newcount);
		_ctr++;
	}
	void print(){
		ObjNode* temp = _head;
		cout << "rctr = " << _rctr << endl;
		for (int i=0; i<_ctr; i++){
			cout << "Location " << i+1 << "\t" << temp->_data.getText() << " : " << *_count[i] << endl;
			temp = temp->_next;
		}
	}
	friend class Inv;
};

class ObjectCtr{

};

class Inv{
private:
	ObjectRep* _rep;
public:
	Inv(): _rep(){}
	Inv(int length): _rep(new ObjectRep(length)){}
	Inv(const Inv& old){
		_rep = old._rep;
		_rep->_rctr++;
	}
	void unique(){
		if (_rep->_rctr==1){
			return;
		}
		_rep->_rctr--;
		_rep = new ObjectRep (*_rep);
	}
	void addNode(string newtext, int newcount){
		unique();
	}
};

int main() {
	cout << "Arugula" << endl; // prints Arugula
	ObjectRep test(3);
	test.addNode("cool", 5);
	test.addNode("story", 7);
	test.addNode("bro", 17);
	test.print();

	ObjectRep a;
	ObjectRep b = a;
	a.print();
	b.print();
	return 0;
}
