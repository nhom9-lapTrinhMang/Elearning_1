using System;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;

const int Cong = 13000;
const int KichThuocBoDem = 65536; // 64KB

Console.WriteLine($"Máy chủ đang lắng nghe tại cổng {Cong}...");
var nguoiLangNghe = new TcpListener(IPAddress.Loopback, Cong);
nguoiLangNghe.Start();

while (true)
{
    TcpClient mayKhach = await nguoiLangNghe.AcceptTcpClientAsync(); _ = XuLyMayKhachAsync(mayKhach);
}

async Task XuLyMayKhachAsync(TcpClient mayKhach)
{
    Socket oCam = mayKhach.Client;

    // 1. Vô hiệu hóa thuật toán Nagle (giảm độ trễ)
    oCam.NoDelay = true;

    // 2. Thiết lập kích thước bộ đệm gửi/nhận (tăng tốc độ)
    oCam.ReceiveBufferSize = KichThuocBoDem;
    oCam.SendBufferSize = KichThuocBoDem;

    // 3. Kích hoạt KeepAlive (duy trì kết nối)
    oCam.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.KeepAlive, true);
    oCam.SetSocketOption(SocketOptionLevel.Tcp, SocketOptionName.TcpKeepAliveTime, 1000);
    oCam.SetSocketOption(SocketOptionLevel.Tcp, SocketOptionName.TcpKeepAliveInterval, 5000);

    Console.WriteLine($"Máy khách đã kết nối. " +
        $"Thiết lập socket: NoDelay={oCam.NoDelay}, BoDemGui={oCam.SendBufferSize}, KeepAlive=True");

    try
    {
        NetworkStream luongDuLieu = mayKhach.GetStream();
        byte[] boDem = new byte[KichThuocBoDem];

        int soByteNhan;
        while ((soByteNhan = await luongDuLieu.ReadAsync(boDem, 0, boDem.Length)) > 0)
        {
            string thongDiep = System.Text.Encoding.UTF8.GetString(boDem, 0, soByteNhan);
            Console.WriteLine($"Máy chủ nhận được: {thongDiep.Trim()}");

            string phanHoi = $"Đã nhận: {thongDiep}";
            byte[] duLieuPhanHoi = System.Text.Encoding.UTF8.GetBytes(phanHoi);
            await luongDuLieu.WriteAsync(duLieuPhanHoi, 0, duLieuPhanHoi.Length);
        }
    }
    catch (Exception loi)
    {
        Console.WriteLine($"Lỗi: {loi.Message}");
    }
    finally
    {
        mayKhach.Close();
        Console.WriteLine("Máy khách đã ngắt kết nối.");
    }
}
