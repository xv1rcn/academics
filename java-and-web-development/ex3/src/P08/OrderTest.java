public class OrderTest {
    public static void main(String[] args) {
        OrderService service = new OrderService();
        int[] testCases = {50, 0, -1, 101};
        for (int i = 0; i < testCases.length; i++) {
            int qty = testCases[i];
            System.out.printf("测试 %d: 下单数量 = %d%n", i + 1, qty);
            try {
                service.placeOrder(qty);
            } catch (BusinessException e) {
                System.out.printf("业务异常: 错误码=%d, 消息=%s%n", e.getErrorCode(), e.getMessage());
            }
        }
    }
}
