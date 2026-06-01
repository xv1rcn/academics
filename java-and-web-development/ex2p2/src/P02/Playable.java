public interface Playable {
    void play();
    void pause();
    void stop();

    default void displayInfo() {
        System.out.println("正在播放媒体...");
    }
}
