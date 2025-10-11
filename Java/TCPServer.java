package com.example.tcpoptimizationjava;

import java.io.*;
import java.net.*;

public class TCPServer {
    public static void main(String[] args) {
        int port = 5000;

        try (ServerSocket serverSocket = new ServerSocket()) {
            // Cho phép tái sử dụng địa chỉ để restart nhanh
            serverSocket.setReuseAddress(true);
            serverSocket.bind(new InetSocketAddress("localhost", port));
            System.out.println("✅ Server đang lắng nghe trên cổng " + port + "...");

            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("👉 Kết nối từ: " + clientSocket.getInetAddress());

                // Bật KeepAlive & tắt Nagle để tối ưu truyền nhỏ
                clientSocket.setKeepAlive(true);
                clientSocket.setTcpNoDelay(true);

                // Xử lý mỗi client trong luồng riêng
                new Thread(() -> handleClient(clientSocket)).start();
            }

        } catch (IOException e) {
            System.err.println("❌ Lỗi Server: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private static void handleClient(Socket socket) {
        try (
                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true)
        ) {
            String line;
            while ((line = in.readLine()) != null) {
                System.out.println("📩 Nhận từ client: " + line);
                out.println("Server đã nhận: " + line);
            }
        } catch (IOException e) {
            System.err.println("⚠️ Client ngắt kết nối: " + e.getMessage());
        }
    }
}
