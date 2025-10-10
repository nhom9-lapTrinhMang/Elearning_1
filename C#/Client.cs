using System;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

const int Cong = 13000;
const string DiaChiMayChu = "127.0.0.1";

try
{
    using TcpClient mayKhach = new TcpClient();
    Console.WriteLine($"Đang kết nối tới máy chủ {DiaChiMayChu}:{Cong}...");
    await mayKhach.ConnectAsync(DiaChiMayChu, Cong);
    Console.WriteLine("Kết nối thành công!");

    using NetworkStream luongDuLieu = mayKhach.GetStream();

    while (true)
    {
        Console.Write("Nhập tin nhắn gửi đến máy chủ (Enter để thoát): ");
        string thongDiep = Console.ReadLine() ?? "";

        if (string.IsNullOrWhiteSpace(thongDiep))
            break; 

        byte[] duLieuGui = Encoding.UTF8.GetBytes(thongDiep);
        await luongDuLieu.WriteAsync(duLieuGui, 0, duLieuGui.Length);

        byte[] boDem = new byte[65536];
        int soByteNhan = await luongDuLieu.ReadAsync(boDem, 0, boDem.Length);
        string phanHoi = Encoding.UTF8.GetString(boDem, 0, soByteNhan);
        Console.WriteLine($"Phản hồi từ máy chủ: {phanHoi}");
    }

    Console.WriteLine("🔚 Đã ngắt kết nối khỏi máy chủ.");
}
catch (SocketException loi)
{
    Console.WriteLine($"Lỗi socket: {loi.Message}");
}
catch (Exception loi)
{
    Console.WriteLine($"Lỗi: {loi.Message}");
}
