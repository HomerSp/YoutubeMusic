;(function($){
	$.extend($.fn, {
		observe: function(opts, callback) {
			var obj = this; 
			var MutationObserver = window.MutationObserver || window.WebKitMutationObserver || window.MozMutationObserver;
			var observer = new MutationObserver(function(mutations) {
				mutations.forEach(function(mutation) {
					callback.call(obj, mutation);
				});
			});

			return this.each(function(idx){
				observer.observe(this, opts);
			});
		}
	})
})(Zepto)

var Youtube = (function() {
	var youtube = {};
	youtube.songInfo = {
		albumArt: "",
		artist: "",
		album: "",
		title: "",
		year: "",
		length: 0,
		trackId: 1
	};
	youtube.songPosition = 0
	youtube.songState = {
		playing: 0, // 0 = stopped, 1 = playing, 2 = paused
		repeat: 0 // 0 = off, 1 = all, 2 = one
	}
	youtube.volume = 0.0
	youtube.user = {
		name: ""
	}

	youtube.songChanged = function(callback) {
		var playerbarEl = document.querySelector("ytmusic-player-bar");
		var playerqueueEl = document.querySelector("ytmusic-player-queue");

		var songChange = function() {
			var thumbnail = youtube.songInfo.albumArt;
			if(playerbarEl.currentItem_ !== null && playerbarEl.currentItem_.thumbnail !== null && playerbarEl.currentItem_.thumbnail.thumbnails.length > 0) {
				thumbnail = playerbarEl.currentItem_.thumbnail.thumbnails[playerbarEl.currentItem_.thumbnail.thumbnails.length - 1].url;
			}

			var metadata = playerbarEl.displayedMetadata_;
			var artist = (metadata.bylineText.length > 0 && metadata.bylineText[0].runs.length > 0) ? metadata.bylineText[0].runs[0].text : "";
			var album = (metadata.bylineText.length > 0 && metadata.bylineText[0].runs.length > 2) ? metadata.bylineText[0].runs[2].text : "";
			var title = (metadata.title !== undefined) ? metadata.title.runs[0].text : "";
			var year = (metadata.bylineText.length > 0 && metadata.bylineText[0].runs.length > 4) ? metadata.bylineText[0].runs[4].text : "";
			var length = playerbarEl.durationSeconds_;
			var trackId = playerqueueEl.selectedQueueItemIndex_ + 1;

			if(youtube.songInfo.albumArt !== thumbnail
				|| youtube.songInfo.artist !== artist
				|| youtube.songInfo.album !== album
				|| youtube.songInfo.title !== title
				|| youtube.songInfo.year !== year
				|| youtube.songInfo.length !== length
				|| youtube.songInfo.trackId !== trackId) {
				youtube.songInfo.albumArt = thumbnail;
				youtube.songInfo.artist = artist;
				youtube.songInfo.album = album;
				youtube.songInfo.title = title;
				youtube.songInfo.year = year;
				youtube.songInfo.length = length;
				youtube.songInfo.trackId = trackId;

				callback.call(youtube);
			}
		}

		playerbarEl.zeptoSongChange = function() { songChange(); };
		playerqueueEl.zeptoSongChange = function() { songChange(); };
		
		playerbarEl._addComplexObserverEffect("zeptoSongChange(displayedMetadata_)");
		playerbarEl._addComplexObserverEffect("zeptoSongChange(durationSeconds_)");
		playerqueueEl._addComplexObserverEffect("zeptoSongChange(selectedQueueItemIndex_)");
	}

	youtube.songPositionChanged = function(callback) {
		var playerbarEl = document.querySelector("ytmusic-player-bar");
		playerbarEl.zeptoProgressChange = function(pos) {
			if(!isNaN(pos) && youtube.songPosition !== pos) {
				youtube.songPosition = pos;
				callback.call(youtube);
			}
		};
		
		playerbarEl._addComplexObserverEffect("zeptoProgressChange(currentSeconds_)");
	}

	youtube.songStateChanged = function(callback) {
		var playerbarEl = document.querySelector("ytmusic-player-bar");
		playerbarEl.zeptoSongStateChange = function() {
			var playing = 0;
			if(playerbarEl.currentItem_ !== null) {
				playing = (playerbarEl.playing_) ? 1 : 2;
			}

			var repeat = 0;
			if(playerbarEl.repeatMode_ == "ALL") {
				repeat = 1;
			} else if(playerbarEl.repeatMode_ == "ONE") {
				repeat = 2;
			}

			if(youtube.songState.playing !== playing
				|| youtube.songState.repeat !== repeat) {
				youtube.songState.playing = playing;
				youtube.songState.repeat = repeat;
				callback.call(youtube);
			}
		};
		playerbarEl._addComplexObserverEffect("zeptoSongStateChange(repeatMode_)");
		playerbarEl._addComplexObserverEffect("zeptoSongStateChange(playing_)");
	}

	youtube.volumeChanged = function(callback) {
		var playerbarEl = document.querySelector("ytmusic-player-bar");
		playerbarEl.zeptoVolumeChange = function(volume) {
			if(!isNaN(volume) && youtube.volume !== volume) {
				youtube.volume = volume;
				callback.call(youtube);
			}
		};

		playerbarEl._addComplexObserverEffect("zeptoVolumeChange(volume_)");
	}

	youtube.userChanged = function(callback) {
		var settingsEl = document.querySelector("ytmusic-settings-button");
		settingsEl.zeptoUserChange = function(data) {
			if(data === undefined || data === null) {
				youtube.user.name = "";
				callback.call(youtube);
				return;
			}

			if(youtube.user.name !== data.name) {
				youtube.user.name = data.name;
				callback.call(youtube);
			}
		};

		settingsEl._addComplexObserverEffect("zeptoUserChange(activeAccount)");
		settingsEl.zeptoUserChange(settingsEl.activeAccount);
	}

	youtube.gotoPage = function(page) {
		var endpoint = {};
		switch(page) {
			case "signout": {
				endpoint = {endpoint: {
					signOutEndpoint: {}
				}};
				document.querySelector("ytmusic-player").fire("yt-navigate", endpoint);
				break;
			}
			case "history": {
				endpoint = {endpoint: {
					browseEndpoint: {
                        browseId: "FEmusic_history"
                    }
				}};
				document.querySelector("ytmusic-player").fire("yt-navigate", endpoint);
				break;
			}
			case "settings": {
				endpoint = {endpoint: {
					applicationSettingsEndpoint: {}
				}};
				document.querySelector("ytmusic-player").fire("yt-navigate", endpoint);
				break;
			}
			case "help": {
				endpoint = {signalServiceEndpoint: {
                    signal: "CLIENT_SIGNAL",
                    actions: [{
                        signalAction: {
                            signal: "HELP"
                        }
                    }]
                }};
                document.querySelector("ytmusic-player").sendServiceRequestAction([endpoint]);
                document.querySelector("ytmusic-player").fire("yt-execute-service-endpoint", endpoint);
				break;
			}
			case "feedback": {
				endpoint = {signalServiceEndpoint: {
                    signal: "CLIENT_SIGNAL",
                    actions: [{
                        sendFeedbackAction: {}
                    }]
                }};
                document.querySelector("ytmusic-player").sendServiceRequestAction([endpoint]);
                document.querySelector("ytmusic-player").fire("yt-execute-service-endpoint", endpoint);
				break;
			}
		}
	}

	youtube.pause = function() {
		if(youtube.songState.playing == 1) {
			youtube.pausePlay();
		}

	}
	youtube.play = function() {
		if(youtube.songState.playing == 2) {
			youtube.pausePlay();
		}
	}
	youtube.pausePlay = function() {
		document.querySelector("ytmusic-player-bar").onPlayPauseButtonTap_();
	}
	youtube.stop = function() {
		console.error("stop not implemented yet");
	}
	youtube.next = function() {
		document.querySelector("ytmusic-player-bar").onNextButtonTap_();
	}
	youtube.previous = function() {
		document.querySelector("ytmusic-player-bar").onPreviousButtonTap_();
	}
	youtube.seek = function(offset) {
		var el = document.querySelector("ytmusic-player-bar");
		el.gatedSeekBy_(offset);
	}
	youtube.setPosition = function(position) {
		var el = document.querySelector("ytmusic-player-bar");
		el.onVideoProgress_(position);
		el.onUserSeek_();
	}
	youtube.loopStatus = function(loop) {
		var repeatMode = "NONE";
		if(loop == 1) {
			repeatMode = "ALL";
		} else if(loop == 2) {
			repeatMode = "ONE";
		}

		document.querySelector("ytmusic-player-bar").dispatch({
			type: "SET_REPEAT",
			payload: repeatMode
        });
	}
	youtube.shuffle = function() {
		document.querySelector("ytmusic-player-bar").onShuffleButtonTap_();
	}

	return youtube;
})()

window.Youtube = Youtube