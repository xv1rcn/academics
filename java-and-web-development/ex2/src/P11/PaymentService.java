public class PaymentService {
    public void processPayment(Payment payment, double amount) {
        payment.pay(amount);
    }
}
