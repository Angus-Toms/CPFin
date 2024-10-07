from statsmodels.tsa.arima.model import ARIMA
from time import perf_counter
import numpy as np

def trainARModel(order, fname):
    # Read data
    with open(fname, 'r') as f:
        ar = np.array(list(map(float, f.read().split())))
    
    # Train model on 80% of data
    start = perf_counter()
    model = ARIMA(ar[:800], order=(order, 0, 0)).fit()
    # Make predictions on remaining 20%
    pred = model.predict(start=800, end=999)
    end = perf_counter()

    rmse = np.sqrt(np.mean((pred - ar[800:])**2))

    return 1_000 * (end - start), rmse

def trainMAModel(order, fname):
    # Read data
    with open(fname, 'r') as f:
        ma = np.array(list(map(float, f.read().split())))
    
    # Train model on 80% of data
    start = perf_counter()
    model = ARIMA(ma[:800], order=(0, 0, order)).fit()
    # Make predictions on remaining 20%
    pred = model.predict(start=800, end=999)
    end = perf_counter()

    rmse = np.sqrt(np.mean((pred - ma[800:])**2))

    return 1_000 * (end - start), rmse

def trainARMAModel(ar_order, ma_order, fname):
    # Read data
    with open(fname, 'r') as f:
        arma = np.array(list(map(float, f.read().split())))
    
    # Train model on 80% of data
    start = perf_counter()
    model = ARIMA(arma[:800], order=(ar_order, 0, ma_order)).fit()
    # Make predictions on remaining 20%
    pred = model.predict(start=800, end=999)
    end = perf_counter()

    rmse = np.sqrt(np.mean((pred - arma[800:])**2))

    return 1_000 * (end - start), rmse

if __name__ == "__main__":
    ar5 = [trainARModel(5, f"ar(5)_{i}.txt") for i in range(5)]
    print("Done AR(5)")
    ar50 = [trainARModel(25, f"ar(25)_{i}.txt") for i in range(5)]
    print("Done AR(25)")
    ma5 = [trainMAModel(5, f"ma(5)_{i}.txt") for i in range(5)]
    print("Done MA(5)")
    ma50 = [trainMAModel(25, f"ma(25)_{i}.txt") for i in range(5)]
    print("Done MA(25)")
    arma55 = [trainARMAModel(5, 5, f"arma(5,5)_{i}.txt") for i in range(5)]
    print("Done ARMA(5,5)")
    arma5050 = [trainARMAModel(25, 25, f"arma(25,25)_{i}.txt") for i in range(5)]
    print("Done ARMA(25,25)")

    print("AR(5):", np.mean(ar5, axis=0))
    print("AR(25):", np.mean(ar50, axis=0))
    print("MA(5):", np.mean(ma5, axis=0))
    print("MA(25):", np.mean(ma50, axis=0))
    print("ARMA(5,5):", np.mean(arma55, axis=0))
    print("ARMA(25,25):", np.mean(arma5050, axis=0))