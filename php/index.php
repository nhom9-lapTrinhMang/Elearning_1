<!DOCTYPE html>
<html lang="vi">
<head>
    <meta charset="UTF-8">
    <title>Async Chat</title>
    <style>
        body { font-family: Arial; background: #f4f4f4; text-align: center; }
        #chat { width: 400px; margin: 40px auto; background: white; border-radius: 10px; padding: 15px; box-shadow: 0 0 5px gray; }
        #messages { height: 200px; overflow-y: scroll; border: 1px solid #ccc; margin-bottom: 10px; text-align: left; padding: 5px; }
        #input { width: 80%; padding: 5px; }
        #send { padding: 5px 10px; }
    </style>
</head>
<body>
    <div id="chat">
        <h3>💬 Chat Room</h3>
        <div id="messages"></div>
        <input type="text" id="input" placeholder="Nhập tin nhắn..." />
        <button id="send">Gửi</button>
    </div>

    <script src="app.js"></script>
</body>
</html>
