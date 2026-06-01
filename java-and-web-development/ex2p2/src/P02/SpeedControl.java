public interface SpeedControl {
    void setSpeed(double speed);

    default void doubleSpeed() {
        setSpeed(2.0);
        System.out.println("已开启倍速模式 (2.0x)");
    }
}
