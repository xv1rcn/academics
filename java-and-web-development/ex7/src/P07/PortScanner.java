import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

public class PortScanner {
    private static final String HOST = "127.0.0.1";
    private static final int START_PORT = 20;
    private static final int END_PORT = 100;
    private static final int THREAD_COUNT = 10;
    private static final int TIMEOUT_MS = 200;
    private static final int DEMO_PORT = 9999;

    public static void main(String[] args) throws Exception {
        ServerSocket serverSocket = new ServerSocket(DEMO_PORT);
        Thread serverThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    serverSocket.accept();
                } catch (IOException ignored) {
                }
            }
        });
        serverThread.setDaemon(true);
        serverThread.start();

        Thread.sleep(200);

        List<Integer> targetPorts = new ArrayList<>();
        for (int port = START_PORT; port <= END_PORT; port++) {
            targetPorts.add(port);
        }
        targetPorts.add(DEMO_PORT);

        ExecutorService executor = Executors.newFixedThreadPool(THREAD_COUNT);
        List<Future<ScanResult>> futures = new ArrayList<>();

        for (int port : targetPorts) {
            final int p = port;
            Future<ScanResult> future = executor.submit(new Callable<ScanResult>() {
                @Override
                public ScanResult call() {
                    try (Socket socket = new Socket()) {
                        socket.connect(new InetSocketAddress(HOST, p), TIMEOUT_MS);
                        return new ScanResult(p, true);
                    } catch (IOException e) {
                        return new ScanResult(p, false);
                    }
                }
            });
            futures.add(future);
        }

        executor.shutdown();
        executor.awaitTermination(30, TimeUnit.SECONDS);

        serverSocket.close();

        List<Integer> openPorts = new ArrayList<>();
        for (Future<ScanResult> future : futures) {
            try {
                ScanResult result = future.get();
                if (result.open) {
                    openPorts.add(result.port);
                }
            } catch (ExecutionException ignored) {
            }
        }

        Collections.sort(openPorts);

        System.out.printf("端口扫描结果 (127.0.0.1, 范围 %d-%d，含端口 %d):%n",
                START_PORT, END_PORT, DEMO_PORT);
        System.out.printf("开放端口数量: %d%n", openPorts.size());
        System.out.printf("开放端口列表:%n");
        for (int port : openPorts) {
            System.out.printf("  端口 %d: 开放%n", port);
        }
    }

    private static class ScanResult {
        final int port;
        final boolean open;

        ScanResult(int port, boolean open) {
            this.port = port;
            this.open = open;
        }
    }
}
