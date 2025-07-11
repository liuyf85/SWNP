import matplotlib.pyplot as plt

# 数据提取
p_values = [0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1]
scores_our = [0.000, 29.771, 46.232, 56.645, 61.523, 67.676, 70.688, 73.728, 75.257, 77.568, 79.488]
scores_swn = [0.000, 22.776, 34.321, 43.424, 50.695, 56.533, 61.242, 64.517, 67.284, 70.104, 72.168]
scores_ls = [0.000, 25.756, 48.776, 62.773, 70.491, 75.005, 78.041, 80.472, 82.103, 83.656, 85.076]

# 绘制折线图
plt.figure(figsize=(10, 6))
plt.plot(p_values, scores_our, marker='o', label='Our Algorithm')
plt.plot(p_values, scores_swn, marker='s', label='SWN Algorithm')
plt.plot(p_values, scores_ls, marker='^', label='LS Algorithm')

# 添加标题和标签
plt.title('Comparison of Algorithm Performance')
plt.xlabel('p')
plt.ylabel('Score after Reallocation')
plt.legend()

# 显示图表
plt.grid(True)
plt.show()