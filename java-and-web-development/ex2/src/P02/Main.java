public class Main {
    public static void main(String[] args) {
        Payment cash = new CashPayment();
        Payment wechat = new WechatPayment();

        double amount1 = 500.0;
        double amount2 = 15000.0;
        double amount3 = -100.0;

        System.out.println("使用现金支付:");
        cash.pay(amount1);
        cash.pay(amount2);
        cash.pay(amount3);

        System.out.println("使用微信支付:");
        wechat.pay(amount1);
        wechat.pay(amount2);
        wechat.pay(amount3);
    }
}
