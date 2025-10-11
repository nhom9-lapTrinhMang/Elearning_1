package com.example.tcpoptimizationjava;

import java.io.*;
import java.net.*;

public class TCPClient {
    public static void main(String[] args) {
        String host = "localhost";
        int port = 5000;

        try (Socket socket = new Socket()) {
            // Kết nối tới server
            socket.connect(new InetSocketAddress(host, port), 3000);

            // Bật KeepAlive & tắt Nagle
            socket.setKeepAlive(true);
            socket.setTcpNoDelay(true);

            // Tùy chỉnh buffer để tối ưu
            socket.setSendBufferSize(8192);
            socket.setReceiveBufferSize(8192);

            System.out.println("✅ Đã kết nối tới server.");

            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            // Gửi nhiều gói tin nhỏ — sẽ thấy khác biệt khi bật/tắt Nagle
            for (int i = 1; i <= 5; i++) {
                out.println("Gói tin #" + i);
                System.out.println("📤 Đã gửi: Gói tin #" + i);
                Thread.sleep(500); // mô phỏng gửi từng gói nhỏ
                System.out.println("📥 Phản hồi: " + in.readLine());
            }

            System.out.println("Hoàn tất gửi dữ liệu.");

        } catch (IOException | InterruptedException e) {
            System.err.println("❌ Lỗi Client: " + e.getMessage());
            e.printStackTrace();
        }
    }
}
