# 查找最后一个等于或者大于key的元素
def findLastEqualSmaller(arr, key):
    left = 0
    right = len(arr) - 1
    length = len(arr)

    # 这里必须是 <=
    while left <= right:
        mid = (left + right) // 2
        if arr[mid] >= key:
            right = mid - 1
        else:
            left = mid + 1
    if left>=length:
        left=length-1
    return left


arr=[6,3,4,1,9]
idx = findLastEqualSmaller(arr,11)
print('Idx:',idx)
print('Value:',arr[idx])