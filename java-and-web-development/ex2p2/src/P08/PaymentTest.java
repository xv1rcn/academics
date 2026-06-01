public class PaymentTest {
    public static void main(String[] args) {
        System.out.println("=== 支付宝 + 9 折 ===");
        PaymentContext ctx1 = new PaymentContext(
            new Alipay(),
            DiscountType.createStrategy("PERCENTAGE", 0.9)
        );
        ctx1.pay(300);

        System.out.println("\n=== 微信 + 满 200 减 20 ===");
        PaymentContext ctx2 = new PaymentContext(
            new WechatPay(),
            DiscountType.createStrategy("THRESHOLD", 200, 20)
        );
        ctx2.pay(300);
        System.out.println("（未满减情况）");
        ctx2.pay(150);

        System.out.println("\n=== 信用卡 + 无优惠 ===");
        PaymentContext ctx3 = new PaymentContext(
            new CreditCard(),
            DiscountType.createStrategy("NONE")
        );
        ctx3.pay(300);

        System.out.println("\n=== 支付宝 + 固定减 50 ===");
        PaymentContext ctx4 = new PaymentContext(
            new Alipay(),
            DiscountType.createStrategy("FIXED", 50)
        );
        ctx4.pay(300);
    }
}
