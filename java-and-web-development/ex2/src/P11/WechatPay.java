public class WechatPay implements Payment {
    @Override
    public void pay(double amount) {
        System.out.println("使用微信支付: " + amount + " 元");
    }
    
}
