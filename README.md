Stock_Analyzer
==============
I used geometric Brownian motion (GBM) of stock prices which means that the stock price follows a random walk and the next 
price movement is "conditionally independent" of past price movements. This theory has two components to estimate stock 
prices one is volatility and other is drift rate.

Volatility and drift rate are calculated as the standard deviation and average of the logarithmic difference of historical 
stock prices respectively. For each time period, my model assumes the price will drift up by the expected return. 
But the drift will be shocked (added or subtracted) by a random shock. By using the above algorithm I used Monte-Carlo 
simulations to get a probability distribution of stock price after a certain time. From which I calculated the most 
probable value of stock price and further calculated the Value at Risk (VaR) at certain confidence level.

Input is provided through a text file which stores the historical data of stock prices. This data is gathered automatically
through Yahoo API.
