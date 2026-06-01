public abstract class Shape {
    public abstract double area();

    public abstract double perimeter();

    public abstract void draw();

    public void printInfo() {
        System.out.println(this.getClass().getCanonicalName() + " -");
        System.out.println("面积: " + area());
        System.out.println("周长: " + perimeter());
    }

    public static Shape creatShape(String type, double... params) {
        switch (type.toLowerCase()) {
            case "circle":
                if (params.length != 1) throw new IllegalArgumentException("圆形需要 1 个参数: 半径");
                return new Circle(params[0]);

            case "rectangle":
                if (params.length != 2) throw new IllegalArgumentException("矩形需要 2 个参数: 宽、高");
                return new Rectangle(params[0], params[1]);

            case "triangle":
                if (params.length != 3) throw new IllegalArgumentException("三角形需要 3 个参数: 三边");
                return new Triangle(params[0], params[1], params[2]);

            default:
                throw new IllegalArgumentException("不支持的图形类型: " + type);
        }
    }
}
