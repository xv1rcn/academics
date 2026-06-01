public class Main {
    public static void main(String[] args) {
        BluetoothAdapter bt1 = BluetoothAdapter.getInstance();
        BluetoothAdapter bt2 = BluetoothAdapter.getInstance();

        System.out.println("bt1 == bt2 : " + (bt1 == bt2));
        bt2.connect();
    }
}
