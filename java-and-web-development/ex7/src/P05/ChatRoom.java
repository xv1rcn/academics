import java.io.*;
import java.net.*;
import java.util.*;

public class ChatRoom {
    private static final List<PrintWriter> clientWriters = Collections.synchronizedList(new ArrayList<>());
    private static final Map<PrintWriter, String> nicknames = Collections.synchronizedMap(new HashMap<>());
    private static volatile ServerSocket serverSocket;

    public static void main(String[] args) throws Exception {
        Thread serverThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    serverSocket = new ServerSocket(9002);
                    System.out.printf("[服务器] 已启动，端口 %d%n", 9002);
                    System.out.flush();
                    while (!serverSocket.isClosed()) {
                        Socket sock = serverSocket.accept();
                        new Thread(new ClientHandler(sock)).start();
                    }
                } catch (IOException e) {
                    System.out.printf("[服务器] 已关闭%n");
                    System.out.flush();
                }
            }
        });
        serverThread.start();
        Thread.sleep(200);

        String[] nickVals = {"用户A", "用户B", "用户C"};
        String[] msgs = {"大家好！", "你好！", "来了！"};
        Thread[] clientThreads = new Thread[3];

        for (int i = 0; i < 3; i++) {
            final String name = nickVals[i];
            final String msg = msgs[i];
            clientThreads[i] = new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Socket sock = new Socket("localhost", 9002);
                        PrintWriter out = new PrintWriter(sock.getOutputStream(), true);
                        BufferedReader in = new BufferedReader(
                                new InputStreamReader(sock.getInputStream()));

                        Thread reader = new Thread(new Runnable() {
                            @Override
                            public void run() {
                                try {
                                    String line;
                                    while ((line = in.readLine()) != null) {
                                        System.out.printf("[%s] %s%n", name, line);
                                        System.out.flush();
                                    }
                                } catch (IOException ignored) {
                                }
                            }
                        });
                        reader.setDaemon(true);
                        reader.start();

                        out.println(name);
                        Thread.sleep(80);
                        out.println(msg);
                        Thread.sleep(400);
                        sock.close();
                    } catch (Exception e) {
                        System.out.printf("客户端异常: %s%n", e.getMessage());
                        System.out.flush();
                    }
                }
            });
            clientThreads[i].start();
            Thread.sleep(120);
        }

        for (int i = 0; i < 3; i++) {
            clientThreads[i].join();
        }

        Thread.sleep(500);
        if (serverSocket != null && !serverSocket.isClosed()) {
            serverSocket.close();
        }
        serverThread.join();
        System.out.printf("[服务器] 聊天结束%n");
        System.out.flush();
    }

    static class ClientHandler implements Runnable {
        private final Socket socket;
        private PrintWriter out;
        private String nickname;

        ClientHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            try {
                BufferedReader in = new BufferedReader(
                        new InputStreamReader(socket.getInputStream()));
                out = new PrintWriter(socket.getOutputStream(), true);

                nickname = in.readLine();
                if (nickname == null) {
                    return;
                }
                clientWriters.add(out);
                nicknames.put(out, nickname);
                System.out.printf("[服务器] %s 加入聊天室%n", nickname);
                broadcastToOthers(nickname + " 加入聊天室");
                System.out.flush();

                String line;
                while ((line = in.readLine()) != null) {
                    System.out.printf("[服务器] %s: %s%n", nickname, line);
                    broadcastToOthers(nickname + ": " + line);
                    System.out.flush();
                }
            } catch (IOException e) {
            } finally {
                if (out != null) {
                    clientWriters.remove(out);
                    if (nickname != null) {
                        nicknames.remove(out);
                        System.out.printf("[服务器] %s 离开聊天室%n", nickname);
                        broadcastToOthers(nickname + " 离开聊天室");
                        System.out.flush();
                    }
                }
                try {
                    socket.close();
                } catch (IOException ignored) {
                }
            }
        }

        private void broadcastToOthers(String msg) {
            synchronized (clientWriters) {
                for (PrintWriter w : clientWriters) {
                    if (w != out) {
                        w.println(msg);
                    }
                }
            }
        }

        private void broadcastToAll(String msg) {
            synchronized (clientWriters) {
                for (PrintWriter w : clientWriters) {
                    w.println(msg);
                }
            }
        }
    }
}
