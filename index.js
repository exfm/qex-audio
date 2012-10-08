(function(){

// constructor
function QEXAudio(){
    var eventEmitter;
    if(typeof module !== "undefined"){
        var EventEmitter = require('event-emitter');
        eventEmitter = new EventEmitter();
    }
    else{
        eventEmitter = new window.EventEmitter();
    }
    $.extend(this, eventEmitter);
    
    // boolean to keep paused state
    this.paused = true;
    
    // internal var for audio source
    this.theSrc = '';
    
    // internal var for audio currentTime
    this.theCurrentTime = 0;
    
    // audio duration
    this.duration = 0;
    
    // audio volume
    this.volume = 1;
    
    // audio buffered array
    this.buffered = {
        'length': 0,
        'start': 0,
        'end': 0
    };
    
    this.addGettersAndSetters();
    
      
    // register event listener with native

    //window.cordova.exec(this.eventHandler.bind(this), this.errorHandler.bind(this), "QEXAudio", "eventHandler", []);
    
    var self = this;
    
    audioplayer.nativeEvent.connect(function(){
		self.eventHandler.apply(self, arguments);
	});
	
	this.sendToNative("init", []);

    console.log('QEXAudio installed');
    
    window.audioPlayer = this;
}

// send to native
QEXAudio.prototype.sendToNative = function(func, vars){
    var toExecute = {
        "func": func,
        "vars": vars
    };

    audioplayer.exec(toExecute);
}

// getters & setters
QEXAudio.prototype.addGettersAndSetters = function(){
    
    // audio source
    Object.defineProperty(this, "src", {
        get: function() {
            return this.theSrc;
        },
        set: function(url) {
            this.theSrc = url;
            QEXAudio.prototype.sendToNative("setSource", [url]);
        },
        enumerable : true
    });
    
    // audio currentTime
    Object.defineProperty(this, "currentTime", {
        get: function() {
            return this.theCurrentTime;
        },
        set: function(time) {
            QEXAudio.prototype.sendToNative("seek", [time]);
        },
        enumerable : true
    });
}

// load a new song
QEXAudio.prototype.load = function(){
    QEXAudio.prototype.sendToNative("load", []);
};

// play a song
QEXAudio.prototype.play = function(){
    QEXAudio.prototype.sendToNative("play", []);
};

// pause the current playing song
QEXAudio.prototype.pause = function(){
    QEXAudio.prototype.sendToNative("pause", []);
};

// set nowPlaying. Sets lock screen on iOS
QEXAudio.prototype.nowPlaying = function(artist, title, album, imageUrl){
    QEXAudio.prototype.sendToNative("nowPlaying", [artist, title, album, imageUrl]);
};

// listen for events from native. emit out to js listeners
QEXAudio.prototype.eventHandler = function(event){
    switch (event.name){
        case 'timeupdate':
            this.theCurrentTime = event.currentTime;
        break;
        case 'durationchanged':
            this.duration = event.duration;
        break;
        case 'pause':
            this.paused = true;
        break;
        case 'playing':
            this.paused = false;
        break;
        case 'play':
            this.paused = false;
        break;
        case 'progress':
            this.buffered.length = event.ranges.length;
            this.buffered.start = event.ranges[0].start;
            this.buffered.end = event.ranges[0].duration;
        break;
        default:
            
        break;
    }
    this.emit(event.name,
        {
            'target': 
                {
                    'src': this.theSrc,
                    'paused': this.paused,
                    'currentTime': this.theCurrentTime,
                    'duration': this.duration,
                    'volume': this.volume,
                    'buffered': this.buffered
                }
        }
    );
}

// listen for errors from native. Throw js error
QEXAudio.prototype.errorHandler = function(event){
    throw new TypeError(event.error);
}


if(typeof module !== "undefined"){
    module.exports = QEXAudio;
}
else{
    window.QEXAudio = QEXAudio;
}

}()); // end wrapper
