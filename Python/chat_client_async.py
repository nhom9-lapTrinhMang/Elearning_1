import asyncio

async def read_messages(reader):
    while True:
        data = await reader.readline()
        if not data:
            break
        print(data.decode().strip())

async def send_messages(writer):
    while True:
        msg = input()
        writer.write((msg + "\n").encode())
        await writer.drain()
        if msg.lower() == "exit":
            print("Ngắt kết nối.")
            writer.close()
            await writer.wait_closed()
            break

async def main():
    reader, writer = await asyncio.open_connection('127.0.0.1', 9999)
    print("Đã kết nối tới chat server. Gõ 'exit' để thoát.")
    await asyncio.gather(read_messages(reader), send_messages(writer))

if __name__ == "__main__":
    asyncio.run(main())
