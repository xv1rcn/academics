public class ThresholdDiscountStrategy implements DiscountStrategy {
    private double threshold;
    private double discount;

    public ThresholdDiscountStrategy(double threshold, double discount) {
        this.threshold = threshold;
        this.discount = discount;
    }

    @Override
    public double applyDiscount(double originalAmount) {
        if (originalAmount >= threshold) {
            return originalAmount - discount;
        }
        return originalAmount;
    }
}
