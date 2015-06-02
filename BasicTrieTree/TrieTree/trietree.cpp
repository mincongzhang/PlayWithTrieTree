#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class Node;
typedef Node* NodePtr;
typedef std::map<char,NodePtr> node_map;
typedef node_map::value_type node_value;


/////////////////////Node///////////////////////////////
class Node {
private:
	char m_content;
	bool m_marker;
	node_map m_children; //use hashmap or map to save children for efficient search

public:
	Node() { m_content = ' '; m_marker = false; }
	~Node() {}

	char content() { return m_content; }
	void setContent(const char c) { m_content = c; }
	void setWordMarker() { m_marker = true; }
	void removeWordMarker(){m_marker = false;}
	bool wordMarker() { return m_marker; }
	node_map children() { return m_children; }
	NodePtr findChild(const char c);
	NodePtr appendChild(char c);
};

NodePtr Node::findChild(const char c){
	node_map::const_iterator iter = m_children.find(c);
	if(iter != m_children.end()){
		return iter->second;
	}

	return NULL;
}

NodePtr Node::appendChild(const char c){
	NodePtr child = new Node();
	child->setContent(c);
	m_children.insert(node_value(c,child)); 
	return child;
}
//////////////////////////////////////////////////////////


class Trie {
private:
	NodePtr m_root;
	std::vector<std::string> m_all_words;//TODO: when would you clear this vector?
	std::string m_tmp_string;
	void deleteNode(NodePtr & current_node);
	void saveWord(NodePtr & current_node);//TODO: use a functor here to avoid m_all_words?
	void preOrderTraverse(const NodePtr m_root,void (Trie::*handleNode)(NodePtr & current_node));
	void postOrderTraverse(const NodePtr m_root,void (Trie::*handleNode)(NodePtr & current_node));

public:
	Trie(){ m_root = new Node(); };
	~Trie();
	void addWord(std::string s);
	bool deleteWord(const std::string s);
	bool search(const std::string s);
	void getAllWords(); 
	void printAllWords();
};

Trie::~Trie(){
	postOrderTraverse(m_root,&Trie::deleteNode);
}

void Trie::deleteNode(NodePtr & current_node){
	cout<<"deleting ["<<current_node->content()<<"]"<<endl;
	delete current_node;
}

void Trie::saveWord(NodePtr & current_node){
	//only suitable for preorder
	m_tmp_string += current_node->content();

	cout<<"Constructing word:["<<m_tmp_string<<"]"<<endl;
	if( current_node->wordMarker() ){
		cout<<"Word construction complete :["<<m_tmp_string<<"]"<<endl;
		m_all_words.push_back(m_tmp_string);
		//if get end, clear buffer
		if(current_node->children().empty())
			m_tmp_string.clear();
	}

}

void Trie::preOrderTraverse(NodePtr current_node,void (Trie::*handleNode)(NodePtr & current_node)){
	if(!current_node) return;

	node_map current_children = current_node->children();

	node_map::iterator iter;
	for(iter = current_children.begin();iter!=current_children.end();++iter){
		(this->*handleNode)(iter->second);
		preOrderTraverse(iter->second,handleNode);
	}
}

void Trie::postOrderTraverse(NodePtr current_node,void (Trie::*handleNode)(NodePtr & current_node)){
	if(!current_node) return;

	node_map current_children = current_node->children();

	node_map::iterator iter;
	for(iter = current_children.begin();iter!=current_children.end();++iter){
		postOrderTraverse(iter->second,handleNode);
		(this->*handleNode)(iter->second);
	}
}

void Trie::addWord(std::string s){
	NodePtr current = m_root;

	for (unsigned int i = 0; i < s.length(); ++i ){   
		NodePtr child = current->findChild(s[i]);
		if ( child == NULL )
			current = current->appendChild(s[i]);
		else
			current = child;
	}

	if(current != m_root)
		current->setWordMarker();
}

bool Trie::search(const std::string s){
	NodePtr current = m_root;

	for (unsigned int i = 0; i < s.length(); i++ ){
		current = current->findChild(s[i]);
		if ( current == NULL )return false;
	}

	return current->wordMarker();
}

bool Trie::deleteWord(const std::string s){
	NodePtr current_node = m_root;

	for (unsigned int i = 0; i < s.length(); i++ ){
		current_node = current_node->findChild(s[i]);
		if ( current_node == NULL )return false;
	}

	if(!current_node->wordMarker())
		return false;

	//simple delete:change wordMarker to false
	current_node->removeWordMarker();
	return true;

	//TODO: delete previous_node father node until meet a wordMarker
	//better to have a virtual tree. i.e.Radix tree
}


void Trie::getAllWords(){
	preOrderTraverse(m_root,&Trie::saveWord);
}

void Trie::printAllWords(){
	getAllWords();

	cout<<"All words acquired. Printing:"<<endl;
	for(unsigned int i=0;i<m_all_words.size();++i)
		std::cout<<"["<< m_all_words.at(i) <<"]"<<std::endl;
}


// Test program
int main()
{

	{
		Trie trie;
		trie.addWord("Hello");
		trie.addWord("Helloo");
		trie.addWord("Balloon");
		trie.addWord("Ball");
		trie.addWord("");

		if ( trie.search("Hell") )
			cout << "Found Hell" << endl;

		if ( trie.search("Hello") )
			cout << "Found Hello" << endl;

		if ( trie.deleteWord("Hello") )
			cout << "Hello deleted" << endl;

		if ( trie.search("Helloo") )
			cout << "Found Helloo" << endl;

		if ( trie.search("Ball") )
			cout << "Found Ball" << endl;

		if ( trie.search("Balloon") )
			cout << "Found Balloon" << endl;

		if(trie.search(""))
			cout<<"Found ''"<<endl;
		else
			cout<<"Failed to insert ''"<<endl;


		cout<<"Printing all words..."<<endl;
		trie.printAllWords();
	}

	system("pause");
}
