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
sup = []
for line in lines:
    edge = line.split()
    u.append(int(edge[0]))
    v.append(int(edge[1]))
    sup.append(int(edge[2]))

# 建立圖形
G = nx.Graph()
for i in range(len(u)):
    G.add_edge(u[i], v[i], weight=sup[i])  # 在這裡將weight加入邊的屬性

# 繪製圖形
pos = nx.spring_layout(G, k=1)  # 設置 k 值為較大的值
nx.draw(G, pos, with_labels=True, node_size=70, node_color='blue', font_size=10)  # 繪製節點
nx.draw_networkx_edge_labels(G, pos, edge_labels={(u[i], v[i]): sup[i] for i in range(len(u))})  # 顯示邊的權重
plt.show()
