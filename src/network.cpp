#include "network.h"
#include "random.h"
using namespace std;

void Network::resize(const size_t &n){
	values.resize(n);
	RNG.normal(values, 0, 1);
}

bool Network::add_link(const size_t& a, const size_t& b){
	bool link_exist(false);
	auto range = links.equal_range(a);
	for (auto i = range.first; i != range.second; ++i){
		if (i->second == b){
			link_exist = true;
		}
	}
	
	bool values_exist(false);
	if ((a < size()) and (b < size()) and (a != b)){
		values_exist = true;
	}
	
	if (!link_exist and values_exist){
		links.insert({a, b});
		return true;
	} else {
		return false;
	}
}

size_t Network::random_connect(const double& a){
	for (size_t i(0); i < size(); ++i){
		links.erase(i);
	}
	
	int links_total(0);
	for (size_t i(0); i < size(); ++i){
		int nb_link(RNG.poisson(a));
		if (nb_link > size()){
			nb_link = size();
		}
		links_total += nb_link;
		for (int j(0) ; j <= nb_link ; ++j){
			add_link(i, floor(RNG.uniform_double(0, size())));
		}
	}
	
	return links_total;
}

size_t Network::set_values(const std::vector<double>& v){
	if (v.size() < size()){
		for (size_t i(0) ; i < v.size() ; ++i){
			values[i] = v[i];
		}
		return v.size();
	} else {
		for (size_t i(0) ; i < size() ; ++i){
			values[i] = v[i];
		}
		return size();
	}
}

size_t Network::size() const{
	return values.size();
}

size_t Network::degree(const size_t &_n) const{
	return 2*links.count(_n);
}

double Network::value(const size_t &_n) const{
	if (_n < size()){
		return values[_n];
	} else {
		return 0;
	}
}

vector<double> Network::sorted_values() const{
	vector <double> sorting;
	sorting = values;
	
	for (size_t i(0); i < sorting.size(); ++i){
		double maximum(sorting[i]);
		size_t indice_max(i);
		for (size_t j(i) ; j < sorting.size() ; ++j){
			if (sorting[j] >= maximum){
				maximum = sorting[j];
				indice_max = j;
			}
		}
		double temp(sorting[i]);
		sorting[i] = maximum;
		sorting[indice_max] = temp;
	}
	
	return sorting;
}

vector<size_t> Network::neighbors(const size_t& t) const{
	vector <size_t> voisins;
		auto range = links.equal_range(t);
		for (auto j = range.first; j != range.second; ++j){
				voisins.push_back(j->second);
		}
		for (size_t i(0); i < size() ; ++i){
			auto range = links.equal_range(i);
			for (auto k = range.first; k != range.second ; ++k){
				if (k->second == t){
					voisins.push_back(k->first);
				}
			}
		}
	
	return voisins;
}
