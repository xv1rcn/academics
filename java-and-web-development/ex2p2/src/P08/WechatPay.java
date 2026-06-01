public class WechatPay implements PaymentMethod {
    @Override
    public void pay(double amount) {
        System.out.printf("微信支付: ￥%.2f%n", amount);
    }
}
