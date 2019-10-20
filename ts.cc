#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<cmath>
#include<algorithm>
#include<iomanip>

	using namespace std;

const int N = 7; //The number of cities
double random_number(){return (double)rand()/(RAND_MAX + 1.0);}


int factorial(int n) { 
   if ((n==0)||(n==1))
      return 1; 
   else
      return n*factorial(n-1);
}

struct City
{
	double x,y;
	string name;
};

struct Path
{
	double length;
	double index[N];
};

void dist_func(City point1, City point2, double &dist){
	dist = sqrt((point1.x-point2.x)*(point1.x-point2.x) + (point1.y-point2.y)*(point1.y-point2.y));
	} 
void swap_cities(Path &path){
	int i = rand()%N;
	int j = rand()%N;
	while(i==j){j = rand()%N;} //Make sure that they are not the same
	int temp = path.index[i];
	path.index[i] = path.index[j];
	path.index[j] = temp; 
	}

double calc_dist(Path path, vector<City> p){
	double dist;
	double delta = 0.0;
	for(int i = 0; i < N; i++){
		dist_func(p[path.index[i%N]],p[path.index[(i+1)%N]],dist);
		delta +=dist;
	}
	return delta;
	}
main(){
	vector<City> p(N);	
	srand(time(NULL));
	for(int i = 0; i < N; i++){
		p[i].x = random_number();
		p[i].y = random_number(); 
	}
	
	
	Path path;
	for(int i = 0; i < N; i++){
 		path.index[i] = i;	
	}
	
	double cooling = 0.999;
	double T_init = 100;
	double T_final = 0.00001;
	double prob,diff;
	double T = T_init;
	int k = 0;	
	Path new_path;
	ofstream outfile("cost.dat");
	while (T > T_final){
			
		for(int i = 0; i < 50; i++){
			new_path = path;
			swap_cities(new_path);		
			diff = calc_dist(new_path,p) - calc_dist(path,p);
			prob = exp(-diff/T);
			if (diff<=0){path = new_path;}
			else if (diff>0 && prob>random_number())
			{	
				path = new_path;	
			}
		}
		T *= cooling;
		k++;
		if (k%10==0){ outfile << T << " " << calc_dist(path,p)<< endl;}	
	}
	
	int m;
	ofstream outfile1("cities_sa.dat");
	for(int i = 0; i < N; i++){
		m =path.index[i];
		outfile1 <<" "<< m <<" "<< p[m].x << "  " << p[m].y << endl;
	}

	int num[N];
        for(int j =0; j<N; j++){num[j] = j;}
        int q = 0;
        double distance;
        vector<Path> route(factorial(N));
	cout<< "The number of paths is "<< factorial(N)<< endl;
        sort(num,num+N);
        do{
                double total_distance = 0;
                for(int j  = 0; j < N; j++){
                                dist_func(p[num[j%N]],p[num[(j+1)%N]],distance);
                                total_distance += distance;
                }
                route[q].length = total_distance;
                for(int i = 0; i < N; i++)
                        route[q].index[i] = num[i];

                q += 1;
        }while(next_permutation(num,num+N));

	int pp = 0;
        double temp = route[0].length;
        for(int k = 1; k < factorial(N); k++){
                if(route[k].length<temp){
			pp = k;
                        temp = route[k].length;
                }
        }

        cout << "The shortest distance is " << setprecision(7) << temp << " k= " << pp << endl;

	ofstream outfile2("cities_bf.dat");
	for(int i = 0; i < N; i++){
		m =route[pp].index[i];
		outfile2 <<" "<< m <<" "<< p[m].x << "  " << p[m].y << endl;
	}

}
