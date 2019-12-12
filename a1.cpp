#include <fstream>
#include <iostream>
#include <stdio.h>      
#include <time.h>   
#include <vector> 
#include <math.h>
#include <algorithm>
#include <random> 
#include <map>
using namespace std;

#define INT_MIN -100000000;
#define INT_MAX 100000000;
#define MAX_REPEATS 20;
// #define MAX_VOCAB_SIZE 

clock_t t;
// vector<string> seq;
vector<int> seq[20];
int K;
// int costs[5][5];		// int or double?
double costs[10][10];
int vocab_size;
vector<char> vocab;

double hill_best;
double best_cost = INT_MAX; ////
int current_length;
int same_cost_counter = MAX_REPEATS;
int maxk_length = 0; // stores the maximum length of the input strings
vector<int> ans[20];
int answer_length;
int CC;
vector<int> original_input[20];


void print_strings()
{
	//cout<<"--------------------------"<<endl;
	for (int i=0; i<K; i++)
	{
		for (int j=0; j<current_length; j++)
			cout<<vocab[seq[i][j]];
		cout<<endl;
	}
	// cout<<"--------------------------"<<endl;
}


// double change_in_cost_shift(int string_index, int col1_index, int col2_index)
// {
// 	for (int i=0; i<K; i++)
// 	{

// 	}
// }

double change_in_cost(int string_index, int col1_index, int col2_index)
{
	int c1 = seq[string_index][col1_index], c2 = seq[string_index][col2_index];
	int change = 0;
	for (int i=0; i<K; i++)		// do I need to check i != string_index
	{
		if (i!=string_index)
		{
			int t1=seq[i][col1_index], t2=seq[i][col2_index];
			change += ((costs[c1][t1] + costs[c2][t2]) - (costs[c1][t2] + costs[c2][t1]));
		}
	}
	return change;
}

double compute_cost(vector<int> v[20], int length)
{
	double value = 0;
	for (int i=0; i<K; i++)
		for (int j=i+1; j<K; j++)
			for (int l=0; l<length; l++)
				value += costs[v[i][l]][v[j][l]];
	for (int i=0; i<K; i++)
		for (int l=0; l<length; l++)
			if (v[i][l] == vocab_size)
				value+=CC;
	return value;
}
// double same_length(vector<int> new_seq[20])
// {
// 	// move dash to random index ya move blocks of dashes
// }
double increase_length2(int l, vector<int> new_seq[20])
{
	
	vector<int>::iterator iterator1;
	for (int i=0; i<K; i++)
	{
		new_seq[i].clear();
		new_seq[i] = original_input[i];
		int dashes_to_insert = l - new_seq[i].size();
		for (int j=0; j<dashes_to_insert; j++)
			new_seq[i].insert(new_seq[i].begin()+(rand()%new_seq[i].size()), vocab_size);
		// {
		// 	iterator1= new_seq[i].begin();
		//   	new_seq[i].insert ( iterator1+ (rand()%new_seq[i].size()), 4);
		// }
	}
	return( compute_cost(new_seq, l) );
}

double increase_length(int l, vector<int> new_seq[20])
{
	vector<int>::iterator iterator1;
	for (int i=0; i<K; i++)
	{
		new_seq[i] = seq[i];
		for (int j=0; j<l-current_length; j++)
		{
			iterator1 = new_seq[i].begin();
			new_seq[i].insert( iterator1+(rand()%(new_seq[i].size() + 1)), vocab_size);
		}
	}
	return compute_cost(new_seq, l);
}
double decrease_length(int l, vector<int> new_seq[20])
{
	for (int i=0; i<K; i++)
	{
		vector<int> indices;
		for (int j=0; j<current_length; j++)
			if (seq[i][j] == vocab_size)
				indices.push_back(j);
		random_shuffle(indices.begin(), indices.end());
		// optimise this
		vector<int> to_remove;
		for (int s=0; s<current_length-l; s++)
		{
			//to_remove.push_back(rand % indices.size());
			to_remove.push_back(indices[s]);
		}
		sort(to_remove.begin(), to_remove.end());
		// for (int s=0; s<to_remove.size(); s++)
		// 	cout<<to_remove[s]<<": ";
		// cout<<endl;
		new_seq[i] = seq[i];
		for (int s=to_remove.size()-1; s>=0 ;s--)
			new_seq[i].erase(new_seq[i].begin() + to_remove[s]);
	}
	return compute_cost(new_seq, l);
}
void random_restart(vector<int> initialise_to[20])
{
	// cout<<"RANDOM"<<endl;
	int new_length;
	if (hill_best < best_cost)
	{
		for (int i=0; i<K; i++)
			ans[i] = seq[i];
		best_cost = hill_best;
		answer_length = current_length;
		// print_strings();
	}
	double min_cost = INT_MAX;
	double temp;
	for (int i=maxk_length; i < 2*maxk_length; i++)
	{
		vector<int> new_seq[20];
		temp = increase_length2(i, new_seq);
		if (temp < min_cost)
		{
			min_cost = temp;
			for (int s=0; s<K; s++)
			{
				initialise_to[s].clear();
				initialise_to[s] = new_seq[s];	// does this work?
			}
			new_length = i;
		}
	}
	// reset current length
	// cout << "Answer from last climb had length " << current_length  << "\n" <<  endl;
	current_length = new_length;
	// cout << "Length chosen = " << current_length  << endl;

	// reset hill_best
	hill_best = min_cost;

	// reset counter
	same_cost_counter = MAX_REPEATS;
}
void random_restart2(vector<int> initialise_to[20])
{
	// cout<<"RANDOM RESTART "<<endl;
	// store hill_best in best_cost if better and store the strings as well
	int new_length = current_length;
	if (hill_best < best_cost)
	{
		for (int i=0; i<K; i++)
			ans[i] = seq[i];
		best_cost = hill_best;
		// print_strings();
	}

	// look for a new random configuration
	double min_cost = INT_MAX;
	double temp;
	// cout<<"hey"<<endl;
	for (int i=maxk_length; i<current_length; i++)
	{
		// cout<<"enters for length: "<<i<<endl;
		vector<int> new_seq[20];
		temp = decrease_length(i, new_seq);
		// cout<<"decrease to length: "<<i<<" cost: "<<temp<<endl;
		if (temp < min_cost)
		{
			// optimise this -- baar baar poora copy mat kro
			min_cost = temp;
			for (int s=0; s<K; s++)
			{
				initialise_to[s].clear();
				initialise_to[s] = new_seq[s];	// does this work?
				new_length = i;
			}
		}
	}
	for (int i=current_length+1; i<2*maxk_length; i++)
	{
		vector<int> new_seq[20];
		temp = increase_length(i, new_seq);
		// cout<<"increase to length: "<<i<<" cost: "<<temp<<endl;
		if (temp < min_cost)
		{
			// optimise this -- baar baar poora copy mat kro
			min_cost = temp;
			for (int s=0; s<K; s++)
			{
				initialise_to[s].clear();
				initialise_to[s] = new_seq[s];	// does this work?
				new_length = i;
			}
		}
	}
	// reset current length
	current_length = new_length;

	// reset hill_best
	hill_best = min_cost;

	// reset counter
	same_cost_counter = MAX_REPEATS;
}
int best_neighbour(int process_time)
{
	// shift dash by 1
	// shift block of dashes
	// increase and decrease length by 1
	double max_cost_change = INT_MIN;
	int string_index, col1_index, col2_index;
	double temp;
	for (int i=0; i<K; i++)		// computing all neighbours' costs
		for (int j=0; j<current_length; j++)
		{
			if (seq[i][j] != vocab_size)
			{
				int ind = j;
				while(ind>0){
					if(seq[i][ind-1] == vocab_size){
						temp = change_in_cost(i, j, ind-1);
						// if (hill_best==888 && i==4) 	cout << "Change of index " << j << " with index " << (ind-1) << " gives delta= " << temp << endl;
						if (temp > max_cost_change)
						{
							string_index = i;
							col1_index = ind-1;
							col2_index = j;
							max_cost_change = temp;
						}
					}
					else
						break;
					ind--;
				}
				ind = j;
				while(ind+1<current_length){
					if (seq[i][ind+1] == vocab_size)
					{
						temp = change_in_cost(i, j, ind+1);
						if (temp > max_cost_change)
						{
							string_index = i;
							col1_index = j;
							col2_index = ind+1;
							max_cost_change = temp;
						}
					}
					else
						break;
					ind++;
				}
				
			}
		}

	double new_cost = hill_best - max_cost_change;
	
	// increase
	vector<int> v1[20];
	for (int i = 0; i < K; i++)
	{
		v1[i] = seq[i];
		v1[i].insert( v1[i].begin() + (rand()%(v1[i].size() + 1)) , vocab_size);
	}
	double c1 = compute_cost(v1, current_length+1);

	// decrease
	vector<int> v2[20];
	vector<int> dash_indices;
	double c2 = INT_MAX;
	if (current_length > maxk_length)
	{
		for (int i=0; i<K; i++)
		{
			dash_indices.clear();
			for (int j=0; j<current_length; j++)
				if (seq[i][j] == vocab_size)
					dash_indices.push_back(j);
			int rand_index = dash_indices[rand()%(dash_indices.size())];
			v2[i] = seq[i];
			v2[i].erase(v2[i].begin()+rand_index);
		}
		c2 = compute_cost(v2, current_length-1);
	}
	// c1 = INT_MAX;
	// c2 = INT_MAX;
	double best_neighbour_cost = min(new_cost, min(c1, c2));
	int changed = 0;
	if ((best_neighbour_cost < hill_best) || (hill_best==best_neighbour_cost && same_cost_counter>0))		/// CHANGED
	{
		changed = 1;
		hill_best = best_neighbour_cost;
		if(best_neighbour_cost == new_cost)
		{
			// cout << "making the changes" << endl;
			int t = seq[string_index][col2_index];
			seq[string_index][col2_index] = seq[string_index][col1_index];
			seq[string_index][col1_index] = t;
		}
		else if(best_neighbour_cost == c1)
		{
			// cout<<"selected"<<endl;
			for (int i=0; i<K; i++)
				seq[i]= v1[i];
			current_length++;
		}
		else
		{
			// cout<<"selected"<<endl;
			for (int i=0; i<K; i++)
				seq[i] = v2[i];
			current_length--;
		}
	}
	else	// forcefully take a move with -ve gain (with some probablity)
	{
		clock_t t3 = clock();
		double delta_E = hill_best - best_neighbour_cost;
		double p = ((double)(t3 - t)/(process_time*CLOCKS_PER_SEC));
		double prob = 1 - p;
		// double prob =  exp((delta_E/p));
		int r = rand()%10;
		if( r<10*prob )	 
		{
			changed = 1;
			hill_best = c1;			
			for (int i=0; i<K; i++)
				seq[i]= v1[i];
			current_length++;
		}
	}
	return changed;

}
void localsearch(double process_time)
{
	// repeat this till time
	int steps = 0;
	while(true)
	// for (int xx=0; xx<100; xx++)
	// for (int steps=0;steps<100000;steps++)
	{
		double last_best_cost = hill_best;
		int changed = best_neighbour(process_time);
		if (changed == 0)
		{
			vector<int> v[20];
			random_restart(v);
			for (int s=0; s<K; s++)
				seq[s] = v[s];
		}
		else if (changed == 1)
		{
			if (hill_best == last_best_cost)
				same_cost_counter--;
			else
				same_cost_counter = MAX_REPEATS;
		}
		// if ((hill_best - last_best_cost) > 0 || ( (hill_best == last_best_cost) && same_cost_counter==0))
		// 	{
		// 		cout<<"difference: "<<(hill_best-last_best_cost)<<endl;
		// 		vector<int> v[20];
		// 		random_restart(v);
		// 		for (int s=0; s<K; s++)
		// 			seq[s] = v[s];
		// 		// print_strings();
		// 	}
		// else if (hill_best == last_best_cost)
		// 	same_cost_counter--;
		// else
		// 	same_cost_counter = MAX_REPEATS;

		// cout<<"clocks ps: "<<CLOCKS_PER_SEC;
		clock_t t2 = clock();
		double time_elapsed = (double)(t2-t)/CLOCKS_PER_SEC;
		// cout<<"steps: "<<steps<<" time elapsed: "<<time_elapsed<<endl;
		steps++;
		// if (steps % 1000000 == 0)
		// 	cout<<"T: "<<time_elapsed<<endl;
		if ( time_elapsed > process_time)
			break;
	}
}
void initialise()
{
	best_cost = 0;
	vector<int>::iterator iterator1;
	for (int i=0; i<K; i++)
	{
		int dashes_to_insert = maxk_length - seq[i].size();
		//best_cost += CC * dashes_to_insert;
		for (int j=0; j<dashes_to_insert; j++)
		{
			iterator1= seq[i].begin();
		  	seq[i].insert ( iterator1+ (rand()%seq[i].size()), vocab_size);
		}
	}
	best_cost = compute_cost(seq, maxk_length);
	current_length = maxk_length;
	hill_best = best_cost;
}
int main(int argc, char const *argv[])
{
	t = clock(); 
	srand(unsigned(time(0)));
	vocab.clear();
	double time; // time read from input file (in min)
	char temp; string temp2;
	ifstream infile(argv[1]);
	ofstream outfile(argv[2]);

	// vector<int> seq[20];

	map<char, int> mymap;	
	infile>>time>>vocab_size;
	time = time*60;
	double process_time = time - min(0.1*time, (double)5);

	// input vocabulary characters
	for (int i=0; i<vocab_size-1; i++)
	{
		infile>>temp;
		vocab.push_back(temp);
		mymap.insert (pair<char,int>(temp,i) );
		infile>>temp;
	}
	infile>>temp;
	vocab.push_back(temp);
	mymap.insert (pair<char,int>(temp,vocab_size-1) );
	vocab.push_back('-');
	infile>>K;


	// input strings
	for (int i=0; i<K; i++)
	{
		infile>>temp2;
		if (temp2.length()>=maxk_length)
			maxk_length = temp2.length();
		for (int s=0; s<temp2.length(); s++)
			seq[i].push_back(mymap.find(temp2[s])->second);
		original_input[i] = seq[i];
	}
	infile>>CC;	
	for (int i=0; i<vocab_size+1; i++)
		for (int j=0; j<vocab_size+1; j++)
			{
				int t;
				infile>>t;
				costs[i][j]=t;
			}
	initialise();
	for (int i=0; i<K; i++)
		ans[i] = seq[i];
	answer_length = current_length;
	localsearch(process_time);
	if (hill_best < best_cost)
	{
		for (int i=0; i<K; i++)
			ans[i] = seq[i];
		answer_length = current_length;
	}

	for (int i=0; i<K; i++)
	{
		for (int j=0; j<answer_length; j++)
			outfile<<vocab[ans[i][j]];
		outfile<<endl;
	}
	// return 0;
}