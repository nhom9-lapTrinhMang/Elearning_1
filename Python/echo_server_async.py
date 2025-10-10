import asyncio

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"[Kết nối mới] {addr}")
    try:
        while True:
            data = await reader.read(100)
            if not data:
                break
            message = data.decode().strip()
            print(f"Nhận từ {addr}: {message}")
            writer.write(data)
            await writer.drain()
    except Exception as e:
        print(f"Lỗi: {e}")
    finally:
        print(f"[Ngắt kết nối] {addr}")
        writer.close()
        await writer.wait_closed()

async def main():
    server = await asyncio.start_server(handle_client, '127.0.0.1', 8888)
    addr = server.sockets[0].getsockname()
    print(f"Server đang chạy tại {addr}")
    async with server:
        await server.serve_forever()

if __name__ == "__main__":
    asyncio.run(main())
