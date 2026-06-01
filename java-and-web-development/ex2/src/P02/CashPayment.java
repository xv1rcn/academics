public class CashPayment implements Payment {
    @Override
    public PayResult pay(double amount) {
        if (amount <= 0) {
            System.out.println("现金支付: 金额无效");
            return PayResult.FAILURE;
        }

        System.out.println("现金支付: 支付成功，金额为 " + amount + " 元");
        return PayResult.SUCCESS;
    }
}
