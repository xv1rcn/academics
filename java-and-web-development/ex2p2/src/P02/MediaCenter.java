public class MediaCenter {
    public static void main(String[] args) {
        Playable[] players = {new AudioPlayer(), new VideoPlayer()};

        for (Playable player : players) {
            player.play();
            player.pause();
            player.stop();
            player.displayInfo();

            if (player instanceof SpeedControl) {
                ((SpeedControl) player).doubleSpeed();
            }

            if (player instanceof VideoPlayer) {
                ((VideoPlayer) player).setBrightness(80);
            }
        }
    }
}
