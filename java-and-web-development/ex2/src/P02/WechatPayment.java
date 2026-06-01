public class WechatPayment implements Payment {
    @Override
    public PayResult pay(double amount) {
        if (amount <= 0) {
            System.out.println("微信支付: 金额无效");
            return PayResult.FAILURE;
        }

        if (amount > 10000) {
            System.out.println("微信支付: 金额超过限额");
            return PayResult.FAILURE;
        }

        System.out.println("微信支付: 支付成功，金额为 " + amount + " 元");
        return PayResult.SUCCESS;
    }
}
