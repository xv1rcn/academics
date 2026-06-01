public class PaymentContext {
    private PaymentMethod paymentMethod;
    private DiscountStrategy discountStrategy;

    public PaymentContext(PaymentMethod paymentMethod, DiscountStrategy discountStrategy) {
        this.paymentMethod = paymentMethod;
        this.discountStrategy = discountStrategy;
    }

    public void pay(double amount) {
        double finalAmount = discountStrategy.applyDiscount(amount);
        System.out.printf("原价: ￥%.2f, 折后: ￥%.2f%n", amount, finalAmount);
        paymentMethod.pay(finalAmount);
    }
}
