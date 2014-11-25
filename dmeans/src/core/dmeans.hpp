#ifndef __DMEANS_HPP
#include<vector>
#include<iostream>
#include<random>
#include "../util/timer.hpp"
#include "../util/results.hpp"
#include "cluster.hpp"
#include "../util/config.hpp"

namespace dmeans{
template<class Model, template<typename> class Alg>
class DMeans{
	public:
		DMeans(Config cfg);
		//initialize a new step and cluster
		Results<Model> cluster(std::vector<typename Model::Data>& obs, uint64_t nRestarts);
		//reset DDP chain
		void reset();
	private:
		bool verbose;
		uint64_t nextLabel;
		Config cfg;

		std::vector<Cluster<Model> > clusters;
		Timer timer;

		Results<Model> getResults() const;
		void finalize();
		void restart();
		void labelNewClusters();
};

#include "dmeans_impl.hpp"

}
#define __DMEANS_HPP
#endif 
