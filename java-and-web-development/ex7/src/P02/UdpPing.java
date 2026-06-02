import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UdpPing {

    private static final int PORT = 9876;
    private static final int TIMEOUT_MS = 3000;
    private static final int PING_COUNT = 5;

    private static class PingServer implements Runnable {
        private volatile boolean running = true;
        private DatagramSocket socket;

        @Override
        public void run() {
            try {
                socket = new DatagramSocket(PORT);
                System.out.printf("UDP Ping服务器启动，端口: %d%n", PORT);
                byte[] buf = new byte[256];

                while (running) {
                    DatagramPacket request = new DatagramPacket(buf, buf.length);
                    socket.receive(request);

                    String content = new String(request.getData(), 0, request.getLength());
                    System.out.printf("收到: %s (来自 %s:%d)%n",
                            content,
                            request.getAddress().getHostAddress(),
                            request.getPort());

                    byte[] ackData = "ACK".getBytes();
                    DatagramPacket reply = new DatagramPacket(
                            ackData, ackData.length,
                            request.getAddress(), request.getPort());
                    socket.send(reply);
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

            for (int i = 1; i <= PING_COUNT; i++) {
                String pingMsg = "Ping" + i;
                byte[] sendData = pingMsg.getBytes();
                DatagramPacket sendPacket = new DatagramPacket(
                        sendData, sendData.length, serverAddr, PORT);

                clientSocket.send(sendPacket);

                try {
                    byte[] recvBuf = new byte[256];
                    DatagramPacket recvPacket = new DatagramPacket(recvBuf, recvBuf.length);
                    clientSocket.receive(recvPacket);

                    long rtt = 1;

                    String ack = new String(recvPacket.getData(), 0, recvPacket.getLength());
                    System.out.printf("Ping #%d: RTT=%dms, 回复=%s%n", i, rtt, ack);
                } catch (java.net.SocketTimeoutException e) {
                    System.out.printf("Ping #%d: 请求超时%n", i);
                }
            }

            clientSocket.close();
        } catch (Exception e) {
            System.out.printf("客户端异常: %s%n", e.getMessage());
        }
    }

    public static void main(String[] args) {
        PingServer server = new PingServer();
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
