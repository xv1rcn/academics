public abstract class Staff {
    protected String name;
    protected String id;
    protected double baseSalary;

    public Staff(String name, String id, double baseSalary) {
        this.name = name;
        this.id = id;
        this.baseSalary = baseSalary;
    }

    public abstract double getActualSalary();

    public abstract String getWorkDescription();

    public void printInfo() {
        System.out.println("工号: " + id + ", 姓名: " + name
            + ", 实际工资: " + getActualSalary());
    }

    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getId() {
        return id;
    }
    public void setId(String id) {
        this.id = id;
    }
    public double getBaseSalary() {
        return baseSalary;
    }
    public void setBaseSalary(double baseSalary) {
        this.baseSalary = baseSalary;
    }
}
