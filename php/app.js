// Kết nối tới server WebSocket PHP (port 8080)
const socket = new WebSocket("ws://localhost:8080");

const messages = document.getElementById("messages");
const input = document.getElementById("input");
const send = document.getElementById("send");

// Khi server gửi tin nhắn về
socket.onmessage = (event) => {
  const msg = document.createElement("div");
  msg.textContent = event.data;
  messages.appendChild(msg);
  messages.scrollTop = messages.scrollHeight;
};

// Gửi tin nhắn khi nhấn nút
send.onclick = () => {
  if (input.value.trim() !== "") {
    socket.send(input.value);
    input.value = "";
  }
};
