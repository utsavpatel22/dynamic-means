#ifndef __MOVINGRINGDATAGEN_HPP
#include<Eigen/Dense>
#include<dmeans/utils>
#include<vector>
#include "movingdatagen.hpp"

class MovingRingDataGenerator : public MovingDataGenerator{
	typedef Eigen::Vector2d V2d;
	protected:
	std::vector<V2d> centers;
	double motionStdDev, clusterStdDev, radius;

	public:
		MovingRingDataGenerator(dmeans::Config cfg) : MovingDataGenerator(cfg){
			radius = cfg.get("radius", dmeans::Config::OPTIONAL, 0.1);
			motionStdDev = cfg.get("motionStdDev", dmeans::Config::OPTIONAL, 0.05);
			clusterStdDev = cfg.get("clusterStdDev", dmeans::Config::OPTIONAL, 0.05);

			std::uniform_real_distribution<double> uniformDist01(0, 1);
			for (uint32_t i = 0; i < this->alive.size(); i++){
					V2d newCenter;
					newCenter(0) = 0.5;
					newCenter(1) = 0.5;
					centers.push_back(newCenter);
			}
		}

		virtual ~MovingRingDataGenerator(){ 
		}

		void transitionCluster(int k){
			std::uniform_real_distribution<double> uniformDistAng(0, 2*M_PI);
			std::normal_distribution<double> transitionDistRadial(0, motionStdDev);

			double steplen = transitionDistRadial(dmeans::RNG::get());
			//angle sampled from uniform
			double stepang = uniformDistAng(dmeans::RNG::get());
			centers[k](0) += steplen*cos(stepang);
			centers[k](1) += steplen*sin(stepang);
			if ( k >= 1){
				while ((centers[k] - centers[k-1]).norm() > radius/(alive.size()*2.0)){
					centers[k] = (centers[k-1]+centers[k])/2.0;
				}
			}
			cout << "Cluster " << k << " moved to " << centers[k].transpose() << endl;
		}

		void createCluster(){
				std::uniform_real_distribution<double> uniformDist01(0, 1);
				V2d newCenter;
				newCenter(0) = uniformDist01(dmeans::RNG::get());
				newCenter(1) = uniformDist01(dmeans::RNG::get());
				centers.push_back(newCenter);
				cout << "Cluster " << centers.size()-1 << " was created at " << centers.back().transpose() << endl;
		}

		std::vector<V2d> genData(int j, int n) const{
			uniform_real_distribution<double> uniformDistAng(0, 2*M_PI);
			normal_distribution<double> likelihoodDistRadial(0, clusterStdDev);
			std::vector<V2d> out;
			for (int k = 0; k < n/(j+1.0); k++){
				V2d newData = centers[j];
				double len = radius*(1.0-j/(alive.size()-1.0))+likelihoodDistRadial(dmeans::RNG::get());
				double ang = uniformDistAng(dmeans::RNG::get());
				newData(0) += len*cos(ang);
				newData(1) += len*sin(ang);
				out.push_back(newData);
			}
			return out;
		}
};
#define __MOVINGRINGDATAGEN_HPP
#endif /* __MOVINGRINGDATAGEN_HPP */
