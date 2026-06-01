public class PercentageDiscountStrategy implements DiscountStrategy {
    private double rate;

    public PercentageDiscountStrategy(double rate) {
        this.rate = rate;
    }

    @Override
    public double applyDiscount(double originalAmount) {
        return originalAmount * rate;
    }
}
