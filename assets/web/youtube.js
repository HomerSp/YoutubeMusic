function loadScript(url, callback)
{
    // Adding the script tag to the head as suggested before
    var head = document.getElementsByTagName('head')[0];
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = url;

    // Then bind the event to the callback function.
    // There are several events for cross browser compatibility.
    script.onreadystatechange = callback;
    script.onload = callback;

    // Fire the loading
    head.appendChild(script);
}

function setupDone() {
	Zepto("<style></style>").attr("is", "custom-style").text(".left-content.ytmusic-nav-bar, .right-content.ytmusic-nav-bar {visibility: hidden !important;}").appendTo("head");

	webIntegration.themeChanged.connect(function(data) {
		var el = Zepto("style#webintegration");
		if(el.size() == 1) {
			el.text(data);
		} else {
			Zepto("<style></style>").attr("id", "webintegration").attr("is", "custom-style").text(data).appendTo("head");
		}
	});

	webIntegration.gotoPage.connect(function(page) {
		Youtube.gotoPage(page);
	});
	webIntegration.pause.connect(function() {
		Youtube.pause();
	});
	webIntegration.play.connect(function() {
		Youtube.play();
	});
	webIntegration.pausePlay.connect(function() {
		Youtube.pausePlay();
	});
	webIntegration.stop.connect(function() {
		Youtube.stop();
	});
	webIntegration.next.connect(function() {
		Youtube.next();
	});
	webIntegration.previous.connect(function() {
		Youtube.previous();
	});
	webIntegration.seek.connect(function(offset) {
		Youtube.seek(offset);
	});
	webIntegration.setPosition.connect(function(position) {
		Youtube.setPosition(position);
	});
	webIntegration.loopStatus.connect(function(loop) {
		Youtube.loopStatus(loop);
	});
	webIntegration.shuffle.connect(function() {
		Youtube.shuffle();
	});

	Youtube.songChanged(function() {
		webIntegration.songChanged(this.songInfo);
	});
	Youtube.songPositionChanged(function() {
		webIntegration.songPositionChanged(this.songPosition);
	});
	Youtube.songStateChanged(function() {
		webIntegration.songStateChanged(this.songState);
	});
	Youtube.volumeChanged(function() {
		webIntegration.volumeChanged(this.volume);
	});
	Youtube.userChanged(function() {
		webIntegration.userChanged(this.user);
	});

	webIntegration.loadTheme();
}

var zeptoLoaded = function() {
	new QWebChannel(qt.webChannelTransport, function(channel) {
		window.webIntegration = channel.objects.webIntegration;
		setupDone();
	});
}

loadScript("qrc:/web/zepto.min.js", function() {
	loadScript("qrc:/web/zepto.youtube.js", function() {
		loadScript("qrc:/qtwebchannel/qwebchannel.js", zeptoLoaded);
	});
});
