public class DataServiceTest {
    public static void main(String[] args) {
        try {
            String data = DataService.getDataFromFile("test.txt");
            System.out.printf("获取到数据: %s%n", data);
        } catch (DataAccessException e) {
            System.out.printf("捕获到数据访问异常: %s%n", e.getMessage());
            System.out.printf("原始异常堆栈跟踪:%n");
            e.getCause().printStackTrace();
        }
    }
}
