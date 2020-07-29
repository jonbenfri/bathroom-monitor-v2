process.setMaxListeners(0);

// global variable
// is bathroom occupied?
var occupied = null;
const WeMosIP = "WeMosIP";

var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function(req, res) {
    res.sendFile(__dirname + '/bathroom-monitor.html');
});

io.on('connection', function(socket){
    console.log('a user connected');

    // get door status once upon loading
    socket.on('poll', () => {
        // emit door status
        if (occupied !== null)
            io.emit(occupied ? "occupied" : "vacant",);
    });

    // ESP8266 sends 'update' signal,
    // which triggers all connected users to receive
    // new door status
    socket.on('doorOpened', () => {
        var address = socket.handshake.headers["x-real-ip"]
        console.log("Client's address: ", address);
        if (address === WeMosIP) {
            occupied = false;
            io.emit('vacant', );
            console.log(occupied ? "Bathroom is occupied." : "Bathroom is vacant.");
        }
    });
    socket.on('doorClosed', () => {
        var address = socket.handshake.headers["x-real-ip"]
        console.log("Client's address: ", address);
        if (address == WeMosIP) {
            occupied = true;
            io.emit('occupied', );
            console.log(occupied ? "Bathroom is occupied." : "Bathroom is vacant.");
        }
    });

    socket.on('disconnect', function(){
        console.log('user disconnected');
    });
});

http.listen(8080, function() {
    console.log('listening on port 8080');
});
