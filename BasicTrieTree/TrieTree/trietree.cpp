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
	bool wordMarker() { return m_marker; }
	node_map children() { return m_children; }
	NodePtr findChild(const char c);
	NodePtr appendChild(char c);
};

NodePtr Node::findChild(const char c)
{
	node_map::const_iterator iter = m_children.find(c);
	if(iter != m_children.end()){
		return iter->second;
	}

	return NULL;
}

NodePtr Node::appendChild(const char c)
{
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

public:
	Trie(){ m_root = new Node(); };
	~Trie(); //TODO: not sure if I should recursively delete every node?

	void addWord(std::string s);
	void deleteWord(const std::string s){/*TODO*/};
	bool searchWord(const std::string s);
	void preOrderTraverse(const NodePtr m_root,void (Trie::*handleNode)(NodePtr & current_node));
	void postOrderTraverse(const NodePtr m_root,void (Trie::*handleNode)(NodePtr & current_node));
	void getAllWords(); 
	void printAllWords();
};

Trie::~Trie(){
	postOrderTraverse(m_root,&Trie::deleteNode);
}

void Trie::deleteNode(NodePtr & current_node){
	delete current_node;
}

void Trie::printAllWords(){
	getAllWords();

	for(unsigned int i=0;i<m_all_words.size();++i)
		std::cout<< m_all_words.at(i) <<std::endl;
}

void Trie::saveWord(NodePtr & current_node){
	//only suitable for preorder
	m_tmp_string += current_node->content();
	if( current_node->wordMarker() ){
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

void Trie::getAllWords()
{
	preOrderTraverse(m_root,&Trie::saveWord);
}


void Trie::addWord(std::string s)
{
	NodePtr current = m_root;

	for (unsigned int i = 0; i < s.length(); ++i )
	{   
		NodePtr child = current->findChild(s[i]);
		if ( child != NULL )
			current = child;
		else
			current = current->appendChild(s[i]);
	}

	if(current != m_root)
		current->setWordMarker();
}

bool Trie::searchWord(const std::string s)
{
	NodePtr current = m_root;

	if ( current != NULL )
	{
		for (unsigned int i = 0; i < s.length(); i++ )
		{
			current = current->findChild(s[i]);
			if ( current == NULL )	return false;
		}

		return current->wordMarker();
	}

	return false;
}


// Test program
int main()
{
	Trie trie;
	trie.addWord("Hello");
	trie.addWord("Helloo");
	trie.addWord("Balloon");
	trie.addWord("Ball");
	trie.addWord("");

	if ( trie.searchWord("Hell") )
		cout << "Found Hell" << endl;

	if ( trie.searchWord("Hello") )
		cout << "Found Hello" << endl;

	if ( trie.searchWord("Helloo") )
		cout << "Found Helloo" << endl;

	if ( trie.searchWord("Ball") )
		cout << "Found Ball" << endl;

	if ( trie.searchWord("Balloon") )
		cout << "Found Balloon" << endl;

	if(trie.searchWord(""))
		cout<<"Found ''"<<endl;
	else
		cout<<"Failed to insert ''"<<endl;


	cout<<"All words:"<<endl;
	trie.printAllWords();

	system("pause");
}
