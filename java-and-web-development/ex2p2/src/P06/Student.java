public class Student implements Comparable<Student> {
    private String name;
    private String id;
    private double chinese;
    private double math;
    private double english;

    public Student(String name, String id,
        double chinese, double math, double english) {
        this.name = name;
        this.id = id;
        this.chinese = chinese;
        this.math = math;
        this.english = english;
    }

    public double getTotal() {
        return chinese + math + english;
    }

    @Override
    public int compareTo(Student other) {
        return Double.compare(other.getTotal(), this.getTotal());
    }

    public String getName() { return name; }
    public String getId() { return id; }
    public double getChinese() { return chinese; }
    public double getMath() { return math; }
    public double getEnglish() { return english; }

    @Override
    public String toString() {
        return "学生 { " +
            "姓名 = '" + name + "', " +
            "学号 = '" + id + "', " +
            "总分 = " + getTotal() + ", " +
            "语文 = " + chinese + ", " +
            "数学 = " + math + ", " +
            "英语 = " + english +
            " }";
    }
}
