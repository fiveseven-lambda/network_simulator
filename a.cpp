#include <vector>
#include <iostream>
#include <map>
#include <memory>

class Router{
	std::vector<std::shared_ptr<Router>> next;
	std::map<int32_t, std::pair<std::vector<std::shared_ptr<Router>>::iterator, int>> table;
public:
	Router(int32_t addr){
		table[addr] = std::make_pair(next.end(), 0);
	}
	friend void connect(std::shared_ptr<Router> a, std::shared_ptr<Router> b){
		a->next.push_back(b);
		b->next.push_back(a);
	}
	bool update(){
		bool ret = false;
		for(auto i = next.begin(); i != next.end(); ++i){
			for(auto j : (*i)->table){
				auto it = table.find(j.first);
				if(it == table.end()){
					table[j.first] = std::make_pair(i, j.second.second + 1);
					ret = true;
				}else if(it->second.second > j.second.second + 1){
					it->second.second = j.second.second + 1;
					ret = true;
				}
			}
		}
		return ret;
	}
	void send(int32_t addr, char message[]){
		auto it = table.find(addr);
		if(it == table.end()) fputs("error\n", stderr);
		else if(!it->second.second) printf("%d received \"%s\"\n", it->first, message);
		else (*it->second.first)->send(addr, message);
	}
};

int main(){
	int n, m, k;
	std::vector<std::shared_ptr<Router>> network;
	std::cin >> n;
	for(int i = 0; i < n; ++i){
		int32_t addr;
		std::cin >> addr;
		network.push_back(std::make_shared<Router>(addr));
	}
	std::cin >> m;
	for(int i = 0; i < m; ++i){
		int a, b;
		std::cin >> a >> b;
		connect(network[a], network[b]);
	}
	for(int i = 0, count = 0; count < n; ++i) if(network[i % n]->update()) count = 0; else ++count;
	std::cin >> k;
	for(int i = 0; i < k; ++i){
		int from;
		int32_t addr;
		char message[1000];
		std::cin >> from >> addr >> message;
		network[from]->send(addr, message);
	}
}
