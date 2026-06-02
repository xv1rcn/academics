import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.text.SimpleDateFormat;
import java.util.Date;

public class UdpTimeServer {

    private static final int PORT = 9877;
    private static final int TIMEOUT_MS = 2000;
    private static final String DATE_FORMAT = "yyyy-MM-dd HH:mm:ss";

    private static class TimeServer implements Runnable {
        private volatile boolean running = true;
        private DatagramSocket socket;

        @Override
        public void run() {
            try {
                socket = new DatagramSocket(PORT);
                System.out.printf("UDP时间服务器启动，端口: %d%n", PORT);
                byte[] buf = new byte[256];
                SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT);

                while (running) {
                    DatagramPacket request = new DatagramPacket(buf, buf.length);
                    socket.receive(request);

                    String timeStr = sdf.format(new Date(System.currentTimeMillis()));
                    byte[] replyData = timeStr.getBytes();

                    DatagramPacket reply = new DatagramPacket(
                            replyData, replyData.length,
                            request.getAddress(), request.getPort());
                    socket.send(reply);

                    System.out.printf("收到请求来自 %s:%d，回复时间: %s%n",
                            request.getAddress().getHostAddress(),
                            request.getPort(), timeStr);
                }
            } catch (Exception e) {
                if (running) {
                    System.out.printf("服务器异常: %s%n", e.getMessage());
                }
            } finally {
                if (socket != null && !socket.isClosed()) {
                    socket.close();
                }
            }
        }

        public void stop() {
            running = false;
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
        }
    }

    private static void runClient() {
        try {
            DatagramSocket clientSocket = new DatagramSocket();
            clientSocket.setSoTimeout(TIMEOUT_MS);
            InetAddress serverAddr = InetAddress.getByName("localhost");
            SimpleDateFormat sdf = new SimpleDateFormat(DATE_FORMAT);

            for (int i = 1; i <= 10; i++) {
                byte[] sendData = "TIME_REQUEST".getBytes();
                DatagramPacket sendPacket = new DatagramPacket(
                        sendData, sendData.length, serverAddr, PORT);

                long t0 = System.currentTimeMillis();
                clientSocket.send(sendPacket);

                byte[] recvBuf = new byte[256];
                DatagramPacket recvPacket = new DatagramPacket(recvBuf, recvBuf.length);
                clientSocket.receive(recvPacket);

                long t1 = System.currentTimeMillis();
                long rtt = t1 - t0;

                String serverTime = new String(recvPacket.getData(), 0, recvPacket.getLength());
                System.out.printf("请求 #%d: 服务器时间=%s, RTT=%dms%n", i, serverTime, rtt);

                if (i < 10) {
                    Thread.sleep(200);
                }
            }

            clientSocket.close();
        } catch (java.net.SocketTimeoutException e) {
            System.out.printf("客户端超时: 服务器无响应(超过%dms)%n", TIMEOUT_MS);
        } catch (Exception e) {
            System.out.printf("客户端异常: %s%n", e.getMessage());
        }
    }

    public static void main(String[] args) {
        TimeServer server = new TimeServer();
        Thread serverThread = new Thread(server);
        serverThread.start();

        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        runClient();

        server.stop();

        try {
            serverThread.join(1000);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        System.out.printf("演示结束%n");
    }
}
