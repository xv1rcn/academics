public class Main {
    public static void main(String[] args) {
        PaymentService service = new PaymentService();
        
        Payment alipay = new Alipay();
        Payment wechat = new WechatPay();
        Payment bank = new BankCardPay();
        
        service.processPayment(alipay, 199.9);
        service.processPayment(wechat, 299.0);
        service.processPayment(bank, 500.0);
    }
}
