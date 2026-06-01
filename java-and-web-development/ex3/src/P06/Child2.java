class Child2 extends Parent {
    @Override
    public void readFile() {
        System.out.printf("子类Child2.readFile() - 重写后不抛出任何异常%n");
    }
}
