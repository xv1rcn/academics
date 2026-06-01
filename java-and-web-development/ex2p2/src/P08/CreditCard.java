public class CreditCard implements PaymentMethod {
    @Override
    public void pay(double amount) {
        System.out.printf("信用卡支付: ￥%.2f%n", amount);
    }
}
