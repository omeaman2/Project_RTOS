import numpy as np
import matplotlib.pyplot as plt

x = [0,15]
y = [0,1,2,3,4]
plt.plot(x,y)
plt.xticks(np.arange(min(x), max(x)+1, 1.0))
plt.show()
