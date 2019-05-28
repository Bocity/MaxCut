#include <bits/stdc++.h>
#define INF 1000000
#define Chromosome_Num 10 //遗传过程中的群体大小
#define Max_Iter 100000   //最大遗传迭代次数
using namespace std;
int N, E;         //算例规模（节点数，无向边数）；
int **Chromosome; //群体中的所有染色体，每条染色体上的每个节点代表图中顶点，用0,1表示其分别位于哪个集合中；
int *Chromosome_CutValue; //群体每条染色体对应分配方案的被切割边数；
int *ParentA, *ParentB;   //遗传过程中用于杂交的父代；
int *Offspring;           //遗传过程所得到的子代；
int Offspring_CutValue;   //遗传过程所得到子代对应分配方案的被切割边数；
int **Graph;              //存储整个图结构
int MaxCutValue;          //多代遗传过后的最大被切割边数；
int *MaxChromosome;       //多代遗传过后最大被切割边数对应的分配方案；
void Init() {
    char ch;
    cin >> ch >> N >> E;
    //所有动态数组的初始化
    ParentA = new int[N + 10];
    ParentB = new int[N + 10];
    Offspring = new int[N + 10];
    MaxChromosome = new int[N + 10];
    Chromosome_CutValue = new int[Chromosome_Num + 10];
    Chromosome = new int *[Chromosome_Num + 10];
    for (int i = 1; i <= Chromosome_Num; ++i) Chromosome[i] = new int[N + 10];
    Graph = new int *[N + 10];
    for (int i = 1; i <= N; ++i) Graph[i] = new int[N + 10];
    for (int i = 1; i <= Chromosome_Num; ++i) {
        Chromosome_CutValue[i] = 0;
        for (int j = 1; j <= N; ++j) Chromosome[i][j] = 0;
    }
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= N; ++j) Graph[i][j] = 0;
    //读入并存储图
    int A, B;
    for (int i = 1; i <= E; ++i) {
        cin >> ch >> A >> B;
        Graph[A][B] = 1;
        Graph[B][A] = 1;
    }
}
void Genetic_Construction() {
    MaxCutValue = -INF;
    for (int P = 1; P <= Chromosome_Num; ++P) {
        //用随机方法构造第P条染色体
        for (int i = 1; i <= N; ++i) Chromosome[P][i] = rand() % 2;
        //计算第P条染色体对应分配方案的被切割边数
        for (int i = 1; i <= N; ++i)
            for (int j = i + 1; j <= N; ++j)
                if (Graph[i][j] == 1 && Chromosome[P][i] != Chromosome[P][j]) Chromosome_CutValue[P]++;
        //更新最大被切割边数及其对应的节点分配方案
        if (MaxCutValue < Chromosome_CutValue[P]) {
            MaxCutValue = Chromosome_CutValue[P];
            for (int i = 1; i <= N; ++i) MaxChromosome[i] = Chromosome[P][i];
        }
    }
}
void Genetic_Crossover() {
    //用轮盘赌方法从群体中随机选择两个父代
    int Sum[100];
    int A, B, Random;
    Sum[0] = 0;
    for (int i = 1; i <= Chromosome_Num; ++i) Sum[i] = Sum[i - 1] + Chromosome_CutValue[i];
    Random = rand() % Sum[Chromosome_Num] + 1;
    for (int i = 1; i <= Chromosome_Num; ++i)
        if (Random <= Sum[i]) {
            A = i;
            break;
        }
    Sum[0] = 0;
    for (int i = 1; i <= Chromosome_Num; ++i)
        if (i != A)
            Sum[i] = Sum[i - 1] + Chromosome_CutValue[i];
        else
            Sum[i] = Sum[i - 1];
    Random = rand() % Sum[Chromosome_Num] + 1;
    for (int i = 1; i <= Chromosome_Num; ++i)
        if (Random <= Sum[i]) {
            B = i;
            break;
        }
    for (int i = 1; i <= N; ++i) {
        ParentA[i] = Chromosome[A][i];
        ParentB[i] = Chromosome[B][i];
    }
    //对选取的父代进行杂交得到子代
    //其中杂交方法为若两个父代的同一节点在相同集合中，则保留；否则，对随机分配该节点至任意集合中；
    for (int i = 1; i <= N; ++i)
        if (ParentA[i] == ParentB[i])
            Offspring[i] = ParentA[i];
        else
            Offspring[i] = rand() % 2;
}
void Genetic_Mutation() {
    //在0.05的概率下，将子代的某个节点从一个集合移动到另一个集合中；
    for (int i = 1; i <= N; ++i)
        if (rand() % 20 == 1) Offspring[i] = 1 - Offspring[i];
    //计算子代染色体对应分配方案的被切割边数；
    Offspring_CutValue = 0;
    for (int i = 1; i <= N; ++i)
        for (int j = i + 1; j <= N; ++j)
            if (Graph[i][j] == 1 && Offspring[i] != Offspring[j]) Offspring_CutValue++;
}
void Genetic_Update() {
    int MinCutValue = INF;
    int MinSign;
    //更新群体：若得到的子代的被切割边数大于群体中最小的被切割边数，则用该子代取代；
    for (int i = 1; i <= Chromosome_Num; ++i)
        if (Chromosome_CutValue[i] < MinCutValue) {
            MinCutValue = Chromosome_CutValue[i];
            MinSign = i;
        }
    if (MinCutValue < Offspring_CutValue) {
        for (int i = 1; i <= N; ++i) Chromosome[MinSign][i] = Offspring[i];
        Chromosome_CutValue[MinSign] = Offspring_CutValue;
        if (MaxCutValue < Chromosome_CutValue[MinSign]) {
            MaxCutValue = Chromosome_CutValue[MinSign];
            for (int i = 1; i <= N; ++i) MaxChromosome[i] = Chromosome[MinSign][i];
        }
    }
}
int Check() {
    //检验分配方案的实际被切割边数与存储的被切割边数是否一致；
    int CutValue = 0;
    for (int i = 1; i <= N; ++i)
        for (int j = i + 1; j <= N; ++j)
            if (Graph[i][j] == 1 && MaxChromosome[i] != MaxChromosome[j]) CutValue++;
    return CutValue;
}
void Output() {
    cout << "*****************************************************************" << endl;
    cout << "Check_Max_Cut = " << Check() << endl;
    cout << "Max_Cut = " << MaxCutValue << endl;
    cout << "Distribution of each vertex : " << endl;
    for (int i = 1; i <= N; ++i) cout << MaxChromosome[i] << " ";
    cout << endl;
    cout << "*****************************************************************" << endl;
}
int main() {
    srand((unsigned) time(NULL));
    Init();                 //初始化数组，读入并存储图；
    Genetic_Construction(); //生成初始群体；
    for (int i = 1; i <= Max_Iter; ++i) {
        Genetic_Crossover(); //染色体交叉；
        Genetic_Mutation();  //染色体变异；
        Genetic_Update();    //生成下一代群体；
    }
    Output(); //结果输出；
    return 0;
}
/*
p 7 18
e 1 4
e 1 5
e 1 6
e 1 7
e 2 3
e 2 4
e 2 5
e 2 6
e 2 7
e 3 4
e 3 5
e 3 6
e 3 7
e 4 5
e 4 6
e 5 6
e 5 7
e 6 7
*/
