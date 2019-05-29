#include <bits/stdc++.h>
#define INF 1000000
#define Chromosome_Num 10
#define Max_Iter 100000
using namespace std;
int N, E;
int **Chromosome;
int *Chromosome_CutValue;
int *ParentA, *ParentB;
int *Offspring;
int Offspring_CutValue;
int **Graph;
int MaxCutValue;
int *MaxChromosome;
void Init() {
    char ch;
    cin >> ch >> N >> E;
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
        for (int i = 1; i <= N; ++i) Chromosome[P][i] = rand() % 2;
        for (int i = 1; i <= N; ++i)
            for (int j = i + 1; j <= N; ++j)
                if (Graph[i][j] == 1 && Chromosome[P][i] != Chromosome[P][j]) Chromosome_CutValue[P]++;
        if (MaxCutValue < Chromosome_CutValue[P]) {
            MaxCutValue = Chromosome_CutValue[P];
            for (int i = 1; i <= N; ++i) MaxChromosome[i] = Chromosome[P][i];
        }
    }
}
void Genetic_Crossover() {
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
    for (int i = 1; i <= N; ++i)
        if (ParentA[i] == ParentB[i])
            Offspring[i] = ParentA[i];
        else
            Offspring[i] = rand() % 2;
}
void Genetic_Mutation() {
    for (int i = 1; i <= N; ++i)
        if (rand() % 20 == 1) Offspring[i] = 1 - Offspring[i];
    Offspring_CutValue = 0;
    for (int i = 1; i <= N; ++i)
        for (int j = i + 1; j <= N; ++j)
            if (Graph[i][j] == 1 && Offspring[i] != Offspring[j]) Offspring_CutValue++;
}
void Genetic_Update() {
    int MinCutValue = INF;
    int MinSign;
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
    Init();
    Genetic_Construction();
    for (int i = 1; i <= Max_Iter; ++i) {
        Genetic_Crossover();
        Genetic_Mutation();
        Genetic_Update();
    }
    Output();
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
