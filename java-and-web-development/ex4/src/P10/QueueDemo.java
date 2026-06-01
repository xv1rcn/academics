import java.util.LinkedList;
import java.util.Queue;

public class QueueDemo {
    public static void main(String[] args) {
        Queue<String> queue = new LinkedList<>();

        queue.offer("任务1");
        queue.offer("任务2");
        queue.offer("任务3");

        System.out.printf("队首元素: %s%n", queue.peek());
        System.out.printf("队列大小: %d%n", queue.size());
        System.out.printf("队列为空: %s%n", queue.isEmpty());

        while (!queue.isEmpty()) {
            System.out.printf("取出: %s%n", queue.poll());
        }

        System.out.printf("取完后队列为空: %s%n", queue.isEmpty());
    }
}
