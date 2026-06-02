import java.io.*;
import java.net.*;
import java.nio.file.*;

public class HttpServer {

    public static void main(String[] args) throws Exception {
        Path buildDir = Paths.get("build");
        Files.createDirectories(buildDir);
        Files.writeString(buildDir.resolve("index.html"), "<h1>欢迎</h1>");
        System.out.printf("已创建 build/index.html%n");

        Thread serverThread = new Thread(() -> {
            try {
                startServer();
            } catch (IOException e) {
                System.err.printf("服务器异常: %s%n", e.getMessage());
            }
        });
        serverThread.setDaemon(true);
        serverThread.start();

        Thread.sleep(200);

        System.out.printf("=== 客户端请求测试 ===%n");
        sendRequest("/");
        sendRequest("/nonexist.html");
    }

    private static void startServer() throws IOException {
        ServerSocket server = new ServerSocket(8080);
        System.out.printf("服务器已启动，监听端口 8080%n");
        while (true) {
            try (Socket client = server.accept()) {
                handleClient(client);
            } catch (IOException e) {
                System.err.printf("处理客户端请求异常: %s%n", e.getMessage());
            }
        }
    }

    private static void handleClient(Socket client) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream(), "UTF-8"));
        OutputStream out = client.getOutputStream();

        String requestLine = in.readLine();
        if (requestLine == null) {
            return;
        }

        String[] parts = requestLine.split(" ");
        String path = parts.length > 1 ? parts[1] : "/";
        if ("/".equals(path)) {
            path = "/index.html";
        }

        String filePath = "build" + path;
        Path resolved = Paths.get(filePath);

        if (Files.exists(resolved)) {
            String content = Files.readString(resolved);
            String response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n" + content;
            out.write(response.getBytes("UTF-8"));
        } else {
            String response = "HTTP/1.1 404 Not Found\r\n\r\n404 Not Found";
            out.write(response.getBytes("UTF-8"));
        }
        out.flush();
    }

    private static void sendRequest(String path) throws IOException {
        try (Socket sock = new Socket("localhost", 8080)) {
            OutputStream out = sock.getOutputStream();
            BufferedReader in = new BufferedReader(new InputStreamReader(sock.getInputStream(), "UTF-8"));

            String request = "GET " + path + " HTTP/1.1\r\nHost: localhost\r\n\r\n";
            out.write(request.getBytes("UTF-8"));
            out.flush();

            System.out.printf("请求: GET %s%n", path);
            String line;
            while ((line = in.readLine()) != null) {
                System.out.println(line);
            }
            System.out.println();
        }
    }
}
