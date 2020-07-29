process.setMaxListeners(0);

// global variable
// is bathroom occupied?
occupied = false;

// console.log(value ? "Bathroom is occupied." : "Bathroom is vacant.");
// io.emit('door change', value);

var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

var occupied = null;

// app.use(express.static('images'));

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
        occupied = false;
        io.emit('vacant', );
        console.log(occupied ? "Bathroom is occupied." : "Bathroom is vacant.");
    });
    socket.on('doorClosed', () => {
        occupied = true;
        io.emit('occupied', );
        console.log(occupied ? "Bathroom is occupied." : "Bathroom is vacant.");
    });

    socket.on('disconnect', function(){
        console.log('user disconnected');
    });
});

http.listen(8080, function() {
    console.log('listening on port 8080');
});
