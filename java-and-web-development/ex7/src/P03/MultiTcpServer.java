import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.atomic.AtomicInteger;

public class MultiTcpServer {

    private static final AtomicInteger onlineCount = new AtomicInteger(0);

    public static void main(String[] args) {
        Thread serverThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try (ServerSocket server = new ServerSocket(9000)) {
                    System.out.printf("服务器启动，端口: %d%n", 9000);
                    while (true) {
                        Socket client = server.accept();
                        new Thread(new ClientHandler(client)).start();
                    }
                } catch (Exception e) {
                    System.out.printf("服务器异常: %s%n", e.getMessage());
                }
            }
        });
        serverThread.setDaemon(true);
        serverThread.start();

        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        for (int i = 1; i <= 3; i++) {
            final int id = i;
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try (Socket socket = new Socket("localhost", 9000);
                         PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                         BufferedReader in = new BufferedReader(
                             new InputStreamReader(socket.getInputStream()))) {

                        String msg = "hello" + id;
                        out.println(msg);
                        String resp = in.readLine();
                        System.out.printf("客户端%d 发送: %s, 收到: %s%n", id, msg, resp);

                        out.println("quit");
                    } catch (Exception e) {
                        System.out.printf("客户端%d 异常: %s%n", id, e.getMessage());
                    }
                }
            }).start();
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }

        try {
            Thread.sleep(500);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    static class ClientHandler implements Runnable {
        private final Socket socket;

        ClientHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            int count = onlineCount.incrementAndGet();
            String remote = socket.getInetAddress().getHostAddress();
            System.out.printf("客户端连接: %s, 当前在线: %d%n", remote, count);
            try (
                BufferedReader in = new BufferedReader(
                    new InputStreamReader(socket.getInputStream()));
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true)
            ) {
                String line;
                while ((line = in.readLine()) != null) {
                    if ("quit".equalsIgnoreCase(line.trim())) {
                        break;
                    }
                    out.println(line.toUpperCase());
                }
            } catch (Exception e) {
                System.out.printf("客户端处理异常: %s%n", e.getMessage());
            } finally {
                count = onlineCount.decrementAndGet();
                System.out.printf("客户端断开: %s, 当前在线: %d%n", remote, count);
            }
        }
    }
}
