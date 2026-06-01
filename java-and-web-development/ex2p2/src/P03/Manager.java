public class Manager extends Staff implements StockOption {
    private double bonus;
    private int shareCount;

    public Manager(String name, String id, double baseSalary, double bonus) {
        super(name, id, baseSalary);
        this.bonus = bonus;
    }

    @Override
    public double getActualSalary() {
        return baseSalary + bonus;
    }

    @Override
    public String getWorkDescription() {
        return "经理负责管理团队和制定战略";
    }

    @Override
    public void printInfo() {
        super.printInfo();
        System.out.println("管理者绩效奖金: " + bonus);
    }

    @Override
    public void grantOptions(int numberOfShares) {
        this.shareCount += numberOfShares;
    }

    @Override
    public int getOutstandingOptions() {
        return shareCount;
    }

    public double getBonus() {
        return bonus;
    }

    public void setBonus(double bonus) {
        this.bonus = bonus;
    }
}
