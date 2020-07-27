process.setMaxListeners(0);

// global variable
// is bathroom occupied?
occupied = false;

// console.log(value ? "Bathroom is occupied." : "Bathroom is vacant.");
// io.emit('door change', value);

var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

// app.use(express.static('images'));

app.get('/', function(req, res) {
    res.sendFile(__dirname + '/monitor.html');
});

// Code to receive door status update
app.put('/update', function(req, res) {
        // set door status
        // (due to physical setup, only receives on-close signal)
        occupied = true;
});

io.on('connection', function(socket){
    console.log('a user connected');

    // var doorStart = magSwitch.readSync();
    // get door status once upon loading
    socket.on('poll', () => {
        io.emit('poll'); // Poll door status
    });

    // ESP8266 sends 'update' signal,
    // which triggers all connected users to receive
    // new door status
    socket.on('doorOpened', (occupied) => {
        occupied = false;
        io.emit('vacant', );
        console.log(occupied ? "Bathroom is occupied." : "Bathroom is vacant.");
    });
    socket.on('doorClosed', (occupied) => {
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
