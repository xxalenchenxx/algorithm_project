import sys
import networkx as nx
import matplotlib.pyplot as plt

# 讀取檔案
file_name = sys.argv[1]
with open(file_name, 'r') as file:
    lines = file.readlines()

# 解析邊的資訊
u = []
v = []
for line in lines:
    edge = line.split()
    u.append(int(edge[0]))
    v.append(int(edge[1]))

# 建立圖形
G = nx.Graph()
for i in range(len(u)):
    G.add_edge(u[i], v[i])

# 繪製圖形
nx.draw(G, with_labels=True)
plt.show()
plt.pause(0.001)
plt.close()