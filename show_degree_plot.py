import sys
import matplotlib.pyplot as plt

# 讀取檔案
file_name = sys.argv[1]
with open(file_name, 'r') as file:
    lines = file.readlines()

# 解析資料
degree = []
number = []

is_first_line = True
for line in lines:
    if is_first_line:
        title = line
        is_first_line = False
    else:
        edge = line.split()
        degree.append(int(edge[0]))
        number.append(int(edge[1]))

# 獲取最小和最大的 degree 值
min_degree = min(degree)
max_degree = max(degree)
one_degree_percent = (number[0]+number[1])/sum(number)*100
# 繪製直方圖
print(f"{one_degree_percent} %")
plt.bar(degree, number)
plt.xlabel('Degree')
plt.ylabel('Number')
plt.title(title)

# 設定橫軸範圍
plt.xlim(min_degree, 100)
plt.show()
