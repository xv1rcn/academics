public interface StockOption {
    void grantOptions(int numberOfShares);
    int getOutstandingOptions();
}
