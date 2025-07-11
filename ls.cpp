#include <stdio.h>
#include <random>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#define N 2001
int n = 200;

int edge[N][N];
int preference[N][N];
int allocation[N], initial_allocation[N];
int out[N];
int dfn_num, dfn[N], low[N];
int queue[N];
int trade[N][N];
int deep[N];
int in_stack[N];

struct interval
{
    int l, r;
};
std::vector<interval> descent[N];

std::stack<int> S;
std::vector<int> N_out;

// 初始化一个图，每条边产生的概率是 p
void create_graph(double p,  int size, int seed)
{
    std::srand(seed);
    // for (int k = 1; k <= n / size; k++)
    // {
    //     for (int i = 1; i <= size; i++)
    //     for (int j = i + 1; j <= size; j++)
    //     {
    //         int x = (k - 1) * size + i;
    //         int y = (k - 1) * size + j;
    //         double t = std::rand() * 1.0 / RAND_MAX;
    //         edge[x][y] = edge[y][x] = (t <= p);
    //     }
    //     if(k * size + 1 <= n)
    //     {
    //         edge[k * size][k * size + 1] = edge[k * size + 1][k * size] = 1;
    //     }
    // }

    // int tmp = n / size;
    // if(size * tmp < n)
    // {
    //     for (int i = 1; i <= n % size; i++)
    //       for (int j = i + 1; j <= n % size; j++)
    //       {
    //           int x = tmp * size + i;
    //           int y = tmp * size + j;
    //           double t = std::rand() * 1.0 / RAND_MAX;
    //           edge[x][y] = edge[y][x] = (t <= p);
    //       }
    // }

    for (int i = 2; i <= n; ++i) 
    {
        int parent = rand() % (i - 1) + 1; // 随机选择一个 [1, i-1] 范围内的节点作为父节点
        edge[i][parent] = edge[parent][i] = 1;
    }
}
// 判断是不是邻居
int is_neighboor(int x, int y)
{
    // 如果已经 leave 或者本来就没有相连，返回 0
    if(out[y]) return 0;
    if(!edge[x][y]) return 0;
    return 1;
}

// 获取所有 agent 到 u 的最短距离
void get_deep(int u)
{
    std::queue<int> q;
    // 赋值成一个很大的值是因为，有些点可能和 1 不连通（图初始化问题）
    memset(deep, 0x3f, sizeof(deep));
    // BFS
    q.push(u);
    deep[u] = 1;
    while(!q.empty())
    {
        int i = q.front();
        q.pop();
        for (int j = 1; j <= n; j++)
        {
            if(!is_neighboor(i, j)) continue;
            if(deep[j] < N) continue;
            deep[j] = deep[i] + 1;
            q.push(j);
        }
    }
}

// 创建一个 1 到 n 的排列
void create_permuation(int *target)
{
    std::vector<int> vec;
	for (int i = 1; i <= n; i++)
	{
		vec.push_back(i);
	}
	random_shuffle(vec.begin(), vec.end());

    for (int i = 1; i <= n; i++) 
    {
        target[i] = vec[i - 1];
    }
}

// trade 数组存了哪些人可以 “指向” 哪些人，A->B 表示 A 想得到 B 的 item，这个是单向的
void create_trade(int bottom)
{
    for(int i = 1; i <= n; i++)
      for(int j = 1; j <= n; j++)
      {
          if(out[i]) continue;
          trade[i][j] = is_neighboor(i, j);
      }
    // 自环也是可以的
    for(int i = 1; i <= n; i++) 
    {
        if(out[i]) continue;
        trade[i][i] = 1;
    }
    // 每一个都可以指向 Stack_bottom
    for(int i = 1; i <= n; i++) 
    {
        if(out[i]) continue;
        trade[i][bottom] = 1;
    }
}

void game_init(int seed)
{
    std::srand(seed);
	for (int i = 1; i <= n; i++)
    {
        // preference[i][j] 表示第 i 个 agent 认为 item j 排在 preference[i][j] 位
        create_permuation(preference[i]);
    }
    // 初始化分配
    create_permuation(allocation);
    for(int i = 1; i <= n; i++)
    {
        initial_allocation[i] = allocation[i];
    }
}

double get_score()
{
    double score = 0;
    // int vis[N];
    // memset(vis, 0, sizeof(vis));
    for (int i = 1; i <= n; i++)
    {
        // if(vis[allocation[i]]) puts("nmsl");
        // vis[allocation[i]] = 1;
        score += (preference[i][initial_allocation[i]] - preference[i][allocation[i]]);
        // if(preference[i][allocation[i]] == 1) score += 10000;
    }
    return score / n;
}

void game_clear()
{
    memset(edge, 0, sizeof(edge));
    // memset(preference, 0, sizeof(preference));
    // memset(allocation, 0, sizeof(allocation));
    // memset(initial_allocation, 0, sizeof(initial_allocation));
    memset(out, 0, sizeof(out));
    dfn_num = 0;
    memset(dfn, 0, sizeof(dfn));
    memset(low, 0, sizeof(low));
    memset(queue, 0, sizeof(queue));
    memset(trade, 0, sizeof(trade));
    memset(deep, 0, sizeof(deep));
    memset(in_stack, 0, sizeof(in_stack));
    for(int i = 1; i <= n; i++)
    {
        descent[i].clear();
    }
    while(!S.empty()) S.pop();
    N_out.clear();
}

void tot_clear()
{
    memset(edge, 0, sizeof(edge));
    memset(preference, 0, sizeof(preference));
    memset(allocation, 0, sizeof(allocation));
    memset(initial_allocation, 0, sizeof(initial_allocation));
    memset(out, 0, sizeof(out));
    dfn_num = 0;
    memset(dfn, 0, sizeof(dfn));
    memset(low, 0, sizeof(low));
    memset(queue, 0, sizeof(queue));
    memset(trade, 0, sizeof(trade));
    memset(deep, 0, sizeof(deep));
    memset(in_stack, 0, sizeof(in_stack));
    for(int i = 1; i <= n; i++)
    {
        descent[i].clear();
    }
    while(!S.empty()) S.pop();
    N_out.clear();
}


double game(double p, int size, int seed)
{
// 初始化
    create_graph(p, size, seed);
    for(int i = 1; i <= n; i++)
    {
        allocation[i] = initial_allocation[i];
    }
    get_deep(1);

    // printf("score before reallocation = %d\n", get_score());

    // 这部分就是对我们伪代码的实现
    while(1)
    {
        // 找到离 1 最近的 agent
        int Min = 1e9, agent = 0;
        for (int i = 1; i <= n; i++)
        {
            if(out[i]) continue;
            if(deep[i] < Min) 
            {
                Min = deep[i];
                agent = i;
            }
        }
        if(agent == 0 || Min > N) break;

        memset(trade, 0, sizeof(trade));

        create_trade(agent);

        memset(in_stack, 0, sizeof(in_stack) );
        // 注意，这里的 N_out 指的只是这一轮 leave 的 agent，对应论文中的 N_out^t
        N_out.clear();
        S.push(agent);
        in_stack[agent] = 1;

        while(!S.empty())
        {
            int u = S.top();

            // 找到 u 能够交换的中最喜欢的
            int Min = 1e9, agent = 0;
            for (int i = 1; i <= n; i++)
            {
                if(out[i]) continue;
                if(!trade[u][i]) continue;
                // preference 是排位，所以越小越喜欢
                if(preference[u][allocation[i]] < Min) 
                {
                    Min = preference[u][allocation[i]];
                    agent = i;
                }
            }

            if(!in_stack[agent])
            {
                S.push(agent);
                in_stack[agent] = 1;
            } 

            // 如果这个已经在栈中了，说明构成了环
            else 
            {
                // 执行交换，并让这些人 leave
                int tmp = allocation[agent];
                while(!S.empty())
                {
                    int u = S.top();
                    S.pop();
                    out[u] = 1;
                    std::swap(allocation[u], tmp);
                    N_out.push_back(u);
                    if(u == agent) break;
                }
            }
            
        }

        // share 过程
        std::vector<int> neighboor;
        neighboor.clear();
        int repeat[N];
        memset(repeat, 0, sizeof(repeat));

        // 获取所有 leave 的 agent 的邻居
        while(!N_out.empty())
        {
            int u = N_out.back();
            N_out.pop_back();
            for (int i = 1; i <= n; i++)
            {
                if(!is_neighboor(u, i)) continue;
                if(repeat[i] == 1) continue;
                repeat[i] = 1;
                neighboor.push_back(i);
            }
        }
        // 让这些邻居两两之间相连
        for(int i = 0; i < neighboor.size(); i++)
          for(int j = 0; j < neighboor.size(); j++)
          {
              if(i == j) continue;
              edge[i][j] = 1;
          }
    }
    
    // printf("p = %.3lf, size = %d, score after reallocation = %.3lf\n", p, size, get_score());
    return get_score();

}

int main()
{
    double final[20];
    memset(final, 0, sizeof(final));
    int count = 0;
    for(n = 100; n <= 200; n += 10)
    {
        count++;
        for(int k = 1; k <= 100; k++)
        {
            tot_clear();
            game_init(k);
            final[count] += game(1, n, k);
        }
        // for(double p = 0; p <= 1; p += 0.1)
        // {
        //     game_clear();
        //     final[++count] += game(p, n, n);
        // }
        // for(double size = 20; size <= 30; size += 1)
        // {
        //     game_clear();
        //     final[++count] += game(0.2, size, k);
        // }
    }
    count = 0;
    for(double p = 100; p <= 200; p += 10)
    {
        printf("n = %.3lf, score after reallocation = %.3lf\n", p, final[++count] / 100);
    }
    // for(double size = 20; size <= 30; size += 1)
    // {
    //     printf("size = %lf, score after reallocation = %.3lf\n", size, final[++count] / 100.0);
    // }
    return 0;
}