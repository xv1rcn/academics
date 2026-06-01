public class BluetoothAdapter {
    private static final BluetoothAdapter instance = new BluetoothAdapter();

    private BluetoothAdapter() {
        if (instance != null) {
            throw new RuntimeException("非法重复创建蓝牙适配器对象");
        }
    }

    public static BluetoothAdapter getInstance() {
        return instance;
    }

    public void connect() {
        System.out.println("连接蓝牙设备");
    }
}
