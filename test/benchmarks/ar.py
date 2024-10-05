import numpy as np 
import pandas as pd 
import random
import statsmodels.api as sm
from statsmodels.tsa.ar_model import AutoReg

from time import perf_counter

def writeARTestData(order: int, count: int, fname: str) -> None:
    ar_params = np.random.normal(0, 0.15, size=order)
    print(f"AR Params: {ar_params}")

    # First p values are random
    vals = [x for x in np.random.rand(order)]

    # Later values then generated as AR process 
    for i in range(order, count):
        val = 7 # Assume 0 mean for now 
        for j in range(order):
            val += ar_params[j] * vals[i-j-1]

        vals.append(val + np.random.normal(0, 0.05))

    print(vals)

    # Write to file 
    with open(fname, 'w') as f:
        f.write('\n'.join(map(str, vals)))

def testARModel(order: int, fname: str) -> None:
    vals = [] 
    with open(fname, 'r') as f:
        vals = list(map(float, f.readlines()))
    
    # Train/Test split
    count = len(vals)
    train_count = int(0.8 * count)
    train_data = vals[:train_count]
    test_data = vals[train_count:]

    # Train model 
    start = perf_counter()
    model = AutoReg(train_data, lags=order)
    model_fit = model.fit()

    # Predict
    predictions = model_fit.predict(start=train_count, end=count-1)
    end = perf_counter()
    
    # Calculate MSE
    mse = np.mean((predictions - test_data)**2)
    runtime = (end - start) * 1_000_000
    # Print results
    print(model_fit.summary())
    print(f"Time: {runtime:.2f} microseconds")
    print(f"MSE: {mse}")


writeARTestData(5, 100, "ar_data.txt")
testARModel(5, "ar_data.txt")