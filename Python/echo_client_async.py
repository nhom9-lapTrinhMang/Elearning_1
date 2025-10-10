import asyncio

async def tcp_echo_client():
    reader, writer = await asyncio.open_connection('127.0.0.1', 8888)
    print("Kết nối đến server thành công! Nhập 'exit' để thoát.")

    while True:
        msg = input("Nhập tin nhắn: ")
        if msg.lower() == 'exit':
            break
        writer.write(msg.encode())
        await writer.drain()

        data = await reader.read(100)
        print(f"Phản hồi từ server: {data.decode()}")

    print("Đóng kết nối.")
    writer.close()
    await writer.wait_closed()

if __name__ == "__main__":
    asyncio.run(tcp_echo_client())
