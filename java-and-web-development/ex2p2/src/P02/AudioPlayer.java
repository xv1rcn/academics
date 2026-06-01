public class AudioPlayer implements Playable {
    @Override
    public void play() {
        System.out.println("音频播放中");
    }

    @Override
    public void pause() {
        System.out.println("音频暂停");
    }
    
    @Override
    public void stop() {
        System.out.println("音频停止");
    }
}
