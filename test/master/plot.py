import numpy as np
import matplotlib.pyplot as plt

sqrt2 = np.sqrt(2)
# import subprocess
# # 
# subprocess.run(['cd', '../../'])
# subprocess.run(['make', 'test'])
# subprocess.run(['./check'])
# subprocess.run(['cd', 'test/master/'])
# # subprocess.run(["make", "test", "-lm", "-O3"])
# subprocess.run(["./a.out"])

data1 = np.loadtxt("coupled1.txt", delimiter= " ")
t1_n = data1[0]
y1_n = data1[1]

print(len(y1_n))

data2 = np.loadtxt("coupled2.txt", delimiter= " ")
t2_n = data2[0]
y2_n = data2[1]

data3 = np.loadtxt("coupled3.txt", delimiter= " ")
t3_n = data3[0]
y3_n = data3[1]
# print(data)
t = np.linspace(0, 2, 2000)
# x = ((5 * sqrt2 - 1) * np.exp(2 * sqrt2 * t - sqrt2 * t + t) / 4 
    #  - (5 * sqrt2 + 1) * np.exp(t - sqrt2 * t) / 4 
    #  + 1 / (2 * np.exp(t)) + 1)
# 
# y = ((5 * sqrt2 - 1) * np.exp(2 * sqrt2 * t - sqrt2 * t + t) / 4 
    #  - (5 * sqrt2 + 1) * np.exp(t - sqrt2 * t) / 4 
    #  - 1 / (2 * np.exp(t)) + 2)
# 
# z = ((5 * sqrt2 - 1) * np.exp(2 * sqrt2 * t - sqrt2 * t + t) / (2 * sqrt2) 
    #  + (5 * sqrt2 + 1) * np.exp(t - sqrt2 * t) / (2 * sqrt2) 
    #  - 4)
# plt.plot(t* np.sqrt(1 - 2 * np.log(t)), t)
plt.plot(t1_n, y1_n, label = "x, rk45")
plt.plot(t2_n, y2_n, label = "y, rk45")
# plt.scatter(t, 3.5 * np.exp(t) + 0.5 * np.exp(-t) - 3, c="r")
# plt.scatter(t, 3.5 * np.exp(t) - 0.5 * np.exp(-t) - 2, c="black")
plt.plot(t3_n, y3_n, label = "z, rk45")
# plt.plot(t, x, label = "x")
# plt.plot(t, y, label = "y")
# plt.plot(t, z, label = "z")
plt.legend()
# plt.plot(t, 2 * np.exp(t) - t - 1)
plt.grid()
plt.show()