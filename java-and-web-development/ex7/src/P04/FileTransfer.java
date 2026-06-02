import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class FileTransfer {

    private static final int PORT = 9001;
    private static final String SERVER_DIR = "build/server_files";

    public static void main(String[] args) {
        Thread serverThread = new Thread(new Runnable() {
            @Override
            public void run() {
                startServer();
            }
        });
        serverThread.setDaemon(true);
        serverThread.start();

        try {
            Thread.sleep(200);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        runClient();
    }

    private static void startServer() {
        try {
            ServerSocket serverSocket = new ServerSocket(PORT);
            System.out.printf("文件传输服务器启动，监听端口 %d%n", PORT);

            while (true) {
                Socket clientSocket = serverSocket.accept();
                new Thread(new ServerHandler(clientSocket)).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class ServerHandler implements Runnable {
        private Socket socket;

        ServerHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            try {
                DataInputStream dis = new DataInputStream(socket.getInputStream());
                DataOutputStream dos = new DataOutputStream(socket.getOutputStream());

                String command = dis.readUTF();
                System.out.printf("服务器收到命令: %s%n", command);

                if (command.startsWith("UPLOAD")) {
                    String filename = command.substring(7).trim();
                    long fileLength = dis.readLong();
                    byte[] fileData = new byte[(int) fileLength];
                    dis.readFully(fileData);

                    File serverDir = new File(SERVER_DIR);
                    if (!serverDir.exists()) {
                        serverDir.mkdirs();
                    }

                    FileOutputStream fos = new FileOutputStream(new File(serverDir, filename));
                    fos.write(fileData);
                    fos.close();

                    System.out.printf("服务器已保存文件: %s (%d 字节)%n", filename, fileLength);
                } else if (command.startsWith("DOWNLOAD")) {
                    String filename = command.substring(9).trim();
                    File file = new File(SERVER_DIR, filename);

                    if (file.exists() && file.isFile()) {
                        long fileLength = file.length();
                        byte[] fileData = new byte[(int) fileLength];
                        FileInputStream fis = new FileInputStream(file);
                        fis.read(fileData);
                        fis.close();

                        dos.writeLong(fileLength);
                        dos.write(fileData);
                        dos.flush();

                        System.out.printf("服务器已发送文件: %s (%d 字节)%n", filename, fileLength);
                    } else {
                        dos.writeLong(-1L);
                        dos.flush();

                        System.out.printf("服务器文件未找到: %s%n", filename);
                    }
                }

                dis.close();
                dos.close();
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static void runClient() {
        try {
            File buildDir = new File("build");
            if (!buildDir.exists()) {
                buildDir.mkdirs();
            }

            String testContent = "hello文件传输测试内容";
            File testFile = new File(buildDir, "test.txt");
            FileOutputStream fos = new FileOutputStream(testFile);
            fos.write(testContent.getBytes("UTF-8"));
            fos.close();
            System.out.printf("客户端已创建测试文件: %s%n", testFile.getPath());

            Socket socket = new Socket("127.0.0.1", PORT);
            DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
            DataInputStream dis = new DataInputStream(socket.getInputStream());

            dos.writeUTF("UPLOAD test.txt");
            byte[] uploadData = testContent.getBytes("UTF-8");
            dos.writeLong(uploadData.length);
            dos.write(uploadData);
            dos.flush();
            System.out.printf("客户端上传文件成功%n");

            socket.close();

            Socket socket2 = new Socket("127.0.0.1", PORT);
            DataOutputStream dos2 = new DataOutputStream(socket2.getOutputStream());
            DataInputStream dis2 = new DataInputStream(socket2.getInputStream());

            dos2.writeUTF("DOWNLOAD test.txt");
            dos2.flush();

            long downloadLength = dis2.readLong();
            if (downloadLength >= 0) {
                byte[] downloadData = new byte[(int) downloadLength];
                dis2.readFully(downloadData);

                File downloadedFile = new File(buildDir, "downloaded.txt");
                FileOutputStream dfos = new FileOutputStream(downloadedFile);
                dfos.write(downloadData);
                dfos.close();

                String downloadedContent = new String(downloadData, "UTF-8");
                System.out.printf("客户端下载文件成功: %s%n", downloadedFile.getPath());
                System.out.printf("下载内容: %s%n", downloadedContent);

                if (downloadedContent.equals(testContent)) {
                    System.out.printf("验证成功：上传文件与下载文件内容一致%n");
                } else {
                    System.out.printf("验证失败：文件内容不匹配%n");
                }
            } else {
                System.out.printf("下载失败：服务器文件不存在%n");
            }

            dis2.close();
            dos2.close();
            socket2.close();

            System.out.printf("文件传输完成%n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
