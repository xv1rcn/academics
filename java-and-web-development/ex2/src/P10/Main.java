public class Main {
    public static void main(String[] args) {
        Logger log1 = Logger.getInstance();
        Logger log2 = Logger.getInstance();

        System.out.println("log1 与 log2 为同一实例: " + (log1 == log2));

        log1.log("系统启动成功");
        log2.log("用户进入首页");
        Logger.getInstance().log("数据加载完成");
    }
}
