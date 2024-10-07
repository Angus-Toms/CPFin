import numpy as np 
import pandas as pd 
import random

def writeARModel(order, fname):
    # Generate AR data of appropriate order
    coeffs = np.random.normal(0, 0.01, order)
    n = 1000

    # Generate AR data
    ar = np.zeros(n)
    for i in range(order, n):
        ar[i] = np.dot(coeffs, ar[i-order:i]) + np.random.normal()

    # Write to file
    with open(fname, 'w') as f:
        f.write(' '.join(map(str, ar)))

def writeMAModel(order, fname):
    coeffs = np.random.normal(0, 0.01, order)
    n = 1000
    residuals = np.random.normal(size=n)

    ma = np.zeros(n)
    for i in range(order, n):
        ma[i] = np.dot(coeffs, residuals[i-order:i]) + np.random.normal()

    with open(fname, 'w') as f:
        f.write(' '.join(map(str, ma)))

def writeARMAModel(ar_order, ma_order, fname):
    ar_coeffs = np.random.normal(0, 0.01, ar_order)
    ma_coeffs = np.random.normal(0, 0.01, ma_order)
    n = 1000

    ar = np.zeros(n)
    for i in range(ar_order, n):
        ar[i] = np.dot(ar_coeffs, ar[i-ar_order:i]) + np.random.normal()

    residuals = np.random.normal(size=n)
    ma = np.zeros(n)
    for i in range(ma_order, n):
        ma[i] = np.dot(ma_coeffs, residuals[i-ma_order:i])

    arma = ar + ma

    with open(fname, 'w') as f:
        f.write(' '.join(map(str, arma)))

if __name__ == "__main__":
    for i in range(5):
        writeARModel(5, f"ar(5)_{i}.txt")
        writeARModel(25, f"ar(25)_{i}.txt")
        writeMAModel(5, f"ma(5)_{i}.txt")
        writeMAModel(25, f"ma(25)_{i}.txt")
        writeARMAModel(5, 5, f"arma(5,5)_{i}.txt")
        writeARMAModel(25, 25, f"arma(25,25)_{i}.txt")