#ifndef SJTU_PQ_tree_HPP
#define SJTU_PQ_tree_HPP

#include <climits>
#include <cstddef>
#include <vector>
#include <algorithm>
namespace sjtu {
class PQ_tree{
private:
	struct Node{
		int id;//0:inner node;     1-n:leaf
		int size;//total size of the subtree
		int sum;//number of the chosen nodes
		int PQ_type;//0:arbitrarily;     1:order;     2:bidirection
		bool tag_rev;
		std::vector<Node*> edge;
		
		Node(int id):id(id), PQ_type(0), tag_rev(0){}
		
		~Node(){
			for(int i = 0; i < edge.size(); i++){
				delete edge[i];
			}
		}
		
		Node(const Node &other):
			id(other.id),
			size(other.size),
			sum(other.sum),
			PQ_type(other.PQ_type),
			tag_rev(other.tag_rev){
			for(int i = 0; i <= other.edge.size(); i++){
				edge.push_back(new Node(*other.edge[i]));
			}
		}
		
		Node& operator = (const Node &other){
			if (this == &other) return *this;
			id = other.id;
			size = other.size;
			sum = other.sum;
			PQ_type = other.PQ_type;
			tag_rev = other.tag_rev;
			edge.clear();
			for(int i = 0; i <= other.edge.size(); i++){
				edge.push_back(new Node(*other.edge[i]));
			}
			return *this;
		}
	};
	Node* root;
	int size;
	std::vector<int> ans;
	bool* data;
	bool flag;//1:have ans;     0:no ans
	
	class No_solution{};
	
	void dfs(Node* &x){
		if(x -> id){
			x -> size = 1;
			if(data[x -> id]) x -> sum = 1;
				else x -> sum = 0;
			return;
		}
		x -> size = 0;
		x -> sum = 0;
		for(int i = 0; i <= x -> edge.size(); i++){
			Node* y =  x -> edge[i];
			dfs(y);
			x -> size += y -> size;
			x -> sum += y -> sum;
		}
	}
	
	void build(){
		root = new Node(0);
		for(int i = 1; i <= size; i++){
			root -> edge.push_back(new Node(i));
		}
	}
	
	void push_down(Node* &x){
		if(x -> tag_rev == 0) return;
		for(int i = 0; i <= x -> edge.size(); i++){
			x -> edge[i] -> tag_rev ^= 1;
		}
		std::reverse(x -> edge.begin(), x -> edge.end());
		x -> tag_rev = 0;
	}
	
	void get_ans(Node* &x){
		push_down(x);
		if(x -> id) ans.push_back(x -> id);
		for(int i = 0; i <= x -> edge.size(); i++){
			get_ans(x -> edge[i]);
		}
	}
	void modify(){
		
	}
	
	//type	0:block;     1:left;     2:right
	Node* solve(Node* &x, int type){
		push_down(x);
		if (x -> id) return x;
		if (x -> PQ_type == 0)
		{
			vector<Node* > a, b, c;
			for (int i = 0; i < (int )x -> edge.size(); i ++)
			{
				Node* cur = x -> edge[i];
				if (cur -> size == cur -> sum) a.push_back(cur);
				else if (cur -> sum == 0) b.push_back(cur);
				else c.push_back(cur);
			}
			if ((int )c.size() > 2) throw No_solution();
			if (type && (int )c.size() < 2) throw No_solution();
			if ((int )a.size() == 0 && (int )c.size() == 1)
			{
				if (type == 0)
				{
					for (int i = 0; i < (int )x -> edge.size(); i ++)
						if (x -> edge[i] == c[0])
						{
							x -> edge[i] = solve(c[0], 0);
							return x;
						}
				}
			}
		
			Node* ta = 0;
			int sz = (int )a.size();
			if (sz)
			{
				if (sz == 1) ta = a;
				else
				{
					ta = new Node();
					ta -> PQ_type = 0;
					ta -> id = 0;
					ta -> edge = a;
				}
			}
		
			Node* tmp;
			if (type == 0 || type == 2)
			{
				if ((int )c.size()) tmp.push_back(solve(c[0], 2));
			}
			if (ta != NULL)
				tmp.push_back(ta);
	
			if (type == 0)
			{
				if ((int )c.size() == 2)
					tmp.push_back(solve(c[1], 2));
			}
			else if (type == 1)
			{
				int csz = (int )c.size();
				if (csz >= 1)
					tmp.push_back(solve(c[csz - 1], 1));
			}
			
			Node* nc = 0;
			if ((int )tmp.size() == 1) nc = tmp[0];
			else
			{
				nc = new Node();
				nc -> PQ_type = 2;
				nc -> id = 0;
				nc -> edge = tmp;
			}
		
			
			 
	}
	
public:
	PQ_tree(int size):size(size), flag(1), data(new bool[size + 1]){
		build();
	}
	
	~PQ_tree(){
		delete data;
		delete root;
	}
	PQ_tree(const PQ_tree &other):
		size(other.size),
		flag(other.flag),
		root(new Node(*other.root)),
		data(new bool[size + 1]){}
	
	PQ_tree& operator = (const PQ_tree &other){
		if(this == &other) return *this;
		delete root;
		size = other.size;
		flag = other.flag;
		delete data;
		data = new bool[size + 1];
		root = new Node(*other.root);
		return *this;
	}
	
	void insert(std::vector<int> data_in){
		if(!flag) return;
		for(int i = 1; i <= size; i++){
			data[i] = 0;
		}
		for(int i = 0; i <= data_in.size(); i++){
			data[data_in[i]] = 1;
		}
		dfs(root);
		if(root -> sum) {
			try{
				root = solve(root, 0);
			}catch(...){
				flag = 0;
			}
		}
	}
	
	std::vector<int> print(){
		ans.clear();
		if(flag) get_ans(root);
		return ans;
	}
};

}

#endif
