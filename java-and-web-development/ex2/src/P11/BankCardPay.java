public class BankCardPay implements Payment {
    @Override
    public void pay(double amount) {
        System.out.println("使用银行卡支付: " + amount + " 元");
    }
}
