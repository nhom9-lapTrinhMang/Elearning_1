import asyncio

clients = set()

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"[Kết nối mới] {addr}")
    clients.add(writer)

    try:
        while True:
            data = await reader.readline()
            if not data:
                break
            message = data.decode().strip()
            print(f"{addr}: {message}")

            # Gửi lại tin nhắn cho tất cả client khác
            for client in clients:
                if client != writer:
                    client.write(f"{addr}: {message}\n".encode())
                    await client.drain()
    except Exception as e:
        print(f"Lỗi: {e}")
    finally:
        print(f"[Ngắt kết nối] {addr}")
        clients.remove(writer)
        writer.close()
        await writer.wait_closed()

async def main():
    server = await asyncio.start_server(handle_client, '127.0.0.1', 9999)
    print("Chat server đang chạy tại cổng 9999...")
    async with server:
        await server.serve_forever()

if __name__ == "__main__":
    asyncio.run(main())
