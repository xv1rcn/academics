public class Buffer {
    private int value;
    boolean isEmpty = true;

    public synchronized void put(int v) {
        while (!isEmpty) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        value = v;
        isEmpty = false;
        System.out.printf("生产: %d%n", v);
        notify();
    }

    public synchronized int take() {
        while (isEmpty) {
            try {
                wait();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        int v = value;
        isEmpty = true;
        System.out.printf("消费: %d%n", v);
        notify();
        return v;
    }
}
