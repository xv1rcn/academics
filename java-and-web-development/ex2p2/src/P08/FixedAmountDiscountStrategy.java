public class FixedAmountDiscountStrategy implements DiscountStrategy {
    private double discount;

    public FixedAmountDiscountStrategy(double discount) {
        this.discount = discount;
    }

    @Override
    public double applyDiscount(double originalAmount) {
        double result = originalAmount - discount;
        return Math.max(result, 0);
    }
}
