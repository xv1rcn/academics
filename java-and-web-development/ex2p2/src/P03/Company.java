public class Company {
    public static void main(String[] args) {
        Staff[] staffs = {
            new Employee("张三", "E001", 5000),
            new Employee("李四", "E002", 5500),
            new Manager("王五", "M001", 8000, 3000),
            new Manager("赵六", "M002", 9000, 4500)
        };

        for (Staff staff : staffs) {
            staff.printInfo();
            System.out.println("工作内容: " + staff.getWorkDescription());

            if (staff instanceof StockOption) {
                StockOption optionHolder = (StockOption) staff;
                optionHolder.grantOptions(1000);
                System.out.println("已授予股票期权数量: " + optionHolder.getOutstandingOptions());
            }
        }
    }
}
