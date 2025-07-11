import matplotlib.pyplot as plt

# 从图像中提取的数据
n_values = [100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200]

# Our算法的得分
scores_our = [
    13.723, 15.397, 16.142, 18.173, 19.196, 
    20.758, 21.840, 23.325, 25.181, 26.599, 28.233
]

# SWN算法的得分
scores_swn = [
    10.869, 12.528, 13.125, 14.506, 15.596, 
    16.661, 17.516, 18.720, 19.775, 21.540, 22.638
]

# LS算法的得分
scores_ls = [
    12.594, 14.525, 15.682, 16.837, 18.219, 
    19.307, 20.157, 21.575, 22.848, 24.586, 26.157
]

# 绘制折线图
plt.figure(figsize=(10, 6))
plt.plot(n_values, scores_our, marker='o', label='Our Algorithm')
plt.plot(n_values, scores_swn, marker='s', label='SWN Algorithm')
plt.plot(n_values, scores_ls, marker='^', label='LS Algorithm')

# 添加标题和标签
plt.title('Comparison of Algorithm Performance')
plt.xlabel('n')
plt.ylabel('Score after Reallocation')
plt.legend()

# 显示图表
plt.grid(True)
plt.show()