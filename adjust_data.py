filename="./dataset/CA1.txt"
output_filename="./dataset/CA1.txt"

# 讀取檔案
with open(filename, "r") as file:
    lines = file.readlines()

# 處理數字對的順序問題
pairs = set()
filtered_lines = []
for i in range(2, len(lines)):  # 從第三行開始處理
    nums = list(map(int, lines[i].strip().split()))
    # 檢查是否有重複的數字對，如果有則跳過
    if nums[0] == nums[1]:
        continue
    pair = (min(nums), max(nums))
    # 確保數字對是以小數為左邊，大數為右邊的形式
    if pair not in pairs:
        pairs.add(pair)
        filtered_lines.append(f"{pair[0]}\t{pair[1]}\n")

# 將原始檔案的前兩行插入到處理後的結果中
filtered_lines.insert(0, lines[0])
filtered_lines.insert(1, lines[1])

# 寫入處理後的結果到新檔案
with open(output_filename, "w") as file:
    file.writelines(filtered_lines)
