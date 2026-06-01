public class Employee extends Staff {
    public Employee(String name, String id, double baseSalary) {
        super(name, id, baseSalary);
    }

    @Override
    public double getActualSalary() {
        return baseSalary;
    }

    @Override
    public String getWorkDescription() {
        return "员工负责日常工作";
    }
}
