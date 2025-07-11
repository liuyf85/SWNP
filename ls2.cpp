#include <stdio.h>
#include <random>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

#define N 101
const int n = 100;

int edge[N][N];
int preference[N][N];
int allocation[N];
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
void create_graph(double p)
{
    for (int i = 1; i <= n; i++)
      for (int j = i + 1; j <= n; j++)
      {
          double t = rand() * 1.0 / RAND_MAX;
          edge[i][j] = edge[j][i] = (t <= p);
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
    // // 每一个都可以指向 Stack_bottom
    // for(int i = 1; i <= n; i++) 
    // {
    //     if(out[i]) continue;
    //     trade[i][bottom] = 1;
    // }
}

void game_init()
{
    create_graph(0.2);
	for (int i = 1; i <= n; i++)
    {
        // preference[i][j] 表示第 i 个 agent 认为 item j 排在 preference[i][j] 位
        create_permuation(preference[i]);
    }
    // 初始化分配
    create_permuation(allocation);
}

int get_score()
{
    int score = 0;
    for (int i = 1; i <= n; i++)
    {
        score += (n - preference[i][allocation[i]] + 1);
    }
    return score;
}

int main()
{
    // 初始化
    game_init();
    get_deep(1);

    printf("score before reallocation = %d\n", get_score());

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
    
    printf("score after reallocation = %d\n", get_score());

    return 0;
}