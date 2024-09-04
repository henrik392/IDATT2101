import matplotlib.pyplot as plt
import numpy as np

# Data for quickSort3WayBubbleHelper
random_bubble = [127.368750, 127.119917, 130.758500, 126.954042, 126.116375, 128.149292, 128.005709, 126.294750, 261.791875, 757.068541, 1849.222666, 4245.077875, 9528.577125, 19490.494958]
duplicates_bubble = [70.037875, 68.402041, 67.981417, 66.404125, 69.398125, 65.903833, 66.058334, 112.541417, 221.964083, 492.485833, 1070.260291, 2385.776125, 4796.787750, 10354.754708]
sorted_bubble = [1308.731209, 1342.696583, 1338.854666, 1303.585708, 1303.108500, 1305.828042, 1307.199375, 1333.405291, 1320.034500, 1329.598542, 1382.029792, 1545.783625, 2091.125125, 3831.508875]

# Data for quickSort3WayInsertionHelper
random_insertion = [129.192125, 131.093125, 128.020500, 127.105417, 130.061208, 127.410666, 127.264333, 128.178000, 159.771125, 289.750458, 576.962542, 1201.330083, 2439.386916, 4908.659375]
duplicates_insertion = [66.050334, 70.523958, 65.748458, 69.831750, 66.780458, 71.498083, 66.152250, 80.002209, 110.481125, 180.106958, 326.267708, 644.720209, 1255.192167, 2542.993959]
sorted_insertion = [1336.356209, 1332.882000, 1321.060000, 1331.411708, 1322.862042, 1334.609834, 1322.162209, 1325.692542, 1321.477875, 1330.851125, 1335.320000, 1352.759625, 1375.346375, 1396.359000]

# Calculate the average for each threshold for both sorting helpers
average_bubble = [(rand + dupes + sorted_) / 3 for rand, dupes, sorted_ in zip(random_bubble, duplicates_bubble, sorted_bubble)]
average_insertion = [(rand + dupes + sorted_) / 3 for rand, dupes, sorted_ in zip(random_insertion, duplicates_insertion, sorted_insertion)]
# Since threshold_bubble and threshold_insertion are the same, we can use one common variable for both
threshold = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192]

# Calculate the global max and min across all datasets for a uniform y-scale
global_max = np.max([random_bubble, duplicates_bubble, sorted_bubble, 
                     random_insertion, duplicates_insertion, sorted_insertion, 
                     average_bubble, average_insertion])

global_min = np.min([random_bubble, duplicates_bubble, sorted_bubble, 
                     random_insertion, duplicates_insertion, sorted_insertion, 
                     average_bubble, average_insertion])

# Plotting quickSort3WayBubbleHelper with log base 2 scale and average
plt.figure(figsize=(14, 6))
plt.subplot(1, 2, 1)
plt.title("Bubble sort as helper function")
plt.xlabel("Threshold")
plt.ylabel("Time (ms)")
plt.xscale("log", base=2)
plt.yscale("log", base=10)
plt.ylim(global_min, global_max)
plt.plot(threshold, random_bubble, label="random", marker='o')
plt.plot(threshold, duplicates_bubble, label="duplicates", marker='x')
plt.plot(threshold, sorted_bubble, label="sorted", marker='s')
plt.plot(threshold, average_bubble, label="Average", marker='d', linestyle='--')
plt.legend()

# Plotting quickSort3WayInsertionHelper with log base 2 scale and average
plt.subplot(1, 2, 2)
plt.title("Insertion sort as helper function")
plt.xlabel("Threshold")
plt.ylabel("Time (ms)")
plt.xscale("log", base=2)
plt.yscale("log", base=10)
plt.ylim(global_min, global_max)
plt.plot(threshold, random_insertion, label="Random", marker='o')
plt.plot(threshold, duplicates_insertion, label="Duplicates", marker='x')
plt.plot(threshold, sorted_insertion, label="Sorted", marker='s')
plt.plot(threshold, average_insertion, label="Average", marker='d', linestyle='--')
plt.legend()

plt.tight_layout()
plt.show()
