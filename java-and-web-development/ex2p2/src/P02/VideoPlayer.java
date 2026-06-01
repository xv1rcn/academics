public class VideoPlayer implements Playable, SubtitleSupport, SpeedControl {
    private double speed = 1.0;
    private int brightness;

    @Override
    public void play() {
        System.out.println("视频播放中");
    }

    @Override
    public void pause() {
        System.out.println("视频暂停");
    }

    @Override
    public void stop() {
        System.out.println("视频停止");
    }

    @Override
    public void loadSubtitle(String subtitleFile) {
        System.out.println("加载字幕文件: " + subtitleFile);
    }

    @Override
    public void displaySubtitle() {
        System.out.println("显示字幕");
    }

    @Override
    public void setSpeed(double speed) {
        this.speed = speed;
        System.out.println("当前播放速度: " + speed + "x");
    }

    public void setBrightness(int level) {
        this.brightness = level;
        System.out.println("亮度已设置为: " + level);
    }
}
