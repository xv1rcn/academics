public class Alipay implements PaymentMethod {
    @Override
    public void pay(double amount) {
        System.out.printf("支付宝支付: ￥%.2f%n", amount);
    }
}
