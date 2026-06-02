import java.io.*;
import java.net.*;

public class TcpEcho {
    public static void main(String[] args) throws Exception {
        new Thread(() -> {
            try {
                ServerSocket serverSocket = new ServerSocket(8888);
                System.out.printf("服务器启动，监听端口 8888%n");
                Socket clientSocket = serverSocket.accept();
                System.out.printf("客户端已连接%n");
                BufferedReader in = new BufferedReader(
                    new InputStreamReader(clientSocket.getInputStream(), "UTF-8"));
                PrintWriter out = new PrintWriter(
                    new OutputStreamWriter(clientSocket.getOutputStream(), "UTF-8"), true);
                String line;
                while ((line = in.readLine()) != null) {
                    System.out.printf("服务器收到: %s%n", line);
                    out.println(line);
                    if ("bye".equalsIgnoreCase(line)) {
                        break;
                    }
                }
                clientSocket.close();
                serverSocket.close();
                System.out.printf("服务器关闭%n");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }).start();

        Thread.sleep(200);

        try {
            Socket socket = new Socket("localhost", 8888);
            System.out.printf("客户端连接到服务器%n");
            PrintWriter out = new PrintWriter(
                new OutputStreamWriter(socket.getOutputStream(), "UTF-8"), true);
            BufferedReader in = new BufferedReader(
                new InputStreamReader(socket.getInputStream(), "UTF-8"));

            String[] messages = {"你好服务器", "hello echo", "BYE"};
            for (String msg : messages) {
                System.out.printf("客户端发送: %s%n", msg);
                out.println(msg);
                String echo = in.readLine();
                System.out.printf("客户端收到回显: %s%n", echo);
            }
            socket.close();
            System.out.printf("客户端关闭%n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
