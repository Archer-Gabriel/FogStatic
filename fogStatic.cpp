#include <iostream>
#include <cstring>
#include <cstdlib>
#include "adjListGraph.cpp"
#include "QASearch.cpp"
using namespace std;


int find_min(int *a, int st, int ed, int noEdge){
	int rst = noEdge;
	for(int i=st; i<=ed; ++i)
		rst = rst<a[i]? rst:a[i];

	return rst;
}

int find_min_index(int *a, int st, int ed, int noEdge){
	int rst = noEdge, index = 0;
	for(int i=st; i<=ed; ++i){
		if(rst>a[i]){
			rst = a[i];
			index = i;
		}
	}

	return index;
}

int main(){
	
	string nodes[8] = {"A","B","C","fog1","fog2","fog3","cloud1","cloud2"};
	int values[8] = {43,72,96,50,60,80,1000,1000};
	int num_end = 3, num_fog = 3, num_cloud = 2, num_sum, noEdge = 200;
	int **dis = new int*[num_end];//dis:dis to fog from node i, [i][0]:dis to cloud
	int *all_dis, *resource, *rst, *index;//all_dis: distence get from dij, resource: values of end nodes
	resource = new int[num_end+num_fog];
	index = new int[num_end];
	num_sum = num_end+num_fog+num_cloud;
	adjListGraph<string, int> deploy(8, nodes, values);

	deploy.insert(0,3,10);
	deploy.insert(0,4,12);
	deploy.insert(0,5,15);
	deploy.insert(0,6,82);
	deploy.insert(0,7,90);
	deploy.insert(1,3,12);
	deploy.insert(1,4,10);
	deploy.insert(1,5,12);
	deploy.insert(1,6,86);
	deploy.insert(1,7,84);
	deploy.insert(2,3,15);
	deploy.insert(2,4,12);
	deploy.insert(2,5,10);
	deploy.insert(2,6,92);
	deploy.insert(2,7,78);
	deploy.insert(3,4,10);
	deploy.insert(3,5,10);
	deploy.insert(3,6,40);
	deploy.insert(3,7,50);
	deploy.insert(4,3,10);
	deploy.insert(4,5,10);
	deploy.insert(4,6,45);
	deploy.insert(4,7,45);
	deploy.insert(5,3,10);
	deploy.insert(5,4,10);
	deploy.insert(5,6,50);
	deploy.insert(5,7,40);
	

	// string *nodes;
	// int *values;
	// int num_end, num_fog, num_cloud, num_sum, num_edge, noEdge;
	// int st, ed, wt;
	// int **dis;//dis:dis to fog from node i, [i][0]:dis to cloud
	// int *all_dis, *resource, *rst, *index;//all_dis: distence get from dij, resource: values of end nodes

	// //read csv & create graph
	// ifstream file("edge_weight.csv", ios::in);
	// file>>num_end>>num_fog>>num_cloud<<num_edge>>noEdge;
	// num_sum = num_end+num_fog+num_cloud
	// nodes = new string[num_sum];
	// dis = new int*[num_end];
	// resource = new int[num_end+num_fog];
	// index = new int[num_end];
	// for(int i=0; i<num_sum; ++i){
	// 	file>>nodes[i];
	// }
	// for(int i=0; i<num_sum){
	// 	if(i<num_end+numfog)
	// 		file>>values[i]
	// 	else
	// 		values[i]=-1;
	// }
	// adjListGraph<string, int> deploy(8, nodes, values);
	// for(int i=0; i<num_edge; ++i){
	// 	file>>st>>ed>>wt;
	// 	deploy.insert(st, ed, wt);
	// }

	// dijkstra to find min weight
	for(int i=0; i<num_end; ++i){
		dis[i] = new int[num_fog];
		all_dis = deploy.dijkstra(nodes[i], noEdge);
		for(int j = num_end+1; j<=num_end+num_fog; ++j)
			dis[i][j-num_end] = noEdge - all_dis[j-1];
		// find index of min cloud
		// dis[i][0] = noEdge - find_min(all_dis, num_end+num_fog, num_end+num_fog+num_cloud-1, noEdge);
		// for(int j=num_end+num_fog; j<num_sum; ++j){
		// 	if(noEdge-dis[i][0]==all_dis[j])
		// 		index[i] = j;
		// }
		index[i] = find_min_index(all_dis, num_end+num_fog, num_sum-1, noEdge);
		dis[i][0] = noEdge - all_dis[index[i]];
	}

	for(int i=0; i<num_end+num_fog; ++i)
		resource[i] = values[i];
	

	// QA Search
	rst = search(dis, resource, num_end, num_fog);

	// Print result
	for(int i=0; i<num_end; ++i){
		if(rst[i]==0)
			cout<<nodes[i]<<"->"<<nodes[index[i]]<<endl;
			//cout<<nodes[i]<<"->cloud"<<endl;
		else
			cout<<nodes[i]<<"->"<<nodes[rst[i]+num_end-1]<<endl;
	}

	for(int i=0; i<num_end; ++i)
		delete []dis[i];
	delete []dis;
	delete []resource;

	return 0;
}
