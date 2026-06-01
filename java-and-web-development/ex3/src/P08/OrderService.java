public class OrderService {
    public void placeOrder(int qty) throws BusinessException {
        if (qty <= 0) {
            throw new BusinessException("订单数量必须大于0", 1001);
        }
        if (qty > 100) {
            throw new BusinessException("订单数量不能超过100", 1002);
        }
        System.out.printf("订单已成功下单，数量: %d%n", qty);
    }
}
