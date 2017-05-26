#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
using namespace std;

#define GROUP_SIZE 30  //num of group
#define PROB_MU 0.1  //prob of mutation
#define PROB_CR  0.8 //prob of crossover
#define MAX_ITER 100 //iterator time

struct node{
    int *bp;
    int adapt;
    double p;
};

int n = 5, m = 20; //n: num of bag(1~n); m: num of item(0~m-1)
int *w, **v, *b; //b: volumn of bag, w: weight, v: value

/*------------------------------------------------*/
//basic function

void qs(int *a, int *b, int low, int high){
    int l = low;
    int r = high;
    int tmp = a[low], tmp2 = b[low];
    if(l>=r) return;
    while(l<r){
        while(l<r&&a[r]>tmp)
            --r;
        if(l<r) {a[l++] = a[r];b[l-1] = b[r];}
        while(l<r&&a[l]<tmp)
            ++l;
        if(l<r) {a[r--] = a[l];b[r+1] = b[l];}
    }
    a[l] = tmp;
    b[l] = tmp2;
    qs(a, b, low, l-1);
    qs(a, b, l+1, high);
}

void qs(double *a, int *b, int low, int high){
    int l = low;
    int r = high;
    double tmp = a[low], tmp2 = b[low];
    if(l>=r) return;
    while(l<r){
        while(l<r&&a[r]>tmp)
            --r;
        if(l<r) {a[l++] = a[r];b[l-1] = b[r];}
        while(l<r&&a[l]<tmp)
            ++l;
        if(l<r) {a[r--] = a[l];b[r+1] = b[l];}
    }
    a[l] = tmp;
    b[l] = tmp2;
    qs(a, b, low, l-1);
    qs(a, b, l+1, high);
}

//push the st to the pos in sorted queue
void bubble(int st, int ed, int *value, int *label){
    int tmp_v = value[st];
    int tmp_l = label[st];
    for(int i=st-1; i>=ed; --i){
        if(tmp_v >= value[i]){
            value[i+1] = tmp_v;
            label[i+1] = tmp_l;
            return;
        }
        value[i+1] = value[i];
        label[i+1] = label[i];
    }
}

//deal with the situation that unvalid
void un_valid(node &sample){
    double *density = new double[m];
    int *label = new int[m];
    int *total_w = new int[n+1];
    int num=0; //number of items in bag
    memset(total_w, 0, (n+1)*sizeof(int));

    for(int i=0; i<m; ++i){
        total_w[sample.bp[i]] += w[i];
    }

    for(int i=1; i<n+1; ++i){
        if(total_w[i] > b[i]){
            num = 0;
            for(int j=0; j<m ;++j){
                if(sample.bp[j]==i){
                    density[num] = double(v[j][i])/w[j];
                    label[num++] = j;
                }
            }
            qs(density, label, 0, num-1); //small to large
            for(int j=0; j<num; ++j){
                sample.bp[label[j]] = 0;
                total_w[i] -= w[label[j]];
                if(total_w[i] <= b[i])
                    break;
            }
        }
    }

    delete []density;
    delete []label;
    delete []total_w;
}

//deal with the situation that unsufficient
void un_suffi(node &sample){
    double *density = new double[m];
    int *label_item = new int[m];
    int *total_w = new int[n+1];
    int *label_bag = new int[m];
    int *remain_w = new int[n+1];
    int num=0, total_v; //num: number of items not in bag
    memset(total_w, 0, (n+1)*sizeof(int));

    for(int i=0; i<m; ++i){
        total_w[sample.bp[i]] += w[i];
        if(sample.bp[i]==0){
            //use mean of v[i] to represent value
            total_v = 0;
            for(int j=0; j<n; ++j)
                total_v += v[i][j];
            density[num] = (double(total_v)/n)/w[i];
            label_item[num++] = i;
        }
    }
    for(int i=1; i<n+1; ++i){
        remain_w[i] = b[i] - total_w[i];
        label_bag[i] = i;
    }

    qs(density, label_item, 0, num-1);
    qs(remain_w, label_bag, 1, n);

    for(int i=num-1; i>=0; --i){
        if(remain_w[n] < w[label_item[i]])
            continue;
        remain_w[n] -= w[label_item[i]];
        sample.bp[label_item[i]] = label_bag[n];
        bubble(n, 1, remain_w, label_bag); //push the remain to the pos
    }

    delete []density;
    delete []label_item;
    delete []total_w;
    delete []label_bag;
    delete []remain_w;
}

void copy_node(node &from, node &to){
    for(int i=0; i<m; ++i)
        to.bp[i] = from.bp[i];
    to.adapt = from.adapt;
    to.p = from.p;
}

//generate a list of index, which two is a pair
int* random_pairs(int n){
    bool *flag = new bool[n];
    int *pairs = new int[n];
    int rem = 0, index;
    memset(flag, 0, n*sizeof(bool));

    while(rem<n){
        index = rand()%n;
        if(!flag[index]){
            flag[index] = 1;
            pairs[rem++] = index;
        }
    }

    delete []flag;
    return pairs;
}

//find best two of p1, p2, s1, s2
//better determine which to exchange
int* find_best2(node p1, node p2, node s1, node s2){
    int* better = new int[2];
    memset(better, 0, 2*sizeof(int));

    if(p1.adapt>p2.adapt){
        if(p1.adapt<s1.adapt){
            better[1] = 1;
            better[0] = p1.adapt<s2.adapt? 2:0;
        }
        else if(p2.adapt<s1.adapt){
            better[1] = s1.adapt>s2.adapt? 1:2;
        }
        else{
            better[1] = p2.adapt<s2.adapt? 2:0;
        }
    }
    else{
        if(p2.adapt<s1.adapt){
            better[0] = 1;
            better[1] = p2.adapt<s2.adapt? 2:0;
        }
        else if(p1.adapt<s1.adapt){
            better[0] = s1.adapt>s2.adapt? 1:2;
        }
        else{
            better[0] = p1.adapt<s2.adapt? 2:0;
        }
    }

    return better;
}

/*---------------------------------------------------------------*/
//module in GA

//initiate the group with random
//TODO: use greedy algorithm to create a chrom, replace the worst
void group_produce(node group[]){
    for(int i=0; i<GROUP_SIZE; ++i){
        group[i].bp = new int[m];
        for(int j=0; j<m; ++j){
            group[i].bp[j] = rand()%n + 1;
        }
        un_valid(group[i]);
        un_suffi(group[i]);
    }
}

//set the adapt and p of each group
void evaluate(node group[]){
    int sum_v = 0;
    int total_v = 0;

    for(int i=0; i<GROUP_SIZE; ++i){
        total_v = 0;
        for(int j=0; j<m; ++j){
            total_v += v[j][group[i].bp[j]];
        }
        group[i].adapt = total_v;
        sum_v += total_v;
    }
    for(int i=0; i<GROUP_SIZE; ++i){
        group[i].p = double(group[i].adapt)/sum_v;
    }
}

//set the adapt for node
void evaluate(node &sample){
    int total_v = 0;

    for(int i=0; i<m; ++i){
        total_v += v[i][sample.bp[i]];
    }
    sample.adapt = total_v;
}

//return the index of the best group
int find_best(node group[]){
    int max_adapt = 0, index = 0;
    for(int i=0; i<GROUP_SIZE; ++i){
        if(max_adapt<group[i].adapt){
            max_adapt = group[i].adapt;
            index = i;
        }
    }
    return index;
}

//use p to determine which to hold, create a new group
void selection(node group[], node grouptemp[]){
    int choice[GROUP_SIZE] = {0};
    double accu_p[GROUP_SIZE] = {group[0].p};
    double prob = 0;
    for(int i=1; i<GROUP_SIZE; ++i)
        accu_p[i] = accu_p[i-1] + group[i].p;

    choice[0] = find_best(group);//elite strategy
    for(int i=1; i<GROUP_SIZE; ++i){
        prob = rand()%10000/double(10000);
        for(int j=0; j<GROUP_SIZE; ++j)
            if(accu_p[j]>prob){
                choice[i] = j;
                break;
            }
    }

    for(int i=0; i<GROUP_SIZE; ++i){
        grouptemp[i].bp = new int[m];
        copy_node(group[i], grouptemp[i]);
    }
    for(int i=0; i<GROUP_SIZE; ++i)
        copy_node(grouptemp[choice[i]], group[i]);
    for(int i=0; i<GROUP_SIZE; ++i)
        delete []grouptemp[i].bp;
}

//uniform crossover, can't ensure sigma(p)=1 after that
void crossover(node group[]){
    int *pairs, *better, choice;
    double crov;
    node son1, son2;
    son1.bp = new int[m];
    son2.bp = new int[m];
    pairs = random_pairs(GROUP_SIZE);

    /*cout<<"The pairs is shown: "<<endl;
    for(int i=0; i<GROUP_SIZE-1; i+=2)
        cout<<pairs[i]<<"-"<<pairs[i+1]<<" ";
    cout<<endl;*/

    for(int i=0; i<GROUP_SIZE-1; i+=2){
        crov = rand()%100/double(100);
        if(crov > PROB_CR)
            continue;

        //crossover
        for(int j=0; j<m; ++j){
            choice = rand()%2;
            son1.bp[j] = group[pairs[i+choice]].bp[j];
            son2.bp[j] = group[pairs[i+1-choice]].bp[j];
        }
        un_valid(son1); un_valid(son2);
        un_suffi(son1); un_suffi(son2);
        evaluate(son1); evaluate(son2);

        //find best two; better[2], 0-no need to exchage, 1-son1, 2-son2
        better = find_best2(group[pairs[i]], group[pairs[i+1]], son1, son2);
        for(int j=0; j<2; ++j){
            if(better[j]==1)
                copy_node(son1, group[pairs[i+j]]);
            else if(better[j]==2)
                copy_node(son2, group[pairs[i+j]]);
        }
    }
}

//uniform mutation, don't ensure adapt
void mutation(node group[]){
    double muta;

    //the best don't mutate
    for(int i=1; i<GROUP_SIZE; ++i){
        muta = rand()%100/double(100);
        if(muta<PROB_MU){
            for(int j=0; j<m; ++j){
                if(rand()%2==0)
                    group[i].bp[j] = rand()%n + 1;
            }
            un_valid(group[i]);
            un_suffi(group[i]);
            //cout<<"Sample "<<i<<" mutated! "<<endl;
        }
    }
}

/*---------------------------------------------------*/
// print function

void show_condition(){
    cout<<"Volumn of bags: ";
    for(int i=1; i<n+1; ++i)
        cout<<b[i]<<" ";
    cout<<endl;
    for(int i=0; i<m; ++i){
        cout<<"weight of "<<setw(2)<<i<<": "<<setw(2)<<w[i]<<" value: "<<v[i][0]<<endl;
    }
}

void show_sample(node sample){
    cout<<"The adapt of the sample is: "<<200*m-sample.adapt<<endl;
    for(int i=0; i<m; ++i)
        cout<<sample.bp[i]<<" ";
    cout<<endl;
}

void show_group(node group[]){
    cout<<"The state of the group:"<<endl;
    for(int i=0; i<GROUP_SIZE; ++i){
        for(int j=0; j<m; ++j)
            cout<<group[i].bp[j]<<" ";
        cout<<group[i].adapt;
        cout<<endl;
    }
}

void show_adapt(node group[]){
    for(int i=0; i<GROUP_SIZE; ++i){
        cout<<"The adapt of the chrom "<<i<<" is "<<group[i].adapt;
        cout<<" prob is "<<group[i].p<<endl;
    }
}

int* search(int **dis, int *resource, int num_end, int num_fog){
    int index;

    srand(time(NULL));
    n = num_fog;
    m = num_end;
    v = dis;//v[i][j], i item in j bag v[m][n]
    w = new int[m];
    b = new int[n+1];
    for(int i=0; i<m; ++i)
        w[i] = resource[i];
    for(int i=1; i<=n; ++i)
        b[i] = resource[i+m-1];
    show_condition();

    node group[GROUP_SIZE], grouptemp[GROUP_SIZE];

    group_produce(group);
    //show_group(group);
    for(int i=0; i<MAX_ITER; ++i){  
        evaluate(group);
        selection(group, grouptemp);
        crossover(group);
        mutation(group);
    }
    evaluate(group);
    index = find_best(group);
    show_sample(group[index]);
    
    delete []w;
    delete []b;
    return group[index].bp;
}
