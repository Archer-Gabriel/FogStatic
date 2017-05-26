#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

template <class elemType>
class queue{
public: 
	virtual bool isEmpty() = 0;
	virtual void enQueue(const elemType &x) = 0;
	virtual elemType  deQueue() = 0;
	virtual elemType getHead() = 0;
	virtual ~queue() {};
 };

template <class elemType>
class linkQueue: public queue<elemType>{
private:
	struct node{
    	elemType  data;
		node *next;
  		node(const elemType &x, node *N = NULL){
  			data = x;
  			next = N;
  		}
		node():next(NULL) {}
		~node() {}
	};
	node *front, *rear;   
public:
	linkQueue(){front = rear = NULL;}
	~linkQueue();
	bool isEmpty(){return front == NULL;}
	void enQueue(const elemType &x);
	elemType deQueue();  
	elemType getHead(){return front->data;}
	elemType gettail(){return rear->data;}
};

template <class elemType>
linkQueue<elemType>::~linkQueue(){
	node *tmp;
	while(front){
		tmp = front;
		front = front->next;
		delete tmp;
	}
}

template <class elemType>
void linkQueue<elemType>::enQueue(const elemType &x){
	if(!front) front = rear = new node(x);
	else{
		rear->next = new node(x);
		rear = rear->next;
	}
}

template <class elemType>
elemType linkQueue<elemType>::deQueue(){
	node *tmp = front;
	elemType res = front->data;
	front = front->next;
	delete tmp;
	if(!front) rear = NULL;
	return res;
}

template <class TypeOfEdge>
class graph{
public:
	virtual bool insert(int u, int v, TypeOfEdge w) = 0;
	virtual bool remove(int u, int v) = 0;
	virtual bool exist(int u, int v) const = 0;
	virtual int numOfVer() const {return Vers;}
	virtual int numOfEdge() const {return Edges;}
protected:
	int Vers, Edges;//graph::Vers;
}; 

template <class TypeOfVer, class TypeOfEdge>
class adjListGraph:public graph<TypeOfEdge>{
public:
    adjListGraph(int vSize, const TypeOfVer d[]);
    adjListGraph(int vSize, const TypeOfVer d[], const TypeOfEdge v[]);
    //adjListGraph(int vSize, const TypeOfVer *d, const TypeOfEdge *v);
	bool insert(int u, int v, TypeOfEdge w);
	bool remove(int u, int v);
	bool exist(int u, int v) const;
	~adjListGraph();

	void dfs();
	void bfs();
	void EulerCircuit(TypeOfVer start);
	void topSort() const;
	void prim(TypeOfEdge noEdge) const;
	int* dijkstra(TypeOfVer start, TypeOfEdge noEdge) const;
private:
    struct edgeNode {
		int end;
		TypeOfEdge weight;
		edgeNode *next;
		edgeNode(int e, TypeOfEdge w, edgeNode *n = NULL){
			end = e; weight = w; next = n;
		}
	};
	struct verNode{
		TypeOfVer ver;
		TypeOfEdge value;
		edgeNode *head;
		verNode(edgeNode *h = NULL, TypeOfEdge v = 0){
			head = h; value = v;}
	};
	struct EulerNode{
		int NodeNum;
		EulerNode *next;
		EulerNode(int ver){NodeNum=ver;next=NULL;}
	};
	verNode *verList;
	int Vers, Edges;//adjListGraph::Vers;

	void dfs(int u, bool visit[]);
	EulerNode* EulerCircuit(int start, EulerNode *&end);
	verNode* clone() const;
};

template <class TypeOfVer, class TypeOfEdge>
adjListGraph<TypeOfVer, TypeOfEdge>::
		adjListGraph(int vSize,  const TypeOfVer d[]){
	Vers = vSize; 
	Edges = 0;

	verList = new verNode[vSize];
	for(int i=0; i<Vers; ++i) verList[i].ver = d[i];
}

template <class TypeOfVer, class TypeOfEdge>
adjListGraph<TypeOfVer, TypeOfEdge>::
		adjListGraph(int vSize,  const TypeOfVer d[], const TypeOfEdge v[]){
	Vers = vSize; 
	Edges = 0;

	verList = new verNode[vSize];
	for(int i=0; i<Vers; ++i){
		verList[i].ver = d[i];
		verList[i].value = v[i];
	}
}

// template <class TypeOfVer, class TypeOfEdge>
// adjListGraph<TypeOfVer, TypeOfEdge>::
// 		adjListGraph(int vSize,  const TypeOfVer *d, const TypeOfEdge *v){
// 	Vers = vSize; 
// 	Edges = 0;

// 	verList = new verNode[vSize];
// 	for(int i=0; i<Vers; ++i){
// 		verList[i].ver = d[i];
// 		verList[i].value = v[i];
// 	}
// }

template <class TypeOfVer, class TypeOfEdge>
adjListGraph<TypeOfVer, TypeOfEdge>::~adjListGraph(){
	int i;
	edgeNode *p;
	for(i=0; i<Vers; ++i) 
		while ((p = verList[i].head) != NULL){
			verList[i].head = p->next;
			delete p;
		}
	delete [] verList;
}

template <class TypeOfVer, class TypeOfEdge>
bool adjListGraph<TypeOfVer, TypeOfEdge>::insert(int u, int v, TypeOfEdge w){
	verList[u].head = new edgeNode(v, w, verList[u].head);
	++Edges;
	return true;
}

template <class TypeOfVer, class TypeOfEdge>
bool adjListGraph<TypeOfVer, TypeOfEdge>::remove(int u, int v){
	edgeNode *p = verList[u].head, *q=NULL;
	while(p!=NULL){
		if(p->end==v){
			if(q!=NULL){
				q->next = p->next;
			}
			else{
				verList[u].head = p->next;
			}
			delete p;
			--Edges;
			return true;
		}
		else{
			q = p;
			p = p->next;
		}
	}
	return false;
}

template <class TypeOfVer, class TypeOfEdge>
bool adjListGraph<TypeOfVer, TypeOfEdge>::exist(int u, int v) const{
	edgeNode *p = verList[u].head;
	while(p!=NULL){
		if(p->end == v) return true;
		else p = p->next;
	}
	return false;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dfs(){
	bool *visit = new bool[Vers];
	for(int i=0; i<Vers; ++i) visit[i]=false;

	for(int i=0; i<Vers; ++i){
		if(!visit[i]){
			dfs(i,visit);
		}
	}
	cout<<endl;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::dfs(int u, bool visit[]){
	cout<<verList[u].ver<<" ";
	visit[u]=true;
	edgeNode *p = verList[u].head;
	while(p!=NULL){
		if(!visit[p->end]){
			dfs(p->end,visit);
		}
		p = p->next;
	}
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::bfs(){
	linkQueue<int> q;
	bool *visit = new bool[Vers];
	for(int i=0; i<Vers; ++i) visit[i]=false;

	for(int i=0; i<Vers; ++i){
		if(!visit[i]){
			q.enQueue(i);
			int tmp;
			while(!q.isEmpty()){
				tmp = q.deQueue();
				if(!visit[tmp]){
					cout<<verList[tmp].ver<<" ";
					visit[tmp]=true;
					edgeNode *p = verList[tmp].head;
					while(p!=NULL){
						if(!visit[p->end]){
							q.enQueue(p->end);
						}
						p = p->next;
					}
				}
			}
		}
	}
	cout<<endl;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::EulerCircuit(TypeOfVer start){
	edgeNode *r;
	int degree, index=Vers;
	verNode *tmp;
	EulerNode *bgn, *end, *p, *q, *tb, *te;

	for(int i=0; i<Vers; ++i){
		degree=0;
		r = verList[i].head;
		while(r!=NULL){
			++degree;
			r = r->next;
		}
		if(degree==0||degree%2==1){
			cout<<"There is no Euler circult!"<<endl;
			return;
		}
	}

	for(int i=0; i<Vers; ++i){
		if(verList[i].ver==start){
			index = i;
			break;
		}
	}
	if(index==Vers){
		cout<<"There is no start point!"<<endl;
		return;
	}

	tmp = clone();

	bgn = EulerCircuit(index,end);
	while(1){
		p = bgn;
		while(p->next!=NULL){
			if(verList[p->next->NodeNum].head!=NULL) break;
			else p = p->next;
		}
		if(p->next==NULL) break;
		q = p->next;
		tb = EulerCircuit(q->NodeNum,te);
		te->next = q->next;
		p->next = tb;
		delete q;
	}

	delete [] verList;
	verList = tmp;

	while(bgn!=NULL){
		cout<<verList[bgn->NodeNum].ver<<" ";
		p = bgn;
		bgn = bgn->next;
		delete p;
	}
	cout<<endl;
}

template <class TypeOfVer, class TypeOfEdge>
typename adjListGraph<TypeOfVer, TypeOfEdge>::EulerNode * adjListGraph<TypeOfVer, TypeOfEdge>::EulerCircuit(int start, EulerNode *&end){
	EulerNode *bgn;
	int nextNode;
	bgn = end = new EulerNode(start);
	while(verList[start].head!=NULL){
		nextNode = verList[start].head->end;
		remove(start,nextNode);
		remove(nextNode,start);
		start = nextNode;
		end->next = new EulerNode(start);
		end = end->next;
	}
	return bgn;
}

template <class TypeOfVer, class TypeOfEdge>
typename adjListGraph<TypeOfVer, TypeOfEdge>::verNode * adjListGraph<TypeOfVer, TypeOfEdge>::clone() const{
	verNode *tmp = new verNode[Vers];
	edgeNode *p;

	for(int i=0; i<Vers; ++i){
		tmp[i].ver = verList[i].ver;
		p = verList[i].head;
		while(p!=NULL){
			tmp[i].head = new edgeNode(p->end, p->weight, tmp[i].head);
			p = p->next;
		}
	}
	return tmp;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::topSort() const{
	linkQueue<int> que;
	int current, *inDegree=new int[Vers];
	edgeNode *p;
	for(int i=0; i<Vers; ++i) inDegree[i]=0;

	for(int i=0; i<Vers; ++i){
		p = verList[i].head;
		while(p!=NULL){
			++inDegree[p->end];
			p = p->next;
		}
	}

	for(int i=0; i<Vers; ++i) if(inDegree[i]==0) que.enQueue(i);

	while(!que.isEmpty()){
		current = que.deQueue();
		cout<<verList[current].ver<<" ";
		p = verList[current].head;
		while(p!=NULL){
			--inDegree[p->end];
			if(inDegree[p->end]==0) que.enQueue(p->end);
			p = p->next;
		}
	}
	cout<<endl;
}

template <class TypeOfVer, class TypeOfEdge>
void adjListGraph<TypeOfVer, TypeOfEdge>::prim(TypeOfEdge noEdge) const{
	bool *flag = new bool[Vers];
	TypeOfEdge *lowCost = new TypeOfEdge[Vers];
	int *startNode = new int[Vers];
	edgeNode *p;
	TypeOfEdge min;
	int start=0;
	for(int i=0; i<Vers; ++i){
		flag[i]=0;
		lowCost[i]=noEdge;
	}

	for(int i=0; i<Vers-1; ++i){
		flag[start]=1;
		p = verList[start].head;
		while(p!=NULL){
			if(!flag[p->end] && p->weight<lowCost[p->end]){
				lowCost[p->end]=p->weight;
				startNode[p->end]=start;
			}
			p = p->next;
		}
		min=noEdge;
		for(int j=0; j<Vers; ++j){
			if(lowCost[j]<min&&!flag[j]){
				min = lowCost[j];
				start=j;
			}
		}

		cout<<"("<<verList[startNode[start]].ver<<","<<verList[start].ver<<") ";
	}
	cout<<endl;
}

template <class TypeOfVer, class TypeOfEdge>
int* adjListGraph<TypeOfVer, TypeOfEdge>::dijkstra(TypeOfVer start, TypeOfEdge noEdge) const{
	bool *known = new bool[Vers];
	TypeOfEdge *distence = new TypeOfEdge[Vers];
	int sNo, u;
	TypeOfEdge min=noEdge;
	edgeNode *p;
	for(int i=0; i<Vers; ++i){
		known[i]=0;
		distence[i]=noEdge;
		if(verList[i].ver==start){
			sNo = i;
		}
	}
	known[sNo]=1;
	distence[sNo]=0;

	for(int i=1; i<Vers; ++i){
		p = verList[sNo].head;
		for(int j=0; j<Vers; ++j){
			if(!known[j]&&distence[j]<min){
				min = distence[j];
				u = j;
			}
		}
		while(p!=NULL){
			distence[p->end] = distence[p->end]<distence[sNo]+p->weight? distence[p->end]:distence[sNo]+p->weight;
			if(!known[p->end]&&distence[p->end]<min){
				min = distence[p->end];
				u = p->end;
			}
			p = p->next;
		}
		sNo = u;
		known[sNo] = 1;
		//distence[sNo] = min;
		min=noEdge;
	}

	for(int i=0; i<Vers; ++i){
		cout<<"from "<<start<<" to "<<verList[i].ver<<" min distence = "<<distence[i]<<endl;
	}

	int* rst = new int[Vers];
	for(int i=0; i<Vers; ++i)
		rst[i] = distence[i];
	return rst;
}


