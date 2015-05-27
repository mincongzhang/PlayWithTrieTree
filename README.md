# playWithTrieTree

A better example:
http://songlee24.github.io/2015/05/09/prefix-tree/

An idea:
Use DP theory to implement an advanced Trie Tree, to keep memory as low as possible  
Already exists? http://dongxicheng.org/structure/trietree/

Extra:
AC自动机?Aho-Corasick automation  
http://www.cppblog.com/mythit/archive/2009/04/21/80633.html

Trie Tree缺点是内存消耗太大。但是，如果使用自动机，就可以在保持Trie树的功能与性能的同时，大大降低内存用量（十几倍，甚至几十倍，几百倍），但是因为这样的自动机实现太困难，工业上的应用很少。不过，现在已经有成熟的实现了：把自动机用作 Key-Value 存储
